using System.ComponentModel;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text.Json;

namespace Lyn.Loader;

internal sealed record CleanupProcess(int Pid, long Started);
internal sealed record CleanupPlan(string Path, string Sha256, CleanupProcess Owner, CleanupProcess Loader);

// Remove only this downloaded executable when the application exits.
// The official SFX remains responsible for its own temporary extraction folder.
internal static class LoaderCleanup
{
    private sealed record Owner(string Path, CleanupProcess Process, CleanupProcess Loader);
    private static Owner? captured;
    private static int scheduled;
    internal static string? CapturedPath => captured?.Path;
    internal static bool ShouldSchedule(bool exiting, bool development) => exiting && !development;

    internal static void Capture(Action<string> report)
    {
        try
        {
            string current = System.IO.Path.GetFullPath(Environment.ProcessPath ?? throw new IOException("No hay ruta de proceso."));
            // Running a DLL through dotnet.exe must never remove the shared
            // runtime host. Renamed loader downloads retain their PE product.
            if (!IsLoaderExecutable(current)) { captured = null; return; }
            using var process = Process.GetCurrentProcess();
            var identity = new CleanupProcess(process.Id, process.StartTime.ToFileTimeUtc());
            captured = new(current, identity, identity);
            int parentId = ParentId();
            if (parentId <= 0) return;
            using var parent = Native.OpenProcess(Native.QueryLimited, false, parentId);
            if (parent.IsInvalid) return;
            string parentPath = Native.ImagePath(parent);
            long parentStarted = Native.Started(parent);
            // Parent IDs may be reused: a real SFX started before its payload.
            if (parentStarted <= identity.Started && IsPackageParent(current, parentPath))
                captured = new(parentPath, new(parentId, parentStarted), identity);
        }
        catch (Exception e) when (e is IOException or UnauthorizedAccessException or Win32Exception or InvalidOperationException or ArgumentException)
        { captured = null; report("No se pudo preparar el autoborrado: " + e.Message); }
    }

    internal static bool IsLoaderExecutable(string path) =>
        string.Equals(FileVersionInfo.GetVersionInfo(path).ProductName, "Lyn.Loader", StringComparison.Ordinal);

    internal static bool IsPackageParent(string payload, string parent)
    {
        string folder = System.IO.Path.GetDirectoryName(System.IO.Path.GetFullPath(payload)) ?? "";
        string temp = System.IO.Path.TrimEndingDirectorySeparator(System.IO.Path.GetFullPath(System.IO.Path.GetTempPath())) + System.IO.Path.DirectorySeparatorChar;
        string name = System.IO.Path.GetFileName(folder);
        if (!folder.StartsWith(temp, StringComparison.OrdinalIgnoreCase) || !name.StartsWith("7zS", StringComparison.OrdinalIgnoreCase) ||
            !System.IO.Path.GetFileName(payload).Equals("Lyn.Loader.exe", StringComparison.OrdinalIgnoreCase) ||
            !System.IO.Path.GetExtension(parent).Equals(".exe", StringComparison.OrdinalIgnoreCase) || !File.Exists(parent)) return false;
        var version = FileVersionInfo.GetVersionInfo(parent);
        if (!string.Equals(version.ProductName, "Lyn.Loader", StringComparison.Ordinal)) return false;
        // Check the actual installer configuration, not a random parent EXE.
        using var file = File.OpenRead(parent);
        byte[] prefix = new byte[(int)Math.Min(file.Length, 2 * 1024 * 1024)];
        file.ReadExactly(prefix);
        string header = System.Text.Encoding.UTF8.GetString(prefix);
        return header.Contains(";!@Install@!UTF-8!", StringComparison.Ordinal) &&
               header.Contains("RunProgram=\"Lyn.Loader.exe\"", StringComparison.Ordinal);
    }

    internal static Task<bool> Schedule(bool exiting, bool development, Action<string> report) =>
        Task.Run(() => ScheduleExit(exiting, development, report));

    // OnExit is synchronous: finish creating the cleanup helper before WPF
    // terminates, without waiting on a continuation on its dispatcher.
    internal static bool ScheduleExit(bool exiting, bool development, Action<string> report)
    {
        if (!ShouldSchedule(exiting, development) || captured is null || Interlocked.CompareExchange(ref scheduled, 1, 0) != 0) return false;
        try
        {
            Owner owner = captured;
            using var file = File.OpenRead(owner.Path);
            string hash = Convert.ToHexString(SHA256.HashData(file)).ToLowerInvariant();
            using var cleanup = Launch(new(owner.Path, hash, owner.Process, owner.Loader));
            return true;
        }
        catch (Exception e) when (e is IOException or UnauthorizedAccessException or Win32Exception or InvalidOperationException)
        { Interlocked.Exchange(ref scheduled, 0); report("No se pudo programar el borrado del ejecutable al cerrar: " + e.Message); return false; }
    }

    internal static Process Launch(CleanupPlan plan)
    {
        string target = System.IO.Path.GetFullPath(plan.Path);
        if (!System.IO.Path.GetExtension(target).Equals(".exe", StringComparison.OrdinalIgnoreCase) ||
            !System.Text.RegularExpressions.Regex.IsMatch(plan.Sha256, "^[a-f0-9]{64}$") ||
            plan.Owner.Pid <= 0 || plan.Loader.Pid <= 0 || plan.Owner.Started <= 0 || plan.Loader.Started <= 0)
            throw new ArgumentException("Plan de limpieza no válido.");
        string shell = System.IO.Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.Windows), "System32", "WindowsPowerShell", "v1.0", "powershell.exe");
        var info = new ProcessStartInfo(shell)
        {
            UseShellExecute = false, CreateNoWindow = true, WindowStyle = ProcessWindowStyle.Hidden,
            RedirectStandardInput = true,
            WorkingDirectory = Environment.GetFolderPath(Environment.SpecialFolder.System)
        };
        foreach (string argument in new[] { "-NoLogo", "-NoProfile", "-NonInteractive", "-WindowStyle", "Hidden", "-Command", "-" }) info.ArgumentList.Add(argument);
        // Unicode paths travel as data in the inherited UTF-16 environment.
        // They are never concatenated into shell source or interpreted as code.
        info.Environment["VS3_CLEANUP_PLAN"] = JsonSerializer.Serialize(plan with { Path = target });
        var process = Process.Start(info) ?? throw new IOException("No se pudo iniciar la limpieza.");
        try { process.StandardInput.WriteLine(Script); process.StandardInput.Close(); return process; }
        catch { process.Dispose(); throw; }
    }

    // No recursive deletion, no elevation, no registry or log cleanup. Wait for
    // the exact process instances, then hash-check the one executable to remove.
    internal const string Script = """
& {
    $ErrorActionPreference = 'Stop'
    $plan = ConvertFrom-Json $env:VS3_CLEANUP_PLAN
    if ($plan.Path -notmatch '(?i)\.exe$' -or $plan.Sha256 -notmatch '^[a-f0-9]{64}$') { exit 1 }
    foreach ($identity in @($plan.Loader, $plan.Owner)) {
        try { $owner = [Diagnostics.Process]::GetProcessById([int]$identity.Pid) }
        catch [ArgumentException] { continue }
        try {
            if ($owner.StartTime.ToFileTimeUtc() -eq [long]$identity.Started -and -not $owner.WaitForExit(120000)) { exit 1 }
        } finally { $owner.Dispose() }
    }
    for ($attempt = 0; $attempt -lt 150; $attempt++) {
        if (-not [IO.File]::Exists($plan.Path)) { exit 0 }
        try {
            $file = [IO.File]::OpenRead($plan.Path)
            try {
                $sha = [Security.Cryptography.SHA256]::Create()
                try { $hash = [BitConverter]::ToString($sha.ComputeHash($file)).Replace('-', '').ToLowerInvariant() }
                finally { $sha.Dispose() }
            } finally { $file.Dispose() }
            if ($hash -ne $plan.Sha256) { exit 1 }
            [IO.File]::Delete($plan.Path)
            exit 0
        } catch [IO.IOException] { [Threading.Thread]::Sleep(100) }
          catch [UnauthorizedAccessException] { [Threading.Thread]::Sleep(100) }
    }
    exit 1
}

""";

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    private struct ProcessEntry
    {
        public uint Size, Usage, Pid;
        public UIntPtr Heap;
        public uint Module, Threads, Parent;
        public int Priority;
        public uint Flags;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 260)] public string Name;
    }
    [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)] private static extern bool Process32FirstW(KernelHandle snapshot, ref ProcessEntry entry);
    [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)] private static extern bool Process32NextW(KernelHandle snapshot, ref ProcessEntry entry);
    private static int ParentId()
    {
        using var snapshot = Native.CreateToolhelp32Snapshot(2, 0);
        if (snapshot.IsInvalid) return 0;
        var entry = new ProcessEntry { Size = (uint)Marshal.SizeOf<ProcessEntry>(), Name = "" };
        if (!Process32FirstW(snapshot, ref entry)) return 0;
        do { if (entry.Pid == Environment.ProcessId) return checked((int)entry.Parent); } while (Process32NextW(snapshot, ref entry));
        return 0;
    }
}
