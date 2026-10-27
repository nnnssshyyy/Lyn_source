#include "world_bounds.h"
#include "esp.h"
#include <cstdio>
#include <limits>
namespace {
unsigned checks{},failures{};
void Expect(bool ok,const char* message){++checks;if(!ok){++failures;std::printf("FAIL: %s\n",message);}}
bool Near(float a,float b){return std::abs(a-b)<.01f;}
game::Matrix3 Pose(float yaw,float roll,source::Vec3 origin,source::Vec3 scale={1,1,1}) {
    const float y=yaw*.017453292519943295f,r=roll*.017453292519943295f;
    const float cy=std::cos(y),sy=std::sin(y),cr=std::cos(r),sr=std::sin(r);
    return {{{cy*scale.x,-sy*cr*scale.y,sy*sr*scale.z,origin.x},
             {sy*scale.x,cy*cr*scale.y,-cy*sr*scale.z,origin.y},
             {0,sr*scale.y,cr*scale.z,origin.z}}};
}
}
int main() {
    const source::Vec3 lo{-40,-2,-3},hi{10,6,4},origin{100,20,10};
    std::array<source::Vec3,8> points{};
    Expect(world_bounds::Build(lo,hi,Pose(90,0,origin),origin,points),"rotated asymmetric weapon produces a world OBB");
    Expect(Near(points[0].x,102)&&Near(points[0].y,-20)&&Near(points[0].z,7),"first corner rotates about the model pivot");
    Expect(Near(points[7].x,94)&&Near(points[7].y,30)&&Near(points[7].z,14),"opposite corner follows the same pivot without re-centring local bounds");
    source::Matrix camera{};camera.m[0][0]=.002f;camera.m[1][2]=.01f;camera.m[3][3]=1;
    for(float yaw:{0.f,30.f,90.f,180.f,270.f})for(float roll:{0.f,45.f,90.f})for(float scale:{.5f,1.f,2.f}) {
        const auto matrix=Pose(yaw,roll,origin,{scale,scale*.75f,scale*1.25f});
        esp::Target target{};target.origin=origin;target.mins={-2,-2,-2};target.maxs={2,2,2};
        target.bounds_source=esp::Target::BoundsSource::Render;
        Expect(world_bounds::Build(lo,hi,matrix,origin,target.render_corners),"yaw, roll and model scale preserve render geometry");
        ImVec2 min{},max{};
        Expect(esp::ProjectBounds(target,camera,{1920,1080},min,max),"transformed corners project as a visible object");
        float x0=FLT_MAX,x1=-FLT_MAX,y0=FLT_MAX,y1=-FLT_MAX;
        for(const auto& p:target.render_corners){const float x=960*(1+.002f*p.x),y=540*(1-.01f*p.z);x0=std::min(x0,x);x1=std::max(x1,x);y0=std::min(y0,y);y1=std::max(y1,y);}
        Expect(Near(min.x,x0)&&Near(max.x,x1)&&Near(min.y,y0)&&Near(max.y,y1),"box encloses exactly the projected native model corners");
        Expect(esp::IntersectsView(target,camera),"culling uses the same transformed corners as drawing");
    }
    esp::Target target{};target.origin=origin;target.mins=lo;target.maxs=hi;ImVec2 a{},b{},c{},d{};
    Expect(esp::ProjectBounds(target,camera,{1920,1080},a,b),"legacy player collision boxes retain their existing projection");
    target.bounds_source=esp::Target::BoundsSource::Render;
    world_bounds::Build(lo,hi,Pose(90,0,origin),origin,target.render_corners);
    Expect(esp::ProjectBounds(target,camera,{1920,1080},c,d)&&!Near(a.x,c.x)&&!Near(b.x,d.x),"world render boxes no longer reuse translated collision extents");
    target.bounds_source=esp::Target::BoundsSource::Missing;
    Expect(!esp::ProjectBounds(target,camera,{1920,1080},a,b)&&!esp::IntersectsView(target,camera),"missing model geometry cannot fall back to a misleading box");
    target.bounds_source=esp::Target::BoundsSource::Render;
    source::Matrix perspective{};perspective.m[0][0]=perspective.m[1][1]=1;perspective.m[3][2]=1;
    const source::Vec3 near_origin{0,0,.5f};
    world_bounds::Build({-1,-1,-1},{1,1,1},Pose(20,30,near_origin),near_origin,target.render_corners);
    Expect(esp::ProjectBounds(target,perspective,{640,480},a,b)&&std::isfinite(a.x)&&std::isfinite(b.y),"rotated objects crossing the near plane stay clipped and finite");
    const source::Vec3 behind{0,0,-4};
    world_bounds::Build({-1,-1,-1},{1,1,1},Pose(20,30,behind),behind,target.render_corners);
    Expect(!esp::ProjectBounds(target,perspective,{640,480},a,b),"an object entirely behind the camera is rejected");
    auto matrix=Pose(0,0,origin);matrix.m[0][1]=NAN;
    Expect(!world_bounds::Build(lo,hi,matrix,origin,points)&&points[7].x==0,"bad matrices clear every previous corner");
    Expect(!world_bounds::Build(hi,lo,Pose(0,0,origin),origin,points),"inverted model extents are rejected");
    Expect(!world_bounds::Build(lo,hi,{},origin,points),"singular transforms cannot produce collapsed boxes");
    Expect(!world_bounds::Build(lo,hi,Pose(0,0,{5000,0,0}),origin,points),"stale transform belonging to another position is rejected");
    Expect(world_bounds::Build({-5,-5,0},{5,5,0},Pose(0,0,origin),origin,points),"flat pickup models remain supported");
    std::printf("%u world bounds checks, %u failed\n",checks,failures);return failures?1:0;
}
