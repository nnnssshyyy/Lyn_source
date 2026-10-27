#pragma once
#include "features.h"
#include "game.h"
#include <algorithm>

namespace esp::style {
enum Flag : unsigned { Ghost=1,Idle=2,OnFire=4,Friend=8,Incapacitated=16,Pinned=32,Healing=64,Reviving=128,Revived=256 };
struct Info { unsigned flags{}; int rage{}; game::Object object{}; };
inline unsigned PlayerColor(const features::Config& c,source::Kind kind,unsigned flags=0) noexcept {
    if(flags&Ghost) return c.esp_color_ghost;
    if(flags&Friend) return c.esp_color_friend;
    using source::Kind;
    switch(kind) {
        case Kind::Survivor:return c.esp_color_survivor;
        case Kind::Common:return c.esp_color_common;
        case Kind::Smoker:return c.esp_color_smoker;
        case Kind::Boomer:return c.esp_color_boomer;
        case Kind::Hunter:return c.esp_color_hunter;
        case Kind::Spitter:return c.esp_color_spitter;
        case Kind::Jockey:return c.esp_color_jockey;
        case Kind::Charger:return c.esp_color_charger;
        case Kind::Tank:return c.esp_color_tank;
        case Kind::Witch:return c.esp_color_witch;
        default:return c.esp_color_common;
    }
}
inline unsigned ObjectColor(const features::Config& c,game::Object object) noexcept {
    using game::Object;
    switch(object) {
        case Object::Health:return c.world_color_health;
        case Object::Ammo:return c.world_color_ammo;
        case Object::Throwable:return c.world_color_throwable;
        case Object::Projectile:return c.world_color_projectile;
        case Object::Objective:return c.world_color_objective;
        default:return c.world_color_weapon;
    }
}
inline unsigned HealthColor(float fraction,bool bands) noexcept {
    fraction=std::isfinite(fraction)?std::clamp(fraction,0.f,1.f):0.f;
    if(bands) {
        if(fraction>=.75f) return IM_COL32(100,220,172,255);
        if(fraction>=.5f) return IM_COL32(245,198,105,255);
        if(fraction>=.25f) return IM_COL32(246,152,102,255);
        return IM_COL32(244,110,120,255);
    }
    const float t=fraction<.5f?fraction*2.f:(fraction-.5f)*2.f;
    const auto channel=[&](int low,int middle,int high){return static_cast<int>(fraction<.5f?low+(middle-low)*t:middle+(high-middle)*t);};
    return IM_COL32(channel(244,245,100),channel(110,198,220),channel(120,105,172),255);
}
}
