#include "aimbot.h"
#include "aim_logic.h"
#include "features.h"
#include <array>
#include <deque>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cfloat>

namespace aimbot {
namespace {
using game::Entity;
using game::Vec3;
using source::Kind;
struct Record { float time{}; Vec3 origin{}; std::vector<game::Point> points; };
struct History { unsigned handle{}; const void* model{}; std::deque<Record> records; };
// One history slot per entity: versus players occupy low indices, campaign
// specials/commons live anywhere in the entity list.
std::array<History,2048> history;
bool history_active{};
void ClearHistory() {for(auto& h:history) h={};history_active=false;}
struct TargetState {
    unsigned handle{};
    float blacklist_until{};
    int sequence{-1};
    bool idle{},burning{},dead{},friend_known{},friend_value{};
    ULONGLONG friend_check{};
};
std::array<TargetState,8192> states;
struct Tracking {
    unsigned local{},weapon{},target{};
    unsigned special_priority{};
    bool class_filter{};
    int command{},mode{-1},priority{-1},preset{-1},mask{},bone{-1},hitbox{-1},sample{};
} tracking;

bool Special(Kind kind) { return kind>=Kind::Smoker && kind<=Kind::Tank; }
bool OneShot(const Entity& weapon,const Entity& target) {
    if(!weapon.fields) return false;
    if(target.kind!=Kind::Common && target.kind!=Kind::Boomer) return false;
    const auto group=weapon.fields->weapon_group;
    const char* name=weapon.fields->name;
    return group==3 || group==4 || group==5 || !strcmp(name,"CMagnumPistol") ||
        !strcmp(name,"CRifle_AK47") || !strcmp(name,"CRifle");
}
TargetState& State(game::Api& api,const Entity& entity) {
    auto& state=states[entity.index];
    if(state.handle!=entity.handle) {state={};state.handle=entity.handle;state.sequence=-1;}
    int sequence{};
    if(entity.fields->sequence>=0 && memory::Field(entity.ptr,entity.fields->sequence,sequence) && sequence!=state.sequence) {
        char name[160]{};
        if(api.Sequence(entity,name,sizeof(name))) {
            // Cache only successful lookups: a transient missing label must
            // not hide a later death update with the same sequence number.
            state.sequence=sequence;
            state.idle=(strstr(name,"Idle") || strstr(name,"idle") || strstr(name,"Walk_Neutral")) && !strstr(name,"Upper_KNIFE");
            state.burning=strstr(name,"Burn") || strstr(name,"burn");
            state.dead=logic::DeathSequence(name);
        }
    }
    return state;
}
bool BestPoint(game::Api& api,const features::Config& cfg,const Entity& local,const Entity& weapon,
               const Entity& target,Vec3 eye,Vec3 view,const std::vector<game::Point>& points,Result& out,bool historical=false) {
    const int preferred=!weapon.fields || (weapon.fields->weapon_group==3 && target.kind==Kind::Witch) || OneShot(weapon,target) ? 2 : 1;
    const unsigned mask=logic::Mask(cfg.hitbox,static_cast<unsigned>(cfg.hitbox_mask));
    if(cfg.hitbox==4 && mask==0) return false;
    game::Filter filter; filter.skip=local.ptr; filter.skip_weapon=weapon.ptr;
    if(!cfg.avoid_teammates) {
        filter.skip_team=local.team;filter.team_offset=local.fields->team;
        filter.entity_image=api.base.client_image;
    }
    std::array<int,7> order{1,2,3,4,5,6,7};
    if(cfg.hitbox==4) {
        const int ranks[]{cfg.rank_head,cfg.rank_chest,cfg.rank_stomach,cfg.rank_left_arm,
                          cfg.rank_right_arm,cfg.rank_left_leg,cfg.rank_right_leg};
        // Stable ties preserve anatomical order; invisible groups fall through
        // to the next priority within the same bounded trace budget.
        std::sort(order.begin(),order.end(),[&](int a,int b){
            return ranks[a-1]==ranks[b-1]?a<b:ranks[a-1]<ranks[b-1];
        });
    } else if(preferred!=1) std::swap(order[0],order[preferred-1]);
    for(const int group:order) {
        if(!logic::Allowed(mask,group,preferred,cfg.hitbox==0)) continue;
        const auto visible=[&](const game::Point& point) {
            if(out.traces>=192 || point.group!=group ||
               game::Length(game::Sub(point.position,eye))>cfg.aim_distance/.0254f ||
               game::Fov(view,game::Angle(eye,point.position))>cfg.fov) return false;
            game::Trace trace{};++out.traces;
            return api.Cast(eye,point.position,filter,trace) &&
                logic::ShotVisible(trace.fraction,trace.start_solid,trace.all_solid,trace.entity==target.ptr,historical) &&
                !(cfg.hitbox==1 && trace.entity==target.ptr && trace.hitgroup!=1);
        };
        const auto use=[&](const game::Point& point) {
            out.point=point.position;out.hitgroup=group;out.bone=point.bone;
            out.hitbox=point.hitbox;out.sample=point.sample;out.radius=point.radius;return true;
        };
        // Recover the center as soon as it is exposed: its interior margin is
        // larger than an edge sample's. Keep a stable edge only behind cover.
        for(const auto& point:points)
            if(point.group==group && point.sample==0 && visible(point)) return use(point);
        if(tracking.target==target.handle) {
            for(const auto& point:points)
                if(point.group==group && point.bone==tracking.bone && point.hitbox==tracking.hitbox &&
                   point.sample==tracking.sample && visible(point)) return use(point);
        }
        {
            float best_fov=FLT_MAX;const game::Point* best_point{};
            for(const auto& point:points) {
                if(point.group!=group || point.sample==0) continue;
                const float fov=game::Fov(view,game::Angle(eye,point.position));
                if(fov>=best_fov || !visible(point)) continue;
                best_fov=fov;best_point=&point;
            }
            if(best_point) return use(*best_point);
        }
    }
    return false;
}
}
namespace {
enum class Rejection {None,Invalid,Team,Ghost,Friend,Type,Dead};
Rejection Filter(game::Api& api,const features::Config& cfg,const Entity& e,const Entity& local) {
    if(e.index<=0 || e.index>=8192 || e.ptr==local.ptr || e.kind==Kind::None) return Rejection::Invalid;
    if(e.team==local.team) return Rejection::Team;
    if(e.ghost) return Rejection::Ghost;
    // Class selection is exclusive while enabled. Reject before state/bones
    // work so unmarked targets cannot become visibility or history fallbacks.
    if(cfg.prioritize_classes &&
       !(features::SpecialPriorityMask(cfg)&features::SpecialPriorityBit(e.kind))) return Rejection::Type;
    auto& state=State(api,e);
    if((e.kind==Kind::Common || e.kind==Kind::Witch) && state.dead) return Rejection::Dead;
    if(e.index<=64 && !cfg.target_friends) {
        if(GetTickCount64()>=state.friend_check) {
            state.friend_known=api.Friend(e,state.friend_value); state.friend_check=GetTickCount64()+2000;
        }
        if(state.friend_known && state.friend_value) return Rejection::Friend;
    }
    switch(e.kind) {
        case Kind::Common:if(!cfg.target_common || (cfg.ignore_idle && state.idle)) return Rejection::Type;break;
        case Kind::Survivor:if(!cfg.target_survivor) return Rejection::Type;break;
        case Kind::Tank:if(!cfg.target_tank) return Rejection::Type;break;
        case Kind::Witch:{
            if(!cfg.target_witch) return Rejection::Type;
            float rage{};
            if(cfg.ignore_calm_witch && (!memory::Field(e.ptr,e.fields->rage,rage) || !std::isfinite(rage) || rage<1.f)) return Rejection::Type;
            break;
        }
        case Kind::Player:return Rejection::Type;
        default:if(!cfg.target_special) return Rejection::Type;break;
    }
    if(cfg.ignore_burning) {
        bool burning{};
        if(state.burning || (e.fields->burning>=0 && memory::Field(e.ptr,e.fields->burning,burning) && burning)) return Rejection::Type;
    }
    return Rejection::None;
}
}
bool Eligible(game::Api& api,const features::Config& cfg,const Entity& e,const Entity& local) {
    return Filter(api,cfg,e,local)==Rejection::None;
}
void Capture(game::Api& api,const features::Config& cfg,std::span<const int> indices) {
    if(!cfg.aim || !cfg.history) {if(history_active) ClearHistory();return;}
    Entity local{}; game::Globals clock{};
    if(!api.Local(local) || !api.Clock(clock)) {if(history_active) ClearHistory();return;}
    history_active=true;
    const float lerp=api.Interpolation(clock.interval);
    for(const int i:indices) {
        if(i<1 || i>=static_cast<int>(history.size())) continue;
        auto& h=history[i]; Entity e{}; float time{};
        if(!api.Read(i,e) || !Eligible(api,cfg,e,local) ||
           !memory::Field(e.ptr,e.fields->simulation,time) || !std::isfinite(time) || time<=0.f) {h={};continue;}
        // Pay SetupBones only for candidates we could realistically engage.
        const float reach=cfg.aim_distance/.0254f+64.f;
        if(i>64 && game::Length(game::Sub(e.origin,local.origin))>reach) {h={};continue;}
        using Model=const void*(__thiscall*)(void*);
        const void* model{};
        if(!memory::Invoke(memory::Virtual<Model>(e.renderable,8,api.base.client_image),model,e.renderable) || !model) {h={};continue;}
        if(h.handle!=e.handle || h.model!=model || (!h.records.empty() && (time<h.records.front().time ||
           game::Length(game::Sub(e.origin,h.records.front().origin))>64.f))) {h={};h.handle=e.handle;}
        h.model=model;
        // These are rendered/interpolated bones, not a reconstructed network
        // pose. Stamp that render time instead of the newer simulation time.
        // Avoid recording extrapolation when the interpolation buffer is empty.
        const float pose_time=clock.time-lerp;
        if(!std::isfinite(pose_time) || pose_time<=0.f || pose_time>time || time-pose_time>.25f) continue;
        if(!h.records.empty() && pose_time-h.records.front().time<clock.interval*.9f) continue;
        Record record{};
        if(!h.records.empty() && (h.records.size()>=32 || clock.time-h.records.back().time>.25f)) {
            record=std::move(h.records.back());h.records.pop_back();
        }
        record.time=pose_time;record.origin=e.origin;
        // Capture bones with the current clock: SetupBones(t=0) recomputes a
        // mid-update pose that does not match the interpolation state and
        // drags aim off target.
        if(!api.Hitboxes(e,record.points,clock.time)) continue;
        h.records.push_front(std::move(record));
        while(h.records.size()>32 || (!h.records.empty() && clock.time-h.records.back().time>.25f)) h.records.pop_back();
    }
}
bool Select(game::Api& api,const features::Config& cfg,const Entity& local,const Entity& weapon,
            const game::Command& original,const game::Globals& clock,Vec3 eye,std::span<const int> indices,Result& result) {
    result={};
    const unsigned special_priority=features::SpecialPriorityMask(cfg);
    if(tracking.local!=local.handle || tracking.weapon!=weapon.handle || tracking.mode!=cfg.aim_mode ||
       tracking.priority!=cfg.priority || tracking.preset!=cfg.hitbox || tracking.mask!=cfg.hitbox_mask ||
       tracking.special_priority!=special_priority || tracking.class_filter!=cfg.prioritize_classes ||
       original.number<tracking.command || original.number-tracking.command>64) tracking={};
    tracking.local=local.handle;tracking.weapon=weapon.handle;tracking.mode=cfg.aim_mode;
    tracking.priority=cfg.priority;tracking.preset=cfg.hitbox;tracking.mask=cfg.hitbox_mask;tracking.command=original.number;
    tracking.special_priority=special_priority;
    tracking.class_filter=cfg.prioritize_classes;
    struct Candidate {Entity entity;float distance{},fov{};bool idle{},blacklisted{};};
    const auto preferred=[&](const Candidate& c){return (special_priority&features::SpecialPriorityBit(c.entity.kind))!=0;};
    static thread_local std::vector<Candidate> candidates;
    candidates.clear();if(candidates.capacity()<64) candidates.reserve(64);
    for(int index:indices) {
        Entity e{};
        // Read replicated ghosts for diagnostics, then reject them explicitly
        // below. They remain unavailable to target selection and traces.
        if(!api.Read(index,e,true,true)) {
            ++result.unreadable;
            if(index>0 && index<static_cast<int>(history.size())) history[index]={};
            continue;
        }
        const auto rejected=Filter(api,cfg,e,local);
        if(rejected!=Rejection::None) {
            if(rejected==Rejection::Friend) ++result.filtered_friends;
            else if(rejected==Rejection::Type) ++result.filtered_types;
            else if(rejected==Rejection::Ghost) ++result.filtered_ghosts;
            else if(rejected==Rejection::Dead) {
                ++result.filtered_dead;
                if(index>0 && index<static_cast<int>(history.size())) history[index]={};
            }
            continue;
        }
        const auto center=game::Add(e.origin,game::Mul(game::Add(e.mins,e.maxs),.5f));
        const float distance=game::Length(game::Sub(center,eye));
        const float fov=game::Fov(original.angles,game::Angle(eye,center));
        if(distance>cfg.aim_distance/.0254f) {++result.outside_range;continue;}
        const float margin=std::atan2(game::Length(game::Sub(e.maxs,e.mins))*.5f,std::max(distance,1.f))*57.29578f;
        if(fov>cfg.fov+margin) {++result.outside_fov;continue;}
        const auto& state=State(api,e);
        result.friend_api=result.friend_api || state.friend_known;
        const bool blacklisted=state.blacklist_until>clock.time && tracking.target!=e.handle;
        if(cfg.priority==2 && blacklisted && OneShot(weapon,e)) continue;
        candidates.push_back({e,distance,fov,state.idle,blacklisted});
    }
    result.candidates=static_cast<unsigned>(candidates.size());
    std::stable_sort(candidates.begin(),candidates.end(),[&](const auto& a,const auto& b) {
        if(preferred(a)!=preferred(b)) return preferred(a);
        if(cfg.priority==0) return a.fov<b.fov;
        if(cfg.priority==1) return a.distance<b.distance;
        if(a.blacklisted!=b.blacklisted) return !a.blacklisted;
        if(a.idle!=b.idle) return !a.idle;
        const auto rank=[](const Candidate& c) {
            if(c.entity.kind==Kind::Smoker || c.entity.kind==Kind::Jockey || c.entity.kind==Kind::Charger) return 0;
            if(c.distance<300.f && c.entity.kind==Kind::Common) return 1;
            return Special(c.entity.kind)?2:3;
        };
        if(rank(a)!=rank(b)) return rank(a)<rank(b);
        return a.distance<b.distance;
    });
    if(tracking.target) {
        const auto retained=std::find_if(candidates.begin(),candidates.end(),[](const auto& c){return c.entity.handle==tracking.target;});
        if(retained!=candidates.end() && retained!=candidates.begin()) {
            const auto& first=candidates.front();
            const auto rank=[](const Candidate& c) {
                if(c.entity.kind==Kind::Smoker || c.entity.kind==Kind::Jockey || c.entity.kind==Kind::Charger) return 0;
                if(c.distance<300.f && c.entity.kind==Kind::Common) return 1;
                return Special(c.entity.kind)?2:3;
            };
            const bool better_band=(preferred(first) && !preferred(*retained)) || (cfg.priority==2 &&
                ((first.idle!=retained->idle && !first.idle) ||
                 (first.idle==retained->idle && rank(first)<rank(*retained))));
            const bool same_band=preferred(first)==preferred(*retained) &&
                (cfg.priority!=2 || (first.idle==retained->idle && rank(first)==rank(*retained)));
            const bool improved=same_band && logic::ClearAdvantage(cfg.priority==0?first.fov:first.distance,
                                                                  cfg.priority==0?retained->fov:retained->distance,cfg.priority);
            if(!better_band && !improved) std::rotate(candidates.begin(),retained,retained+1);
        }
    }
    float best=FLT_MAX;
    Result retained_result{};float retained_fov=FLT_MAX;
    static thread_local std::vector<game::Point> points;
    points.clear();if(points.capacity()<128) points.reserve(128);
    const float lerp=api.Interpolation(clock.interval);
    const float correction=std::clamp(lerp+api.NetworkLatency(),0.f,1.f);
    for(size_t i=0;i<candidates.size() && i<64 && result.traces<192;++i) {
        const auto& candidate=candidates[i]; const auto& e=candidate.entity;
        if(cfg.priority!=0 && result.entity.ptr) break; // Sorted distance / smart-shot order.
        // Once a marked class has a valid point, FOV comparisons must stay in
        // that class band. Hidden or otherwise invalid marked targets fall through.
        if(result.entity.ptr && (special_priority&features::SpecialPriorityBit(result.entity.kind)) && !preferred(candidate)) break;
        bool found=false; Result point_result{};
        point_result.traces=result.traces;
        // Every mode shoots the same current pose. History is a visibility
        // fallback, not the first choice for an exposed moving infected.
        if(api.Hitboxes(e,points,clock.time)) {
            ++result.hitboxes;
            found=BestPoint(api,cfg,local,weapon,e,eye,original.angles,points,point_result);
            memory::Field(e.ptr,e.fields->simulation,point_result.simulation_time);
        }
        if(!found && cfg.history && e.index<static_cast<int>(history.size()) && history[e.index].handle==e.handle) {
            if(!history[e.index].records.empty()) ++result.hitboxes;
            for(const auto& record:history[e.index].records) {
                if(!logic::ValidRecord(clock.time,record.time,correction,clock.interval,cfg.history_ms*.001f)) continue;
                int shot_tick{};
                if(!logic::HistoricalTick(true,record.time,lerp,clock.interval,original.tick,
                    clock.time,correction,cfg.history_ms*.001f,shot_tick)) continue;
                ++result.records;
                if(BestPoint(api,cfg,local,weapon,e,eye,original.angles,record.points,point_result,true)) {
                    point_result.simulation_time=record.time; point_result.historical=true; found=true; break;
                }
                if(!cfg.accurate_unlag) break;
            }
        }
        result.traces=point_result.traces;
        if(!found) continue;
        const float fov=game::Fov(original.angles,game::Angle(eye,point_result.point));
        if(e.handle==tracking.target) {retained_result=point_result;retained_result.entity=e;retained_fov=fov;}
        if(fov>=best && cfg.priority==0) continue;
        best=fov;
        result.entity=e; result.point=point_result.point; result.hitgroup=point_result.hitgroup;
        result.bone=point_result.bone;result.hitbox=point_result.hitbox;result.sample=point_result.sample;result.radius=point_result.radius;
        result.simulation_time=point_result.simulation_time; result.historical=point_result.historical;
    }
    if(cfg.priority==0 && retained_result.entity.ptr &&
       bool(special_priority&features::SpecialPriorityBit(result.entity.kind))==bool(special_priority&features::SpecialPriorityBit(retained_result.entity.kind)) &&
       !logic::ClearAdvantage(best,retained_fov,0)) {
        result.entity=retained_result.entity;result.point=retained_result.point;result.hitgroup=retained_result.hitgroup;
        result.bone=retained_result.bone;result.hitbox=retained_result.hitbox;result.sample=retained_result.sample;result.radius=retained_result.radius;
        result.simulation_time=retained_result.simulation_time;result.historical=retained_result.historical;
    }
    if(result.entity.ptr) {
        tracking.target=result.entity.handle;tracking.bone=result.bone;tracking.hitbox=result.hitbox;tracking.sample=result.sample;
    } else tracking.target=0;
    return result.entity.ptr!=nullptr;
}
bool SelectInfected(game::Api& api,const features::Config& config,const Entity& local,
                    const game::Command& original,const game::Globals& clock,
                    Vec3 eye,std::span<const int> indices,Result& result) {
    result={};
    if(!config.aim || !config.infected_aim || local.team!=3 || local.health<=0 ||
       local.ghost || !features::SpecialPriorityBit(local.kind)) {ReleaseTarget();return false;}
    auto cfg=config;
    cfg.aim_mode=0;cfg.history=false;cfg.prioritize_classes=false;
    cfg.target_survivor=true;
    cfg.target_common=cfg.target_special=cfg.target_tank=cfg.target_witch=false;
    if(cfg.hitbox==0) cfg.hitbox=2;
    return Select(api,cfg,local,Entity{},original,clock,eye,indices,result);
}
void OnShot(const Result& result,const game::Globals& clock,float latency) {
    if(result.entity.index<1 || result.entity.index>=8192) return;
    auto& state=states[result.entity.index];
    if(state.handle==result.entity.handle) state.blacklist_until=clock.time+latency+clock.interval*2.f;
}
void ReleaseTarget() {tracking={};}
void Reset() {ReleaseTarget();states.fill({});ClearHistory();}
}
