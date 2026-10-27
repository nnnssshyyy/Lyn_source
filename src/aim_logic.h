#pragma once
#include <algorithm>
#include <cmath>
#include <string_view>

namespace aimbot::logic {
inline bool DeathSequence(std::string_view name) noexcept {
    // Common-infected death poses retain their collision for part of the
    // animation. Labels are model-specific, so do not hardcode sequence IDs.
    constexpr std::string_view death="death";
    for(size_t i=0;i+death.size()<=name.size();++i) {
        size_t j{};
        for(;j<death.size();++j) {
            char c=name[i+j];if(c>='A' && c<='Z') c=static_cast<char>(c-'A'+'a');
            if(c!=death[j]) break;
        }
        if(j==death.size()) return true;
    }
    return false;
}
inline unsigned Mask(int preset,unsigned custom) noexcept {
    switch(preset) {
        case 1:return 1;
        case 2:return 6;
        case 3:return 127;
        case 4:return custom&127;
        default:return 0;
    }
}
inline bool Allowed(unsigned mask,int group,int preferred,bool automatic) noexcept {
    if(group<1 || group>7) return false;
    // Automatic orders the preferred group first; it must still fall back to
    // another visible hitbox when that zone is covered or absent on a model.
    (void)preferred;
    return automatic || (mask&(1u<<(group-1)))!=0;
}
inline bool ValidRecord(float now,float simulation,float interpolation,float interval,float window) noexcept {
    if(!std::isfinite(now) || !std::isfinite(simulation) || !std::isfinite(interpolation) ||
       !std::isfinite(interval) || interval<=0.f || !std::isfinite(window)) return false;
    const float age=now-simulation;
    return simulation>0.f && age>=-interval && age<=window && std::abs(interpolation-age)<=.2f;
}
inline float SmoothFraction(float percent,float interval=1.f/30.f) noexcept {
    if(!std::isfinite(percent)) percent=0.f;
    if(!std::isfinite(interval) || interval<=0.f) interval=1.f/30.f;
    const float base=std::clamp(1.f-percent/100.f,.01f,1.f);
    // Preserve the existing 30 Hz response at other command rates.
    return 1.f-std::pow(1.f-base,std::clamp(interval,.001f,.1f)*30.f);
}
inline bool ClearAdvantage(float challenger,float retained,int priority) noexcept {
    const float margin=priority==0?std::max(.35f,retained*.15f):std::max(24.f,retained*.1f);
    return challenger+margin<retained;
}
inline bool Visible(float fraction,bool start_solid,bool all_solid,bool hit_target) noexcept {
    return !start_solid && !all_solid && (hit_target || fraction>=.999f);
}
inline bool ShotVisible(float fraction,bool start_solid,bool all_solid,bool hit_target,bool historical) noexcept {
    // Live bones must agree with the engine's hit trace. A ray that merely
    // reaches an empty/stale point is not a confirmed live shooting solution.
    return std::isfinite(fraction) && fraction>=0.f && fraction<=1.f && !start_solid && !all_solid &&
        (hit_target || (historical && fraction==1.f));
}
inline bool Aligned(float error,float tolerance) noexcept {
    return std::isfinite(error) && std::isfinite(tolerance) && error>=0.f && tolerance>=0.f && error<=tolerance;
}
enum class PacketAction {Keep,Hold,Send};
inline PacketAction SilentPacket(int mode,bool aimed_attack,bool flush,bool burst,bool force) noexcept {
    // Decide once: marking a flush for the NEXT command must not immediately
    // overwrite this command's hold with send=true.
    if(force) return PacketAction::Send;
    return mode==2 && aimed_attack && !flush && !burst ? PacketAction::Hold : PacketAction::Keep;
}
inline int ShotButtons(int buttons,int mode,bool selected,bool flush,float error,float tolerance) noexcept {
    // A server-silent flush restores the mouse view and must not fire another
    // unaligned bullet. Normal manual fire waits for the smoothed solution too.
    return (mode==2 && flush) || (mode==0 && selected && !Aligned(error,tolerance)) ? buttons&~1 : buttons;
}
inline float ShotTolerance(float distance,float radius) noexcept {
    if(!std::isfinite(distance) || distance<=0.f || !std::isfinite(radius) || radius<=0.f) return 0.f;
    return std::min(.5f,std::atan2(radius*.75f,distance)*57.2957795f);
}
inline bool HistoricalTick(bool historical,float pose_time,float lerp,float interval,int original,
                           float now,float correction,float window,int& tick) noexcept {
    if(!historical || !ValidRecord(now,pose_time,correction,interval,window) ||
       !std::isfinite(lerp) || lerp<0.f || lerp>1.f || original<0) return false;
    const double candidate=std::round((static_cast<double>(pose_time)+lerp)/interval);
    // A time bound scales with tickrate; the previous 64 ticks meant over two
    // seconds at 30 Hz and a different policy at every other rate.
    if(candidate<0 || candidate>2147483647. || std::abs(candidate-original)*interval>window+lerp+interval*2.f) return false;
    tick=static_cast<int>(candidate);return true;
}
}
