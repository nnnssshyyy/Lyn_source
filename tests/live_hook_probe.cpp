#include "license.h"
#include "remote_hooks.h"
#include <Windows.h>
#include <MinHook.h>
#include <cstdio>
#include <cstring>
#include <iostream>
namespace {using Fn=int(__cdecl*)();Fn original{};int __cdecl Detour(){return original()+10;}}
int main(){
 const auto id=licensing::Current();std::printf("%u %llu\n",id.pid,id.created);std::fflush(stdout);std::string go;std::getline(std::cin,go);
 auto* code=static_cast<BYTE*>(VirtualAlloc(nullptr,4096,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE));if(!code)return 2;
 std::memset(code,0x90,4096);code[8]=0xb8;code[9]=7;std::memset(code+10,0,3);code[13]=0xc3;const auto call=reinterpret_cast<Fn>(code);
 if(MH_Initialize()!=MH_OK || MH_SetRemoteMode(TRUE)!=MH_OK || MH_CreateHook(code,reinterpret_cast<void*>(&Detour),reinterpret_cast<void**>(&original))!=MH_OK || MH_EnableHook(code)!=MH_OK)return 3;
 if(!licensing::RefreshFromLoader() || !licensing::Active())return 4;
 int activated=-1;for(int i=0;i<3 && activated<0;++i){activated=remote_hooks::Pump();if(activated<0)Sleep(500);}
 if(activated!=1 || call()!=17)return 5;
 MH_Uninitialize();if(call()!=7)return 6;VirtualFree(code,0,MEM_RELEASE);
 std::puts("PASS: real service signature, production loader pipe, native activation and restoration");return 0;
}
