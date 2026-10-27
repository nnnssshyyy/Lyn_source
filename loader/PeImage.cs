using System.IO;
using System.Reflection.PortableExecutable;

namespace Lyn.Loader;

internal sealed record DllInfo(string FullPath, long Length, DateTime Modified)
{
    public string Name => Path.GetFileName(FullPath);
    public string Description => $"DLL nativa · x86 · {Length / 1048576d:0.00} MB";
}

internal static class PeImage
{
    public static DllInfo Inspect(string path)
    {
        string fullPath = Path.GetFullPath(path);
        if (!string.Equals(Path.GetExtension(fullPath), ".dll", StringComparison.OrdinalIgnoreCase))
            throw new InvalidDataException("Selecciona un archivo .dll.");
        if (fullPath.Length >= 260)
            throw new InvalidDataException("Mueve la DLL a una ruta de menos de 260 caracteres para poder confirmar su carga.");
        using var stream = new FileStream(fullPath, FileMode.Open, FileAccess.Read, FileShare.Read);
        Inspect(stream);
        return new(fullPath, stream.Length, File.GetLastWriteTime(fullPath));
    }

    public static void Inspect(Stream stream)
    {
        using var pe = new PEReader(stream, PEStreamOptions.LeaveOpen);
        var headers = pe.PEHeaders;
        if (headers.CoffHeader.Machine != Machine.I386 || headers.PEHeader?.Magic != PEMagic.PE32)
            throw new InvalidDataException("L4D2 requiere una DLL Win32 (x86). Este archivo tiene otra arquitectura.");
        if ((headers.CoffHeader.Characteristics & Characteristics.Dll) == 0 || headers.CorHeader != null)
            throw new InvalidDataException("Selecciona la DLL nativa del proyecto; un ejecutable o una biblioteca .NET no son compatibles.");
        stream.Position = 0;
    }
}
