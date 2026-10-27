#pragma once
#include "game.h"
#include <algorithm>

namespace aimbot {
// Covers prediction after aiming/shot commands, including silent cooldowns.
// Rendering always reads fresh input; no angle is frozen in this lease.
struct VisibleCamera {
    unsigned owner{};
    ULONGLONG until{};
    void Reset() noexcept {owner=0;until=0;}
    void Update(unsigned handle,ULONGLONG now) noexcept {owner=handle;until=now+250;}
    bool Active(unsigned handle,ULONGLONG now) const noexcept {return owner && handle==owner && now<until;}
};
inline game::Vec3 VisibleViewAngles(game::Vec3 input,game::Vec3 punch,bool hide_recoil) noexcept {
    if(!hide_recoil && source::Finite(punch) && game::Length(punch)<90.f) input=game::Add(input,punch);
    return {std::clamp(game::Normalize(input.x),-89.f,89.f),game::Normalize(input.y),0.f};
}
}
