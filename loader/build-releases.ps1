param([ValidateSet('Release','Debug')][string]$Configuration='Release')
$ErrorActionPreference='Stop'
& (Join-Path $PSScriptRoot 'build-loader.ps1') -Configuration $Configuration
if($LASTEXITCODE -ne 0){throw 'Falló la compilación completa.'}
$workspacePath=Split-Path -Parent $PSScriptRoot
$compactPath=Join-Path $workspacePath 'build\loader-compact'
& dotnet publish (Join-Path $PSScriptRoot 'Lyn.Loader.csproj') -c $Configuration -r win-x86 --self-contained false -p:EnableCompressionInSingleFile=false -p:DebugType=None -p:DebugSymbols=false -o $compactPath
if($LASTEXITCODE -ne 0){throw 'Falló la compilación compacta.'}
Write-Output "Compacto (.NET Desktop Runtime 8 x86): $(Join-Path $compactPath 'Lyn.Loader.exe')"
