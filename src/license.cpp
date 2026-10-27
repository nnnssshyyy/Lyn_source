#include "license.h"
#include <Windows.h>
#include <bcrypt.h>
#include <array>
#include <atomic>
#include <charconv>
#include <string>
#include <vector>
#include <cstring>

namespace licensing {
namespace {
std::atomic<std::uint64_t> deadline{};
std::atomic<bool> authorized{};
bool Decode(std::string_view text,std::vector<unsigned char>& bytes) {
    unsigned accumulator{},bits{};
    for(char ch:text) {
        unsigned value;
        if(ch>='A' && ch<='Z') value=ch-'A';
        else if(ch>='a' && ch<='z') value=ch-'a'+26;
        else if(ch>='0' && ch<='9') value=ch-'0'+52;
        else if(ch=='-') value=62; else if(ch=='_') value=63; else return false;
        accumulator=(accumulator<<6)|value;bits+=6;
        if(bits>=8) {bits-=8;bytes.push_back(static_cast<unsigned char>(accumulator>>bits));}
    }
    return bits!=6 && (!bits || !(accumulator&((1u<<bits)-1)));
}
bool Number(std::string_view text,std::uint64_t& value) {
    if(text.empty() || text.size()>20 || (text.size()>1 && text.front()=='0')) return false;
    const auto result=std::from_chars(text.data(),text.data()+text.size(),value);
    return result.ec==std::errc{} && result.ptr==text.data()+text.size();
}
std::uint64_t Utc() noexcept {
    FILETIME ft{};GetSystemTimeAsFileTime(&ft);
    return ((static_cast<std::uint64_t>(ft.dwHighDateTime)<<32)|ft.dwLowDateTime)/10000000ull-11644473600ull;
}
struct ClockAnchor {std::uint64_t utc{Utc()},tick{GetTickCount64()};};
const ClockAnchor& Anchor() noexcept {static const ClockAnchor anchor;return anchor;}
}
std::uint64_t Now() noexcept {
    const auto& anchor=Anchor();return anchor.utc+(GetTickCount64()-anchor.tick)/1000;
}
Identity Current() noexcept {
    FILETIME created{},exit{},kernel{},user{};
    if(!GetProcessTimes(GetCurrentProcess(),&created,&exit,&kernel,&user)) return {};
    return {GetCurrentProcessId(),(static_cast<std::uint64_t>(created.dwHighDateTime)<<32)|created.dwLowDateTime};
}
namespace {
bool Transfer(HANDLE pipe,void* buffer,DWORD size,bool write,ULONGLONG until) noexcept {
    auto* next=static_cast<unsigned char*>(buffer);
    while(size) {
        OVERLAPPED operation{};operation.hEvent=CreateEventW(nullptr,TRUE,FALSE,nullptr);
        if(!operation.hEvent) return false;
        DWORD read{};bool ok=(write?WriteFile(pipe,next,size,&read,&operation):ReadFile(pipe,next,size,&read,&operation))!=FALSE;
        if(!ok && GetLastError()==ERROR_IO_PENDING) {
            const auto tick=GetTickCount64();
            if(tick<until && WaitForSingleObject(operation.hEvent,static_cast<DWORD>(until-tick))==WAIT_OBJECT_0)
                ok=GetOverlappedResult(pipe,&operation,&read,FALSE)!=FALSE;
            else {CancelIoEx(pipe,&operation);GetOverlappedResult(pipe,&operation,&read,TRUE);}
        }
        CloseHandle(operation.hEvent);
        if(!ok || !read) return false;
        next+=read;size-=read;
    }
    return true;
}
}
bool Exchange(std::string_view request,std::string& response) noexcept {
    response.clear();
    try {
        if(request.empty() || request.size()>32768) return false;
        const auto identity=Current();
        const auto name=L"\\\\.\\pipe\\VS3."+std::to_wstring(identity.pid)+L"."+std::to_wstring(identity.created);
        HANDLE pipe=INVALID_HANDLE_VALUE;
        for(unsigned attempt=0;attempt<50;++attempt) {
            pipe=CreateFileW(name.c_str(),GENERIC_READ|GENERIC_WRITE,0,nullptr,OPEN_EXISTING,FILE_FLAG_OVERLAPPED|SECURITY_SQOS_PRESENT|SECURITY_IDENTIFICATION,nullptr);
            if(pipe!=INVALID_HANDLE_VALUE) break;
            const auto error=GetLastError();if(error!=ERROR_PIPE_BUSY && error!=ERROR_FILE_NOT_FOUND) break;
            Sleep(10);
        }
        if(pipe==INVALID_HANDLE_VALUE) return false;
        struct PipeScope {HANDLE handle;~PipeScope(){CloseHandle(handle);}} scope{pipe};
        const auto until=GetTickCount64()+5000;
        DWORD size=static_cast<DWORD>(request.size());
        bool ok=Transfer(pipe,&size,sizeof(size),true,until) && Transfer(pipe,const_cast<char*>(request.data()),size,true,until) && Transfer(pipe,&size,sizeof(size),false,until) && size>0 && size<=32768;
        if(ok) {response.resize(size);ok=Transfer(pipe,response.data(),size,false,until);}
        unsigned char ack=1;
        if(ok) ok=Transfer(pipe,&ack,1,true,until);
        if(!ok) response.clear();
        return ok;
    } catch(...) {response.clear();return false;}
}
bool VerifySignature(std::string_view payload,std::string_view signature,std::string_view x,std::string_view y) noexcept {
    try {
        if(payload.empty() || payload.size()>32768 || signature.size()!=86 || x.size()!=43 || y.size()!=43) return false;
        std::vector<unsigned char> keybytes,signaturebytes;
        BCRYPT_ECCKEY_BLOB header{BCRYPT_ECDSA_PUBLIC_P256_MAGIC,32};
        keybytes.resize(sizeof(header));std::memcpy(keybytes.data(),&header,sizeof(header));
        if(!Decode(x,keybytes) || !Decode(y,keybytes) || keybytes.size()!=72 || !Decode(signature,signaturebytes) || signaturebytes.size()!=64) return false;
        BCRYPT_ALG_HANDLE algorithm{};BCRYPT_KEY_HANDLE key{};
        if(BCryptOpenAlgorithmProvider(&algorithm,BCRYPT_ECDSA_P256_ALGORITHM,nullptr,0)<0) return false;
        const auto imported=BCryptImportKeyPair(algorithm,nullptr,BCRYPT_ECCPUBLIC_BLOB,&key,keybytes.data(),static_cast<ULONG>(keybytes.size()),0);
        bool valid=false;
        if(imported>=0) {
            std::array<unsigned char,32> hash{};
            if(BCryptHash(BCRYPT_SHA256_ALG_HANDLE,nullptr,0,reinterpret_cast<PUCHAR>(const_cast<char*>(payload.data())),static_cast<ULONG>(payload.size()),hash.data(),static_cast<ULONG>(hash.size()))>=0)
                valid=BCryptVerifySignature(key,nullptr,hash.data(),static_cast<ULONG>(hash.size()),signaturebytes.data(),static_cast<ULONG>(signaturebytes.size()),0)>=0;
            BCryptDestroyKey(key);
        }
        BCryptCloseAlgorithmProvider(algorithm,0);return valid;
    } catch(...) {return false;}
}
bool Verify(std::string_view wire,Identity identity,std::uint64_t now,
            std::string_view x,std::string_view y,std::uint64_t& expires) noexcept {
    expires=0;
    try {
        if(wire.empty() || wire.size()>1024) return false;
        const auto split=wire.find('\n');if(split==wire.npos) return false;
        const auto payload=wire.substr(0,split),signature=wire.substr(split+1);
        if(signature.size()!=86 || x.size()!=43 || y.size()!=43) return false;
        std::array<std::string_view,9> fields{};size_t offset{};
        for(size_t i=0;i<fields.size();++i) {
            const auto end=payload.find('|',offset);
            if((i==fields.size()-1)!=(end==payload.npos)) return false;
            fields[i]=payload.substr(offset,end==payload.npos?end:end-offset);offset=end+1;
        }
        std::uint64_t issued{},expiry{},pid{},created{};
        if(fields[0]!="VS3" || fields[1]!="1" || fields[7]!="l4d2" || fields[8]!="1" ||
           !Number(fields[2],issued) || !Number(fields[3],expiry) || !Number(fields[4],pid) ||
           !Number(fields[5],created) || !identity.pid || !identity.created ||
           pid!=identity.pid || created!=identity.created || expiry<=issued || expiry-issued>120 ||
           expiry<=now || issued>now+5 || (now>issued && now-issued>120) ||
           fields[6].size()<32 || fields[6].size()>64) return false;
        for(char ch:fields[6]) if(!((ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z')||(ch>='0'&&ch<='9')||ch=='-'||ch=='_')) return false;
        const bool valid=VerifySignature(payload,signature,x,y);
        if(valid) expires=expiry;
        return valid;
    } catch(...) {return false;}
}
bool Active() noexcept {
    // The first verified ticket authorizes the whole game session: the loader
    // may close afterwards and the features stay unlocked until the game exits.
    return authorized.load(std::memory_order_acquire)||GetTickCount64()<deadline.load(std::memory_order_acquire);
}
bool RefreshFromLoader() noexcept {
    try {
        const auto identity=Current();
        std::string wire;
        std::uint64_t expires{};
        if(!Exchange("LEASE",wire) || !Verify(wire,identity,Now(),PublicX,PublicY,expires)) return false;
        // The same ticket never extends its absolute expiry on replay.
        const auto& anchor=Anchor();
        deadline.store(anchor.tick+(expires-anchor.utc)*1000,std::memory_order_release);
        authorized.store(true,std::memory_order_release);
        return true;
    } catch(...) {return false;}
}
}
