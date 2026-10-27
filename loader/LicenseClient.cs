using System.Globalization;
using System.IO;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Net.Http.Json;
using System.Security.Cryptography;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace Lyn.Loader;
internal sealed record LicenseInfo
{
    [JsonPropertyName("expires_at")] public long ExpiresAt { get; init; }
    public string Label { get; init; } = "VS 3 / L4D2";
}
internal sealed record HardwareReceipt(string Fingerprint, string KeyHash, long Revision);
internal sealed record LoginResult(string Token, string Username, LicenseInfo? License, HardwareReceipt? Hardware, string? RecoveryCode);
internal sealed class HardwareApprovalRequired(string message, string code, long expiresAt) : InvalidOperationException(message)
{
    internal string RequestCode { get; } = code;
    internal long ExpiresAt { get; } = expiresAt;
}
internal sealed record LeaseTicket(string Payload, string Signature, long ExpiresAt, long LicenseExpiresAt, string Username);
internal sealed record SignedPlan(string Payload,string Signature,long ExpiresAt);
internal sealed record ModuleManifest(string Payload,string Signature,long ExpiresAt,string Sha256,long Bytes,string Version);
internal sealed record HookRow(uint Target,uint Detour,uint Site,string Before);
internal sealed class LicenseClient : IDisposable
{
    internal const string Origin = "https://vs3-accounts-n7q4.rockyknoll128.chatgpt.site";
    internal const string PublicX = "5N647usF0bNqc5qiBTKAA5ciZgA3BvgegkqiiHusrMM";
    internal const string PublicY = "bdjo7-fe4MCu_xtzV6mDB2c9dcwN8goXFxt1hIApxzA";
    private readonly HttpClient client;
    private readonly TimeProvider clock;
    private readonly SemaphoreSlim clockLock = new(1, 1);
    private long serverTime, serverTimestamp;
    private bool clockReady;
    private long ServerNow => serverTime + (long)clock.GetElapsedTime(serverTimestamp).TotalSeconds;
    private IHardwareIdentity? hardware;
    private IHardwareIdentity Hardware => hardware ??= HardwareIdentity.Open();
    private readonly JsonSerializerOptions json = new(JsonSerializerDefaults.Web) { MaxDepth = 16 };
    private string token = "";
    internal string Username { get; private set; } = "";
    internal LicenseInfo? License { get; private set; }
    internal bool SignedIn => token.Length > 0;
    internal bool Licensed => SignedIn && clockReady && License?.ExpiresAt > ServerNow;
    private string recoveryCode = "";
    internal string TakeRecoveryCode() { string value = recoveryCode; recoveryCode = ""; return value; }
    internal LicenseClient(IHardwareIdentity? identity = null, HttpClient? transport = null, TimeProvider? timeProvider = null)
    {
        hardware = identity;
        clock = timeProvider ?? TimeProvider.System;
        client = transport ?? new HttpClient(new HttpClientHandler { AllowAutoRedirect = false, UseCookies = false });
        client.BaseAddress ??= new Uri(Origin + "/");
        client.Timeout = TimeSpan.FromSeconds(15); client.MaxResponseContentBufferSize = 32768;
    }
    private sealed record ClockResult(string Nonce, long ServerTime);
    private async Task SynchronizeClock(CancellationToken cancellation)
    {
        await clockLock.WaitAsync(cancellation);
        try
        {
            if (clockReady && clock.GetElapsedTime(serverTimestamp) is var age && age >= TimeSpan.Zero && age < TimeSpan.FromMinutes(5)) return;
            string nonce = HardwareIdentity.Base64(RandomNumberGenerator.GetBytes(32));
            using var request = new HttpRequestMessage(HttpMethod.Get, new Uri(client.BaseAddress!, "api/loader/time?nonce=" + nonce));
            request.Headers.CacheControl = new CacheControlHeaderValue { NoCache = true, NoStore = true };
            using var response = await client.SendAsync(request, cancellation);
            if (!response.IsSuccessStatusCode) throw new InvalidOperationException("No se pudo sincronizar con el servidor. Vuelve a intentarlo.");
            var result = JsonSerializer.Deserialize<ClockResult>(await response.Content.ReadAsStringAsync(cancellation), json);
            if (result == null || result.Nonce != nonce || result.ServerTime is < 1704067200 or > 4102444800)
                throw new InvalidOperationException("La hora recibida del servidor no es válida. Vuelve a intentarlo.");
            // Use elapsed time rather than Windows wall time, which may differ on another installation.
            serverTime = result.ServerTime; serverTimestamp = clock.GetTimestamp(); clockReady = true;
        }
        finally { clockLock.Release(); }
    }
    private void ClearSession() { token = ""; Username = ""; License = null; recoveryCode = ""; }
    private async Task<T> Request<T>(string path, object? body, CancellationToken cancellation, string? credential = null)
    {
        await SynchronizeClock(cancellation);
        using var request = new HttpRequestMessage(body == null ? HttpMethod.Get : HttpMethod.Post, new Uri(client.BaseAddress!, path));
        byte[] bytes = body == null ? [] : JsonSerializer.SerializeToUtf8Bytes(body, json);
        if (body != null) { request.Content = new ByteArrayContent(bytes); request.Content.Headers.ContentType = new MediaTypeHeaderValue("application/json") { CharSet = "utf-8" }; }
        if (token.Length > 0) request.Headers.Authorization = new AuthenticationHeaderValue("Bearer", token);
        HardwareIdentity.Attach(request, bytes, credential ?? token, Hardware, ServerNow);
        using var response = await client.SendAsync(request, cancellation);
        string text = await response.Content.ReadAsStringAsync(cancellation);
        if (!response.IsSuccessStatusCode) ThrowApiError(response.StatusCode, path, text);
        return JsonSerializer.Deserialize<T>(text, json) ?? throw new InvalidOperationException("Respuesta del servidor no válida.");
    }
    private void ThrowApiError(System.Net.HttpStatusCode status, string path, string text)
    {
        string message = "No se pudo validar el acceso. Comprueba tu conexión e inténtalo de nuevo.", code = "", requestCode = ""; long expires = 0;
        try
        {
            using var document = JsonDocument.Parse(text); var root = document.RootElement;
            if (root.TryGetProperty("error", out var e)) message = e.GetString() ?? message;
            if (root.TryGetProperty("code", out var c)) code = c.GetString() ?? "";
            if (root.TryGetProperty("requestCode", out var r)) requestCode = r.GetString() ?? "";
            if (root.TryGetProperty("expiresAt", out var x)) x.TryGetInt64(out expires);
        }
        catch (Exception error) when (error is JsonException or InvalidOperationException) { }
        if (status == System.Net.HttpStatusCode.Unauthorized || code == "hardware_proof_invalid") ClearSession();
        if (path.StartsWith("api/loader/", StringComparison.Ordinal) && status == System.Net.HttpStatusCode.Forbidden && code != "reauthentication_failed" && code != "hardware_pending") License = null;
        if (code == "hardware_pending" && System.Text.RegularExpressions.Regex.IsMatch(requestCode, "^[A-F0-9]{12}$")) throw new HardwareApprovalRequired(message, requestCode, expires);
        throw new InvalidOperationException(message);
    }
    private void AcceptLogin(LoginResult result)
    {
        var local = Hardware.Device; var receipt = result.Hardware;
        if (string.IsNullOrEmpty(result.Token) || !System.Text.RegularExpressions.Regex.IsMatch(result.Token,"^[A-Za-z0-9_-]{43}$") || receipt == null ||
            receipt.Fingerprint != local.Fingerprint || receipt.KeyHash != HardwareIdentity.Hash(local.KeyX + "." + local.KeyY) || receipt.Revision < 1)
            throw new InvalidOperationException("El servidor no confirmó la vinculación de este equipo. Actualiza el loader y vuelve a intentarlo.");
        token = result.Token; Username = result.Username; License = result.License; recoveryCode = result.RecoveryCode ?? "";
    }
    internal async Task Login(string username, string password, CancellationToken cancellation)
    {
        ClearSession(); username = username.Trim().ToLowerInvariant();
        var result = await Request<LoginResult>("api/loader/login", new { username, password, device = Hardware.Device }, cancellation, "login:" + username);
        AcceptLogin(result);
    }
    private sealed record HardwareLink(string Token, long ExpiresAt);
    internal async Task<Uri> HardwareManagementLink(CancellationToken cancellation)
    {
        var result = await Request<HardwareLink>("api/loader/hardware/web-link", new { }, cancellation);
        if (!System.Text.RegularExpressions.Regex.IsMatch(result.Token, "^[A-Za-z0-9_-]{43}$") || result.ExpiresAt <= ServerNow)
            throw new InvalidOperationException("No se pudo crear el enlace de equipos.");
        return new Uri("https://vs3-accounts.vercel.app/#hardware-link=" + result.Token);
    }
    internal async Task Activate(string key, CancellationToken cancellation)
    {
        var result = await Request<ActivationResult>("api/licenses/activate", new { key }, cancellation);
        License = result.License;
    }
    private sealed record ActivationResult(LicenseInfo License);
    internal async Task<LeaseTicket> Lease(GameTarget game, CancellationToken cancellation)
    {
        string nonce = Convert.ToBase64String(RandomNumberGenerator.GetBytes(24)).TrimEnd('=').Replace('+', '-').Replace('/', '_');
        string created = game.Started.ToString(CultureInfo.InvariantCulture);
        var ticket = await Request<LeaseTicket>("api/loader/lease", new { pid = game.Pid, created, nonce }, cancellation);
        if (!Verify(ticket, game.Pid, created, nonce, ServerNow))
            throw new InvalidOperationException("La autorización del servidor no superó la verificación de firma.");
        return ticket;
    }
    internal static byte[] Decode(string text) => Convert.FromBase64String(text.Replace('-', '+').Replace('_', '/').PadRight((text.Length + 3) / 4 * 4, '='));
    internal static bool VerifySignature(string payload,string signature)
    {
        try {
            if (payload.Length is 0 or >32768 || signature.Length != 86) return false;
            using var verifier = ECDsa.Create(new ECParameters { Curve=ECCurve.NamedCurves.nistP256,Q=new ECPoint{X=Decode(PublicX),Y=Decode(PublicY)} });
            return verifier.VerifyData(Encoding.UTF8.GetBytes(payload),Decode(signature),HashAlgorithmName.SHA256,DSASignatureFormat.IeeeP1363FixedFieldConcatenation);
        } catch (Exception error) when (error is CryptographicException or FormatException or ArgumentException) { return false; }
    }
    internal async Task<string> Hooks(GameTarget game,string nonce,HookRow[] hooks,CancellationToken cancellation)
    {
        var plan=await Request<SignedPlan>("api/loader/prepare-hooks",new {pid=game.Pid,created=game.Started.ToString(CultureInfo.InvariantCulture),nonce,hooks},cancellation);
        string header=plan.Payload.Split('\n')[0];string[] parts=header.Split('|');long now=ServerNow;
        if (parts.Length!=8 || parts[0]!="VSH" || parts[1]!="1" || parts[4]!=game.Pid.ToString(CultureInfo.InvariantCulture) || parts[5]!=game.Started.ToString(CultureInfo.InvariantCulture) || parts[6]!=nonce || parts[7]!=hooks.Length.ToString(CultureInfo.InvariantCulture) ||
            !long.TryParse(parts[2],out long issued) || !long.TryParse(parts[3],out long expires) || issued>now+5 || issued<now-30 || expires<=now || expires<=issued || expires-issued>30 || expires!=plan.ExpiresAt || !VerifySignature(plan.Payload,plan.Signature))
            throw new InvalidOperationException("El plan de hooks no superó la verificación.");
        return plan.Payload+"\n"+plan.Signature;
    }
    internal async Task<string> PrepareModule(GameTarget game,CancellationToken cancellation)
    {
        string nonce=Convert.ToHexString(RandomNumberGenerator.GetBytes(16)).ToLowerInvariant(),created=game.Started.ToString(CultureInfo.InvariantCulture);
        var release=await Request<ModuleManifest>("api/loader/manifest",new{pid=game.Pid,created,nonce},cancellation);
        long now=ServerNow;string[] f=release.Payload.Split('|');
        if(f.Length!=11 || f[0]!="VSM" || f[1]!="1" || f[4]!=game.Pid.ToString(CultureInfo.InvariantCulture) || f[5]!=created || f[6]!=nonce || f[7]!=release.Sha256 || f[8]!=release.Bytes.ToString(CultureInfo.InvariantCulture) || f[9]!=release.Version || f[10]!="2" ||
           !long.TryParse(f[2],out long issued) || !long.TryParse(f[3],out long expires) || issued>now+5 || issued<now-120 || expires<=now || expires<=issued || expires-issued>120 || expires!=release.ExpiresAt || release.Bytes is <512 or >16777216 || !System.Text.RegularExpressions.Regex.IsMatch(release.Sha256,"^[a-f0-9]{64}$") || !VerifySignature(release.Payload,release.Signature))
            throw new InvalidOperationException("El módulo no superó la verificación del servidor.");
        string directory=Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),"lyn","loader","modules",release.Sha256);
        Directory.CreateDirectory(directory);string path=Path.Combine(directory,"lyn.dll");
        if(File.Exists(path) && await Matches(path,release,cancellation))return path;
        string temporary=Path.Combine(directory,Guid.NewGuid().ToString("N")+".tmp");
        using var downloadDeadline=CancellationTokenSource.CreateLinkedTokenSource(cancellation);
        downloadDeadline.CancelAfter(TimeSpan.FromSeconds(90));
        var downloadToken=downloadDeadline.Token;
        try {
            using var request=new HttpRequestMessage(HttpMethod.Get,new Uri(client.BaseAddress!,"api/loader/module/"+release.Sha256));
            request.Headers.Authorization=new AuthenticationHeaderValue("Bearer",token);
            await SynchronizeClock(downloadToken);
            HardwareIdentity.Attach(request,[],token,Hardware,ServerNow);
            using var response=await client.SendAsync(request,HttpCompletionOption.ResponseHeadersRead,downloadToken);
            if(!response.IsSuccessStatusCode){
                await using var errorStream=await response.Content.ReadAsStreamAsync(downloadToken);
                using var errorBytes=new MemoryStream();byte[] errorBlock=new byte[4096];int errorCount;
                while((errorCount=await errorStream.ReadAsync(errorBlock,downloadToken))>0){if(errorBytes.Length+errorCount>32768)throw new InvalidDataException("Respuesta del servidor demasiado grande.");errorBytes.Write(errorBlock,0,errorCount);}
                ThrowApiError(response.StatusCode,"api/loader/module/"+release.Sha256,Encoding.UTF8.GetString(errorBytes.ToArray()));
            }
            if(response.Content.Headers.ContentLength is long length && length!=release.Bytes)throw new InvalidDataException("Tamaño del módulo no válido.");
            await using(var input=await response.Content.ReadAsStreamAsync(downloadToken))
            await using(var output=new FileStream(temporary,FileMode.CreateNew,FileAccess.Write,FileShare.None,81920,true)) {
                byte[] block=new byte[81920];long total=0;int count;
                while((count=await input.ReadAsync(block,downloadToken))>0) {total+=count;if(total>release.Bytes)throw new InvalidDataException("Descarga demasiado grande.");await output.WriteAsync(block.AsMemory(0,count),downloadToken);}
                if(total!=release.Bytes)throw new InvalidDataException("Descarga incompleta.");
            }
            if(!await Matches(temporary,release,cancellation))throw new InvalidDataException("La DLL descargada no coincide con su firma.");
            using(var file=File.OpenRead(temporary))PeImage.Inspect(file);
            File.Move(temporary,path,true);return path;
        } finally {if(File.Exists(temporary))File.Delete(temporary);}
    }
    private static async Task<bool> Matches(string path,ModuleManifest manifest,CancellationToken cancellation)
    {
        await using var file=File.OpenRead(path);return file.Length==manifest.Bytes && Convert.ToHexString(await SHA256.HashDataAsync(file,cancellation)).Equals(manifest.Sha256,StringComparison.OrdinalIgnoreCase);
    }
    internal static bool Verify(LeaseTicket ticket, int pid, string created, string nonce, long now)
    {
        try
        {
            if (string.IsNullOrEmpty(ticket.Payload) || string.IsNullOrEmpty(ticket.Signature)) return false;
            string[] fields = ticket.Payload.Split('|');
            if (fields.Length != 9 || fields[0] != "VS3" || fields[1] != "1" || fields[4] != pid.ToString(CultureInfo.InvariantCulture) ||
                fields[5] != created || fields[6] != nonce || fields[7] != "l4d2" || fields[8] != "1" ||
                !long.TryParse(fields[2], out long issued) || !long.TryParse(fields[3], out long expires) ||
                issued > now + 5 || issued < now - 120 || expires <= now || expires <= issued || expires - issued > 120 || expires != ticket.ExpiresAt) return false;
            using var verifier = ECDsa.Create(new ECParameters { Curve = ECCurve.NamedCurves.nistP256, Q = new ECPoint { X = Decode(PublicX), Y = Decode(PublicY) } });
            return verifier.VerifyData(Encoding.UTF8.GetBytes(ticket.Payload), Decode(ticket.Signature), HashAlgorithmName.SHA256, DSASignatureFormat.IeeeP1363FixedFieldConcatenation);
        }
        catch (Exception error) when (error is CryptographicException or FormatException or ArgumentException) { return false; }
    }
    internal async Task Logout()
    {
        try { if (SignedIn) await Request<JsonElement>("api/auth/logout", new { }, CancellationToken.None); }
        finally { ClearSession(); }
    }
    public void Dispose() { ClearSession(); client.Dispose(); hardware?.Dispose(); }
}
