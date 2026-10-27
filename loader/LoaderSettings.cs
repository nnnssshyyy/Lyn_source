using System.IO;
using System.Text.Json;

namespace Lyn.Loader;

internal sealed class LoaderSettings
{
    public int Theme { get; set; }
    public string DllPath { get; set; } = "";
    public string Username { get; set; } = "";
    public bool RememberUsername { get; set; } = true;
    public bool RememberCredentials { get; set; }
    public int PendingPid { get; set; }
    public long PendingStarted { get; set; }
    public static string DirectoryPath => Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "lyn", "loader");
    private static string FilePath => Path.Combine(DirectoryPath, "settings.json");

    public static LoaderSettings Read()
    {
        try
        {
            if (File.Exists(FilePath))
            {
                var result = JsonSerializer.Deserialize<LoaderSettings>(File.ReadAllText(FilePath));
                if (result != null) { result.Theme = Math.Clamp(result.Theme, 0, 4); return result; }
            }
        }
        catch (Exception e) when (e is IOException or UnauthorizedAccessException or JsonException)
        { LoaderLog.Write("No se pudieron leer las preferencias: " + e.Message); }
        return new();
    }

    public void Save()
    {
        Directory.CreateDirectory(DirectoryPath);
        var temporary = FilePath + ".tmp";
        File.WriteAllText(temporary, JsonSerializer.Serialize(this, new JsonSerializerOptions { WriteIndented = true }));
        File.Move(temporary, FilePath, true);
    }

    // Dev mode resolves a local build: the last local selection, else the
    // newest lyn.dll beside the loader or in a walk-up build\Release folder.
    public static string FindDevDll(string selected)
    {
        if (IsLocalDll(selected)) return Path.GetFullPath(selected);
        string? best = null;
        void Consider(string candidate)
        {
            if (!IsLocalDll(candidate)) return;
            if (best is null || File.GetLastWriteTimeUtc(candidate) > File.GetLastWriteTimeUtc(best)) best = candidate;
        }
        Consider(Path.Combine(AppContext.BaseDirectory, "lyn.dll"));
        var directory = new DirectoryInfo(AppContext.BaseDirectory);
        for (int i = 0; i < 7 && directory != null; ++i, directory = directory.Parent)
            Consider(Path.Combine(directory.FullName, "build", "Release", "lyn.dll"));
        return best is null ? "" : Path.GetFullPath(best);
    }

    private static bool IsLocalDll(string path)
    {
        if (string.IsNullOrWhiteSpace(path) || !File.Exists(path)) return false;
        string full = Path.GetFullPath(path);
        return !DevModule.IsDevPath(full) && !BundledModule.IsBundledPath(full);
    }
}

internal static class LoaderLog
{
    private static readonly object Gate = new();
    public static string FilePath => Path.Combine(LoaderSettings.DirectoryPath, "loader.log");
    public static void Write(string message)
    {
        lock (Gate)
        {
            try
            {
                Directory.CreateDirectory(LoaderSettings.DirectoryPath);
                if (File.Exists(FilePath) && new FileInfo(FilePath).Length > 1024 * 1024)
                    File.Move(FilePath, FilePath + ".previous", true);
                File.AppendAllText(FilePath, $"{DateTimeOffset.Now:yyyy-MM-dd HH:mm:ss zzz}  {message}{Environment.NewLine}");
            }
            catch (Exception e) when (e is IOException or UnauthorizedAccessException) { System.Diagnostics.Debug.WriteLine(e); }
        }
    }
}
