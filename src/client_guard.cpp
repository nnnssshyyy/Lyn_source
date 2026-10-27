#include "client_guard.h"
#include "log.h"
#include <MinHook.h>
#include <atomic>
#include <cstring>

namespace client_guard {
namespace {
enum Flag:unsigned {Query=1,Setting=2,Command=4,Message=8,Crosshair=16,Crc=32,Null=64,Heart=128,Logging=256};
std::atomic_uint options{};
std::atomic<std::shared_ptr<const Rules>> rules;
bool(*is_active)(){};
memory::Range engine_image{};
std::array<std::array<char,64>,128> message_names{};
unsigned message_count{};
Targets installed{};
using Send=bool(__thiscall*)(void*,void*,bool,bool);
using Process=bool(__thiscall*)(void*);
using Dispatch=bool(__thiscall*)(void*,int,void*);
using Sound=void(__thiscall*)(void*,void*,int,int,const char*,float,int,int,int,const void*,const void*,void*,bool,float,int);
Send original_send{};Process original_setting{},original_command{},original_crosshair{};
Dispatch original_dispatch{};Sound original_sound{};
bool Enabled(unsigned flag) noexcept {return (options.load(std::memory_order_acquire)&flag)&&is_active&&is_active();}
bool Text(const char* source,char* out,size_t capacity) noexcept {
    if(!source)return false;
    for(size_t i=0;i<capacity;++i) {
        if(!memory::Read(source+i,out[i]))return false;
        if(!out[i])return true;
    }
    out[0]=0;return false;
}
bool Type(void* msg,int expected,const char* expected_name) noexcept {
    using Number=int(__thiscall*)(void*);using Name=const char*(__thiscall*)(void*);
    int type{};const char* label{};char name[64]{};
    return memory::Invoke(memory::Virtual<Number>(msg,8,engine_image),type,msg)&&type==expected&&
        memory::Invoke(memory::Virtual<Name>(msg,10,engine_image),label,msg)&&Text(label,name,sizeof(name))&&
        !std::strcmp(name,expected_name);
}
void Log(const char* event,const char* name) noexcept {
    if(!Enabled(Logging))return;
    // A hostile server must not cause unbounded log I/O; never execute echo.
    static std::atomic<ULONGLONG> deadline{};
    auto previous=deadline.load(std::memory_order_relaxed);const auto now=GetTickCount64();
    if(now>=previous&&deadline.compare_exchange_strong(previous,now+125))app::Log("Client guard: %s [%.255s]",event,name);
}
struct Restore {
    void* destination{};std::array<unsigned char,20> before{};size_t size{};
    bool Write(void* target,const void* value,size_t bytes) noexcept {
        if(bytes>before.size()||!memory::Copy(before.data(),target,bytes))return false;
        destination=target;size=bytes;
        return memory::Copy(target,value,bytes);
    }
    ~Restore(){if(destination)memory::Copy(destination,before.data(),size);}
};
bool __fastcall SendHook(void* self,void*,void* msg,bool reliable,bool voice) {
    if(!Enabled(Query|Crc))return original_send(self,msg,reliable,voice);
    try {
        if(Enabled(Crc)&&Type(msg,14,"clc_FileCRCCheck")) {Log("CRC blocked","clc_FileCRCCheck");return true;}
        const auto list=rules.load(std::memory_order_acquire);
        if(list&&Enabled(Query)&&Type(msg,13,"clc_RespondCvarValue")) {
            const char* name{};char text[256]{};
            if(memory::Field(msg,native::QueryName,name)&&Text(name,text,sizeof(text))) {
                if(const auto rule=Find(list->queries,text)) {
                    if(rule->value.empty()){Log("query blocked",text);return true;}
                    const char* replacement=rule->value.c_str();Restore restore;
                    if(restore.Write(static_cast<unsigned char*>(msg)+native::QueryValue,&replacement,sizeof(replacement))) {
                        Log("query substituted",text);return original_send(self,msg,reliable,voice);
                    }
                }
            }
        }
    } catch(...) {}
    return original_send(self,msg,reliable,voice);
}
bool __fastcall SettingHook(void* msg,void*) {
    if(!Enabled(Setting))return original_setting(msg);
    try {
        const auto list=rules.load(std::memory_order_acquire);
        if(list&&Enabled(Setting)&&Type(msg,6,"net_SetConVar")) {
            native::Vector vector{};
            if(memory::Field(msg,native::Payload,vector)&&vector.size>=0&&vector.size<=64&&vector.size<=vector.allocated&&
               (!vector.size||(vector.data&&reinterpret_cast<uintptr_t>(vector.data)<=UINTPTR_MAX-64*sizeof(native::Cvar)))) {
                std::array<native::Cvar,64> copy{};int count{};bool changed{};
                for(int i=0;i<vector.size;++i) {
                    native::Cvar item{};
                    if(!memory::Read(static_cast<native::Cvar*>(vector.data)+i,item)||
                       !std::memchr(item.name,0,sizeof(item.name))||!std::memchr(item.value,0,sizeof(item.value)))return original_setting(msg);
                    if(const auto rule=Find(list->settings,item.name)) {
                        changed=true;Log(rule->value.empty()?"setting blocked":"setting substituted",item.name);
                        if(rule->value.empty())continue;
                        std::memset(item.value,0,sizeof(item.value));std::memcpy(item.value,rule->value.data(),rule->value.size());
                    }
                    copy[count++]=item;
                }
                if(changed) {
                    auto replacement=vector;replacement.data=copy.data();replacement.elements=copy.data();
                    replacement.size=count;replacement.allocated=64;replacement.grow=0;Restore restore;
                    if(restore.Write(static_cast<unsigned char*>(msg)+native::Payload,&replacement,sizeof(replacement)))return original_setting(msg);
                }
            }
        }
    } catch(...) {}
    return original_setting(msg);
}
bool __fastcall CommandHook(void* msg,void*) {
    if(!Enabled(Command))return original_command(msg);
    const auto list=rules.load(std::memory_order_acquire);
    if(list&&Enabled(Command)&&Type(msg,5,"net_StringCmd")) {
        const char* command{};char text[4096]{};
        if(memory::Field(msg,native::Payload,command)&&Text(command,text,sizeof(text))&&BlockCommand(text,list->commands)) {
            Log("command batch blocked",text);return true;
        }
    }
    return original_command(msg);
}
bool __fastcall CrosshairHook(void* msg,void*) {
    if(Enabled(Crosshair)&&Type(msg,20,"svc_CrosshairAngle")){Log("angle blocked","svc_CrosshairAngle");return true;}
    return original_crosshair(msg);
}
bool __fastcall DispatchHook(void* self,void*,int id,void* data) {
    if(!Enabled(Message))return original_dispatch(self,id,data);
    const auto list=rules.load(std::memory_order_acquire);
    if(list&&Enabled(Message)&&id>=0&&static_cast<unsigned>(id)<message_count&&Listed(list->messages,message_names[id].data())) {
        Log("user message blocked",message_names[id].data());return true;
    }
    return original_dispatch(self,id,data);
}
void __fastcall SoundHook(void* self,void*,void* filter,int entity,int channel,const char* sample,float volume,
                         int level,int flags,int pitch,const void* origin,const void* direction,void* origins,bool update,float time,int speaker) {
    if(Enabled(Null|Heart)) {
        char text[1024]{};
        // Forward STOP messages: existing looping sounds must still be stopped.
        if(!(flags&((1<<2)|(1<<5)))&&Text(sample,text,sizeof(text))&&
           ((Enabled(Null)&&NullSound(text))||(Enabled(Heart)&&Heartbeat(text)))) {Log("sound blocked",text);return;}
    }
    original_sound(self,filter,entity,channel,sample,volume,level,flags,pitch,origin,direction,origins,update,time,speaker);
}
bool Hook(void* target,void* detour,void** original,memory::Range image,void*& installed_target) noexcept {
    if(!image.Contains(target)||!memory::Executable(target))return false;
    if(MH_CreateHook(target,detour,original)!=MH_OK)return false;
    if(MH_EnableHook(target)==MH_OK){installed_target=target;return true;}
    MH_RemoveHook(target);return false;
}
}
bool SetRules(const Rules& value) noexcept {
    if(!Valid(value))return false;
    try {rules.store(std::make_shared<const Rules>(value),std::memory_order_release);return true;}catch(...){return false;}
}
std::shared_ptr<const Rules> GetRules() noexcept {return rules.load(std::memory_order_acquire);}
void SetOptions(Options value) noexcept {
    options.store((value.queries?Query:0)|(value.settings?Setting:0)|(value.commands?Command:0)|(value.messages?Message:0)|
        (value.crosshair?Crosshair:0)|(value.crc?Crc:0)|(value.null_sound?Null:0)|(value.heartbeat?Heart:0)|(value.logging?Logging:0),std::memory_order_release);
}
void Suspend() noexcept {options.store(0,std::memory_order_release);}
void Shutdown() noexcept {
    Suspend();
    for(void* target:{installed.send,installed.settings,installed.command,installed.crosshair,installed.dispatch,installed.sound})
        if(target){MH_DisableHook(target);MH_RemoveHook(target);}
    installed={};
}
bool ResolveMessages(void* registration,memory::Range image) noexcept {
    message_names={};message_count=0;
    auto cursor=static_cast<unsigned char*>(registration);void* registrar{};void* manager{};
    for(unsigned i=0;i<message_names.size();++i,cursor+=18) {
        unsigned char code[18]{};
        if(!image.Contains(cursor,sizeof(code))||!memory::Copy(code,cursor,sizeof(code)))return false;
        if(code[0]==0xc3) {message_count=i;return i>46;}
        const char* name{};void* location{};int displacement{};unsigned k{},mask{};
        while(k<13) {
            if(k+6<=13&&code[k]==0x8b&&code[k+1]==0x0d&&!(mask&1)) {std::memcpy(&location,code+k+2,4);k+=6;mask|=1;}
            else if(k+2<=13&&code[k]==0x6a&&!(mask&2)) {k+=2;mask|=2;}
            else if(k+5<=13&&code[k]==0x68&&!(mask&4)) {std::memcpy(&name,code+k+1,4);k+=5;mask|=4;}
            else return false;
        }
        if(k!=13||mask!=7||code[13]!=0xe8)return false;
        std::memcpy(&displacement,code+14,4);
        auto call=cursor+18+displacement;
        if(i&&(registrar!=call||manager!=location))return false;
        registrar=call;manager=location;
        if(!image.Contains(call)||!image.Contains(location,4)||!memory::String(image,name,message_names[i].data(),message_names[i].size()))return false;
    }
    return false;
}
Availability Install(Targets targets,memory::Range engine,memory::Range client,bool(*active)()) noexcept {
    Shutdown();is_active=active;engine_image=engine;
    if(!GetRules()) {try{SetRules(DefaultRules());}catch(...){}}
    Availability a{};
    a.send=Hook(targets.send,reinterpret_cast<void*>(&SendHook),reinterpret_cast<void**>(&original_send),engine,installed.send);
    a.settings=Hook(targets.settings,reinterpret_cast<void*>(&SettingHook),reinterpret_cast<void**>(&original_setting),engine,installed.settings);
    a.command=Hook(targets.command,reinterpret_cast<void*>(&CommandHook),reinterpret_cast<void**>(&original_command),engine,installed.command);
    a.crosshair=Hook(targets.crosshair,reinterpret_cast<void*>(&CrosshairHook),reinterpret_cast<void**>(&original_crosshair),engine,installed.crosshair);
    a.messages=Hook(targets.dispatch,reinterpret_cast<void*>(&DispatchHook),reinterpret_cast<void**>(&original_dispatch),client,installed.dispatch);
    a.sound=Hook(targets.sound,reinterpret_cast<void*>(&SoundHook),reinterpret_cast<void**>(&original_sound),engine,installed.sound);
    return a;
}
}
