#include "remote_hooks.h"
#include <bcrypt.h>
#include <array>
#include <charconv>
#include <cstdio>
namespace remote_hooks {
namespace {
std::string Hex(const unsigned char* bytes,size_t count) {
    constexpr char alphabet[]="0123456789abcdef";std::string out;out.reserve(count*2);
    for(size_t i=0;i<count;++i) {out+=alphabet[bytes[i]>>4];out+=alphabet[bytes[i]&15];}return out;
}
std::string Row(const MH_REMOTE_PATCH& row) {
    char addresses[28]{};
    std::snprintf(addresses,sizeof(addresses),"%08x,%08x,%08x,",reinterpret_cast<unsigned>(row.target),reinterpret_cast<unsigned>(row.detour),reinterpret_cast<unsigned>(row.site));
    return addresses+Hex(row.before,row.size);
}
bool Number(std::string_view s,std::uint64_t& n) {
    if(s.empty() || s.size()>20 || (s.size()>1 && s.front()=='0')) return false;
    const auto r=std::from_chars(s.data(),s.data()+s.size(),n);return r.ec==std::errc{} && r.ptr==s.data()+s.size();
}
int Digit(char ch) {return ch>='0'&&ch<='9'?ch-'0':ch>='a'&&ch<='f'?ch-'a'+10:-1;}
}
std::string Request(std::span<const MH_REMOTE_PATCH> rows,std::string_view nonce) {
    std::string out="VS3Q|2|";out+=nonce;out+='|';out+=std::to_string(rows.size());
    for(const auto& row:rows) {out+='\n';out+=Row(row);}return out;
}
bool VerifyPlan(std::string_view wire,std::span<MH_REMOTE_PATCH> rows,licensing::Identity identity,
                std::string_view nonce,std::uint64_t now,std::string_view x,std::string_view y) noexcept {
    try {
        if(wire.size()>32768 || rows.empty() || rows.size()>128 || nonce.size()<32 || nonce.size()>64) return false;
        const auto signature=wire.rfind('\n');if(signature==wire.npos) return false;
        const auto payload=wire.substr(0,signature);
        if(!licensing::VerifySignature(payload,wire.substr(signature+1),x,y)) return false;
        const auto header_end=payload.find('\n');if(header_end==payload.npos) return false;
        const auto header=payload.substr(0,header_end);
        std::array<std::string_view,8> parts{};size_t offset{};
        for(size_t i=0;i<parts.size();++i) {
            const auto end=header.find('|',offset);
            if((i==parts.size()-1)!=(end==header.npos)) return false;
            parts[i]=header.substr(offset,end==header.npos?end:end-offset);offset=end+1;
        }
        std::uint64_t issued{},expires{},pid{},created{},count{};
        if(parts[0]!="VSH" || parts[1]!="1" || !Number(parts[2],issued) || !Number(parts[3],expires) ||
           !Number(parts[4],pid) || !Number(parts[5],created) || !Number(parts[7],count) ||
           pid!=identity.pid || created!=identity.created || !pid || !created || parts[6]!=nonce ||
           count!=rows.size() || expires<=now || expires<=issued || expires-issued>30 || issued>now+5 || (issued<now && now-issued>30)) return false;
        offset=header_end+1;
        for(auto& row:rows) {
            if(row.size!=5 && row.size!=7) return false;
            const auto end=payload.find('\n',offset);
            const auto line=payload.substr(offset,end==payload.npos?end:end-offset);
            const auto prefix=Row(row)+",";
            if(!line.starts_with(prefix) || line.size()!=prefix.size()+row.size*2) return false;
            for(unsigned i=0;i<row.size;++i) {
                const int hi=Digit(line[prefix.size()+i*2]),lo=Digit(line[prefix.size()+i*2+1]);
                if(hi<0 || lo<0) return false;row.patch[i]=static_cast<BYTE>((hi<<4)|lo);
            }
            if(&row==&rows.back()) return end==payload.npos;
            if(end==payload.npos) return false;offset=end+1;
        }
        return false;
    } catch(...) {return false;}
}
int Pump() noexcept {
    try {
        if(!licensing::Active()) return 0;
        std::array<MH_REMOTE_PATCH,128> rows{};UINT count{};
        const MH_STATUS collected=MH_CollectRemoteHooks(rows.data(),static_cast<UINT>(rows.size()),&count);
        // After the first applied plan the module works locally; nothing left to ask for.
        if(collected==MH_ERROR_NOT_INITIALIZED) return 0;
        if(collected!=MH_OK) return -1;
        if(!count) return 0;
        std::array<unsigned char,16> random{};
        if(BCryptGenRandom(nullptr,random.data(),static_cast<ULONG>(random.size()),BCRYPT_USE_SYSTEM_PREFERRED_RNG)<0) return -1;
        const auto nonce=Hex(random.data(),random.size());const std::span plan{rows.data(),count};
        std::string response;
        if(!licensing::Exchange(Request(plan,nonce),response) || !licensing::Active() ||
           !VerifyPlan(response,plan,licensing::Current(),nonce,licensing::Now()) ||
           MH_ApplyRemoteHooks(plan.data(),count)!=MH_OK) return -1;
        std::string ready;
        licensing::Exchange("VS3R",ready); // Best effort: lets the loader close once the plan is applied.
        return static_cast<int>(count);
    } catch(...) {return -1;}
}
}
