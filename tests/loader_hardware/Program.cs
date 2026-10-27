using System.Net;
using System.Net.Http;
using System.Security.Cryptography;
using System.Text;
using System.Text.Json;
using Lyn.Loader;

int checks = 0;
void Check(bool value, string label) { if (!value) throw new Exception(label); checks++; }
var clock = new TestClock();
var handler = new DeviceServer();
using var identity = new TestIdentity();
handler.Identity = identity;
using var client = new LicenseClient(identity, new HttpClient(handler), clock);
await client.Login("  Hardware_QA  ", "Dummy.password.2026!", default);
Check(client.SignedIn && client.Username == "hardware_qa", "Successful bound login");
Check(client.TakeRecoveryCode().StartsWith("VS3-R-") && client.TakeRecoveryCode() == "", "Recovery code consumed once");
var link = await client.HardwareManagementLink(default);
Check(link.Host == "vs3-accounts.vercel.app" && link.Query == "" && link.Fragment.StartsWith("#hardware-link="), "Trust token stays in fragment of fixed origin");
await client.Logout();
Check(!client.SignedIn && handler.Paths.Contains("/api/auth/logout"), "Logout signed before local revocation");
handler.Mode = "pending";
try { await client.Login("hardware_qa", "Dummy.password.2026!", default); throw new Exception("Pending accepted"); }
catch (HardwareApprovalRequired e) { Check(e.RequestCode == "012345ABCDEF" && !client.SignedIn, "Pending device never receives session"); }
handler.Mode = "missing";
try { await client.Login("hardware_qa", "Dummy.password.2026!", default); throw new Exception("Legacy response accepted"); }
catch (InvalidOperationException) { Check(!client.SignedIn, "Missing hardware receipt rejected"); }
handler.Mode = "wrong";
try { await client.Login("hardware_qa", "Dummy.password.2026!", default); throw new Exception("Foreign receipt accepted"); }
catch (InvalidOperationException) { Check(!client.SignedIn, "Receipt for another key rejected"); }
handler.Mode = "ok";
await client.Login("hardware_qa", "Dummy.password.2026!", default);
handler.Mode = "expired";
try { await client.HardwareManagementLink(default); throw new Exception("Expired session accepted"); }
catch (InvalidOperationException) { Check(!client.SignedIn, "Server revocation clears local session"); }
Check(handler.Verified == 8 && handler.Nonces.Count == handler.Verified, "Every login and authenticated request has valid independent ECDSA proof and unique nonce");
Check(handler.ClockRequests == 1, "Server time is reused without a request for each signed operation");
clock.Advance(TimeSpan.FromMinutes(6));
handler.Mode="pending";
try { await client.Login("hardware_qa", "Dummy.password.2026!", default); throw new Exception("Pending accepted"); }
catch (HardwareApprovalRequired) { Check(handler.ClockRequests == 2, "Server time refreshes after five minutes"); }
using (var badIdentity = new TestIdentity())
using (var badClockClient = new LicenseClient(badIdentity, new HttpClient(new DeviceServer { Identity=badIdentity, Mode="bad-clock" }), clock))
{
 try { await badClockClient.Login("hardware_qa", "Dummy.password.2026!", default); throw new Exception("Mismatched time accepted"); }
 catch (InvalidOperationException) { Check(!badClockClient.SignedIn, "Cached or mismatched time nonce cannot sign a login"); }
}

byte[] firmware = new byte[34];
System.Buffers.Binary.BinaryPrimitives.WriteUInt32LittleEndian(firmware.AsSpan(4), 26);
firmware[8] = 1; firmware[9] = 24;
for (int i = 0; i < 16; i++) firmware[16+i] = (byte)(i+1);
Check(HardwareIdentity.ParseFirmwareUuid(firmware) == "0102030405060708090a0b0c0d0e0f10", "SMBIOS type 1 identity extracted");
Array.Fill(firmware, (byte)255, 16, 16);
Check(HardwareIdentity.ParseFirmwareUuid(firmware) == "unavailable" && HardwareIdentity.ParseFirmwareUuid([1,2,3]) == "unavailable", "Missing and malformed firmware rejected");
if (args.Contains("--windows-identity"))
{
 using var local=HardwareIdentity.Open(); using var reopened=HardwareIdentity.Open();
 Check(local.Device==reopened.Device,"Windows CNG identity survives reopening");
 using var verifier=ECDsa.Create(new ECParameters{Curve=ECCurve.NamedCurves.nistP256,Q=new(){X=LicenseClient.Decode(local.Device.KeyX),Y=LicenseClient.Decode(local.Device.KeyY)}});
 byte[] payload=Encoding.UTF8.GetBytes("VS3 local identity QA");
 Check(verifier.VerifyData(payload,local.Sign(payload),HashAlgorithmName.SHA256,DSASignatureFormat.IeeeP1363FixedFieldConcatenation),"Windows protected key signs successfully");
}

// Export only public proof data for the independent JavaScript API interoperability check.
string json = JsonSerializer.Serialize(new {username="hardware_qa",password="Dummy.password.2026!",device=identity.Device}, new JsonSerializerOptions(JsonSerializerDefaults.Web));
using var request = new HttpRequestMessage(HttpMethod.Post, LicenseClient.Origin + "/api/loader/login");
HardwareIdentity.Attach(request, Encoding.UTF8.GetBytes(json), "login:hardware_qa", identity);
Directory.CreateDirectory("build/hardware-match");
File.WriteAllText("build/hardware-match/dotnet-proof.json", JsonSerializer.Serialize(new {body=json,headers=request.Headers.ToDictionary(h=>h.Key,h=>string.Join(",",h.Value))}));
Console.WriteLine($"PASS {checks} loader hardware checks; {handler.Verified} signed requests independently verified.");

sealed class TestIdentity : IHardwareIdentity
{
 readonly ECDsa key = ECDsa.Create(ECCurve.NamedCurves.nistP256);
 public HardwareDevice Device {get;}
 public TestIdentity() {var q=key.ExportParameters(false).Q;Device=new(new string('a',64),HardwareIdentity.Base64(q.X!),HardwareIdentity.Base64(q.Y!),"PC prueba");}
 public byte[] Sign(byte[] payload)=>key.SignData(payload,HashAlgorithmName.SHA256,DSASignatureFormat.IeeeP1363FixedFieldConcatenation);
 public void Dispose()=>key.Dispose();
}
namespace Lyn.Loader { internal sealed record GameTarget(int Pid, long Started); }
sealed class DeviceServer : HttpMessageHandler
{
 public TestIdentity Identity = null!;
 public string Mode = "ok";
 public int Verified;
 public int ClockRequests;
 public HashSet<string> Nonces = new();
 public List<string> Paths = new();
 protected override async Task<HttpResponseMessage> SendAsync(HttpRequestMessage r, CancellationToken ct)
 {
  string body=r.Content==null?"":await r.Content.ReadAsStringAsync(ct), path=r.RequestUri!.PathAndQuery;
  if(r.RequestUri.AbsolutePath=="/api/loader/time") {
   ClockRequests++;
   if(r.Headers.Authorization!=null||r.Headers.Contains("X-VS3-Signature"))throw new Exception("Clock request included credentials");
   var nonce=r.RequestUri.Query.Split('=')[1];
   return new HttpResponseMessage(HttpStatusCode.OK){Content=new StringContent(JsonSerializer.Serialize(new {nonce=Mode=="bad-clock"?"stale":nonce,serverTime=DateTimeOffset.UtcNow.ToUnixTimeSeconds()}),Encoding.UTF8,"application/json")};
  }
  string H(string name)=>r.Headers.GetValues(name).Single();
  if(Math.Abs(long.Parse(H("X-VS3-Time"))-DateTimeOffset.UtcNow.ToUnixTimeSeconds())>15)throw new Exception("Signed with incorrect Windows wall time");
  string credential=path=="/api/loader/login"?"login:hardware_qa":new string('T',43);
  if(path!="/api/loader/login" && r.Headers.Authorization?.Parameter!=credential)throw new Exception("Missing bearer");
  var d=Identity.Device;
  var payload=string.Join('\n',"VSHW|1",r.Method.Method,path,HardwareIdentity.Hash(body),HardwareIdentity.Hash(credential),d.Fingerprint,H("X-VS3-Time"),H("X-VS3-Nonce"));
  using var verifier=ECDsa.Create(new ECParameters{Curve=ECCurve.NamedCurves.nistP256,Q=new(){X=LicenseClient.Decode(d.KeyX),Y=LicenseClient.Decode(d.KeyY)}});
  if(H("X-VS3-Hardware")!="1" || H("X-VS3-Device")!=d.Fingerprint || !Nonces.Add(H("X-VS3-Nonce")) || !verifier.VerifyData(Encoding.UTF8.GetBytes(payload),LicenseClient.Decode(H("X-VS3-Signature")),HashAlgorithmName.SHA256,DSASignatureFormat.IeeeP1363FixedFieldConcatenation))throw new Exception("Invalid independent proof");
  Verified++;Paths.Add(path);
  object result;HttpStatusCode status=HttpStatusCode.OK;
  if(Mode=="expired"){status=HttpStatusCode.Unauthorized;result=new {error="Revoked",code="hardware_session_invalid"};}
  else if(path=="/api/loader/login" && Mode=="pending"){status=HttpStatusCode.Forbidden;result=new {error="Autoriza desde la web",code="hardware_pending",requestCode="012345ABCDEF",expiresAt=DateTimeOffset.UtcNow.ToUnixTimeSeconds()+600};}
  else if(path=="/api/loader/login")result=new {token=new string('T',43),username="hardware_qa",hardware=Mode=="missing"?null:new {fingerprint=d.Fingerprint,keyHash=Mode=="wrong"?new string('b',64):HardwareIdentity.Hash(d.KeyX+"."+d.KeyY),revision=1},recoveryCode="VS3-R-01234567-01234567-01234567-01234567-01234567"};
  else if(path=="/api/loader/hardware/web-link")result=new {token=new string('L',43),expiresAt=DateTimeOffset.UtcNow.ToUnixTimeSeconds()+300};
  else result=new {ok=true};
  return new HttpResponseMessage(status){Content=new StringContent(JsonSerializer.Serialize(result),Encoding.UTF8,"application/json")};
 }
}
sealed class TestClock : TimeProvider
{
 long timestamp;
 public override long TimestampFrequency=>TimeSpan.TicksPerSecond;
 public override long GetTimestamp()=>timestamp;
 public override DateTimeOffset GetUtcNow()=>DateTimeOffset.UtcNow.AddHours(-7);
 public void Advance(TimeSpan duration)=>timestamp+=duration.Ticks;
}
