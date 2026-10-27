#pragma once
#include <array>
#include <cmath>
#include <cstdint>

// Storm-master/Copy_Command.hpp:199-225,300-302 and Run_Command.hpp:31-45.
// Sequence correction and command prediction are one operation. The history
// survives landing, checkbox changes and focus loss until its commands are ACKed.
namespace storm_fall {
inline constexpr int RingSize=150,MaxTickbase=100000000;
inline bool Requested(float fall_speed,float interval,bool grounded) noexcept {
    return !grounded && std::isfinite(fall_speed) && fall_speed>=0.f &&
        std::isfinite(interval) && interval>=.001f && interval<=.1f &&
        fall_speed+800.f*interval>=560.f;
}
inline int SequenceShift(int tickbase,int extra_commands=0) noexcept {
    if(tickbase<=0 || tickbase>=MaxTickbase || extra_commands<0 || extra_commands>800) return 0;
    const auto value=(static_cast<int64_t>(tickbase)-extra_commands+149)/150*150+300;
    return value>0 && value<=MaxTickbase+450?static_cast<int>(value):0;
}
inline int PredictionEnd(int incoming,int outgoing,bool pending) noexcept {
    if(!pending || incoming<0 || outgoing<incoming) return outgoing;
    return incoming+(outgoing-incoming)%RingSize;
}
inline int MovementButtons(int buttons,bool ducked) noexcept {return (buttons&~1564)|(ducked?4:0);}
class History {
    struct Entry {int number{},seed{},shift{};};
    std::array<Entry,RingSize> entries_{};
    unsigned owner_{};int last_{},ack_{};unsigned count_{};
public:
    void Reset(unsigned owner=0) noexcept {*this={};owner_=owner;}
    unsigned Owner() const noexcept {return owner_;}
    bool Empty() const noexcept {return count_==0;}
    unsigned Count() const noexcept {return count_;}
    bool CanStore(unsigned owner,int number) const noexcept {
        if(!owner || owner!=owner_ || number<=ack_ || number<=last_) return false;
        return entries_[static_cast<unsigned>(number)%RingSize].number==0;
    }
    bool Store(unsigned owner,int number,int seed,int shift) noexcept {
        if(!CanStore(owner,number) || shift<=0 || shift>MaxTickbase+450) return false;
        entries_[static_cast<unsigned>(number)%RingSize]={number,seed,shift};
        last_=number;++count_;return true;
    }
    int Get(unsigned owner,int number,int seed) const noexcept {
        if(!owner || owner!=owner_ || number<=ack_ || number<=0) return 0;
        const auto& e=entries_[static_cast<unsigned>(number)%RingSize];
        return e.number==number && e.seed==seed?e.shift:0;
    }
    void Retire(int ack) noexcept {
        if(ack<=ack_) return;
        ack_=ack;
        for(auto& e:entries_) if(e.number && e.number<=ack) {e={};--count_;}
    }
};
}
