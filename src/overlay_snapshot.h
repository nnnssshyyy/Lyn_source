#pragma once
#include "features.h"
#include "esp_style.h"
#include <algorithm>

namespace features::snapshot {
struct Overlay {
    source::Matrix matrix{};
    std::array<esp::Target,384> targets{};
    std::array<bool,384> world{};
    std::array<esp::style::Info,384> info{};
    struct SkeletonRange {unsigned short first{},count{};};
    std::array<SkeletonRange,384> skeletons{};
    std::array<source::BoneSegment,8192> bones{};
    size_t bone_count{},count{};
    RadarSnapshot radar{};
    source::Vec3 aim_point{};
    bool has_target{},valid{};
    float fov{90.f};ULONGLONG time{};
};
template<class T,size_t N> size_t Prefix(std::array<T,N>& out,const std::array<T,N>& in,size_t count) noexcept {
    count=std::min(count,N);std::copy_n(in.begin(),count,out.begin());return count*sizeof(T);
}
inline size_t CopyRadar(RadarSnapshot& out,const RadarSnapshot& in) noexcept {
    out.count=std::min(in.count,in.points.size());
    out.origin=in.origin;out.yaw=in.yaw;out.captured=in.captured;out.valid=in.valid;
    return sizeof(out.count)+sizeof(out.origin)+sizeof(out.yaw)+sizeof(out.captured)+sizeof(out.valid)+
        Prefix(out.points,in.points,out.count);
}
// Unused tails stay allocated but are never read. Shrinking publishes counts
// atomically with their payload under the caller's snapshot lock.
inline size_t Copy(Overlay& out,const Overlay& in) noexcept {
    out.matrix=in.matrix;out.count=std::min(in.count,in.targets.size());
    out.bone_count=std::min(in.bone_count,in.bones.size());
    out.aim_point=in.aim_point;out.has_target=in.has_target;out.valid=in.valid;
    out.fov=in.fov;out.time=in.time;
    return sizeof(out.matrix)+sizeof(out.count)+sizeof(out.bone_count)+sizeof(out.aim_point)+
        sizeof(out.has_target)+sizeof(out.valid)+sizeof(out.fov)+sizeof(out.time)+
        Prefix(out.targets,in.targets,out.count)+Prefix(out.world,in.world,out.count)+
        Prefix(out.info,in.info,out.count)+Prefix(out.skeletons,in.skeletons,out.count)+
        Prefix(out.bones,in.bones,out.bone_count)+CopyRadar(out.radar,in.radar);
}
}
