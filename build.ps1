param([ValidateSet('Release','Debug')][string]$Configuration = 'Release')
$ErrorActionPreference = 'Stop'
Push-Location $PSScriptRoot
try {
    $cmakeTool = Get-Command cmake.exe -ErrorAction SilentlyContinue
    $cmakePath = if ($cmakeTool) { $cmakeTool.Source } else { $null }
    if (!$cmakePath) {
        $bundledCMake = Get-ChildItem -Path "$env:ProgramFiles\Microsoft Visual Studio\*\*\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($bundledCMake) { $cmakePath = $bundledCMake.FullName }
    }
    if (!$cmakePath) { throw 'No se encontro CMake. Instala las herramientas C++ de Visual Studio o agrega CMake al PATH.' }
    $configureArgs = @('-S', '.', '-B', 'build', '-A', 'Win32')
    if (Test-Path -LiteralPath 'build/CMakeCache.txt') {
        $cachedRoot = Select-String -LiteralPath 'build/CMakeCache.txt' -Pattern '^CMAKE_HOME_DIRECTORY:INTERNAL=(.*)$'
        if ($cachedRoot -and $cachedRoot.Matches[0].Groups[1].Value.Replace('/', '\') -ne $PSScriptRoot) {
            $configureArgs += '--fresh'
        }
    }
    & $cmakePath @configureArgs
    if ($LASTEXITCODE -ne 0) { throw 'CMake no pudo configurar el proyecto.' }
    $buildArgs = @('--build', 'build', '--config', $Configuration, '--target', 'lyn', '--parallel')
    # Copied builds or a clock correction can leave object files newer than
    # edited sources, causing MSBuild to silently reuse an older implementation.
    $futureObjects = @(Get-ChildItem -LiteralPath "build/lyn.dir/$Configuration" -Filter '*.obj' -File -ErrorAction SilentlyContinue |
        Where-Object { $_.LastWriteTimeUtc -gt [DateTime]::UtcNow.AddMinutes(1) })
    if ($futureObjects.Count -gt 0) { $buildArgs += '--clean-first' }
    & $cmakePath @buildArgs
    if ($LASTEXITCODE -ne 0) { throw 'La compilacion fallo.' }
    Write-Output (Join-Path $PSScriptRoot "build/$Configuration/lyn.dll")
} finally {
    Pop-Location
}
