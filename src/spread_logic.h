#pragma once
#include "game.h"
#include <cmath>

namespace spread_logic {
// Shot identity also includes serial handles: an entity index alone can be
// reused after a weapon is dropped, destroyed, or replaced between commands.
struct Identity {
    int number{},tick{},seed{};
    unsigned player{},weapon{};
    int tickbase{};
    float interval{};
    bool operator==(const Identity&) const = default;
};
struct Sample {
    Identity identity{};
    source::Vec3 offset{},result{};
    bool valid{};
    float strength{1.f};
    bool Matches(const Identity& value) const noexcept {return valid && identity==value;}
};
inline bool Correct(const Sample* previous,const Identity& identity,source::Vec3 input,
                    source::Vec3 offset,source::Vec3& output,float strength=1.f) noexcept {
    if(identity.number<=0 || !identity.player || !identity.weapon ||
       !source::Finite(input) || !source::Finite(offset) ||
       std::abs(offset.x)>90.f || std::abs(offset.y)>90.f ||
       !std::isfinite(strength) || strength<0.f || strength>1.f) return false;
    if(previous && previous->Matches(identity) && input.x==previous->result.x && input.y==previous->result.y) {
        if(previous->strength==strength) {output=input;return true;}
        // Re-prediction can revisit one command after its intensity changes.
        // Recover its original view before applying the newly selected amount.
        if(!source::Finite(previous->offset) || !std::isfinite(previous->strength) ||
           previous->strength<0.f || previous->strength>1.f) return false;
        input.x+=previous->offset.x*previous->strength;
        input.y=std::remainder(input.y+previous->offset.y*previous->strength,360.f);
    }
    const float pitch=input.x-offset.x*strength;
    // Clipping pitch after correction would silently miss the requested axis.
    if(!std::isfinite(pitch) || pitch<-89.f || pitch>89.f) return false;
    output={pitch,std::remainder(input.y-offset.y*strength,360.f),input.z};
    return true;
}
}
