#include "license.h"
#include "remote_hooks.h"
#include <Windows.h>
#include <bcrypt.h>
#include <array>
#include <cstdio>
#include <string>
#include <vector>
#include <cstring>

namespace {
int checks{},failures{};
void Check(bool value,const char* label) {++checks;if(!value){++failures;std::printf("FAIL: %s\n",label);}}
std::string Encode(const unsigned char* bytes,size_t size) {
    constexpr char alphabet[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
    std::string result;unsigned bits{},accumulator{};
    for(size_t i=0;i<size;++i) {accumulator=(accumulator<<8)|bytes[i];bits+=8;while(bits>=6){bits-=6;result+=alphabet[(accumulator>>bits)&63];}}
    if(bits) result+=alphabet[(accumulator<<(6-bits))&63];return result;
}
}
int main() {
    BCRYPT_ALG_HANDLE algorithm{};BCRYPT_KEY_HANDLE key{};
    if(BCryptOpenAlgorithmProvider(&algorithm,BCRYPT_ECDSA_P256_ALGORITHM,nullptr,0)<0 ||
       BCryptGenerateKeyPair(algorithm,&key,256,0)<0 || BCryptFinalizeKeyPair(key,0)<0) return 2;
    std::array<unsigned char,72> blob{};ULONG count{};
    BCryptExportKey(key,nullptr,BCRYPT_ECCPUBLIC_BLOB,blob.data(),static_cast<ULONG>(blob.size()),&count,0);
    const auto x=Encode(blob.data()+8,32),y=Encode(blob.data()+40,32);
    const licensing::Identity identity{1234,133700000000000000ull};
    const auto payload=[](std::uint64_t issued,std::uint64_t expires,std::string nonce=std::string(32,'A')) {
        return "VS3|1|"+std::to_string(issued)+"|"+std::to_string(expires)+"|1234|133700000000000000|"+nonce+"|l4d2|1";
    };
    const auto sign=[&](const std::string& body) {
        std::array<unsigned char,32> hash{};std::array<unsigned char,64> signature{};ULONG written{};
        BCryptHash(BCRYPT_SHA256_ALG_HANDLE,nullptr,0,reinterpret_cast<PUCHAR>(const_cast<char*>(body.data())),static_cast<ULONG>(body.size()),hash.data(),32);
        if(BCryptSignHash(key,nullptr,hash.data(),32,signature.data(),64,&written,0)<0) return std::string{};
        return body+"\n"+Encode(signature.data(),written);
    };
    std::uint64_t expiry{};const auto wire=sign(payload(1000,1120));
    Check(licensing::Verify(wire,identity,1000,x,y,expiry) && expiry==1120,"valid P-256 raw signature");
    Check(licensing::Verify(wire,identity,1119,x,y,expiry),"last valid second");
    Check(!licensing::Verify(wire,identity,1120,x,y,expiry) && expiry==0,"expired and output cleared");
    Check(!licensing::Verify(wire,identity,994,x,y,expiry),"future issue time");
    Check(!licensing::Verify(wire,{1235,identity.created},1000,x,y,expiry),"wrong pid");
    Check(!licensing::Verify(wire,{1234,identity.created+1},1000,x,y,expiry),"pid reused different creation");
    Check(!licensing::Verify(wire,{0,0},1000,x,y,expiry),"missing identity");
    auto tampered=wire;tampered[12]^=1;
    Check(!licensing::Verify(tampered,identity,1000,x,y,expiry),"tampered payload");
    tampered=wire;tampered[tampered.size()-3]^=1;
    Check(!licensing::Verify(tampered,identity,1000,x,y,expiry),"tampered signature");
    Check(!licensing::Verify(wire,identity,1000,y,x,expiry),"wrong public key");
    Check(!licensing::Verify(sign(payload(1000,1121)),identity,1000,x,y,expiry),"lease longer than 120 seconds");
    Check(!licensing::Verify(sign(payload(1000,1000)),identity,1000,x,y,expiry),"empty lease");
    Check(!licensing::Verify(sign(payload(1000,1120,"short")),identity,1000,x,y,expiry),"short nonce");
    Check(!licensing::Verify(sign(payload(1000,1120,std::string(32,'|'))),identity,1000,x,y,expiry),"delimiter injection");
    Check(!licensing::Verify(wire+"\n",identity,1000,x,y,expiry),"trailing input");
    Check(!licensing::Verify(std::string(1025,'A'),identity,1000,x,y,expiry),"oversized wire");
    Check(!licensing::Verify("",identity,1000,x,y,expiry),"empty wire");
    Check(!licensing::Active(),"no authorization before signed ticket");
    MH_REMOTE_PATCH row{};row.target=reinterpret_cast<void*>(0x100000);row.detour=reinterpret_cast<void*>(0x200000);row.site=row.target;row.size=5;
    std::memset(row.before,0x90,5);const std::string nonce(32,'a');
    const auto plan=sign("VSH|1|1000|1030|1234|133700000000000000|"+nonce+"|1\n00100000,00200000,00100000,9090909090,e9fbff0f00");
    Check(remote_hooks::VerifyPlan(plan,{&row,1},identity,nonce,1000,x,y) && row.patch[0]==0xe9,"signed service plan binds the complete hook snapshot");
    Check(!remote_hooks::VerifyPlan(plan,{&row,1},identity,std::string(32,'b'),1000,x,y),"hook plan nonce replay is rejected");
    Check(!remote_hooks::VerifyPlan(plan,{&row,1},identity,nonce,1030,x,y),"hook plan expires after thirty seconds");
    Check(!remote_hooks::VerifyPlan(plan,{&row,1},{1235,identity.created},nonce,1000,x,y),"hook plan cannot move to another process");
    row.before[0]=0x91;
    Check(!remote_hooks::VerifyPlan(plan,{&row,1},identity,nonce,1000,x,y),"signed original bytes must match the requested snapshot");row.before[0]=0x90;
    Check(!remote_hooks::VerifyPlan(plan+"\n",{&row,1},identity,nonce,1000,x,y),"trailing hook plan data is rejected");
    Check(!remote_hooks::VerifyPlan(plan,{&row,1},identity,nonce,1000,y,x),"hook plan rejects a different signing key");
    BCryptDestroyKey(key);BCryptCloseAlgorithmProvider(algorithm,0);
    std::printf("%d checks, %d failures\n",checks,failures);return failures?1:0;
}
