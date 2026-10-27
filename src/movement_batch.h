#pragma once
#include <algorithm>
#include <cmath>
#include "rapid_fire.h"

namespace movement_batch {
// Speedhack batches one movement message. Rapidfire instead replays the
// native CL_Move send path for each extra and keeps its own configured limit.
inline constexpr int MaxExtra=14;
inline constexpr float MaxMultiplier=static_cast<float>(MaxExtra+1);
struct SpeedBudget {
    double fraction{};
    unsigned owner{};
    void Reset() noexcept {fraction=0.;owner=0;}
    int Next(float multiplier,bool enabled,unsigned handle) noexcept {
        if(!enabled || !handle || !std::isfinite(multiplier) || multiplier<=1.f) {Reset();return 0;}
        if(owner!=handle) Reset();
        owner=handle;
        fraction+=static_cast<double>(std::clamp(multiplier,1.f,MaxMultiplier))-1.;
        const int extra=static_cast<int>(std::floor(fraction+1.e-9));
        fraction-=extra;
        return std::clamp(extra,0,MaxExtra);
    }
};
inline int Combined(int speed,int rapid) noexcept {
    // Both features can use the same fresh command; never add their counts.
    return std::max(std::clamp(speed,0,MaxExtra),std::clamp(rapid,0,rapid_fire::MaxExtraMoves));
}
inline bool RapidTick(int index,int rapid) noexcept {return index>0 && index<=rapid;}
template<class Run> void RunExtras(int count,bool final,Run&& run) {
    count=std::clamp(count,0,MaxExtra);
    for(int index=1;index<=count;++index)
        if(!run(index,final && index==count)) break;
}
}
