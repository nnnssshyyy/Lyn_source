#include "visuals.h"
#include "esp_style.h"
#include "license.h"
#include "log.h"
#include "weapon_catalog.h"
#include "visual_events.h"
#include "grenade_prediction.h"
#include <atomic>
#include <cstring>
#include <mutex>

namespace visuals {
namespace {
using source::Kind;
using namespace events;
using Event=events::Raw;
struct Player {int userid{},index{};unsigned handle{};};
struct Action {int userid{},subject{};unsigned flag{};std::uint64_t time{};};
std::array<Player,64> players{};
std::array<Action,64> actions{};
events::DamageDuplicates damage_duplicates{};
struct Cached {unsigned handle{};source::Vec3 point{};std::uint64_t time{};int peak{};};
std::array<Cached,4096> positions{};
std::array<Event,128> queue{};unsigned queued{};
std::mutex event_lock;
SRWLOCK snapshot_lock=SRWLOCK_INIT;
Snapshot working{},published{};
memory::Range engine_image{};void* manager{};
std::atomic_bool accepting{};
unsigned registered{};int local_user{},local_index{};
std::uint64_t next_registration{},next_players{},next_entities{};
grenade_prediction::Cache path_cache;
float gravity=800.f;std::uint64_t next_gravity{};
logic::V To(source::Vec3 p) {return {p.x,p.y,p.z};}
source::Vec3 From(logic::V p) {return {p.x,p.y,p.z};}
class Listener {
public:
    virtual ~Listener()=default;
    virtual void FireGameEvent(void* event) noexcept {
        if(!event || !accepting.load(std::memory_order_acquire) || !licensing::Active())return;
        Event raw{};if(!events::Decode(event,engine_image,raw))return;raw.time=GetTickCount64();
        std::lock_guard lock(event_lock);
        if(accepting.load(std::memory_order_relaxed) && queued<queue.size())queue[queued++]=raw;
    }
    virtual int GetEventDebugID() {return 42;}
} listener;
void Register(bool enable,std::uint64_t now) {
    if(!manager)return;
    if(!enable) {
        if(registered) {using Remove=void(__thiscall*)(void*,Listener*);memory::InvokeVoid(memory::Virtual<Remove>(manager,5,engine_image),manager,&listener);}
        registered=0;next_registration=0;return;
    }
    if(now<next_registration)return;
    next_registration=now+5000;
    using Add=bool(__thiscall*)(void*,Listener*,const char*,bool);
    const auto add=memory::Virtual<Add>(manager,3,engine_image);
    for(unsigned i=0;i<EventCount;++i)if(!(registered&(1u<<i))) {
        bool ok{};if(memory::Invoke(add,ok,manager,&listener,Names[i],false)&&ok)registered|=1u<<i;
    }
}
int Index(int userid) {for(const auto& p:players)if(userid>0&&p.userid==userid)return p.index;return 0;}
void UpdatePlayers(const game::Api& api,const game::Entity& local,std::uint64_t now) {
    if(now<next_players&&local.index==local_index&&local_user>0)return;
    local_index=local.index;
    next_players=now+250;players={};int maximum{};memory::Invoke(api.base.max_clients,maximum,api.base.engine);
    for(int i=1;i<=std::clamp(maximum,0,64);++i) {
        source::PlayerInfo info{};bool ok{};game::Entity entity{};
        if(memory::Invoke(api.base.player_info,ok,api.base.engine,i,&info)&&ok&&info.userid>0) {
            // Identity is engine metadata and remains valid when a player's
            // render entity is dormant, dead or temporarily unavailable.
            api.Read(i,entity,false,true);players[i-1]={info.userid,i,entity.handle};
        }
    }
    const int previous=local_user;local_user=0;
    for(const auto& p:players)if(p.index==local.index)local_user=p.userid;
    if(previous!=local_user) {working.damage={};damage_duplicates={};actions={};}
}
void Process(const game::Api& api,const features::Config& cfg,std::uint64_t now) {
    std::array<Event,128> incoming{};unsigned count{};
    {std::lock_guard lock(event_lock);count=queued;std::copy_n(queue.begin(),count,incoming.begin());queued=0;}
    for(unsigned i=0;i<count;++i) {
        auto e=incoming[i];if(now<e.time || now-e.time>1000)continue;
        if(e.kind==RoundStart) {working.damage={};working.deaths={};damage_duplicates={};actions={};positions={};continue;}
        e.attacker=events::Attacker(e,local_user,local_index);
        if(e.kind==PlayerHurt || e.kind==InfectedHurt || e.kind==PlayerHurtConcise) {
            if(!cfg.hitmarker&&!cfg.damage_numbers)continue;
            if(e.attacker!=local_user || local_user<=0 || e.amount<=0 || e.amount>1000000 || (e.kind!=InfectedHurt&&e.victim==local_user))continue;
            if(damage_duplicates.Consume(e)) {working.damage.head|=e.head!=0;continue;}
            // A confirmed marker does not require the victim to still exist.
            working.damage.Confirm(local_user,e.attacker,e.head!=0,e.time);
            const int index=e.kind==InfectedHurt?e.victim:Index(e.victim);
            game::Entity victim{};logic::V point{};bool positioned{};unsigned identity{};
            if(index>0&&api.Read(index,victim,false,true)) {
                identity=victim.handle;point=To(victim.origin);point.z+=std::clamp(victim.maxs.z*.8f,20.f,100.f);positioned=true;
            } else if(index>0&&index<int(positions.size())&&positions[index].handle&&now-positions[index].time<350) {
                identity=positions[index].handle;point=To(positions[index].point);positioned=true;
            }
            // A despawned victim still confirms the central marker; no invented
            // world position. Distinct user IDs cannot merge after slot reuse.
            if(!identity&&e.kind!=InfectedHurt&&e.victim>0)identity=0x80000000u|unsigned(e.victim);
            if(!identity&&index>0)identity=0x40000000u|unsigned(index);
            working.damage.Add(local_user,e.attacker,identity,e.amount,point,positioned,e.head!=0,e.time);
        } else if(e.kind==PlayerDeath||e.kind==InfectedDeath) {
            if(e.aborted)continue;
            if(cfg.hitmarker&&(e.kind==InfectedDeath||e.victim!=local_user))
                working.damage.Confirm(local_user,e.attacker,e.head!=0,e.time);
            if(!cfg.death_particles||e.kind!=PlayerDeath)continue;
            const int index=e.victim>0?Index(e.victim):e.entity;
            logic::V point{e.x,e.y,e.z};bool positioned=e.position;
            unsigned handle{};game::Entity victim{};
            if(index>0&&api.Read(index,victim,false,true)) {
                handle=victim.handle;if(!positioned){point=To(victim.origin);positioned=true;}
            } else if(!positioned&&index>0&&index<int(positions.size())&&positions[index].handle&&
                      now>=positions[index].time&&now-positions[index].time<350) {
                handle=positions[index].handle;point=To(positions[index].point);point.z-=30.f;positioned=true;
            }
            const std::uint64_t identity=e.victim>0?(0x100000000ull|unsigned(e.victim)):
                handle?std::uint64_t(handle):e.entity>0?(0x200000000ull|unsigned(e.entity)):0;
            // infected_id is not an entity index. World particles only use
            // player_death's explicit victim/coordinates, including NPC deaths.
            if(positioned) {point.z+=30.f;working.deaths.Add(identity,point,e.time);}
        } else if(e.victim>0 && Index(e.victim)>0) {
            const int index=Index(e.victim)-1;if(index<0||index>=int(actions.size()))continue;
            const bool begin=e.kind==HealBegin || e.kind==ReviveBegin;
            if(begin)actions[index]={e.victim,e.subject,e.kind==HealBegin?esp::style::Healing:esp::style::Reviving,e.time};
            else actions[index]={};
        }
    }
    working.damage.Expire(now,static_cast<unsigned>(cfg.damage_lifetime*1000));
    working.deaths.Expire(now);if(!cfg.death_particles)working.deaths={};
}
void Path(const game::Api& api,const features::Config& cfg,const game::Entity& local,std::uint64_t now) {
    const auto clear=[&] {working.path={};path_cache.Clear();};
    if(!cfg.trajectory || !AliveForTrajectory(local)) {clear();return;}
    game::Entity weapon{};source::Vec3 eye{},angles{},velocity{};
    if(!api.ActiveWeapon(local,weapon)||!api.Eye(local,eye)||!api.Angles(angles)) {clear();return;}
    const int index=api.WeaponIndex(weapon);if(index<0||size_t(index)>=weapons::Count){clear();return;}
    const std::string_view key=weapons::Catalog[index].key;
    if(key!="molotov"&&key!="pipe_bomb"&&key!="vomitjar") {clear();return;}
    if(now>=next_gravity) {float value{};if(api.ConVarFloat("sv_gravity",value)&&std::isfinite(value)&&value>=50&&value<=4000)gravity=value;next_gravity=now+1000;}
    if(!memory::Field(local.ptr,local.fields->velocity,velocity)||!logic::Finite(To(velocity))) {clear();return;}
    const grenade_prediction::Pose pose{local.handle,weapon.handle,To(eye),To(angles),To(velocity),gravity,cfg.trajectory_seconds,key!="pipe_bomb"};
    if(path_cache.Reuse(pose,now))return;
    clear();
    const auto launch=grenade_prediction::NormalThrow(pose.eye,pose.angles,pose.velocity);
    game::Filter filter{};filter.skip=local.ptr;filter.skip_weapon=weapon.ptr;
    const auto cast=[&](logic::V a,logic::V b) {
        game::Trace trace{};const bool ok=api.Cast(From(a),From(b),filter,trace,0x200400Bu,2.f);
        return logic::Collision{To(trace.end),To(trace.normal),trace.fraction,ok,trace.start_solid||trace.all_solid};
    };
    // Ballistic flight remains estimated: moving props, server scripts,
    // release animation timing and VPhysics can change the eventual impact.
    working.path=grenade_prediction::Predict(launch,gravity,cfg.trajectory_seconds,pose.break_on_impact,cast);
    if(working.path.valid&&working.path.count>1)path_cache.Remember(pose,now);
}
void Publish() {AcquireSRWLockExclusive(&snapshot_lock);published=working;ReleaseSRWLockExclusive(&snapshot_lock);}
}
void Initialize(const game::Api&) noexcept {
    engine_image=memory::Module(GetModuleHandleW(L"engine.dll"));manager=game::Interface(L"engine.dll","GAMEEVENTSMANAGER002");
    app::Log("Visual HUD: GameEventManager=%p",manager);
}
#ifdef VISUALS_TESTING
void InitializeEventFixture(void* value,memory::Range code) noexcept {manager=value;engine_image=code;}
#endif
bool Needed(const features::Config& c) noexcept {return c.hitmarker||c.damage_numbers||c.death_particles||c.offscreen||c.boss_hud||c.trajectory;}
void Capture(const game::Api& api,const features::Config& cfg,std::span<const int> indices,const game::Entity& local,const source::Matrix& matrix,bool valid) noexcept {
    try {
        const auto now=GetTickCount64();
        const bool events=cfg.hitmarker||cfg.damage_numbers||cfg.death_particles||(cfg.esp&&(cfg.esp_flags||cfg.esp_status_icons));
        accepting.store(events,std::memory_order_release);Register(events,now);
        UpdatePlayers(api,local,now);Process(api,cfg,now);
        working.matrix=matrix;working.time=now;working.valid=valid;working.origin=local.origin;source::Vec3 view{};
        if(api.Angles(view))working.yaw=view.y;else working.valid=false;
        working.listener=(registered&1)!=0;
        const bool scan_entities=now>=next_entities;
        if(scan_entities) {working.boss_count=working.threat_count=0;next_entities=now+50;}
        // Only the small threat/boss snapshot is scanned here; no bones or names.
        if(scan_entities&&(cfg.offscreen||cfg.boss_hud||cfg.damage_numbers||cfg.death_particles))for(int index:indices) {
            game::Entity e{};
            if(index==local.index||!api.Read(index,e,true,false)||e.kind==Kind::None)continue;
            if(index>0&&index<int(positions.size())) {
                auto& cache=positions[index];const int peak=cache.handle==e.handle?std::max(cache.peak,e.health):e.health;
                auto point=e.origin;point.z+=std::clamp(e.maxs.z*.8f,20.f,100.f);cache={e.handle,point,now,peak};
            }
            const float distance=game::Length(game::Sub(e.origin,local.origin))*.0254f;
            if(!std::isfinite(distance))continue;
            if(cfg.boss_hud&&(e.kind==Kind::Tank||e.kind==Kind::Witch)) {
                int maximum{};float rage{};bool burning{};
                memory::Field(e.ptr,e.fields->max_health,maximum);memory::Field(e.ptr,e.fields->rage,rage);memory::Field(e.ptr,e.fields->burning,burning);
                if(maximum<=0&&index>0&&index<int(positions.size()))maximum=positions[index].peak;
                maximum=e.health>0?std::clamp(std::max(maximum,e.health),1,1000000):0;
                if(working.boss_count<working.bosses.size())working.bosses[working.boss_count++]={e.handle,e.kind,e.health,maximum,
                    std::isfinite(rage)?int(std::clamp(rage,0.f,1.f)*100):0,distance,burning};
            }
            const bool show=e.kind!=Kind::Survivor&&e.kind!=Kind::Player&&(e.kind!=Kind::Common||cfg.offscreen_common)&&e.team!=local.team;
            if(cfg.offscreen&&show&&distance<=cfg.offscreen_range&&working.threat_count<working.threats.size())
                working.threats[working.threat_count++]={e.origin,e.kind,distance};
        }
        std::sort(working.bosses.begin(),working.bosses.begin()+working.boss_count,[](const auto& a,const auto& b){return a.distance<b.distance;});
        Path(api,cfg,local,now);Publish();
    } catch(...) {working.valid=false;Publish();}
}
unsigned ActionFlags(const game::Entity& e,std::uint64_t now) noexcept {
    unsigned flags{};int userid{};for(const auto& p:players)if(p.index==e.index&&p.handle==e.handle)userid=p.userid;
    if(!userid)return 0;
    for(const auto& a:actions)if(a.userid&&now>=a.time&&now-a.time<10000) {
        if(a.userid==userid)flags|=a.flag;
        if(a.subject==userid)flags|=a.flag==esp::style::Reviving?esp::style::Revived:esp::style::Healing;
    }
    return flags;
}
void ClearLevel() noexcept {
    if(!working.time&&!registered)return;
    accepting.store(false,std::memory_order_release);Register(false,0);
    {std::lock_guard lock(event_lock);queued=0;}
    working={};path_cache.Clear();players={};positions={};actions={};damage_duplicates={};local_user=local_index=0;next_players=next_gravity=next_entities=0;Publish();
}
void Suspend() noexcept {accepting.store(false,std::memory_order_release);}
Snapshot ReadSnapshot() noexcept {Snapshot copy{};AcquireSRWLockShared(&snapshot_lock);copy=published;ReleaseSRWLockShared(&snapshot_lock);return copy;}
}
