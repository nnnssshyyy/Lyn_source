#pragma once
#include "game.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <string_view>

namespace shotgun_pattern {
// Offline checked against the installed vstdlib CUniformRandomStream. No
// calls into the game's global RNG, and no per-frame seed search.
inline uint32_t Hash(uint32_t seed,int extra) noexcept {
    uint32_t value=~0u;
    const auto byte=[&](unsigned char b) {
        value^=b;
        for(int j=0;j<8;++j)value=(value>>1)^(0xedb88320u&(0u-(value&1)));
    };
    for(int i=0;i<4;++i)byte(static_cast<unsigned char>(seed>>(8*i)));
    for(int i=0;i<4;++i)byte(static_cast<unsigned char>(static_cast<uint32_t>(extra)>>(8*i)));
    for(unsigned char b:std::string_view("CTerrorPlayer::FireBullet Spread"))byte(b);
    return ~value;
}
inline float Radius(uint32_t seed) noexcept {
    const int signed_seed=static_cast<int>(seed);
    int state=signed_seed==std::numeric_limits<int>::min()?1:std::max(1,std::abs(signed_seed));
    std::array<int,32> table{};
    const auto step=[&] {
        const int k=state/127773;
        state=16807*(state-k*127773)-2836*k;
        if(state<0)state+=2147483647;
    };
    for(int j=39;j>=0;--j){step();if(j<32)table[j]=state;}
    const int index=table[0]/67108864;step();
    return std::abs(2.f*std::min((1.f/2147483647.f)*static_cast<float>(table[index]),1.f-1.2e-7f)-1.f);
}
inline float MaxRadius(int seed,int pellets) noexcept {
    if(seed<0 || pellets<8 || pellets>11)return 1.f;
    float result{};
    // Native FireBullet centres the first pellet; its additional seed is 1.
    // The scattered pellets use additional seeds 2 through pellet count.
    for(int i=2;i<=pellets;++i)result=std::max(result,Radius(Hash(static_cast<uint32_t>(seed),i)));
    return result;
}
inline int Pellets(int weapon_index) noexcept {
    switch(weapon_index){case 6:return 10;case 7:return 8;case 8:return 11;case 9:return 9;default:return 0;}
}
struct Candidate {int number{},seed{};std::array<float,4> radius{};};
#include "shotgun_patterns.inc"
inline const Candidate* Select(int native_seed,int pellets,float strength) noexcept {
    if(native_seed<0 || pellets<8 || pellets>11 || !std::isfinite(strength) || strength<=0.f || strength>1.f)return nullptr;
    const auto index=static_cast<size_t>(pellets-8);
    float best=1.f;
    for(const auto& c:Candidates)best=std::min(best,c.radius[index]);
    const float target=1.f-(1.f-best)*strength;
    if(MaxRadius(native_seed,pellets)<=target)return nullptr; // Already tighter.
    const Candidate* chosen{};
    for(const auto& c:Candidates)
        if(c.radius[index]<=target+1e-6f && (!chosen || c.radius[index]>chosen->radius[index]))chosen=&c;
    return chosen;
}
// Preserve the engine's sequence number and ring identity locally. Only the
// temporary copies given to its native serializer carry the selected number.
// Seed is written before the native verified command and prediction run.
class History {
    struct Entry {const game::Command* address{};int number{},seed{},wire{};};
    std::array<Entry,150> entries{};
public:
    void Clear() noexcept {entries.fill({});}
    bool Store(const game::Command* address,const game::Command& command,int wire) noexcept {
        if(!address || command.number<=0 || command.seed<0 || wire<=0)return false;
        entries[static_cast<size_t>(command.number)%entries.size()]={address,command.number,command.seed,wire};
        return true;
    }
    bool Translate(const game::Command* address,game::Command& copy) const noexcept {
        if(!address || copy.number<=0 || copy.seed<0)return false;
        const auto& e=entries[static_cast<size_t>(copy.number)%entries.size()];
        if(e.address!=address || e.number!=copy.number || e.seed!=copy.seed || e.wire<=0)return false;
        copy.number=e.wire;return true;
    }
};
}
