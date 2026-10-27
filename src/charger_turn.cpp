#include "charger_turn.h"
#include <array>
#include <cstring>

namespace charger_turn {
bool Patch::Bind(memory::Range image,void* block) noexcept {
    if(applied_&&!Set(false))return false;
    site_=nullptr;conflict_=false;
    std::array<unsigned char,26> code{};
    if(!image.Contains(block,code.size())||!memory::Copy(code.data(),block,code.size()))return false;
    if(code[0]!=0xf6||code[1]!=0x86||code[6]!=0x20||code[7]!=0x74||code[9]!=0x80||code[10]!=0xbe||
       code[15]!=0||code[16]!=0x74||code[18]!=0xd9||code[19]!=0x86||code[24]!=0x8d||code[25]!=0x47)return false;
    unsigned flags{},frozen{},angles{};
    std::memcpy(&flags,code.data()+2,4);std::memcpy(&frozen,code.data()+11,4);std::memcpy(&angles,code.data()+20,4);
    if(flags<4||flags>0x40000||angles<4||angles>0x3fff0||frozen!=angles+12)return false;
    const auto distance=static_cast<signed char>(code[17]);
    if(distance<=0||distance>100)return false;
    auto destination=static_cast<unsigned char*>(block)+18+distance;
    std::array<unsigned char,7> mark{};
    if(!image.Contains(destination,mark.size())||!memory::Copy(mark.data(),destination,mark.size())||
       mark[0]!=0xc6||mark[1]!=0x86||mark[6]!=1||std::memcmp(mark.data()+2,&frozen,4))return false;
    site_=static_cast<unsigned char*>(block)+16;displacement_=code[17];return true;
}
bool Patch::Change(unsigned char expected,unsigned char replacement) noexcept {
    unsigned char current[2]{};
    if(!memory::Copy(current,site_,sizeof(current))||current[0]!=expected||current[1]!=displacement_){conflict_=true;applied_=false;return false;}
    DWORD before{};
    if(!VirtualProtect(site_,1,PAGE_EXECUTE_READWRITE,&before))return false;
    const bool wrote=memory::Copy(site_,&replacement,1);
    const bool flushed=wrote&&FlushInstructionCache(GetCurrentProcess(),site_,1);
    DWORD ignored{};
    bool protected_again=VirtualProtect(site_,1,before,&ignored)!=FALSE;
    if(!wrote||!flushed||!protected_again) {
        // Roll back a partial transaction before reporting a failure.
        DWORD writable{};
        if(VirtualProtect(site_,1,PAGE_EXECUTE_READWRITE,&writable)) {
            memory::Copy(site_,&expected,1);FlushInstructionCache(GetCurrentProcess(),site_,1);
            protected_again=VirtualProtect(site_,1,before,&ignored)!=FALSE;
        }
        unsigned char observed{};applied_=memory::Read(site_,observed)&&observed==0xeb;
        return false;
    }
    applied_=replacement==0xeb;return true;
}
bool Patch::Set(bool enabled) noexcept {
    if(!Available())return !enabled&&!applied_;
    if(enabled==applied_)return true;
    return Change(enabled?0x74:0xeb,enabled?0xeb:0x74);
}
}
