using System.Security.Cryptography;
using Lyn.Loader;

string temp = Path.Combine(Path.GetTempPath(), "lyn-bundle-test-" + Guid.NewGuid().ToString("N"));
LoaderSettings.DirectoryPath = temp;
int checks = 0;
void Check(bool condition, string name) { ++checks; if (!condition) throw new Exception(name); }
var created = new List<string>();
try
{
    string path = BundledModule.Prepare();created.Add(path);
    Check(File.Exists(path), "bundled module extracts without a game process");
    byte[] contents = File.ReadAllBytes(path);
    string hash = Convert.ToHexString(SHA256.HashData(contents)).ToLowerInvariant();
    Check(Path.GetFileName(Path.GetDirectoryName(path)) == hash, "version directory matches actual DLL hash");
    Check(BundledModule.IsBundledPath(path) && !BundledModule.IsBundledPath(temp + "-other/lyn.dll"), "bundled ownership is path bounded");
    DateTime stamp = File.GetLastWriteTimeUtc(path);
    Check(BundledModule.Prepare() == path && File.GetLastWriteTimeUtc(path) == stamp, "matching extracted module is reused without rewriting");
    File.WriteAllText(path, "damaged");
    Check(BundledModule.Prepare() == path && File.ReadAllBytes(path).SequenceEqual(contents), "damaged module is replaced by embedded bytes");
    Check(Directory.GetFiles(Path.GetDirectoryName(path)!).Length == 1, "extraction leaves no temporary file");
    Console.WriteLine($"{checks} loader bundle checks passed; dll_sha256={hash}");
}
finally
{
    foreach (string path in created) if (File.Exists(path)) File.Delete(path);
    // Remove only the known empty directories created by this test.
    if (Directory.Exists(BundledModule.Root))
    {
        foreach (string directory in Directory.GetDirectories(BundledModule.Root)) Directory.Delete(directory, false);
        Directory.Delete(BundledModule.Root, false);
    }
    if (Directory.Exists(temp)) Directory.Delete(temp, false);
}
namespace Lyn.Loader { internal static class LoaderSettings { internal static string DirectoryPath { get; set; } = ""; } }
