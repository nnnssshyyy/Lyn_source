param(
    [string]$OutputPath = (Join-Path $PSScriptRoot 'Assets\vs3.ico'),
    [string]$SourcePath = (Join-Path $PSScriptRoot 'Assets\loader-icon.png')
)
$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.Drawing
$sourceImage = [Drawing.Image]::FromFile((Resolve-Path -LiteralPath $SourcePath).Path)
$frames = @()
try {
    foreach ($size in @(16,20,24,32,40,48,64,128,256)) {
        $bitmap = New-Object Drawing.Bitmap($size,$size,[Drawing.Imaging.PixelFormat]::Format32bppArgb)
        $graphics = [Drawing.Graphics]::FromImage($bitmap)
        $stream = New-Object IO.MemoryStream
        try {
            $graphics.Clear([Drawing.Color]::Transparent)
            $graphics.InterpolationMode = 'HighQualityBicubic'
            $graphics.PixelOffsetMode = 'HighQuality'
            $graphics.CompositingQuality = 'HighQuality'
            # Fit the full portrait inside the square icon, preserving its proportions.
            $scale = [Math]::Min($size / [double]$sourceImage.Width, $size / [double]$sourceImage.Height)
            $width = [Math]::Max(1,[int][Math]::Round($sourceImage.Width * $scale))
            $height = [Math]::Max(1,[int][Math]::Round($sourceImage.Height * $scale))
            $left = [int][Math]::Floor(($size-$width)/2.0)
            $top = [int][Math]::Floor(($size-$height)/2.0)
            $graphics.DrawImage($sourceImage,$left,$top,$width,$height)
            $bitmap.Save($stream,[Drawing.Imaging.ImageFormat]::Png)
            $frames += ,@{Size=$size;Bytes=$stream.ToArray()}
        } finally { $stream.Dispose();$graphics.Dispose();$bitmap.Dispose() }
    }
} finally { $sourceImage.Dispose() }
$absoluteOutput = [IO.Path]::GetFullPath($OutputPath)
[IO.Directory]::CreateDirectory([IO.Path]::GetDirectoryName($absoluteOutput)) | Out-Null
$file = [IO.File]::Create($absoluteOutput)
$writer = New-Object IO.BinaryWriter($file)
try {
    $writer.Write([uint16]0);$writer.Write([uint16]1);$writer.Write([uint16]$frames.Count)
    $offset = 6+16*$frames.Count
    foreach ($frame in $frames) {
        $dimension = if ($frame.Size -eq 256) {0} else {$frame.Size}
        $writer.Write([byte]$dimension);$writer.Write([byte]$dimension);$writer.Write([byte]0);$writer.Write([byte]0)
        $writer.Write([uint16]1);$writer.Write([uint16]32);$writer.Write([uint32]$frame.Bytes.Length);$writer.Write([uint32]$offset)
        $offset += $frame.Bytes.Length
    }
    foreach ($frame in $frames) {$writer.Write([byte[]]$frame.Bytes)}
} finally {$writer.Dispose()}
Write-Output "Icono del loader: $absoluteOutput"
