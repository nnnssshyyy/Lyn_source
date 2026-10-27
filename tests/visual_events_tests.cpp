#include "visual_events.h"
#include <cstdio>
namespace {
void* table[10]{};
struct Mock {void** vtable=table;const char* name="player_hurt";int attacker=5,victim=8,entity=123,damage=12,amount=25,hitgroup=1,subject=9,attacker_entity=2;bool positioned=true,aborted=false;} event;
const char* __fastcall Name(void* self,void*) {return static_cast<Mock*>(self)->name;}
int __fastcall Integer(void* self,void*,const char* key,int fallback) {
    const auto& e=*static_cast<Mock*>(self);
    if(!std::strcmp(key,"attacker"))return e.attacker;
    if(!std::strcmp(key,"attackerentid"))return e.attacker_entity;
    if(!std::strcmp(key,"userid"))return e.victim;
    if(!std::strcmp(key,"entityid"))return e.entity;
    if(!std::strcmp(key,"dmg_health"))return e.damage<0?fallback:e.damage;
    if(!std::strcmp(key,"amount"))return e.amount;
    if(!std::strcmp(key,"hitgroup"))return e.hitgroup;
    if(!std::strcmp(key,"subject"))return e.subject;
    return fallback;
}
bool __fastcall Boolean(void* self,void*,const char* key,bool fallback) {
    if(!std::strcmp(key,"headshot"))return true;
    if(!std::strcmp(key,"abort"))return static_cast<Mock*>(self)->aborted;
    return fallback;
}
float __fastcall Real(void* self,void*,const char* key,float fallback) {
    if(!static_cast<Mock*>(self)->positioned)return fallback;
    if(!std::strcmp(key,"victim_x"))return 0.f;
    if(!std::strcmp(key,"victim_y"))return 12.f;
    if(!std::strcmp(key,"victim_z"))return 25.f;
    return fallback;
}
unsigned checks{},failed{};
void expect(bool pass,const char* name){++checks;if(!pass){++failed;std::printf("FAIL: %s\n",name);}}
}
int main(){
    table[1]=reinterpret_cast<void*>(&Name);table[6]=reinterpret_cast<void*>(&Integer);
    table[5]=reinterpret_cast<void*>(&Boolean);table[8]=reinterpret_cast<void*>(&Real);
    const auto code=memory::Module(GetModuleHandleW(nullptr));visuals::events::Raw out{};
    expect(visuals::events::Decode(&event,code,out)&&out.attacker==5&&out.victim==8&&out.amount==12&&out.head,"player_hurt decodes the x86 event ABI");
    event.damage=-1;expect(visuals::events::Decode(&event,code,out)&&out.amount==25,"amount fallback used only when dmg_health is absent");
    event.name="infected_hurt";expect(visuals::events::Decode(&event,code,out)&&out.victim==123&&out.amount==25,"infected_hurt uses entity ID and amount");
    event.name="revive_begin";expect(visuals::events::Decode(&event,code,out)&&out.victim==8&&out.subject==9&&out.kind==visuals::events::ReviveBegin,"revive actor and subject retain user IDs");
    event.name="heal_success";expect(visuals::events::Decode(&event,code,out)&&out.kind==visuals::events::HealSuccess,"heal completion clears the matching action");
    event.name="player_hurt_concise";event.attacker=0;event.damage=18;
    expect(visuals::events::Decode(&event,code,out)&&out.attacker_entity==2&&out.amount==18,"networked concise damage decodes the attacker entity index");
    expect(visuals::events::Attacker(out,99,2)==99&&visuals::events::Attacker(out,99,3)==0,"entity index is resolved only against our actual slot");
    out.attacker=7;expect(visuals::events::Attacker(out,99,2)==7,"another player's explicit user ID takes precedence over entity fallback");
    event.name="player_death";event.victim=0;
    expect(visuals::events::Decode(&event,code,out)&&out.entity==123&&out.position&&out.x==0&&out.y==12&&out.z==25&&out.head,"NPC deaths retain entity ID, real coordinates and boolean headshot");
    event.aborted=true;event.positioned=false;
    expect(visuals::events::Decode(&event,code,out)&&out.aborted&&!out.position,"aborted deaths and absent coordinates are not invented positions");
    event.name="infected_death";
    expect(visuals::events::Decode(&event,code,out)&&!out.position&&out.kind==visuals::events::InfectedDeath,"infected death confirms a kill without treating infected_id as a world position");
    using namespace visuals::events;
    DamageDuplicates duplicate{};Raw detailed{};detailed.kind=PlayerHurt;detailed.attacker=7;detailed.victim=9;detailed.amount=20;detailed.time=1000;
    expect(!duplicate.Consume(detailed),"first damage event is retained");
    Raw concise=detailed;concise.kind=PlayerHurtConcise;concise.time=1010;
    expect(duplicate.Consume(concise)&&!duplicate.Consume(concise),"each detailed event suppresses only one concise counterpart");
    detailed.time=1100;expect(duplicate.Consume(detailed),"concise-first arrival also pairs once");
    detailed.time=1300;expect(!duplicate.Consume(detailed)&&!duplicate.Consume(detailed),"equal same-format pellets remain separate hits");
    concise.time=1500;expect(!duplicate.Consume(concise),"later shots cannot pair with stale events");
    event.name="unrelated";expect(!visuals::events::Decode(&event,code,out),"unrelated events ignored");
    expect(!visuals::events::Decode(nullptr,code,out),"null event rejected");
    event.name="player_hurt";table[6]=nullptr;expect(!visuals::events::Decode(&event,code,out),"missing GetInt vtable method rejected");
    table[6]=reinterpret_cast<void*>(&Integer);expect(!visuals::events::Decode(&event,{code.base+code.size,128},out),"event methods outside the engine image rejected");
    std::printf("%u visual event ABI checks, %u failed\n",checks,failed);return failed?1:0;
}
