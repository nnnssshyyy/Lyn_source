param(
    [ValidateRange(1, 65535)][int]$Port = 8745,
    [string]$Python = (Join-Path $env:USERPROFILE '.codex\tools\ida-pro-mcp-venv\Scripts\python.exe'),
    [string]$StateDirectory = (Join-Path $env:USERPROFILE '.codex\tools\ida-pro-mcp-state')
)

$ErrorActionPreference = 'Stop'
$endpoint = "http://127.0.0.1:$Port/mcp"
$probe = @{
    jsonrpc = '2.0'; id = 1; method = 'initialize'
    params = @{
        protocolVersion = '2025-06-18'; capabilities = @{}
        clientInfo = @{ name = 'local-ida-launcher'; version = '1.0' }
    }
} | ConvertTo-Json -Depth 5 -Compress

function Test-IdaMcp {
    try {
        $response = Invoke-RestMethod -Uri $endpoint -Method Post -ContentType 'application/json' -Body $probe -TimeoutSec 2
        return $response.result.serverInfo.name -eq 'ida-pro-mcp'
    } catch {
        return $false
    }
}

if (Test-IdaMcp) {
    Write-Host "IDA MCP ya esta activo: $endpoint"
    return
}
if (-not (Test-Path -LiteralPath $Python -PathType Leaf)) {
    throw "No existe el Python de la instalacion: $Python"
}
New-Item -ItemType Directory -Path $StateDirectory -Force | Out-Null
$server = Start-Process -FilePath $Python -ArgumentList @(
    '-m', 'ida_pro_mcp.idalib_supervisor', '--host', '127.0.0.1', '--port', "$Port"
) -WindowStyle Hidden -RedirectStandardOutput (Join-Path $StateDirectory 'server.stdout.log') `
  -RedirectStandardError (Join-Path $StateDirectory 'server.stderr.log') -PassThru
$server.Id | Set-Content -LiteralPath (Join-Path $StateDirectory 'server.pid') -Encoding ascii
for ($attempt = 0; $attempt -lt 20; $attempt++) {
    if (Test-IdaMcp) {
        Write-Host "IDA MCP iniciado: $endpoint"
        return
    }
    Start-Sleep -Milliseconds 300
}
throw "El servidor no respondio. Revisa los logs de $StateDirectory"
