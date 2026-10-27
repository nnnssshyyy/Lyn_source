#pragma once
#include "rapid_fire.h"

namespace combat_input {
// A pin pauses assistance, not the player's native primary input. Only a
// positively resolved semi-auto weapon may have that input pulsed; unknown
// weapons and held automatic weapons must retain it unchanged.
inline int PausedPrimary(int buttons,bool pulse_enabled,bool semi,
                         rapid_fire::PistolEdge& edge) noexcept {
    if(!pulse_enabled || !semi) {
        edge.Next(false);
        return buttons;
    }
    const bool expose=edge.Next((buttons&1)!=0);
    return (buttons&~1)|(expose?1:0);
}
}
