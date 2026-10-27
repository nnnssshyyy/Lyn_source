using System.Text.Json;
using Lyn.Loader;
Console.WriteLine("Ready for signed ticket JSON on stdin.");
string input = Console.ReadLine() ?? throw new Exception("Missing fixture");
var fixture = JsonSerializer.Deserialize<Fixture>(input, new JsonSerializerOptions(JsonSerializerDefaults.Web))!;
var identity = fixture.Identity;
long now = DateTimeOffset.UtcNow.ToUnixTimeSeconds();
int checks = 0;
void Check(bool value) { checks++; if (!value) throw new Exception("License verification check failed: " + checks); }
Check(LicenseClient.Verify(fixture.Lease, identity.Pid, identity.Created, identity.Nonce, now));
Check(!LicenseClient.Verify(fixture.Lease, identity.Pid + 1, identity.Created, identity.Nonce, now));
Check(!LicenseClient.Verify(fixture.Lease, identity.Pid, identity.Created + "0", identity.Nonce, now));
Check(!LicenseClient.Verify(fixture.Lease, identity.Pid, identity.Created, new string('A',32), now));
Check(!LicenseClient.Verify(fixture.Lease, identity.Pid, identity.Created, identity.Nonce, fixture.Lease.ExpiresAt));
Check(!LicenseClient.Verify(fixture.Lease with { Payload = fixture.Lease.Payload.Replace("|l4d2|", "|other|") }, identity.Pid, identity.Created, identity.Nonce, now));
Check(!LicenseClient.Verify(fixture.Lease with { Signature = new string('A',86) }, identity.Pid, identity.Created, identity.Nonce, now));
Check(!LicenseClient.Verify(fixture.Lease with { ExpiresAt = fixture.Lease.ExpiresAt + 1 }, identity.Pid, identity.Created, identity.Nonce, now));
Console.WriteLine($"{checks} loader signature checks passed against a real server ticket.");
internal sealed record Identity(int Pid,string Created,string Nonce);
internal sealed record Fixture(Identity Identity,LeaseTicket Lease);
namespace Lyn.Loader { internal sealed record GameTarget(int Pid,long Started,string Path); }
