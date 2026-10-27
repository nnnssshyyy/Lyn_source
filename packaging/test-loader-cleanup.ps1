param([ValidateSet('package','compact')][string]$Edition='package',[switch]$ExitPath)
$ErrorActionPreference = 'Stop'
$workspacePath = Split-Path -Parent $PSScriptRoot
$qaRoot = [IO.Path]::GetFullPath((Join-Path $workspacePath ('build\loader-cleanup-integration\'+[guid]::NewGuid().ToString('N'))))
New-Item -ItemType Directory -Path $qaRoot | Out-Null
$qaExe = Join-Path $qaRoot "VS3 ' & ñ.exe"
$qaReport = Join-Path $qaRoot 'result.json'
$probePath=if($Edition -eq 'compact'){'build\loader-cleanup-probe-compact\Lyn.Loader.exe'}else{'build\loader-cleanup-probe\VS3.Probe.exe'}
Copy-Item -LiteralPath (Join-Path $workspacePath $probePath) -Destination $qaExe
$qaInfo = New-Object Diagnostics.ProcessStartInfo
$qaInfo.FileName = $qaExe
$qaInfo.UseShellExecute = $false
$qaInfo.CreateNoWindow = $true
$qaInfo.WindowStyle = 'Hidden'
$qaInfo.EnvironmentVariables['VS3_CLEANUP_QA_REPORT'] = $qaReport
if($ExitPath){$qaInfo.EnvironmentVariables['VS3_CLEANUP_QA_SYNC']='1'}
$qaProcess = [Diagnostics.Process]::Start($qaInfo)
if (-not $qaProcess.WaitForExit(45000)) { throw 'El probe no terminó.' }
$qaExit = $qaProcess.ExitCode
$qaProcess.Dispose()
for ($attempt=0; $attempt -lt 100 -and (Test-Path -LiteralPath $qaExe); $attempt++) { Start-Sleep -Milliseconds 100 }
if (-not (Test-Path -LiteralPath $qaReport)) { throw 'No se generó el informe.' }
$qaData = Get-Content -LiteralPath $qaReport -Raw | ConvertFrom-Json
$qaResult = [ordered]@{
    exitCode=$qaExit
    scheduled=$qaData.scheduled
    duplicateBlocked=$qaData.duplicateBlocked
    moduleLoaded=$qaData.moduleLoaded
    exitPath=$qaData.exitPath
    edition=$Edition
    correctPackageOwner=($qaData.owner -eq $qaExe)
    downloadDeleted=(-not (Test-Path -LiteralPath $qaExe))
    payloadDeleted=(-not (Test-Path -LiteralPath $qaData.payload))
    tempFolderDeleted=(-not (Test-Path -LiteralPath (Split-Path -Parent $qaData.payload)))
    report=$qaReport
}
$qaResult | ConvertTo-Json | Set-Content -LiteralPath (Join-Path $qaRoot 'validation.json') -Encoding utf8
$qaResult | ConvertTo-Json
if ($qaExit -ne 0 -or -not $qaResult.scheduled -or -not $qaResult.duplicateBlocked -or $qaResult.moduleLoaded -or -not $qaResult.correctPackageOwner -or
    -not $qaResult.downloadDeleted -or -not $qaResult.payloadDeleted -or ($Edition -eq 'package' -and -not $qaResult.tempFolderDeleted)) {
    throw 'La limpieza del paquete no pasó la prueba completa.'
}
