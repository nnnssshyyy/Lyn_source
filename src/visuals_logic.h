#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>

namespace visuals::logic {
struct V {float x{},y{},z{};};
inline V Add(V a,V b) {return {a.x+b.x,a.y+b.y,a.z+b.z};}
inline V Mul(V a,float f) {return {a.x*f,a.y*f,a.z*f};}
inline float Dot(V a,V b) {return a.x*b.x+a.y*b.y+a.z*b.z;}
inline bool Finite(V a) {return std::isfinite(a.x)&&std::isfinite(a.y)&&std::isfinite(a.z);}
struct Damage {unsigned identity{};int amount{};V point{};std::uint64_t first{},last{};bool head{},position{};};
struct DamageHistory {
    std::array<Damage,32> items{};
    std::uint64_t hit_time{};bool head{};
    bool Confirm(int local_user,int attacker,bool headshot,std::uint64_t now) {
        if(local_user<=0||attacker!=local_user||!now)return false;
        hit_time=now;head=headshot;return true;
    }
    bool Add(int local_user,int attacker,unsigned identity,int amount,V point,bool positioned,bool headshot,std::uint64_t now) {
        if(local_user<=0 || attacker!=local_user || !identity || amount<=0 || amount>1000000 || !now || (positioned&&!Finite(point))) return false;
        Damage* slot=nullptr;
        for(auto& d:items) if(d.identity==identity && d.last && now>=d.last && now-d.last<=65 && now-d.first<=110) {slot=&d;break;}
        if(!slot) slot=&*std::min_element(items.begin(),items.end(),[](const auto& a,const auto& b){return a.last<b.last;});
        if(!slot->last || slot->identity!=identity || now<slot->last || now-slot->last>65 || now-slot->first>110)
            *slot={identity,0,point,now,now,headshot,positioned};
        slot->amount=std::min(1000000,slot->amount+amount);slot->last=now;slot->head|=headshot;
        if(positioned) {slot->point=point;slot->position=true;}
        return Confirm(local_user,attacker,headshot,now);
    }
    void Expire(std::uint64_t now,unsigned lifetime) {
        for(auto& d:items) if(d.last && (now<d.last || now-d.last>=lifetime)) d={};
    }
};
struct DeathBurst {std::uint64_t identity{},time{};V point{};};
struct DeathHistory {
    static constexpr unsigned MaxBursts=24,MaxParticles=32;
    std::array<DeathBurst,MaxBursts> items{};
    bool Add(std::uint64_t identity,V point,std::uint64_t now) {
        if(!identity||!now||!Finite(point))return false;
        for(const auto& burst:items)if(burst.identity==identity&&burst.time&&now>=burst.time&&now-burst.time<300)return false;
        auto& slot=*std::min_element(items.begin(),items.end(),[](const auto& a,const auto& b){return a.time<b.time;});
        slot={identity,now,point};return true;
    }
    void Expire(std::uint64_t now) {for(auto& burst:items)if(burst.time&&(now<burst.time||now-burst.time>=2000))burst={};}
};
struct Spark {V point{},tail{};float fade{},size{};};
inline Spark DeathParticle(const DeathBurst& burst,unsigned particle,float age,float lifetime) {
    if(particle>=DeathHistory::MaxParticles||!Finite(burst.point)||!std::isfinite(age)||age<0||
       !std::isfinite(lifetime)||lifetime<=0||age>=lifetime)return {};
    std::uint32_t seed=static_cast<std::uint32_t>(burst.identity^burst.time)^(particle+1)*0x9e3779b9u;
    const auto random=[&]() {seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return (seed&0xffff)/65535.f;};
    const float angle=random()*6.2831853f,speed=22.f+random()*38.f;
    const V velocity{std::cos(angle)*speed,std::sin(angle)*speed,25.f+random()*65.f};
    const auto position=[&](float t) {auto p=Add(burst.point,Mul(velocity,t));p.z-=38.f*t*t;return p;};
    return {position(age),position(std::max(0.f,age-.045f)),std::clamp((lifetime-age)/std::min(lifetime,.45f),0.f,1.f),1.f+random()*1.1f};
}
struct Arrow {float x{},y{},dx{},dy{};bool valid{};};
inline Arrow Edge(float dx,float dy,float yaw,float width,float height,float radius) {
    if(!std::isfinite(dx)||!std::isfinite(dy)||!std::isfinite(yaw)||width<100||height<100||!std::isfinite(radius)) return {};
    const float a=yaw*.01745329252f;
    float sx=dx*std::sin(a)-dy*std::cos(a),sy=-(dx*std::cos(a)+dy*std::sin(a));
    const float length=std::hypot(sx,sy);if(length<.01f) return {};
    sx/=length;sy/=length;radius=std::clamp(radius,.35f,.9f);
    const float rx=std::max(25.f,(width*.5f-38)*radius),ry=std::max(25.f,(height*.5f-38)*radius);
    const float t=1.f/std::max(std::abs(sx)/rx,std::abs(sy)/ry);
    return {width*.5f+sx*t,height*.5f+sy*t,sx,sy,true};
}
struct Collision {V point{},normal{};float fraction{1.f};bool valid{true},solid{};};
struct Path {std::array<V,129> points{};unsigned count{},bounces{},casts{};bool valid{},landed{};float seconds{};};
// Fixed work ceiling: <=128 hull traces per update, never on the render thread.
template<class Cast> Path Simulate(V start,V velocity,float gravity,float seconds,bool break_on_impact,Cast&& cast) {
    Path p{};
    if(!Finite(start)||!Finite(velocity)||!std::isfinite(gravity)||gravity<=0||!std::isfinite(seconds)) return p;
    constexpr float dt=1.f/32.f;
    const unsigned steps=static_cast<unsigned>(std::ceil(std::clamp(seconds,.5f,4.f)/dt));
    p.points[p.count++]=start;p.valid=true;
    for(unsigned i=0;i<steps;++i) {
        auto end=Add(start,Mul(velocity,dt));end.z-=gravity*dt*dt*.5f;
        const auto c=cast(start,end);++p.casts;
        if(!c.valid || c.solid || !Finite(c.point) || !Finite(c.normal) || !std::isfinite(c.fraction) || c.fraction<0 || c.fraction>1) {p.valid=false;break;}
        p.seconds+=dt*c.fraction;
        velocity.z-=gravity*dt*c.fraction;
        start=c.fraction<1?c.point:end;p.points[p.count++]=start;
        if(c.fraction<1) {
            const float n=Dot(c.normal,c.normal);if(n<.8f||n>1.2f){p.valid=false;break;}
            if(break_on_impact || (c.normal.z>.6f && Dot(velocity,velocity)<6400) || p.bounces>=6) {p.landed=true;break;}
            velocity=Mul(Add(velocity,Mul(c.normal,-2.f*Dot(velocity,c.normal))),.45f);
            start=Add(start,Mul(c.normal,2.f));++p.bounces;
        }
    }
    return p;
}
struct Rect {float x{},y{},w{},h{};};
inline bool Overlap(Rect a,Rect b) {return a.x<b.x+b.w+2&&a.x+a.w+2>b.x&&a.y<b.y+b.h+2&&a.y+a.h+2>b.y;}
struct Labels {
    // A 64x72 occupancy grid bounds crowded-scene layout cost per label.
    std::array<bool,64*72> cells{};
    bool Place(Rect& r,float width,float height) {
        if(!std::isfinite(r.x)||!std::isfinite(r.y)||!std::isfinite(r.w)||!std::isfinite(r.h)||
           !std::isfinite(width)||!std::isfinite(height)||r.w<=0||r.h<=0||width<1||height<1||r.w>width||r.h>height)return false;
        r.x=std::clamp(r.x,0.f,width-r.w);
        for(int attempt=0;attempt<4;++attempt) {
            Rect q=r;q.y=std::clamp(r.y-(r.h+3)*attempt,0.f,height-r.h);
            const int x0=std::clamp(int(q.x/width*64),0,63),x1=std::clamp(int((q.x+q.w+2)/width*64),0,63);
            const int y0=std::clamp(int(q.y/height*72),0,71),y1=std::clamp(int((q.y+q.h+2)/height*72),0,71);
            bool free=true;for(int y=y0;y<=y1;++y)for(int x=x0;x<=x1;++x)free&=!cells[y*64+x];
            if(!free)continue;
            for(int y=y0;y<=y1;++y)for(int x=x0;x<=x1;++x)cells[y*64+x]=true;
            r=q;return true;
        }
        return false;
    }
};
}
