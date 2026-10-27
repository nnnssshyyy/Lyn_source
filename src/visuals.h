#pragma once
#include "features.h"
#include "game.h"
#include "visuals_logic.h"

namespace visuals {
// Visual captures deliberately use Api::Local(..., false), whose health member
// is unset. Validate the live netvars instead of treating that placeholder as HP.
inline bool AliveForTrajectory(const game::Entity& local) noexcept {
    int health{};unsigned char life{};
    return local.ptr&&local.fields&&!local.ghost&&
        memory::Field(local.ptr,local.fields->health,health)&&health>0&&health<=1000000&&
        (local.fields->life<0||(memory::Field(local.ptr,local.fields->life,life)&&life==0));
}
struct Boss {unsigned handle{};source::Kind kind{};int health{},maximum{},rage{};float distance{};bool burning{};};
struct Threat {source::Vec3 position{};source::Kind kind{};float distance{};};
struct Snapshot {
    source::Matrix matrix{};source::Vec3 origin{};float yaw{};
    std::array<Boss,8> bosses{};unsigned boss_count{};
    std::array<Threat,96> threats{};unsigned threat_count{};
    logic::DamageHistory damage{};logic::DeathHistory deaths{};logic::Path path{};
    std::uint64_t time{};bool valid{},listener{};
};
void Initialize(const game::Api&) noexcept;
#ifdef VISUALS_TESTING
void InitializeEventFixture(void* manager,memory::Range code) noexcept;
#endif
bool Needed(const features::Config&) noexcept;
void Capture(const game::Api&,const features::Config&,std::span<const int>,const game::Entity&,const source::Matrix&,bool valid) noexcept;
void ClearLevel() noexcept;
void Suspend() noexcept;
Snapshot ReadSnapshot() noexcept;
unsigned ActionFlags(const game::Entity&,std::uint64_t now) noexcept;
}
