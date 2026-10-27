#pragma once
#include <cmath>

namespace radar {
struct Point { float x{},y{}; bool valid{}; };
// Normalized circle coordinates. Source's positive yaw turns toward +Y.
inline Point Project(float dx,float dy,float yaw,float range_units) noexcept {
    if(!std::isfinite(dx) || !std::isfinite(dy) || !std::isfinite(yaw) ||
       !std::isfinite(range_units) || range_units<=0.f) return {};
    const float angle=yaw*.017453292519943295f;
    float x=(dx*std::sin(angle)-dy*std::cos(angle))/range_units;
    float y=-(dx*std::cos(angle)+dy*std::sin(angle))/range_units;
    const float distance=std::hypot(x,y);
    if(!std::isfinite(distance)) return {};
    if(distance>1.f) {x/=distance;y/=distance;}
    return {x,y,true};
}
}
