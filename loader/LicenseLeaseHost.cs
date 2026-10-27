using System.IO;
using System.IO.Pipes;
using System.Net.Http;
using System.Text;
using System.Globalization;

namespace Lyn.Loader;
internal sealed class LicenseLeaseHost : IDisposable
{
    private readonly CancellationTokenSource lifetime = new();
    private readonly LicenseClient client;
    private readonly GameTarget game;
    private readonly Action<string> report;
    private LeaseTicket ticket;
    private Task? pipeTask, renewTask;
    private volatile bool armed;
    internal LicenseLeaseHost(LicenseClient client, GameTarget game, LeaseTicket ticket, Action<string> report)
    { this.client = client; this.game = game; this.ticket = ticket; this.report = report; }
    internal void Start() { pipeTask = Serve(lifetime.Token); renewTask = Renew(lifetime.Token); }
    internal bool IsFor(GameTarget? target) => !lifetime.IsCancellationRequested && target?.Pid == game.Pid && target.Started == game.Started;
    // El módulo avisa cuando el plan firmado quedó aplicado; el loader ya puede cerrarse.
    internal bool Armed => armed;
    private async Task Serve(CancellationToken cancellation)
    {
        string name = $"VS3.{game.Pid}.{game.Started}";
        while (!cancellation.IsCancellationRequested)
        {
            try
            {
                using var pipe = new NamedPipeServerStream(name, PipeDirection.InOut, 1, PipeTransmissionMode.Byte,
                    PipeOptions.Asynchronous | PipeOptions.CurrentUserOnly);
                await pipe.WaitForConnectionAsync(cancellation);
                using var timeout=CancellationTokenSource.CreateLinkedTokenSource(cancellation);timeout.CancelAfter(TimeSpan.FromSeconds(4));
                byte[] length=new byte[4];await pipe.ReadExactlyAsync(length,timeout.Token);
                int size=BitConverter.ToInt32(length);if(size is <1 or >32768)continue;
                byte[] request=new byte[size];await pipe.ReadExactlyAsync(request,timeout.Token);
                string text=Encoding.UTF8.GetString(request),wire;
                bool ready=false;
                if(text=="LEASE") {var current=Volatile.Read(ref ticket);wire=current.Payload+"\n"+current.Signature;}
                else if(text=="VS3R") {wire="OK";ready=true;}
                else {
                    var parsed=ParseHookRequest(text);
                    wire=await client.Hooks(game,parsed.Nonce,parsed.Rows,timeout.Token);
                }
                byte[] bytes=Encoding.UTF8.GetBytes(wire);if(bytes.Length>32768)throw new InvalidDataException("Plan demasiado grande.");
                await pipe.WriteAsync(BitConverter.GetBytes(bytes.Length),timeout.Token);
                await pipe.WriteAsync(bytes,timeout.Token);
                await pipe.FlushAsync(timeout.Token);
                byte[] ack=new byte[1];await pipe.ReadExactlyAsync(ack,timeout.Token);
                if(ready)armed=true;
            }
            catch(OperationCanceledException) {if(cancellation.IsCancellationRequested)return;}
            catch(Exception error) when(error is IOException or InvalidDataException or InvalidOperationException or HttpRequestException or FormatException or OverflowException) {
                // An abandoned connection must not kill the server or its
                // independent renewal task. The native worker retries safely.
                report("Se reintentará la comunicación con el módulo.");
            }
            catch(UnauthorizedAccessException) {lifetime.Cancel();report("No se pudo abrir la sesión local del módulo.");return;}
        }
    }
    internal static (string Nonce,HookRow[] Rows) ParseHookRequest(string text)
    {
        string[] lines=text.Split('\n'),header=lines[0].Split('|');
        if(header.Length!=4 || header[0]!="VS3Q" || header[1]!="2" || !System.Text.RegularExpressions.Regex.IsMatch(header[2],"^[a-f0-9]{32}$") || !int.TryParse(header[3],out int count) || count is <1 or >128 || lines.Length!=count+1)throw new InvalidDataException("Petición no válida.");
        var rows=new HookRow[count];var seen=new HashSet<uint>();
        for(int i=0;i<count;i++) {
            string[] f=lines[i+1].Split(',');
            if(f.Length!=4 || f.Take(3).Any(v=>!System.Text.RegularExpressions.Regex.IsMatch(v,"^[a-f0-9]{8}$")) || !System.Text.RegularExpressions.Regex.IsMatch(f[3],"^(?:[a-f0-9]{10}|[a-f0-9]{14})$"))throw new InvalidDataException("Hook no válido.");
            uint target=uint.Parse(f[0],NumberStyles.HexNumber,CultureInfo.InvariantCulture),detour=uint.Parse(f[1],NumberStyles.HexNumber,CultureInfo.InvariantCulture),site=uint.Parse(f[2],NumberStyles.HexNumber,CultureInfo.InvariantCulture);
            if(target<65536 || detour<65536 || site<65536 || !seen.Add(target) || site!=target-(f[3].Length==14?5u:0u))throw new InvalidDataException("Dirección no válida.");
            rows[i]=new(target,detour,site,f[3]);
        }
        return(header[2],rows);
    }
    private async Task Renew(CancellationToken cancellation)
    {
        while (!cancellation.IsCancellationRequested)
        {
            try
            {
                await Task.Delay(TimeSpan.FromSeconds(30), cancellation);
                using var process = Native.OpenProcess(Native.QueryLimited, false, game.Pid);
                if (process.IsInvalid || Native.Started(process) != game.Started) { lifetime.Cancel(); return; }
                Volatile.Write(ref ticket, await client.Lease(game, cancellation));
            }
            catch (OperationCanceledException) { return; }
            catch (Exception error) when (error is HttpRequestException or InvalidOperationException or IOException or System.ComponentModel.Win32Exception)
            { report("No se renovó la licencia. Las funciones se pausarán si la sesión caduca."); }
        }
    }
    public void Dispose() { lifetime.Cancel(); }
}
