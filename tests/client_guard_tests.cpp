#include "client_guard.h"
#include <MinHook.h>
#include <array>
#include <cstdio>
#include <cstring>
#include <random>

using namespace client_guard;
namespace {
unsigned checks{},failures{},sends{},sets{},commands{},crosshairs{},dispatches{},sounds{};
bool running=true,reliable_seen{},voice_seen{};void* self_seen{};void* data_seen{};
std::string value_seen,sample_seen;std::vector<native::Cvar> settings_seen;
bool Active(){return running;}
void Expect(bool ok,const char* text){++checks;if(!ok){++failures;std::printf("FAIL: %s\n",text);}}
struct Msg {void** table{};unsigned char base[80]{};unsigned cookie{};const char* name{};const char* value{};int status{};};
static_assert(offsetof(Msg,cookie)==84&&offsetof(Msg,value)==92);
int __fastcall QueryType(void*,void*){return 13;}int __fastcall CrcType(void*,void*){return 14;}
int __fastcall SetType(void*,void*){return 6;}int __fastcall CmdType(void*,void*){return 5;}
int __fastcall CrossType(void*,void*){return 20;}
const char* __fastcall QueryName(void*,void*){return "clc_RespondCvarValue";}
const char* __fastcall CrcName(void*,void*){return "clc_FileCRCCheck";}
const char* __fastcall SetName(void*,void*){return "net_SetConVar";}
const char* __fastcall CmdName(void*,void*){return "net_StringCmd";}
const char* __fastcall CrossName(void*,void*){return "svc_CrosshairAngle";}
__declspec(noinline) bool __fastcall NativeSend(void* self,void*,void* ptr,bool reliable,bool voice) {
    ++sends;self_seen=self;reliable_seen=reliable;voice_seen=voice;value_seen=static_cast<Msg*>(ptr)->value;return false;
}
__declspec(noinline) bool __fastcall NativeSet(void* msg,void*) {
    ++sets;native::Vector vector{};memory::Field(msg,native::Payload,vector);settings_seen.clear();
    if(vector.size>=0&&vector.size<=64&&vector.data)for(int i=0;i<vector.size;++i)settings_seen.push_back(static_cast<native::Cvar*>(vector.data)[i]);
    return false;
}
__declspec(noinline) bool __fastcall NativeCommand(void* msg,void*){++commands;data_seen=msg;return false;}
__declspec(noinline) bool __fastcall NativeCrosshair(void* msg,void*){++crosshairs;data_seen=msg;return false;}
__declspec(noinline) bool __fastcall NativeDispatch(void* self,void*,int id,void* data){++dispatches;self_seen=self;data_seen=data;return id<0;}
__declspec(noinline) void __fastcall NativeSound(void* self,void*,void* filter,int entity,int channel,const char* sample,float volume,
    int level,int /*flags*/,int pitch,const void* origin,const void* direction,void* origins,bool update,float time,int speaker) {
    ++sounds;self_seen=self;data_seen=filter;sample_seen=sample?sample:"";
    Expect(entity==7&&channel==3&&volume==.75f&&level==75&&pitch==100&&origin==direction&&origins==filter&&update&&time==1.25f&&speaker==-1,"sound hook preserves all x86 stack arguments");
}
bool SlowMatch(std::string_view text,std::string_view pattern) {
    std::vector<bool> previous(pattern.size()+1),next(pattern.size()+1);previous[0]=true;
    for(size_t j=0;j<pattern.size();++j)previous[j+1]=previous[j]&&pattern[j]=='*';
    for(char c:text) {next.assign(next.size(),false);for(size_t j=0;j<pattern.size();++j)
        next[j+1]=pattern[j]=='*'?(next[j]||previous[j+1]):previous[j]&&(pattern[j]=='?'||Lower(c)==Lower(pattern[j]));previous.swap(next);}
    return previous.back();
}
std::array<unsigned char,65*18+18> registration{};
std::array<std::array<char,32>,65> names{};
unsigned manager{};
void Registry() {
    for(unsigned i=0;i<65;++i) {
        std::snprintf(names[i].data(),names[i].size(),"Message%u",i);
        if(i==11)strcpy_s(names[i].data(),names[i].size(),"Shake");if(i==12)strcpy_s(names[i].data(),names[i].size(),"Fade");if(i==46)strcpy_s(names[i].data(),names[i].size(),"BlurFade");
        auto* c=registration.data()+i*18;const void* location=&manager;const char* name=names[i].data();
        const void* registrar=reinterpret_cast<void*>(&NativeSet);const auto relative=static_cast<int>(reinterpret_cast<intptr_t>(registrar)-reinterpret_cast<intptr_t>(c+18));
        const std::array<unsigned char,18> code{0x8b,0x0d,0,0,0,0,0x6a,0xff,0x68,0,0,0,0,0xe8,0,0,0,0};
        std::memcpy(c,code.data(),18);std::memcpy(c+2,&location,4);std::memcpy(c+9,&name,4);std::memcpy(c+14,&relative,4);
        if(i%3==1) {std::array<unsigned char,8> first{};std::memcpy(first.data(),c,8);c[0]=first[6];c[1]=first[7];std::memcpy(c+2,first.data(),6);}
        if(i%3==2) {std::array<unsigned char,13> first{};std::memcpy(first.data(),c,13);std::memcpy(c,first.data()+6,2);std::memcpy(c+2,first.data()+8,5);std::memcpy(c+7,first.data(),6);}
    }
    registration[65*18]=0xc3;
}
}
int main() {
    auto r=DefaultRules();Expect(Valid(r)&&r.queries.size()==25&&r.settings.size()==2,"supplied lists are ported and valid");
    Expect(Match("SV_Pure","sv_pure")&&!Match("sv_pure_extra","sv_pure"),"names compare case-insensitively and in full");
    Expect(Match("","*")&&Match("abcdef","a*?f")&&!Match("abc","a*d"),"wildcards terminate and support zero-or-more characters");
    std::mt19937 random(42);bool correct=true;
    for(int i=0;i<12000;++i) {std::string a,b;for(unsigned j=random()%12;j;--j)a+="abCD"[random()%4];for(unsigned j=random()%12;j;--j)b+="abCD*?"[random()%6];correct=correct&&(Match(a,b)==SlowMatch(a,b));}
    Expect(correct,"12000 randomized wildcard cases agree with an independent dynamic-programming matcher");
    for(const auto text:{"bind x y","  ExEc\tfoo.cfg","echo ok; bind x y","echo ok\nexec a","// ignored\ncl_consistencycheck","\"bind\" x y","echo \"foo;bar\"; exec a"})
        Expect(BlockCommand(text,r.commands),"blocked commands are recognized at every batch boundary");
    for(const auto text:{"echo \"exec;bind\"","unbind x","execution a","echo ok // bind x y","echo \"hello\nworld\"","say bind","//bind\necho fine"})
        Expect(!BlockCommand(text,r.commands),"arguments, quoted separators, comments and prefixes are preserved");
    Expect(NullSound("common/NULL.wav")&&!NullSound("weapons/nullification.wav"),"null sound filter matches only the null basename");
    Expect(Heartbeat("player/HeartbeatLoop.wav")&&!Heartbeat("player/heartbeat.wav"),"heartbeat loop filter is independent");
    auto invalid=r;invalid.commands.emplace_back("bad;exec");Expect(!Valid(invalid)&&!SetRules(invalid),"malformed configuration cannot replace rules");
    Expect(SetRules(r),"validated rules publish atomically");Registry();const auto image=memory::Module(GetModuleHandleW(nullptr));
    Expect(ResolveMessages(registration.data(),image),"native registry recognizes all three compiler instruction orders");
    Expect(MH_Initialize()==MH_OK,"native hook backend initializes");
    Targets targets{reinterpret_cast<void*>(&NativeSend),reinterpret_cast<void*>(&NativeSet),reinterpret_cast<void*>(&NativeCommand),
        reinterpret_cast<void*>(&NativeCrosshair),reinterpret_cast<void*>(&NativeDispatch),reinterpret_cast<void*>(&NativeSound)};
    auto a=Install(targets,image,image,&Active);Expect(a.send&&a.settings&&a.command&&a.crosshair&&a.messages&&a.sound,"all six ABI routes install");
    std::array<void*,11> query{},crc{},setting{},command{},cross{};
    for(auto* table:{&query,&crc,&setting,&command,&cross})table->fill(reinterpret_cast<void*>(&QueryType));
    query[8]=reinterpret_cast<void*>(&QueryType);query[10]=reinterpret_cast<void*>(&QueryName);
    crc[8]=reinterpret_cast<void*>(&CrcType);crc[10]=reinterpret_cast<void*>(&CrcName);
    setting[8]=reinterpret_cast<void*>(&SetType);setting[10]=reinterpret_cast<void*>(&SetName);
    command[8]=reinterpret_cast<void*>(&CmdType);command[10]=reinterpret_cast<void*>(&CmdName);
    cross[8]=reinterpret_cast<void*>(&CrossType);cross[10]=reinterpret_cast<void*>(&CrossName);
    Msg msg{query.data()};msg.name="sv_pure";msg.value="7";const auto original=msg.value;
    int owner{};Expect(!NativeSend(&owner,nullptr,&msg,true,false)&&value_seen=="7","disabled query filtering preserves native return and contents");
    SetOptions({true,true,true,true,true,true,true,true,false});
    Expect(!NativeSend(&owner,nullptr,&msg,true,false)&&value_seen=="2"&&self_seen==&owner&&reliable_seen&&!voice_seen,"query replacement reaches the real serializer with unchanged transport arguments");
    Expect(msg.value==original,"serializer substitution restores the caller's pointer");
    msg.name="unknown";NativeSend(&owner,nullptr,&msg,false,true);Expect(value_seen=="7"&&!reliable_seen&&voice_seen,"unlisted queries and voice routing pass through untouched");
    r.queries={{"sv_*",""}};SetRules(r);msg.name="sv_test";auto before=sends;
    Expect(NativeSend(&owner,nullptr,&msg,false,false)&&sends==before&&msg.value==original,"an empty replacement suppresses only the matched response");
    msg.table=crc.data();Expect(NativeSend(&owner,nullptr,&msg,false,false)&&sends==before,"CRC suppression actually skips the outgoing function");
    crc[10]=reinterpret_cast<void*>(&QueryName);NativeSend(&owner,nullptr,&msg,false,false);Expect(sends==before+1,"a type/name mismatch fails closed");crc[10]=reinterpret_cast<void*>(&CrcName);
    struct SetMsg {void** table;unsigned char base[80]{};native::Vector list;};SetMsg set{setting.data()};
    std::array<native::Cvar,3> cvars{};strcpy_s(cvars[0].name,"sv_pure");strcpy_s(cvars[0].value,"2");strcpy_s(cvars[1].name,"keep");strcpy_s(cvars[1].value,"yes");strcpy_s(cvars[2].name,"remove");strcpy_s(cvars[2].value,"no");
    r=DefaultRules();r.settings.emplace_back(Rule{"remove",""});SetRules(r);set.list={cvars.data(),3,0,3,cvars.data()};const auto old_list=set.list;
    Expect(!NativeSet(&set,nullptr)&&settings_seen.size()==2&&std::strcmp(settings_seen[0].value,"0")==0&&std::strcmp(settings_seen[1].name,"keep")==0,"setting override and removal reach the native consumer in one filtered list");
    Expect(!std::memcmp(&old_list,&set.list,sizeof(old_list))&&!std::strcmp(cvars[0].value,"2"),"native vector and original server values are restored without allocator ownership changes");
    set.list.size=65;before=sets;NativeSet(&set,nullptr);Expect(sets==before+1,"oversized native vectors are forwarded without reading unbounded elements");set.list=old_list;
    Msg cmd{command.data()};const char* text="echo ok;exec bad.cfg";std::memcpy(&cmd.cookie,&text,4);before=commands;
    Expect(NativeCommand(&cmd,nullptr)&&commands==before,"chained server command is suppressed before handler execution");
    text="echo ok";std::memcpy(&cmd.cookie,&text,4);Expect(!NativeCommand(&cmd,nullptr)&&commands==before+1,"ordinary command returns the handler's result");
    Msg cm{cross.data()};Expect(NativeCrosshair(&cm,nullptr)&&crosshairs==0,"crosshair network message is filtered on its separate dispatcher");
    int payload{};Expect(NativeDispatch(&owner,nullptr,11,&payload)&&dispatches==0,"Shake is resolved by name rather than a network message ID");
    NativeDispatch(&owner,nullptr,14,&payload);NativeDispatch(&owner,nullptr,20,&payload);
    Expect(dispatches==2&&data_seen==&payload&&self_seen==&owner,"unrelated user IDs 14 and 20 are preserved");
    NativeDispatch(&owner,nullptr,46,&payload);Expect(dispatches==2,"BlurFade uses the installed registry name");
    const auto sound=[&](const char* sample,int flags=0){NativeSound(&owner,nullptr,&payload,7,3,sample,.75f,75,flags,100,&owner,&owner,&payload,true,1.25f,-1);};
    sound("common/null.wav");sound("player/heartbeatloop.wav");Expect(sounds==0,"null and heartbeat starts are suppressed without invalid pitch or volume");
    sound("common/null.wav",1<<2);sound("ambient/normal.wav");Expect(sounds==2,"stop-loop requests and ordinary sounds reach the original sound ABI");
    running=false;before=sends;msg.table=query.data();msg.name="sv_pure";NativeSend(&owner,nullptr,&msg,false,false);
    Expect(sends==before+1&&value_seen=="7","license/runtime deactivation immediately forwards original query contents");
    NativeDispatch(&owner,nullptr,11,&payload);Expect(dispatches==3,"inactive runtime preserves all user messages");running=true;
    Suspend();NativeCrosshair(&cm,nullptr);Expect(crosshairs==1,"suspending restores crosshair behavior immediately");
    Shutdown();before=sends;NativeSend(&owner,nullptr,&msg,false,false);Expect(sends==before+1&&value_seen=="7","shutdown restores executable routes");
    registration[0]=0;Expect(!ResolveMessages(registration.data(),image),"unknown registry bytecode disables name-based filtering");
    a=Install({},image,image,&Active);Expect(!a.send&&!a.messages&&!a.sound,"missing targets report unavailable instead of installing partial guesses");
    Shutdown();MH_Uninitialize();std::printf("%u client guard checks (plus 12000 matcher cases), %u failed\n",checks,failures);return failures?1:0;
}
