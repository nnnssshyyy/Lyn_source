#include "visuals.h"
#include "visual_events.h"
#include <cstdio>
#include <cstring>

namespace licensing {bool Active() noexcept {return true;}}
namespace {
unsigned checks{},failures{},registrations{};
void Expect(bool pass,const char* label) {++checks;if(!pass){++failures;std::printf("FAIL: %s\n",label);}}
struct Listener {virtual ~Listener()=default;virtual void FireGameEvent(void*) noexcept=0;virtual int GetEventDebugID()=0;};
Listener* listener{};
void* manager_table[6]{};void* engine_table[20]{};void* event_table[10]{};
struct Object {void** table;};
Object manager{manager_table},engine{engine_table};
struct Event {
    void** table=event_table;const char* name="player_hurt_concise";
    int attacker{},attacker_entity=1,victim=9,entity{},amount=18;bool head{},aborted{},position{};
} event;
int local_user=77;
int __fastcall Maximum(void*,void*) {return 2;}
bool __fastcall Info(void*,void*,int index,source::PlayerInfo* info) {*info={};info->userid=index==1?local_user:9;return true;}
void __fastcall Angles(void*,void*,source::Vec3* out) {*out={};}
bool __fastcall Register(void*,void*,Listener* value,const char*,bool) {listener=value;++registrations;return true;}
void __fastcall Remove(void*,void*,Listener*) {listener=nullptr;}
const char* __fastcall Name(void* self,void*) {return static_cast<Event*>(self)->name;}
int __fastcall Integer(void* self,void*,const char* key,int fallback) {
    const auto& e=*static_cast<Event*>(self);
    if(!std::strcmp(key,"attacker"))return e.attacker;
    if(!std::strcmp(key,"attackerentid"))return e.attacker_entity;
    if(!std::strcmp(key,"userid"))return e.victim;
    if(!std::strcmp(key,"entityid"))return e.entity;
    if(!std::strcmp(key,"dmg_health")||!std::strcmp(key,"amount"))return e.amount;
    return fallback;
}
bool __fastcall Boolean(void* self,void*,const char* key,bool fallback) {
    const auto& e=*static_cast<Event*>(self);
    if(!std::strcmp(key,"headshot"))return e.head;
    if(!std::strcmp(key,"abort"))return e.aborted;
    return fallback;
}
float __fastcall Real(void* self,void*,const char* key,float fallback) {
    if(!static_cast<Event*>(self)->position)return fallback;
    if(!std::strcmp(key,"victim_x"))return 10.f;
    if(!std::strcmp(key,"victim_y"))return 20.f;
    if(!std::strcmp(key,"victim_z"))return 30.f;
    return fallback;
}
unsigned Bursts() {unsigned count{};for(const auto& item:visuals::ReadSnapshot().deaths.items)count+=item.time!=0;return count;}
}
int main() {
    manager_table[3]=reinterpret_cast<void*>(&Register);manager_table[5]=reinterpret_cast<void*>(&Remove);
    engine_table[19]=reinterpret_cast<void*>(&Angles);
    event_table[1]=reinterpret_cast<void*>(&Name);event_table[5]=reinterpret_cast<void*>(&Boolean);
    event_table[6]=reinterpret_cast<void*>(&Integer);event_table[8]=reinterpret_cast<void*>(&Real);
    visuals::InitializeEventFixture(&manager,memory::Module(GetModuleHandleW(nullptr)));
    game::Api api{};api.base.engine=&engine;api.base.max_clients=reinterpret_cast<source::GetInt>(&Maximum);
    api.base.player_info=reinterpret_cast<source::GetInfo>(&Info);
    // No entity provider: this reproduces the previous identity failure when
    // render entities are unavailable, independently from engine player info.
    game::Entity local{};local.index=1;local.team=2;
    source::Matrix matrix{};matrix.m[3][3]=1;
    features::Config cfg{};cfg.esp=false;cfg.hitmarker=cfg.damage_numbers=cfg.death_particles=true;
    const auto capture=[&](){visuals::Capture(api,cfg,{},local,matrix,true);};
    const auto emit=[&](){if(listener)listener->FireGameEvent(&event);capture();};
    capture();Expect(registrations==visuals::events::EventCount&&listener,"all HUD event listeners register without ESP");
    emit();auto first=visuals::ReadSnapshot();
    Expect(first.valid&&first.damage.hit_time&&first.damage.items[0].amount==18,"concise impact survives missing local and victim render entities");
    event.name="player_hurt";event.attacker=77;emit();
    Expect(visuals::ReadSnapshot().damage.items[0].amount==18,"detailed counterpart does not double the concise damage");
    event.name="round_start";emit();Expect(!visuals::ReadSnapshot().damage.hit_time&&Bursts()==0,"round reset removes stale effects");
    event.name="player_hurt_concise";event.attacker=0;event.attacker_entity=2;emit();
    Expect(!visuals::ReadSnapshot().damage.hit_time,"another attacker's entity index cannot trigger our hitmarker");
    event.name="player_death";event.attacker=77;event.victim=0;event.entity=121;event.position=true;event.head=true;
    emit();auto death=visuals::ReadSnapshot();
    Expect(Bursts()==1&&death.deaths.items[0].point.z==60,"NPC death creates particles at the event position even after despawn");
    Expect(death.damage.hit_time&&death.damage.head&&!death.damage.items[0].last,"kill confirms the marker without fabricating damage amounts");
    emit();Expect(Bursts()==1,"duplicate death notification creates only one burst");
    event.victim=9;event.entity=0;event.attacker=0;event.attacker_entity=0;emit();
    Expect(Bursts()==2,"another player's death also produces its cosmetic effect");
    event.victim=11;event.position=false;emit();Expect(Bursts()==2,"missing position does not create particles at the world origin");
    event.position=true;event.aborted=true;emit();Expect(Bursts()==2,"aborted death does not create a burst");
    event.aborted=false;event.name="infected_death";event.attacker=77;emit();
    Expect(Bursts()==2,"infected death summary does not duplicate world particles");
    cfg.death_particles=false;capture();Expect(Bursts()==0,"disabling the effect clears its existing particles");
    visuals::ClearLevel();Expect(listener==nullptr&&!visuals::ReadSnapshot().valid,"level cleanup unregisters and clears the published snapshot");
    std::printf("%u visual runtime checks, %u failed\n",checks,failures);return failures?1:0;
}
