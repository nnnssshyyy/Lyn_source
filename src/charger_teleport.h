#pragma once
#include "source.h"
#include <bit>
#include <cstdint>

namespace charger_teleport {
inline constexpr std::uint32_t RollBits=0x7cf0bdc2u;
inline constexpr float Roll=std::bit_cast<float>(RollBits);
struct Input {
    bool enabled{},focused{},in_game{},alive{},ghost{},charger{},channel{},carry_known{},frozen{};
    int team{},command{};
    unsigned owner{},victim{};
};
// One committed command per carry, including command replays. A failed field
// read cannot manufacture a release; a failed command write can be retried.
class Pulse {
    unsigned owner_{},victim_{};
    int committed_{},pending_{};
public:
    void Reset() noexcept {*this={};}
    bool Tracking() const noexcept {return owner_!=0;}
    bool Prepare(const Input& in) noexcept {
        pending_=0;
        if(!in.in_game || !in.alive || in.ghost || !in.charger || in.team!=3 || !in.owner || in.owner==UINT32_MAX) {
            Reset();return false;
        }
        if(owner_!=in.owner) {Reset();owner_=in.owner;}
        if(!in.carry_known) return false;
        if(victim_!=in.victim) {victim_=in.victim;committed_=0;}
        if(!victim_ || victim_==UINT32_MAX || !in.enabled || !in.focused || !in.channel || in.frozen || in.command<=0) return false;
        if(committed_ && committed_!=in.command) return false;
        pending_=in.command;return true;
    }
    bool Commit() noexcept {
        if(!pending_) return false;
        const bool first=committed_==0;
        committed_=pending_;pending_=0;return first;
    }
};
inline void Apply(source::Vec3& angles) noexcept {angles.z=Roll;}
}
