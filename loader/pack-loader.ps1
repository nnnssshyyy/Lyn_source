param(
    [Parameter(Mandatory = $true)][string]$InputPath,
    [Parameter(Mandatory = $true)][string]$OutputPath,
    [string]$SdkPath = ''
)
$ErrorActionPreference = 'Stop'
$workspacePath = Split-Path -Parent $PSScriptRoot
$InputPath = (Resolve-Path -LiteralPath $InputPath).Path
$OutputPath = [IO.Path]::GetFullPath($OutputPath)
if ($InputPath -eq $OutputPath) { throw 'El original y el paquete deben tener rutas diferentes.' }
if ([string]::IsNullOrWhiteSpace($SdkPath)) { $SdkPath = Join-Path $workspacePath 'build\tools\lzma-sdk-26.04' }
$SdkPath = [IO.Path]::GetFullPath($SdkPath)

# Official LZMA SDK release. Verify the release digest before using its tools.
$sdkHash = 'bd98058d0e12c5da970dd4e5e1d8fa24f2d704914d96e69c28f92ed45421a439'
$archiverHash = '256feca8e274e5da655e2a284fabafd9f554365eb164862089dacd4e8276d282'
$sfxHash = '837a2d84a4d15366af7a91670908aa29a958b721f40db46da734621f6234fea2'
$archiver = Join-Path $SdkPath 'bin\7zr.exe'
$sfx = Join-Path $SdkPath 'bin\7zSD.sfx'
if (-not (Test-Path -LiteralPath $archiver) -or -not (Test-Path -LiteralPath $sfx)) {
    New-Item -ItemType Directory -Path $SdkPath -Force | Out-Null
    $sdkArchive = Join-Path $SdkPath 'lzma2604.7z'
    Invoke-WebRequest -Uri 'https://github.com/ip7z/7zip/releases/download/26.04/lzma2604.7z' -OutFile $sdkArchive
    if ((Get-FileHash -LiteralPath $sdkArchive -Algorithm SHA256).Hash -ne $sdkHash) { throw 'El SDK LZMA no coincide con la versión oficial.' }
    & tar -xf $sdkArchive -C $SdkPath
    if ($LASTEXITCODE -ne 0) { throw 'No se pudo extraer el SDK LZMA.' }
}
if ((Get-FileHash -LiteralPath $archiver -Algorithm SHA256).Hash -ne $archiverHash -or
    (Get-FileHash -LiteralPath $sfx -Algorithm SHA256).Hash -ne $sfxHash) { throw 'Las herramientas de compresión fueron modificadas.' }

$workPath = Join-Path $workspacePath ('build\loader-pack\' + [guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $workPath -Force | Out-Null
$payloadCopy = Join-Path $workPath 'Lyn.Loader.exe'
Copy-Item -LiteralPath $InputPath -Destination $payloadCopy
$archivePath = Join-Path $workPath 'payload.7z'
Push-Location $workPath
try {
    & $archiver a $archivePath 'Lyn.Loader.exe' -t7z -m0=LZMA2 -mx=9 -md=128m -ms=on -mmt=2 -bsp0
    if ($LASTEXITCODE -ne 0) { throw 'Falló la compresión LZMA2.' }
} finally { Pop-Location }
& $archiver t $archivePath -bsp0
if ($LASTEXITCODE -ne 0) { throw 'El archivo comprimido no pasó la comprobación de integridad.' }

$verifyPath = Join-Path $workPath 'verified'
& $archiver x $archivePath "-o$verifyPath" -bsp0
if ($LASTEXITCODE -ne 0) { throw 'No se pudo verificar la extracción del paquete.' }
$inputHash = (Get-FileHash -LiteralPath $InputPath -Algorithm SHA256).Hash.ToLowerInvariant()
if ((Get-FileHash -LiteralPath (Join-Path $verifyPath 'Lyn.Loader.exe') -Algorithm SHA256).Hash -ne $inputHash) {
    throw 'La descompresión no recuperó el ejecutable original.'
}

$stub = Join-Path $workPath 'package.sfx'
Copy-Item -LiteralPath $sfx -Destination $stub
# Preserve the loader version and asInvoker manifest in the outer executable.
# Only the official SFX version/manifest resources are replaced, before appending data.
if (-not ('Lyn.PackageResources' -as [type])) {
    Add-Type -TypeDefinition @'
using System;
using System.ComponentModel;
using System.IO;
using System.Runtime.InteropServices;
namespace Lyn {
    public static class PackageResources {
        [DllImport("kernel32", CharSet=CharSet.Unicode, SetLastError=true)] static extern IntPtr LoadLibraryEx(string path, IntPtr file, uint flags);
        [DllImport("kernel32", SetLastError=true)] static extern bool FreeLibrary(IntPtr module);
        [DllImport("kernel32", CharSet=CharSet.Unicode, SetLastError=true)] static extern IntPtr FindResource(IntPtr module, IntPtr name, IntPtr type);
        [DllImport("kernel32", SetLastError=true)] static extern uint SizeofResource(IntPtr module, IntPtr resource);
        [DllImport("kernel32", SetLastError=true)] static extern IntPtr LoadResource(IntPtr module, IntPtr resource);
        [DllImport("kernel32", SetLastError=true)] static extern IntPtr LockResource(IntPtr resource);
        [DllImport("kernel32", CharSet=CharSet.Unicode, SetLastError=true)] static extern IntPtr BeginUpdateResource(string path, bool deleteExisting);
        [DllImport("kernel32", CharSet=CharSet.Unicode, SetLastError=true)] static extern bool UpdateResource(IntPtr update, IntPtr type, IntPtr name, ushort language, byte[] data, uint size);
        [DllImport("kernel32", SetLastError=true)] static extern bool EndUpdateResource(IntPtr update, bool discard);
        static byte[] Read(IntPtr module, int type) {
            var resource = FindResource(module, (IntPtr)1, (IntPtr)type);
            if (resource == IntPtr.Zero) throw new Win32Exception();
            var data = new byte[checked((int)SizeofResource(module, resource))];
            var pointer = LockResource(LoadResource(module, resource));
            if (pointer == IntPtr.Zero || data.Length == 0) throw new Win32Exception();
            Marshal.Copy(pointer, data, 0, data.Length);
            return data;
        }
        static void CopyIcon(IntPtr update, string path) {
            byte[] file = File.ReadAllBytes(path);
            using (var input = new BinaryReader(new MemoryStream(file)))
            using (var group = new MemoryStream())
            using (var output = new BinaryWriter(group)) {
                if (input.ReadUInt16() != 0 || input.ReadUInt16() != 1) throw new InvalidDataException("ICO no valido.");
                ushort count = input.ReadUInt16();
                if (count < 1 || count > 16) throw new InvalidDataException("ICO no valido.");
                output.Write((ushort)0); output.Write((ushort)1); output.Write(count);
                for (ushort i = 0; i < count; i++) {
                    byte[] dimensions = input.ReadBytes(8);
                    uint size = input.ReadUInt32(), offset = input.ReadUInt32();
                    if (offset > file.Length || size > file.Length - offset) throw new InvalidDataException("ICO incompleto.");
                    byte[] icon = new byte[size]; Array.Copy(file, offset, icon, 0, size);
                    foreach (ushort language in new ushort[] { 0, 0x409 })
                        if (!UpdateResource(update, (IntPtr)3, (IntPtr)(i+1), language, icon, size)) throw new Win32Exception();
                    output.Write(dimensions); output.Write(size); output.Write((ushort)(i+1));
                }
                byte[] bytes = group.ToArray();
                foreach (ushort language in new ushort[] { 0, 0x409 })
                    if (!UpdateResource(update, (IntPtr)14, (IntPtr)1, language, bytes, (uint)bytes.Length)) throw new Win32Exception();
            }
        }
        public static void Copy(string source, string destination, string iconPath) {
            var module = LoadLibraryEx(source, IntPtr.Zero, 0x22); // data/image resources; never execute payload
            if (module == IntPtr.Zero) throw new Win32Exception();
            byte[] version, manifest;
            try { version = Read(module, 16); manifest = Read(module, 24); }
            finally { FreeLibrary(module); }
            var update = BeginUpdateResource(destination, false);
            if (update == IntPtr.Zero) throw new Win32Exception();
            bool commit = false;
            try {
                // The SDK SFX version resource uses English (0x409).
                if (!UpdateResource(update, (IntPtr)16, (IntPtr)1, 0x409, version, (uint)version.Length) ||
                    !UpdateResource(update, (IntPtr)24, (IntPtr)1, 0x409, manifest, (uint)manifest.Length)) throw new Win32Exception();
                CopyIcon(update, iconPath);
                commit = true;
            } finally { if (!EndUpdateResource(update, !commit) && commit) throw new Win32Exception(); }
        }
    }
}
'@
}
[Lyn.PackageResources]::Copy($InputPath, $stub, (Join-Path $PSScriptRoot 'Assets\vs3.ico'))
$config = [Text.Encoding]::UTF8.GetBytes(";!@Install@!UTF-8!`nTitle=`"VS 3`"`nProgress=`"no`"`nRunProgram=`"Lyn.Loader.exe`"`n;!@InstallEnd@!`n")
$candidate = Join-Path $workPath 'VS3.Loader.exe'
$stream = [IO.File]::Create($candidate)
try {
    $stubBytes = [IO.File]::ReadAllBytes($stub)
    $stream.Write($stubBytes, 0, $stubBytes.Length)
    $stream.Write($config, 0, $config.Length)
    $payload = [IO.File]::OpenRead($archivePath)
    try { $payload.CopyTo($stream) } finally { $payload.Dispose() }
} finally { $stream.Dispose() }
& $archiver t $candidate -bsp0
if ($LASTEXITCODE -ne 0) { throw 'El ejecutable final no pasó la comprobación de integridad.' }
New-Item -ItemType Directory -Path (Split-Path -Parent $OutputPath) -Force | Out-Null
Copy-Item -LiteralPath $candidate -Destination $OutputPath -Force
$info = [ordered]@{
    version = [Diagnostics.FileVersionInfo]::GetVersionInfo($InputPath).ProductVersion
    compression = 'LZMA2 / 7-Zip SFX 26.04'
    extractionProgress = $false
    applicationIcon = 'VS 3'
    deleteDownloadedExeOnExit = $true
    sdkSha256 = $sdkHash
    bytes = (Get-Item -LiteralPath $OutputPath).Length
    sha256 = (Get-FileHash -LiteralPath $OutputPath -Algorithm SHA256).Hash.ToLowerInvariant()
    payloadBytes = (Get-Item -LiteralPath $InputPath).Length
    payloadSha256 = $inputHash
    roundtripVerified = $true
    builtAt = [DateTimeOffset]::UtcNow.ToString('o')
}
$info | ConvertTo-Json | Set-Content -LiteralPath (Join-Path (Split-Path -Parent $OutputPath) 'loader-build-info.json') -Encoding UTF8
Write-Output "Paquete comprimido: $OutputPath ($($info.bytes) bytes)"
