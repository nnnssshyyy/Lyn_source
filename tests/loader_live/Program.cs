using System.Diagnostics;
using System.Security.Cryptography;
using System.Text.Json;
using Lyn.Loader;
Console.WriteLine("Ready for ephemeral QA credentials on stdin.");
var config=JsonSerializer.Deserialize<Config>((await Console.In.ReadLineAsync())!,new JsonSerializerOptions(JsonSerializerDefaults.Web))!;
using var native=Process.Start(new ProcessStartInfo(Path.GetFullPath("build-current/Release/live_hook_probe.exe")){UseShellExecute=false,CreateNoWindow=true,RedirectStandardInput=true,RedirectStandardOutput=true})!;
try {
 string[] identity=(await native.StandardOutput.ReadLineAsync())!.Split(' ');var game=new GameTarget(int.Parse(identity[0]),long.Parse(identity[1]),"");
 using var client=new LicenseClient();using var timeout=new CancellationTokenSource(TimeSpan.FromSeconds(90));
 await client.Login(config.Username,config.Password,timeout.Token);if(!client.Licensed)throw new Exception("QA license missing");
 string path=await client.PrepareModule(game,timeout.Token);
 await using(var file=File.OpenRead(path)){if(Convert.ToHexString(await SHA256.HashDataAsync(file,timeout.Token)).ToLowerInvariant()!=config.Sha256)throw new Exception("Module mismatch");}
 if(await client.PrepareModule(game,timeout.Token)!=path)throw new Exception("Verified cache not reused");
 var ticket=await client.Lease(game,timeout.Token);
 using var host=new LicenseLeaseHost(client,game,ticket,_=>{});host.Start();await native.StandardInput.WriteLineAsync("go");
 string? result=await native.StandardOutput.ReadLineAsync().WaitAsync(TimeSpan.FromSeconds(30));
 await native.WaitForExitAsync().WaitAsync(TimeSpan.FromSeconds(10));if(native.ExitCode!=0 || result?.StartsWith("PASS:")!=true)throw new Exception("Native live probe failed: "+native.ExitCode);
 Console.WriteLine("PASS: signed module download/cache, authenticated hook plan, native patch, trampoline and restore.");await client.Logout();
}finally{if(!native.HasExited)native.Kill();}
internal sealed record Config(string Username,string Password,string Sha256);
namespace Lyn.Loader {internal sealed record GameTarget(int Pid,long Started,string Path);}
