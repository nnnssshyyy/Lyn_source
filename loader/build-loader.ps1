param(
    [ValidateSet('Release', 'Debug')][string]$Configuration = 'Release',
    [string]$DllPath = '',
    [switch]$NoPackageCompression
)
$ErrorActionPreference = 'Stop'
$workspacePath = Split-Path -Parent $PSScriptRoot
$projectPath = Join-Path $PSScriptRoot 'Lyn.Loader.csproj'
$outputPath = Join-Path $workspacePath 'build\loader'
$payloadPath = Join-Path $workspacePath 'build\loader-payload'
# Doble barra final: PowerShell 5.1 la reduce a una sola al invocar dotnet (ruta con espacios).
$binaryBuildPath = (Join-Path $workspacePath 'build\loader-bin') + '\\'
if ([string]::IsNullOrWhiteSpace($DllPath)) {
    $DllPath = Join-Path $workspacePath 'build\Release\lyn.dll'
}
if (-not (Test-Path -LiteralPath $DllPath -PathType Leaf)) {
    throw "No existe la DLL: $DllPath. Compila primero lyn o usa -DllPath."
}
$sourceDllPath = (Resolve-Path -LiteralPath $DllPath).Path
$dotnetCommand = Get-Command dotnet -ErrorAction Stop
$iconPath = Join-Path $PSScriptRoot 'Assets\vs3.ico'
$iconSourcePath = Join-Path $PSScriptRoot 'Assets\loader-icon.png'
if (-not (Test-Path -LiteralPath $iconPath) -or
    (Get-Item -LiteralPath $iconSourcePath).LastWriteTimeUtc -gt (Get-Item -LiteralPath $iconPath).LastWriteTimeUtc) {
    & (Join-Path $PSScriptRoot 'create-icon.ps1')
}
if ($NoPackageCompression) { $publishPath = $outputPath; $innerCompression = 'true' }
else { $publishPath = $payloadPath; $innerCompression = 'false' }
& $dotnetCommand.Source publish $projectPath --configuration $Configuration --runtime win-x86 --self-contained true --output $publishPath "-p:BaseOutputPath=$binaryBuildPath" "-p:EnableCompressionInSingleFile=$innerCompression" -p:DebugType=None -p:DebugSymbols=false
if ($LASTEXITCODE -ne 0) { throw "La publicación falló (código $LASTEXITCODE)." }
if (-not $NoPackageCompression) {
    & (Join-Path $PSScriptRoot 'pack-loader.ps1') -InputPath (Join-Path $payloadPath 'Lyn.Loader.exe') -OutputPath (Join-Path $outputPath 'Lyn.Loader.exe')
}
$packagedDllPath = Join-Path $outputPath 'lyn.dll'
if ($sourceDllPath -ne $packagedDllPath) {
    Copy-Item -LiteralPath $sourceDllPath -Destination $packagedDllPath -Force
}
Copy-Item -LiteralPath (Join-Path $PSScriptRoot 'README.md') -Destination (Join-Path $outputPath 'LEEME.md') -Force
Write-Output "Loader: $(Join-Path $outputPath 'Lyn.Loader.exe')"
Write-Output "Módulo: $(Join-Path $outputPath 'lyn.dll')"
