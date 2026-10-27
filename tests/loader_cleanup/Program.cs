using System.Diagnostics;
using System.Security.Cryptography;
using Lyn.Loader;

int checks=0, failed=0;
void Check(bool ok,string label) {++checks;if(!ok){++failed;Console.WriteLine("FAIL: "+label);}}
Check(!LoaderCleanup.ShouldSchedule(false,false),"running without an exit request does not schedule deletion");
Check(!LoaderCleanup.ShouldSchedule(true,true),"development builds are preserved");
Check(LoaderCleanup.ShouldSchedule(true,false),"production exit schedules cleanup without needing module activation");
LoaderCleanup.Capture(Console.WriteLine);
Check(LoaderCleanup.CapturedPath is null,"shared runtime and test hosts are never chosen for deletion");
Check(LoaderCleanup.IsLoaderExecutable(Path.GetFullPath("build/loader/Lyn.Loader.exe")),"a renamed production loader retains its product identity");
string packedProbe=Path.GetFullPath("build/loader-cleanup-probe/VS3.Probe.exe");
Check(File.Exists(packedProbe)&&LoaderCleanup.IsPackageParent(Path.Combine(Path.GetTempPath(),"7zS8BA3C52E","Lyn.Loader.exe"),packedProbe),"current SDK folders without a tmp suffix resolve the outer package");
Check(File.Exists(packedProbe)&&LoaderCleanup.IsPackageParent(Path.Combine(Path.GetTempPath(),"7zS1234.tmp","Lyn.Loader.exe"),packedProbe),"older SDK folder naming also resolves the package");

string work=Path.GetFullPath(Path.Combine("build","loader-cleanup-tests",Guid.NewGuid().ToString("N")));
Directory.CreateDirectory(work);
string shell=Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.Windows),"System32","WindowsPowerShell","v1.0","powershell.exe");
Process Sleeper(int milliseconds) {
    var info=new ProcessStartInfo(shell){UseShellExecute=false,CreateNoWindow=true,WindowStyle=ProcessWindowStyle.Hidden};
    foreach(string arg in new[]{"-NoProfile","-NonInteractive","-Command","[Threading.Thread]::Sleep("+milliseconds+")"})info.ArgumentList.Add(arg);
    return Process.Start(info)!;
}
string Hash(string path){using var file=File.OpenRead(path);return Convert.ToHexString(SHA256.HashData(file)).ToLowerInvariant();}
CleanupPlan Plan(string file,Process owner,string? hash=null) {
    var identity=new CleanupProcess(owner.Id,owner.StartTime.ToFileTimeUtc());return new(file,hash??Hash(file),identity,identity);
}
async Task<bool> Finished(Process job) {
    using var timeout=new CancellationTokenSource(TimeSpan.FromSeconds(25));
    try{await job.WaitForExitAsync(timeout.Token);return job.ExitCode==0;}catch(OperationCanceledException){job.Kill();return false;}
}

// A fresh dummy executable is the only file eligible for each deletion test.
// Quotes, Unicode and shell punctuation must remain literal filename data.
string odd=Path.Combine(work,"VS 3 ' & $ (ñ) test.exe");File.WriteAllText(odd,"MZ test fixture; no executable code");
using(var owner=Sleeper(1600))using(var job=LoaderCleanup.Launch(Plan(odd,owner))) {
    await Task.Delay(150);
    Check(File.Exists(odd),"the executable is kept while its original process is alive");
    Check(await Finished(job)&&!File.Exists(odd),"cleanup deletes exactly the intended Unicode filename after exit");
}
string replaced=Path.Combine(work,"replaced.exe");File.WriteAllText(replaced,"MZ original");
using(var owner=Sleeper(800)) {
    var plan=Plan(replaced,owner);File.WriteAllText(replaced,"MZ replacement");
    using var job=LoaderCleanup.Launch(plan);
    Check(!await Finished(job)&&File.ReadAllText(replaced)=="MZ replacement","a replacement file with a different hash is preserved");
}
string adjacent=Path.Combine(work,"keep.exe"),target=Path.Combine(work,"target.exe");File.WriteAllText(adjacent,"MZ keep");File.WriteAllText(target,"MZ remove");
using(var owner=Sleeper(200))using(var job=LoaderCleanup.Launch(Plan(target,owner))) {
    Check(await Finished(job)&&!File.Exists(target)&&File.Exists(adjacent),"neighboring executables are untouched");
}
using(var owner=Sleeper(200)) {
    bool rejected=false;try{using var bad=LoaderCleanup.Launch(Plan(adjacent,owner,"bad hash"));}catch(ArgumentException){rejected=true;}
    Check(rejected,"malformed cleanup plans are rejected before launching a helper");
    rejected=false;try{using var bad=LoaderCleanup.Launch(Plan(Path.Combine(work,"notes.txt"),owner,new string('a',64)));}catch(ArgumentException){rejected=true;}
    Check(rejected,"non-executable cleanup targets are rejected");
}
Check(File.Exists(replaced)&&File.Exists(adjacent),"protected fixtures still exist after the full cleanup run");
Console.WriteLine($"{checks} loader cleanup checks, {failed} failed; fixtures: {work}");
return failed==0?0:1;
