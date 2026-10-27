using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;

namespace Lyn.Loader;

internal sealed record SavedCredentials(string Username, string Password);
internal static class CredentialVault
{
    internal const string Target = "VS3/Accounts/Credentials/v1";
    private const uint Generic = 1;
    private const int NotFound = 1168;
    internal static SavedCredentials? Read(string target = Target)
    {
        if (!CredRead(target, Generic, 0, out var pointer))
        {
            int error = Marshal.GetLastWin32Error();
            if (error == NotFound) return null;
            throw new Win32Exception(error);
        }
        try
        {
            var value = Marshal.PtrToStructure<Credential>(pointer);
            if (value.Type != Generic || value.CredentialBlobSize is < 24 or > 256 || value.CredentialBlobSize % 2 != 0 || value.CredentialBlob == IntPtr.Zero) return null;
            string username = value.UserName ?? "";
            if (!Regex.IsMatch(username, "^[a-z0-9][a-z0-9_.-]{2,31}$")) return null;
            string secret = Marshal.PtrToStringUni(value.CredentialBlob, (int)value.CredentialBlobSize / 2) ?? "";
            return new(username, secret);
        }
        finally { CredFree(pointer); }
    }
    internal static void Save(string username, string password, string target = Target)
    {
        if (!Regex.IsMatch(username, "^[a-z0-9][a-z0-9_.-]{2,31}$") || password.Length is < 12 or > 128) throw new ArgumentException("Credenciales no válidas.");
        IntPtr secret = Marshal.StringToCoTaskMemUni(password);
        try
        {
            var credential = new Credential { Type = Generic, TargetName = target, UserName = username,
                CredentialBlobSize = checked((uint)password.Length * 2), CredentialBlob = secret,
                Persist = 2, Comment = "VS 3 · Acceso al loader" };
            // LOCAL_MACHINE means persistence on this PC for this Windows user,
            // not access by all users or roaming to another computer.
            if (!CredWrite(ref credential, 0)) throw new Win32Exception(Marshal.GetLastWin32Error());
        }
        finally { Marshal.ZeroFreeCoTaskMemUnicode(secret); }
    }
    internal static void Delete(string target = Target)
    {
        if (!CredDelete(target, Generic, 0) && Marshal.GetLastWin32Error() != NotFound) throw new Win32Exception(Marshal.GetLastWin32Error());
    }
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    private struct Credential
    {
        public uint Flags, Type;
        public string? TargetName, Comment;
        public System.Runtime.InteropServices.ComTypes.FILETIME LastWritten;
        public uint CredentialBlobSize;
        public IntPtr CredentialBlob;
        public uint Persist, AttributeCount;
        public IntPtr Attributes;
        public string? TargetAlias, UserName;
    }
    [DllImport("advapi32.dll", EntryPoint = "CredReadW", CharSet = CharSet.Unicode, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)] private static extern bool CredRead(string target, uint type, uint flags, out IntPtr credential);
    [DllImport("advapi32.dll", EntryPoint = "CredWriteW", CharSet = CharSet.Unicode, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)] private static extern bool CredWrite(ref Credential credential, uint flags);
    [DllImport("advapi32.dll", EntryPoint = "CredDeleteW", CharSet = CharSet.Unicode, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)] private static extern bool CredDelete(string target, uint type, uint flags);
    [DllImport("advapi32.dll")] private static extern void CredFree(IntPtr credential);
}
