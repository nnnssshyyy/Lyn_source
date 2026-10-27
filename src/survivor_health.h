#pragma once
#include <algorithm>
#include <cmath>

namespace survivor_health {
struct Sample {
    int permanent{}, maximum{100}, incap_maximum{300};
    bool incapacitated{};
    float buffer{}, buffer_time{}, game_time{}, decay_rate{.27f};
};
struct Result { int current{}, maximum{}, temporary{}; };

inline Result Calculate(const Sample& s) noexcept {
    const int permanent=std::clamp(s.permanent,0,1000000);
    const int maximum=std::clamp(s.maximum,1,1000000);
    const bool valid_buffer=std::isfinite(s.buffer) && s.buffer>0.f && s.buffer<=1000000.f;
    const float buffer=valid_buffer?s.buffer:0.f;

    const bool valid_clock=std::isfinite(s.game_time) && std::isfinite(s.buffer_time) &&
        s.game_time>=s.buffer_time && s.buffer_time>=0.f;
    const float elapsed=valid_clock?s.game_time-s.buffer_time:0.f;
    const float rate=std::isfinite(s.decay_rate)?std::clamp(s.decay_rate,0.f,100.f):.27f;
    const float remaining=std::max(0.f,buffer-elapsed*rate);
    // C_TerrorPlayer::GetHealth adds its decayed buffer to m_iHealth and
    // converts the sum with cvttss2si (truncate toward zero). Incap bleed-out
    // is already reflected by the server in m_iHealth; it is not subtracted a
    // second time from the temporary-health buffer.
    const int current=static_cast<int>(std::min(1000000.f,permanent+remaining));
    return {current,s.incapacitated?std::max(std::clamp(s.incap_maximum,1,1000000),current):maximum,
            std::max(0,current-permanent)};
}
}
