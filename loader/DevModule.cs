using System.IO;
using System.Security.Cryptography;

namespace Lyn.Loader;

// Dev mode injects a locally built DLL instead of the signed module from the
// service. The hashed copy keeps the build output unlocked while the game runs
// and gives every build a stable path for the "another version loaded" check.
internal static class DevModule
{
    internal static string Root => Path.Combine(LoaderSettings.DirectoryPath, "dev");
    internal static bool IsDevPath(string path)
    {
        if (string.IsNullOrWhiteSpace(path)) return false;
        return Path.GetFullPath(path).StartsWith(Path.GetFullPath(Root) + Path.DirectorySeparatorChar,
            StringComparison.OrdinalIgnoreCase);
    }

    // Copying never loads the file into the game. Preparing the same bytes
    // again reuses the existing copy.
    internal static string Prepare(string path)
    {
        string source = Path.GetFullPath(path);
        byte[] bytes = File.ReadAllBytes(source);
        string hash = Convert.ToHexString(SHA256.HashData(bytes)).ToLowerInvariant();
        string directory = Path.Combine(Root, hash);
        string output = Path.Combine(directory, "lyn.dll");
        if (File.Exists(output) && new FileInfo(output).Length == bytes.Length &&
            Convert.ToHexString(SHA256.HashData(File.ReadAllBytes(output))).Equals(hash, StringComparison.OrdinalIgnoreCase))
            return output;
        Directory.CreateDirectory(directory);
        string temporary = Path.Combine(directory, Guid.NewGuid().ToString("N") + ".tmp");
        try
        {
            File.WriteAllBytes(temporary, bytes);
            File.Move(temporary, output, true);
        }
        finally
        {
            if (File.Exists(temporary)) File.Delete(temporary);
        }
        return output;
    }
}
