using System.Diagnostics;
using System.IO.Pipes;
using Lyn.Loader;
int checks=0;
void Check(bool value,string label){checks++;if(!value)throw new Exception(label);}
var parsed=LicenseLeaseHost.ParseHookRequest("VS3Q|2|"+new string('a',32)+"|1\n00100000,00200000,00100000,9090909090");
Check(parsed.Rows.Length==1 && parsed.Rows[0].Target==0x100000,"wire request preserves UInt32 addresses");
foreach(string invalid in new[]{"", "VS3Q|2|short|1\n00100000,00200000,00100000,9090909090", "VS3Q|2|"+new string('a',32)+"|1\n00100000,00200000,00100001,9090909090"}) {
    bool rejected=false;try{LicenseLeaseHost.ParseHookRequest(invalid);}catch(InvalidDataException){rejected=true;}Check(rejected,"invalid hook request rejected");
}
using var native=Process.Start(new ProcessStartInfo(Path.GetFullPath("build-current/Release/remote_hooks_tests.exe"),"--transport"){UseShellExecute=false,CreateNoWindow=true,RedirectStandardInput=true,RedirectStandardOutput=true})!;
string[] identity=(await native.StandardOutput.ReadLineAsync())!.Split(' ');var game=new GameTarget(int.Parse(identity[0]),long.Parse(identity[1]),"");
using var client=new LicenseClient();using var host=new LicenseLeaseHost(client,game,new LeaseTicket("probe","signature",long.MaxValue,long.MaxValue,"probe"),_=>{});host.Start();
// Simulate a client that vanishes halfway through a framed request.
await using(var abandoned=new NamedPipeClientStream(".",$"VS3.{game.Pid}.{game.Started}",PipeDirection.InOut,PipeOptions.Asynchronous)) {
    await abandoned.ConnectAsync(2000);await abandoned.WriteAsync(new byte[]{5,0});
}
await native.StandardInput.WriteLineAsync("go");
Check(await native.StandardOutput.ReadLineAsync().WaitAsync(TimeSpan.FromSeconds(8))=="first ok","native duplex request survives a previous early disconnect");
// Another early disconnect after requesting a response must not stop serving.
await using(var abandoned=new NamedPipeClientStream(".",$"VS3.{game.Pid}.{game.Started}",PipeDirection.InOut,PipeOptions.Asynchronous)) {
    await abandoned.ConnectAsync(2000);await abandoned.WriteAsync(new byte[]{5,0,0,0,76,69,65,83,69});
}
await native.StandardInput.WriteLineAsync("go");
Check(await native.StandardOutput.ReadLineAsync().WaitAsync(TimeSpan.FromSeconds(8))=="second ok","response abandonment preserves the next ACK exchange");
await native.WaitForExitAsync().WaitAsync(TimeSpan.FromSeconds(5));Check(native.ExitCode==0,"native probe completed without a game");
Console.WriteLine($"{checks} loader request / native pipe checks passed.");
namespace Lyn.Loader {internal sealed record GameTarget(int Pid,long Started,string Path);}
