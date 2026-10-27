#pragma once
#include "visuals_logic.h"

namespace grenade_prediction {
using visuals::logic::V;
using visuals::logic::Finite;
using visuals::logic::Add;
using visuals::logic::Mul;
struct Launch {V eye{},start{},velocity{};bool valid{};};
inline Launch NormalThrow(V eye,V angles,V player_velocity) noexcept {
    if(!Finite(eye)||!Finite(angles)||!Finite(player_velocity))return {};
    const float pitch=angles.x*.017453292519943295f,yaw=angles.y*.017453292519943295f;
    const V forward{std::cos(pitch)*std::cos(yaw),std::cos(pitch)*std::sin(yaw),-std::sin(pitch)};
    // Installed server.dll CBaseCSGrenade normal-throw branch: a 16-unit
    // forward release hull, forward * 750 plus the player's velocity.
    // No artificial pitch bias or additional vertical/lateral launch impulse.
    const auto start=Add(eye,Mul(forward,16.f));
    const auto velocity=Add(Mul(forward,750.f),player_velocity);
    return {eye,start,velocity,Finite(start)&&Finite(velocity)};
}
struct Pose {
    unsigned owner{},weapon{};
    V eye{},angles{},velocity{};
    float gravity{},seconds{};
    bool break_on_impact{};
};
class Cache {
    Pose previous{};
    std::uint64_t sampled{};
    bool valid{};
    static bool Same(V a,V b) noexcept {return a.x==b.x&&a.y==b.y&&a.z==b.z;}
public:
    void Clear() noexcept {valid=false;previous={};sampled=0;}
    bool Reuse(const Pose& pose,std::uint64_t now) const noexcept {
        // Static poses can refresh at 10 Hz; changed aim/position/momentum
        // always recomputes this frame instead of displaying an old arc.
        return valid&&now>=sampled&&now-sampled<100&&pose.owner==previous.owner&&pose.weapon==previous.weapon&&
            Same(pose.eye,previous.eye)&&Same(pose.angles,previous.angles)&&Same(pose.velocity,previous.velocity)&&
            pose.gravity==previous.gravity&&pose.seconds==previous.seconds&&pose.break_on_impact==previous.break_on_impact;
    }
    void Remember(const Pose& pose,std::uint64_t now) noexcept {previous=pose;sampled=now;valid=true;}
};
// One release trace plus at most 128 flight traces; called on the game thread.
template<class Cast> visuals::logic::Path Predict(const Launch& launch,float gravity,float seconds,bool stop,Cast&& cast) {
    if(!launch.valid||!std::isfinite(gravity)||gravity<=0||!std::isfinite(seconds))return {};
    const auto release=cast(launch.eye,launch.start);
    if(!release.valid||release.solid||!Finite(release.point)||!std::isfinite(release.fraction)||release.fraction<0||release.fraction>1)return {};
    // The native spawn uses the traced endpoint, including a partial hull hit.
    return visuals::logic::Simulate(release.point,launch.velocity,gravity*.4f,seconds,stop,cast);
}
}
