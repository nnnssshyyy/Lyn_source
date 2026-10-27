#pragma once
#include "game.h"
#include <span>

namespace features { struct Config; }
namespace aimbot {
struct Result {
    game::Entity entity{};
    game::Vec3 point{};
    float simulation_time{},radius{};
    bool historical{},friend_api{};
    unsigned candidates{},hitboxes{},traces{},records{};
    unsigned unreadable{},filtered_friends{},filtered_types{},filtered_ghosts{},filtered_dead{},outside_range{},outside_fov{};
    int hitgroup{},bone{-1},hitbox{-1},sample{};
};
// Called only on the game thread while features owns its runtime lock.
// Captures eligible rendered poses after interpolation. Server support for
// rewinding each entity category depends on the game/server configuration.
void Capture(game::Api& api,const features::Config& config,std::span<const int> indices);
bool Select(game::Api& api,const features::Config& config,const game::Entity& local,
            const game::Entity& weapon,const game::Command& original,const game::Globals& clock,
            game::Vec3 eye,std::span<const int> indices,Result& result);
// Visible tracking for infected abilities. Uses current survivor poses and
// leaves attack buttons, cooldowns and projectile trajectories to the player.
bool SelectInfected(game::Api& api,const features::Config& config,const game::Entity& local,
                    const game::Command& original,const game::Globals& clock,
                    game::Vec3 eye,std::span<const int> indices,Result& result);
bool Eligible(game::Api& api,const features::Config& config,const game::Entity& entity,const game::Entity& local);
void OnShot(const Result& target,const game::Globals& clock,float latency);
void ReleaseTarget();
void Reset();
}
