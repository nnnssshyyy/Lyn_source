#pragma once
#include "memory.h"
#include <array>
#include <cmath>
#include <cstring>
#include <limits>

namespace visuals::events {
// GAMEEVENTSMANAGER002 / IGameEvent ABI from Valve's L4D2 SDK.
// https://github.com/alliedmodders/hl2sdk/blob/l4d2/public/igameevents.h
enum EventKind {PlayerHurt,InfectedHurt,HealBegin,HealEnd,HealSuccess,ReviveBegin,ReviveEnd,ReviveSuccess,RoundStart,PlayerHurtConcise,PlayerDeath,InfectedDeath,EventCount};
inline constexpr const char* Names[]{"player_hurt","infected_hurt","heal_begin","heal_end","heal_success","revive_begin","revive_end","revive_success","round_start","player_hurt_concise","player_death","infected_death"};
struct Raw {
    int kind{},attacker{},victim{},amount{},head{},subject{},attacker_entity{},entity{};
    std::uint64_t time{};float x{},y{},z{};bool position{},aborted{};
};
inline int Attacker(const Raw& event,int local_user,int local_index) noexcept {
    // User IDs and entity indices are distinct namespaces. Prefer a supplied
    // user ID; concise events only carry the attacker's entity index.
    if(event.attacker>0)return event.attacker;
    return local_index>0&&event.attacker_entity==local_index?local_user:0;
}
struct DamageDuplicates {
    std::array<Raw,64> recent{};unsigned next{};
    bool Consume(const Raw& event) noexcept {
        if(event.kind!=PlayerHurt&&event.kind!=PlayerHurtConcise)return false;
        for(auto& prior:recent)if(prior.time&&prior.kind!=event.kind&&
            prior.attacker==event.attacker&&prior.victim==event.victim&&prior.amount==event.amount&&
            event.time>=prior.time&&event.time-prior.time<=100) {
            prior={};return true;
        }
        recent[next++%recent.size()]=event;return false;
    }
};
inline bool Decode(void* event,memory::Range code,Raw& out) noexcept {
    using Name=const char*(__thiscall*)(void*);
    using Int=int(__thiscall*)(void*,const char*,int);
    using Bool=bool(__thiscall*)(void*,const char*,bool);
    using Float=float(__thiscall*)(void*,const char*,float);
    out={};const char* name{};char label[48]{};
    if(!memory::Invoke(memory::Virtual<Name>(event,1,code),name,event)||!name)return false;
    for(size_t i=0;i<sizeof(label)-1;++i) {if(!memory::Read(name+i,label[i]))return false;if(!label[i])break;}
    int kind=-1;for(int i=0;i<EventCount;++i)if(!std::strcmp(label,Names[i])){kind=i;break;}
    if(kind<0)return false;
    const auto method=memory::Virtual<Int>(event,6,code);
    if(!method)return false;
    const auto get=[&](const char* key,int fallback=0) {int v=fallback;memory::Invoke(method,v,event,key,fallback);return v;};
    out.kind=kind;out.attacker=get("attacker");out.victim=get(kind==InfectedHurt?"entityid":"userid");out.subject=get("subject");
    out.attacker_entity=get("attackerentid");out.entity=get("entityid");
    out.amount=get(kind==InfectedHurt?"amount":"dmg_health",-1);if(out.amount<0)out.amount=get("amount");
    out.head=get("hitgroup")==1;
    if(kind==PlayerDeath||kind==InfectedDeath) {
        const auto boolean=memory::Virtual<Bool>(event,5,code);bool head{},aborted{};
        memory::Invoke(boolean,head,event,"headshot",false);memory::Invoke(boolean,aborted,event,"abort",false);
        out.head=head;out.aborted=aborted;
        if(kind==PlayerDeath) {
            // L4D2's GetUint64 precedes GetFloat. Coordinates are supplied by
            // player_death for players and NPCs (resource/modevents.res).
            const auto real=memory::Virtual<Float>(event,8,code);
            const float missing=std::numeric_limits<float>::quiet_NaN();
            out.x=out.y=out.z=missing;
            memory::Invoke(real,out.x,event,"victim_x",missing);
            memory::Invoke(real,out.y,event,"victim_y",missing);
            memory::Invoke(real,out.z,event,"victim_z",missing);
            out.position=std::isfinite(out.x)&&std::isfinite(out.y)&&std::isfinite(out.z);
        }
    }
    return true;
}
}
