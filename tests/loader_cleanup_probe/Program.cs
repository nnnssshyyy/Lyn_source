using System.Text.Json;
using Lyn.Loader;

// A separate disposable probe verifies the real SFX parent/child cleanup. It
// never authenticates, opens a game or injects a module. No QA switch ships in
// the production loader.
string report=Environment.GetEnvironmentVariable("VS3_CLEANUP_QA_REPORT")??throw new InvalidOperationException("Missing QA report path");
var messages=new List<string>();LoaderCleanup.Capture(messages.Add);
string? owner=LoaderCleanup.CapturedPath;
bool exitPath=Environment.GetEnvironmentVariable("VS3_CLEANUP_QA_SYNC")=="1";
bool scheduled=exitPath?LoaderCleanup.ScheduleExit(true,false,messages.Add):await LoaderCleanup.Schedule(true,false,messages.Add);
bool duplicateBlocked=!LoaderCleanup.ScheduleExit(true,false,messages.Add);
await File.WriteAllTextAsync(report,JsonSerializer.Serialize(new{owner,payload=Environment.ProcessPath,scheduled,duplicateBlocked,exitPath,moduleLoaded=false,messages}));
return scheduled?0:1;
