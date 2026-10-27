using System.IO;
using System.Reflection;
using System.Security.Cryptography;

namespace Lyn.Loader;

internal static class BundledModule
{
    internal const string ResourceName = "Lyn.Loader.Bundled.lyn.dll";
    internal static string Root => Path.Combine(LoaderSettings.DirectoryPath, "modules");
    internal static bool IsBundledPath(string path)
    {
        if (string.IsNullOrWhiteSpace(path)) return false;
        return Path.GetFullPath(path).StartsWith(Path.GetFullPath(Root) + Path.DirectorySeparatorChar,
            StringComparison.OrdinalIgnoreCase);
    }

    // Versioned paths avoid overwriting a DLL which a running game still owns.
    // Preparing this file never loads it into the game.
    internal static string Prepare()
    {
        using var resource = Assembly.GetExecutingAssembly().GetManifestResourceStream(ResourceName);
        if (resource == null) return "";
        using var buffer = new MemoryStream();
        resource.CopyTo(buffer);
        byte[] bytes = buffer.ToArray();
        string hash = Convert.ToHexString(SHA256.HashData(bytes)).ToLowerInvariant();
        string directory = Path.Combine(Root, hash);
        string output = Path.Combine(directory, "lyn.dll");
        if (File.Exists(output) && Convert.ToHexString(SHA256.HashData(File.ReadAllBytes(output))).Equals(hash,
                StringComparison.OrdinalIgnoreCase)) return output;
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
