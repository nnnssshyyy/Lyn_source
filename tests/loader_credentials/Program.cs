using Lyn.Loader;
string target="VS3/QA/Credentials/"+Guid.NewGuid().ToString("N");int checks=0;
void Check(bool valid){if(!valid)throw new Exception("Credential vault assertion failed");checks++;}
try{
 Check(CredentialVault.Read(target)==null);
 CredentialVault.Save("qa_user","Test-only password 2026!",target);
 var first=CredentialVault.Read(target);Check(first?.Username=="qa_user"&&first.Password=="Test-only password 2026!");
 CredentialVault.Save("qa_updated","Unicode prueba: ñ中🔐2026",target);
 var updated=CredentialVault.Read(target);Check(updated?.Username=="qa_updated"&&updated.Password=="Unicode prueba: ñ中🔐2026");
 try{CredentialVault.Save("qa_user","short",target);throw new Exception("Invalid secret accepted");}catch(ArgumentException){checks++;}
 CredentialVault.Delete(target);Check(CredentialVault.Read(target)==null);CredentialVault.Delete(target);checks++;
 Console.WriteLine($"PASS {checks} real Windows credential vault checks; disposable credential removed.");
}finally{CredentialVault.Delete(target);}
