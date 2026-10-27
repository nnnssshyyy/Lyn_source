using System.IO;
using System.Net.Http;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;
using Microsoft.Win32;

namespace Lyn.Loader;

internal sealed record HardwareDevice(string Fingerprint, string KeyX, string KeyY, string Label);
internal interface IHardwareIdentity : IDisposable
{
    HardwareDevice Device { get; }
    byte[] Sign(byte[] payload);
}

internal sealed class HardwareIdentity : IHardwareIdentity
{
    private readonly ECDsa signer;
    private readonly object sync = new();
    public HardwareDevice Device { get; }
    private HardwareIdentity(ECDsa signer, string fingerprint)
    {
        this.signer = signer;
        var point = signer.ExportParameters(false).Q;
        Device = new(fingerprint, Base64(point.X!), Base64(point.Y!), "PC Windows");
    }

    internal static HardwareIdentity Open()
    {
        if (!OperatingSystem.IsWindows()) throw new PlatformNotSupportedException("La verificación de equipo requiere Windows.");
        string machine;
        using (var hive = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry64))
        using (var key = hive.OpenSubKey(@"SOFTWARE\Microsoft\Cryptography"))
            machine = key?.GetValue("MachineGuid") as string ?? "";
        if (!Guid.TryParse(machine, out var machineGuid)) throw new InvalidOperationException("No se pudo identificar Windows en este equipo.");
        var firmware = FirmwareUuid();
        // Hardware/OS identifiers remain local. Only this product-specific hash is sent.
        string fingerprint = Hash("VS3-HARDWARE|1\n" + machineGuid.ToString("N") + "\n" + firmware);
        const string name = "VS3.DeviceIdentity.v1";
        var platform = new CngProvider("Microsoft Platform Crypto Provider");
        var software = CngProvider.MicrosoftSoftwareKeyStorageProvider;
        CngKey? identity = null;
        foreach (var provider in new[] { platform, software })
        {
            try { if (CngKey.Exists(name, provider)) { identity = CngKey.Open(name, provider); break; } }
            catch (CryptographicException) { }
        }
        if (identity == null)
        {
            foreach (var provider in new[] { platform, software })
            {
                try
                {
                    identity = CngKey.Create(CngAlgorithm.ECDsaP256, name, new CngKeyCreationParameters
                    { Provider = provider, ExportPolicy = CngExportPolicies.None, KeyUsage = CngKeyUsages.Signing });
                    break;
                }
                catch (CryptographicException)
                {
                    // A second loader may have created this key while we opened it.
                    try { if (CngKey.Exists(name, provider)) { identity = CngKey.Open(name, provider); break; } }
                    catch (CryptographicException) { }
                }
            }
        }
        if (identity == null) throw new InvalidOperationException("Windows no pudo preparar la clave protegida de este equipo.");
        using (identity) return new HardwareIdentity(new ECDsaCng(identity), fingerprint);
    }

    private static string FirmwareUuid()
    {
        const uint rsmb = 0x52534d42;
        uint size = GetSystemFirmwareTable(rsmb, 0, null, 0);
        if (size is < 8 or > 1048576) return "unavailable";
        byte[] bytes = new byte[size];
        if (GetSystemFirmwareTable(rsmb, 0, bytes, size) != size) throw new InvalidOperationException("No se pudo leer la identidad del equipo. Vuelve a intentarlo.");
        return ParseFirmwareUuid(bytes);
    }

    internal static string ParseFirmwareUuid(ReadOnlySpan<byte> bytes)
    {
        if (bytes.Length < 8) return "unavailable";
        uint length = System.Buffers.Binary.BinaryPrimitives.ReadUInt32LittleEndian(bytes[4..8]);
        if (length > bytes.Length - 8) return "unavailable";
        int end = 8 + (int)length;
        for (int offset = 8; offset + 4 <= end;)
        {
            int formatted = bytes[offset + 1];
            if (formatted < 4 || offset + formatted > end) break;
            if (bytes[offset] == 1 && formatted >= 24)
            {
                var uuid = bytes.Slice(offset + 8, 16);
                bool zeros = true, ones = true;
                foreach (byte value in uuid) { zeros &= value == 0; ones &= value == 255; }
                if (!zeros && !ones) return Convert.ToHexString(uuid).ToLowerInvariant();
            }
            if (bytes[offset] == 127) break;
            int next = offset + formatted;
            while (next + 1 < end && (bytes[next] != 0 || bytes[next + 1] != 0)) next++;
            if (next + 1 >= end) break;
            offset = next + 2;
        }
        return "unavailable";
    }

    internal static void Attach(HttpRequestMessage request, byte[] body, string credential, IHardwareIdentity identity,
        long? timestamp = null, string? suppliedNonce = null)
    {
        string time = (timestamp ?? DateTimeOffset.UtcNow.ToUnixTimeSeconds()).ToString(System.Globalization.CultureInfo.InvariantCulture);
        string nonce = suppliedNonce ?? Base64(RandomNumberGenerator.GetBytes(32));
        var device = identity.Device;
        string payload = string.Join('\n', "VSHW|1", request.Method.Method.ToUpperInvariant(), request.RequestUri!.PathAndQuery,
            Convert.ToHexString(SHA256.HashData(body)).ToLowerInvariant(), Hash(credential), device.Fingerprint, time, nonce);
        request.Headers.Add("X-VS3-Hardware", "1"); request.Headers.Add("X-VS3-Device", device.Fingerprint);
        request.Headers.Add("X-VS3-Time", time); request.Headers.Add("X-VS3-Nonce", nonce);
        request.Headers.Add("X-VS3-Signature", Base64(identity.Sign(Encoding.UTF8.GetBytes(payload))));
    }

    internal static string Base64(byte[] value) => Convert.ToBase64String(value).TrimEnd('=').Replace('+', '-').Replace('/', '_');
    internal static string Hash(string value) => Convert.ToHexString(SHA256.HashData(Encoding.UTF8.GetBytes(value))).ToLowerInvariant();
    public byte[] Sign(byte[] payload) { lock (sync) return signer.SignData(payload, HashAlgorithmName.SHA256, DSASignatureFormat.IeeeP1363FixedFieldConcatenation); }
    public void Dispose() => signer.Dispose();
    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern uint GetSystemFirmwareTable(uint provider, uint table, [Out] byte[]? buffer, uint size);
}
