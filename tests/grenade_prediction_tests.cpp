#include "grenade_prediction.h"
#include "game.h"
#include <cstdio>
#include <limits>
namespace {
unsigned checks{},failures{};
void Expect(bool ok,const char* text){++checks;if(!ok){++failures;std::printf("FAIL: %s\n",text);}}
float Dot(source::Vec3 a,source::Vec3 b){return a.x*b.x+a.y*b.y+a.z*b.z;}
source::Matrix Camera(source::Vec3 eye,source::Vec3 angles,float aspect,float fov) {
    const auto f=game::Forward(angles);const float yaw=angles.y*.017453292519943295f,pitch=angles.x*.017453292519943295f;
    const source::Vec3 right{std::sin(yaw),-std::cos(yaw),0},up{std::sin(pitch)*std::cos(yaw),std::sin(pitch)*std::sin(yaw),std::cos(pitch)};
    const float scale=1.f/std::tan(fov*.008726646259971648f);source::Matrix m{};
    const source::Vec3 rows[]{game::Mul(right,scale),game::Mul(up,scale*aspect),f};
    const int indices[]{0,1,3};for(int i=0;i<3;++i){auto& row=m.m[indices[i]];row[0]=rows[i].x;row[1]=rows[i].y;row[2]=rows[i].z;row[3]=-Dot(rows[i],eye);}return m;
}
}
int main() {
    using namespace grenade_prediction;
    const auto level=NormalThrow({0,0,64},{0,0,0},{});
    Expect(level.valid&&level.start.x==16&&level.start.y==0&&level.start.z==64,"level release stays on the eye ray without pitch offset");
    Expect(level.velocity.x==750&&level.velocity.y==0&&level.velocity.z==0,"level launch has no invented upward impulse");
    const auto moving=NormalThrow({0,0,64},{0,0,0},{100,-50,20});
    Expect(moving.velocity.x==850&&moving.velocity.y==-50&&moving.velocity.z==20,"player momentum is inherited once without the old multiplier");
    const auto right=NormalThrow({0,0,64},{0,90,0},{});
    Expect(std::abs(right.start.x)<.001f&&std::abs(right.start.y-16)<.001f,"yaw rotates the release about the eye instead of a fixed screen offset");
    const source::Vec3 eye{212,-135,64};float maximum_error{};
    for(float pitch:{-70.f,-30.f,0.f,30.f,70.f})for(float yaw:{-179.f,-90.f,0.f,45.f,179.f})
        for(const auto viewport:{source::Vec3{640,480,70},source::Vec3{1920,1080,90},source::Vec3{2560,1080,120}}) {
            const source::Vec3 angles{pitch,yaw,0};const auto camera=Camera(eye,angles,viewport.x/viewport.y,viewport.z);
            const auto launch=NormalThrow({eye.x,eye.y,eye.z},{pitch,yaw,0},{});float x{},y{};
            const bool projected=game::Project(camera,{launch.start.x,launch.start.y,launch.start.z},viewport.x,viewport.y,x,y);
            const float error=std::hypot(x-viewport.x*.5f,y-viewport.y*.5f);maximum_error=std::max(maximum_error,error);
            Expect(projected&&error<.02f,"release projects to the reticle across pitch, yaw, FOV and aspect ratios");
        }
    unsigned casts{};V first_from{},first_to{};
    auto clear=[&](V a,V b){if(!casts){first_from=a;first_to=b;}++casts;return visuals::logic::Collision{b,{},1,true,false};};
    auto path=Predict(level,800,4,false,clear);
    Expect(path.valid&&path.count==129&&casts==129,"work stays bounded including the release hull");
    Expect(first_from.z==64&&first_from.x==0&&first_to.x==16&&path.points[0].x==16,"release trace begins at the eye and flight begins at its endpoint");
    casts=0;
    path=Predict(level,800,.5f,true,[&](V,V b){++casts;if(casts==1)return visuals::logic::Collision{{8,0,64},{-1,0,0},.5f,true,false};return visuals::logic::Collision{b,{},1,true,false};});
    Expect(path.valid&&path.points[0].x==8,"nearby walls shorten the release to the native hull endpoint");
    casts=0;path=Predict(level,800,3,false,[&](V,V){++casts;return visuals::logic::Collision{{},{},0,true,true};});
    Expect(!path.valid&&casts==1,"start-solid release cannot invent a path through a wall");
    Expect(!NormalThrow({NAN,0,0},{},{ }).valid&&!NormalThrow({},{0,NAN,0},{}).valid&&!NormalThrow({},{},{0,0,NAN}).valid,"invalid eye, view or momentum fails closed");
    casts=0;Predict({},800,3,false,clear);Predict(level,NAN,3,false,clear);Predict(level,800,NAN,false,clear);
    Expect(casts==0,"invalid launch or settings perform no engine traces");
    Pose pose{1,2,{0,0,64},{0,0,0},{},800,3,true};Cache cache;
    Expect(!cache.Reuse(pose,1000),"an empty cache never displays a previous curve");cache.Remember(pose,1000);
    Expect(cache.Reuse(pose,1001)&&cache.Reuse(pose,1099),"unchanged poses reuse the bounded static preview");
    Expect(!cache.Reuse(pose,1100)&&!cache.Reuse(pose,999),"static cache expires and rejects a backwards clock");
    auto changed=pose;changed.angles.y=.1f;Expect(!cache.Reuse(changed,1001),"yaw change updates immediately within the former 100 ms delay");
    changed=pose;changed.angles.x=.1f;Expect(!cache.Reuse(changed,1001),"pitch change updates immediately");
    changed=pose;changed.eye.x=.001f;Expect(!cache.Reuse(changed,1001),"movement or stance change invalidates even a very small eye shift");
    changed=pose;changed.velocity.y=1;Expect(!cache.Reuse(changed,1001),"sideways momentum updates immediately");
    changed=pose;++changed.weapon;Expect(!cache.Reuse(changed,1001),"weapon serial changes cannot reuse an old preview");
    changed=pose;++changed.owner;Expect(!cache.Reuse(changed,1001),"another local player cannot reuse an old preview");
    changed=pose;changed.gravity=600;Expect(!cache.Reuse(changed,1001),"server gravity changes invalidate the cached flight");
    changed=pose;changed.seconds=4;Expect(!cache.Reuse(changed,1001),"horizon changes invalidate the cached flight");
    changed=pose;changed.break_on_impact=false;Expect(!cache.Reuse(changed,1001),"pipe-bomb bounce mode cannot reuse a breaking throwable path");
    cache.Clear();Expect(!cache.Reuse(pose,1001),"unequip, disable, failed reads and level reset discard the cache");
    std::printf("%u grenade prediction checks, %u failed; maximum reticle error %.6f px\n",checks,failures,maximum_error);return failures?1:0;
}
