#include "visuals_logic.h"
#include <cstdio>
#include <limits>
using namespace visuals::logic;
unsigned checks{},failures{};
void expect(bool pass,const char* label){++checks;if(!pass){++failures;std::printf("FAIL: %s\n",label);}}
int main(){
    DamageHistory h{};
    expect(!h.Add(7,8,12,50,{},false,false,1000),"other attackers cannot confirm our hit");
    expect(!h.Add(0,0,12,50,{},false,false,1000),"unknown local identity fails closed");
    expect(!h.Add(7,7,12,0,{},false,false,1000),"zero damage cannot trigger a marker");
    expect(h.Add(7,7,12,20,{1,2,3},true,false,1000),"positive local event is accepted");
    h.Add(7,7,12,15,{1,2,3},true,true,1040);
    expect(h.items[0].amount==35&&h.items[0].head,"shotgun pellets aggregate per victim");
    h.Add(7,7,13,8,{},false,false,1041);
    expect(h.items[1].amount==8,"different victims stay separate");
    h.Add(7,7,12,9,{},false,false,1200);
    expect(h.items[2].amount==9,"later shots create a new floating label");
    h.Expire(2200,1000);expect(h.items[0].last==0&&h.items[2].last==0,"expired damage removed");
    for(unsigned i=1;i<80;++i)h.Add(7,7,100+i,1,{},false,false,3000+i);
    unsigned count{};for(const auto& d:h.items)if(d.last)++count;
    expect(count==32,"damage queue remains bounded under bursts");
    expect(!h.Add(7,7,2,1,{std::numeric_limits<float>::quiet_NaN(),0,0},true,false,4000),"invalid positions rejected");
    DamageHistory kill{};
    expect(kill.Confirm(7,7,true,4100)&&kill.hit_time==4100&&kill.head,"confirmed kill supplies a center marker without fabricating damage");
    expect(kill.items[0].last==0&&!kill.Confirm(7,8,false,4200),"kill marker adds no damage number and excludes other attackers");
    DeathHistory deaths{};
    expect(deaths.Add(12,{0,0,30},1000)&&!deaths.Add(12,{0,0,30},1010),"death burst accepts real coordinates including zero and rejects duplicates");
    expect(!deaths.Add(0,{},1000)&&!deaths.Add(13,{NAN,0,0},1000),"death burst requires identity and finite coordinates");
    for(unsigned i=0;i<200;++i)deaths.Add(100+i,{1,2,3},2000+i);
    unsigned bursts{};for(const auto& burst:deaths.items)if(burst.time)++bursts;
    expect(bursts==DeathHistory::MaxBursts,"hordes cannot exceed the fixed death burst budget");
    const auto spark=DeathParticle(deaths.items[0],3,.2f,1.1f);
    expect(Finite(spark.point)&&Finite(spark.tail)&&spark.fade>0&&spark.size>0,"death particles move with finite positions and a fading trail");
    expect(DeathParticle(deaths.items[0],3,1.2f,1.1f).fade==0&&DeathParticle(deaths.items[0],32,.2f,1.1f).fade==0,"expired particles and out-of-budget indices generate no geometry");
    deaths.Expire(5000);expect(std::all_of(deaths.items.begin(),deaths.items.end(),[](const auto& b){return !b.time;}),"all death bursts expire without incoming events");
    const auto front=Edge(100,0,0,1920,1080,.8f),right=Edge(0,-100,0,1920,1080,.8f),behind=Edge(-100,0,0,1920,1080,.8f);
    expect(front.valid&&front.y<540&&std::abs(front.x-960)<1,"forward maps to top");
    expect(right.x>960&&std::abs(right.y-540)<1,"right maps to right");
    expect(behind.y>540,"rear maps to bottom");
    expect(!Edge(0,0,0,1920,1080,.8f).valid,"coincident threats have no arrow");
    const auto turned=Edge(0,100,90,640,480,.9f);expect(turned.valid&&std::abs(turned.x-320)<1&&turned.y>15,"yaw and small viewport respected");
    unsigned casts{};
    auto clear=[&](V,V b){++casts;return Collision{b,{},1,true,false};};
    const auto flight=Simulate({0,0,100},{100,0,100},320,1,false,clear);
    expect(flight.valid&&flight.count==33&&!flight.landed,"free-flight path sampled at 32Hz");
    expect(std::abs(flight.points[32].x-100)<.01f&&std::abs(flight.points[32].z-40)<.01f,"ballistic gravity matches analytic flight");
    expect(casts==32,"no unbounded trajectory work");
    casts=0;const auto long_path=Simulate({},{1000,0,300},320,100,false,clear);
    expect(long_path.count==129&&casts==128,"horizon clamps to fixed capacity");
    auto floor=[](V a,V b){if(b.z<0){const float f=a.z/(a.z-b.z);return Collision{Add(a,Mul(Add(b,Mul(a,-1)),f)),{0,0,1},f,true,false};}return Collision{b,{},1,true,false};};
    const auto impact=Simulate({0,0,10},{30,0,-50},320,2,true,floor);
    expect(impact.valid&&impact.landed&&std::abs(impact.points[impact.count-1].z)<.01f,"impact stops at traced floor");
    const auto bounced=Simulate({0,0,20},{180,0,-100},320,4,false,floor);
    expect(bounced.valid&&bounced.bounces>0&&bounced.bounces<=6&&bounced.casts<=128,"pipe bounce reflection has a fixed budget");
    const auto solid=Simulate({},{1,0,0},320,1,false,[](V,V b){return Collision{b,{},0,true,true};});
    expect(!solid.valid&&solid.casts==1,"start-solid fails closed");
    const auto invalid=Simulate({},{1,0,0},320,1,false,[](V,V b){return Collision{b,{},1,false,false};});
    expect(!invalid.valid,"missing trace support never invents a path");
    Labels labels{};Rect a{100,200,100,16},b=a;
    expect(labels.Place(a,1920,1080),"first ESP label placed");
    const bool placed=labels.Place(b,1920,1080);
    expect(!placed||!Overlap(a,b),"labels move or hide when they collide");
    Rect outside{-20,-10,60,16};expect(labels.Place(outside,640,480)&&outside.x>=0&&outside.y>=0,"labels clamp to viewport");
    Rect invalid_label{NAN,0,60,16};expect(!labels.Place(invalid_label,640,480),"nonfinite label positions never reach occupancy grid indices");
    Rect valid_label{0,0,60,16};expect(!labels.Place(valid_label,NAN,480),"invalid viewport cannot index the label grid");
    std::printf("%u visual logic checks, %u failed\n",checks,failures);return failures?1:0;
}
