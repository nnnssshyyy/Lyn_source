#pragma once
#include "game.h"
#include <algorithm>
#include <cmath>

namespace world_bounds {
// Local render bounds must be transformed as an OBB. Translating collision
// mins/maxs alone shifts asymmetric or rotated weapon models away from the box.
inline bool Build(source::Vec3 lo,source::Vec3 hi,const game::Matrix3& transform,
                  source::Vec3 origin,std::array<source::Vec3,8>& output) noexcept {
    output.fill({});
    if(!source::Finite(lo)||!source::Finite(hi)||!source::Finite(origin)||
       lo.x>hi.x||lo.y>hi.y||lo.z>hi.z)return false;
    const source::Vec3 span{hi.x-lo.x,hi.y-lo.y,hi.z-lo.z};
    if(std::max({span.x,span.y,span.z})<.01f||std::max({span.x,span.y,span.z})>16384.f)return false;
    for(const auto& row:transform.m)for(float v:row)if(!std::isfinite(v))return false;
    const auto& m=transform.m;
    const float dx=m[0][3]-origin.x,dy=m[1][3]-origin.y,dz=m[2][3]-origin.z;
    if(dx*dx+dy*dy+dz*dz>256.f*256.f)return false;
    const float determinant=m[0][0]*(m[1][1]*m[2][2]-m[1][2]*m[2][1])-
        m[0][1]*(m[1][0]*m[2][2]-m[1][2]*m[2][0])+m[0][2]*(m[1][0]*m[2][1]-m[1][1]*m[2][0]);
    if(!std::isfinite(determinant)||std::abs(determinant)<1e-8f||std::abs(determinant)>262144.f)return false;
    std::array<source::Vec3,8> points{};
    for(unsigned i=0;i<points.size();++i) {
        const source::Vec3 p{(i&1)?hi.x:lo.x,(i&2)?hi.y:lo.y,(i&4)?hi.z:lo.z};
        auto& q=points[i];
        q={m[0][0]*p.x+m[0][1]*p.y+m[0][2]*p.z+m[0][3],
           m[1][0]*p.x+m[1][1]*p.y+m[1][2]*p.z+m[1][3],
           m[2][0]*p.x+m[2][1]*p.y+m[2][2]*p.z+m[2][3]};
        if(!source::Finite(q)||std::abs(q.x)>131072.f||std::abs(q.y)>131072.f||std::abs(q.z)>131072.f)return false;
    }
    output=points;return true;
}
}
