#pragma once
#include "game.h"
#include <cmath>

namespace game {
// The spread updater consumes frame_time. Give it one simulation tick rather
// than the render frame duration, then restore only the globals we own.
// This scope deliberately does not simulate another movement command.
class ScopedShotClock {
    enum : unsigned {Seed=1,Time=2,FrameTime=4,Tick=8};
    Globals* globals_{};
    int* seed_{};
    float time_{},frame_time_{};
    int tick_{},old_seed_{};
    unsigned restore_{};
    bool active_{};
    template<class T> bool Write(unsigned bit,T* field,const T& value) noexcept {
        restore_|=bit;
        return memory::Copy(field,&value,sizeof(value));
    }
    template<class T> bool RestoreField(unsigned bit,T* field,const T& value) noexcept {
        if(!(restore_&bit)) return true;
        if(!memory::Copy(field,&value,sizeof(value))) return false;
        restore_&=~bit;return true;
    }
public:
    ScopedShotClock()=default;
    ScopedShotClock(const ScopedShotClock&)=delete;
    ScopedShotClock& operator=(const ScopedShotClock&)=delete;
    ~ScopedShotClock() {Restore();}
    bool Begin(Globals* globals,int* seed,int tickbase,int command_seed) noexcept {
        if(active_ || restore_ || !globals || !seed || tickbase<=0 || tickbase>=100000000 || command_seed<0) return false;
        Globals clock{};int previous_seed{};
        if(!memory::Read(globals,clock) || !memory::Read(seed,previous_seed) ||
           !std::isfinite(clock.time) || clock.time<0.f ||
           !std::isfinite(clock.frame_time) || clock.frame_time<=0.f ||
           !std::isfinite(clock.interval) || clock.interval<.001f || clock.interval>.1f) return false;
        const float time=static_cast<float>(tickbase)*clock.interval;
        if(!std::isfinite(time)) return false;
        globals_=globals;seed_=seed;old_seed_=previous_seed;
        time_=clock.time;frame_time_=clock.frame_time;tick_=clock.tick;
        if(!Write(Seed,seed_,command_seed) || !Write(Time,&globals_->time,time) ||
           !Write(FrameTime,&globals_->frame_time,clock.interval) || !Write(Tick,&globals_->tick,tickbase)) {
            Restore();return false;
        }
        active_=true;return true;
    }
    bool Restore() noexcept {
        if(!restore_) {active_=false;return true;}
        // Attempt every restoration even when an earlier field fails. Pending
        // bits remain set so the destructor can retry a failed write once.
        bool ok=RestoreField(Tick,&globals_->tick,tick_);
        ok=RestoreField(FrameTime,&globals_->frame_time,frame_time_) && ok;
        ok=RestoreField(Time,&globals_->time,time_) && ok;
        ok=RestoreField(Seed,seed_,old_seed_) && ok;
        active_=false;return ok;
    }
};
}
