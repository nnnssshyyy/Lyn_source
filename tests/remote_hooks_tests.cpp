#include <Windows.h>
#include <MinHook.h>
#include "license.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
namespace {using Fn=int(__cdecl*)();Fn original{};int __cdecl Detour(){return original()+10;}}
int main(int argc,char** argv) {
    if(argc>1 && !std::strcmp(argv[1],"--transport")) {
        const auto id=licensing::Current();std::printf("%u %llu\n",id.pid,id.created);std::fflush(stdout);
        std::string go,response;std::getline(std::cin,go);
        if(!licensing::Exchange("LEASE",response) || response!="probe\nsignature")return 2;
        std::puts("first ok");std::fflush(stdout);std::getline(std::cin,go);
        if(!licensing::Exchange("LEASE",response) || response!="probe\nsignature")return 3;
        std::puts("second ok");return 0;
    }
    unsigned checks{},failed{};const auto expect=[&](bool value,const char* name){++checks;if(!value){++failed;std::printf("FAIL: %s\n",name);}};
    auto* code=static_cast<BYTE*>(VirtualAlloc(nullptr,4096,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE));if(!code)return 2;
    std::memset(code,0x90,4096);code[8]=0xb8;code[9]=7;std::memset(code+10,0,3);code[13]=0xc3;
    const auto call=reinterpret_cast<Fn>(code);
    expect(call()==7,"probe starts unmodified");
    expect(MH_Initialize()==MH_OK && MH_SetRemoteMode(TRUE)==MH_OK,"remote mode initializes");
    expect(MH_CreateHook(code,reinterpret_cast<void*>(&Detour),reinterpret_cast<void**>(&original))==MH_OK,"trampoline prepared locally");
    expect(MH_EnableHook(code)==MH_OK && MH_ApplyQueued()==MH_OK && call()==7,"local enable cannot activate a remote hook");
    MH_REMOTE_PATCH row{};UINT count{};
    expect(MH_CollectRemoteHooks(&row,1,&count)==MH_OK && count==1 && row.size==5,"snapshot captures target, original and patch site");
    MH_DisableHook(code);
    expect(MH_CollectRemoteHooks(&row,1,&count)==MH_OK && count==0,"disabling a pending hook cancels remote activation");
    MH_EnableHook(code);
    expect(MH_CollectRemoteHooks(&row,1,&count)==MH_OK && count==1,"cancelled hook can be queued again");
    expect(MH_SetRemoteMode(FALSE)!=MH_OK,"active hook table cannot change activation mode");
    row.patch[0]=0xe9;UINT32 rel=reinterpret_cast<UINT32>(row.detour)-reinterpret_cast<UINT32>(row.site)-5;std::memcpy(row.patch+1,&rel,4);
    auto bad=row;bad.patch[1]^=1;
    expect(MH_ApplyRemoteHooks(&bad,1)!=MH_OK && call()==7,"wrong service destination is rejected without patching");
    bad=row;bad.before[0]^=1;
    expect(MH_ApplyRemoteHooks(&bad,1)!=MH_OK && call()==7,"stale original bytes are rejected");
    bad=row;bad.target=code+1;
    expect(MH_ApplyRemoteHooks(&bad,1)!=MH_OK && call()==7,"foreign target is rejected");
    code[0]=0x91;
    expect(MH_ApplyRemoteHooks(&row,1)!=MH_OK,"changed live prologue requires a new snapshot");code[0]=0x90;
    expect(MH_ApplyRemoteHooks(&row,1)==MH_OK && call()==17,"validated patch calls detour and relocated original");
    expect(MH_ApplyRemoteHooks(&row,1)!=MH_OK && call()==17,"replayed plan cannot re-enable an active hook");
    expect(MH_DisableHook(code)==MH_OK && call()==7,"offline disable restores original bytes");
    expect(MH_Uninitialize()==MH_OK && MH_SetRemoteMode(FALSE)==MH_OK,"teardown permits default mode");
    expect(MH_Initialize()==MH_OK && MH_CreateHook(code,reinterpret_cast<void*>(&Detour),reinterpret_cast<void**>(&original))==MH_OK && MH_EnableHook(code)==MH_OK && call()==17,"upstream local mode still works");
    expect(MH_Uninitialize()==MH_OK && call()==7,"local teardown restores original");
    VirtualFree(code,0,MEM_RELEASE);std::printf("%u remote hook checks, %u failed\n",checks,failed);return failed?1:0;
}
