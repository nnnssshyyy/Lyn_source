#include "features.h"
#include "esp_layout.h"
#include "esp_health_text.h"
#include "aimbot.h"
#include "player_name.h"
#include "aim_logic.h"
#include "aim_camera.h"
#include "aim_tracking.h"
#include "spread_logic.h"
#include "shotgun_pattern.h"
#include "esp_style.h"
#include "overlay_paint.h"
#include "overlay_snapshot.h"
#include "survivor_health.h"
#include "movement_batch.h"
#include "air_movement.h"
#include "movement_assists.h"
#include "rapid_fire.h"
#include "combat_input.h"
#include "storm_fall.h"
#include "chat_spy.h"
#include "visuals.h"
#include "visual_removals.h"
#include "client_guard.h"
#include "charger_turn.h"
#include "charger_teleport.h"
#include "hooks.h"
#include "license.h"
#include "log.h"
#include <MinHook.h>
#include <algorithm>
#include <array>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <intrin.h>
#include <limits>

namespace features {
namespace {
using game::Vec3;
using game::Entity;
using source::Kind;
game::Api api;
std::recursive_mutex game_mutex;
SRWLOCK shared_lock=SRWLOCK_INIT;
Config pending{},cfg{};
Config base_config{};
std::uint64_t pending_revision{1},game_config_revision{},overlay_revision{};
int profile_index{-2};
Status status{},published_status{};
std::atomic_bool active{false};
bool Running() noexcept { return active.load(std::memory_order_acquire) && licensing::Active(); }
std::atomic_bool restore_requested{false};
std::atomic_bool models_enabled{false};
std::atomic_bool third_active{false};
std::atomic_bool third_requested{false};
charger_turn::Patch charger_patch;
charger_teleport::Pulse teleport_pulse;
std::atomic<void*> third_owner{};
uintptr_t third_override_return{};
std::vector<int> indices;
ULONGLONG next_scan{},trigger_since{};
unsigned trigger_handle{};
aimbot::VisibleCamera aim_camera{};
aimbot::NormalTracking normal_tracking{};
aimbot::VisibleCamera spread_camera{};
std::array<spread_logic::Sample,150> shotgun_samples{};
shotgun_pattern::History shotgun_history;
using WriteUsercmd=void(__cdecl*)(void*,const game::Command*,const game::Command*);
WriteUsercmd original_write_usercmd{};
using Move=void(__thiscall*)(void*,game::Command*);
uintptr_t packet_return{};
uintptr_t command_return{};
int packet_displacement{};
bool force_next_send{};
// Rapidfire: Raze's shared extra-move counter and per-command correction ring.
// These fields are touched on the game thread from CL_Move / CreateMove /
// RunCommand. rapid_frame_ready is published atomically for the render thread.
int extra_index{};               // 0 = original frame command; 1..N = burst extras
int speed_extras{},rapid_extras{};
int rapid_extra_moves{-1};       // Raze 0x103F6DBC: -1 = base command may arm
rapid_fire::Ring rapid_ring{};
game::ChannelPatch rapid_channel;
game::ChannelMoveBudget rapid_send_budget;
double rapid_pacing_removed{},rapid_pacing_peak{};
bool rapid_pacing_warned{};
rapid_fire::ReleaseRecovery rapid_recovery;
std::atomic_bool rapid_frame_ready{false};
bool rapid_armed{},rapid_channel_failed{},rapid_prediction_failed{},rapid_base_corrected{};
float rapid_interval{};
void* rapid_tickbase_entity{};
int rapid_tickbase_offset{-1};
unsigned rapid_owner_handle{};
storm_fall::History storm_history;
void* storm_player{};
int storm_tickbase{-1},storm_ducked{-1};
uintptr_t storm_prediction_return{};
bool storm_update_hook{},storm_movement_hook{},storm_failed{},inside_storm_prediction{};
using PredictionUpdate=void(__fastcall*)(void*,void*,int,bool,int,int);
PredictionUpdate original_prediction_update{};
using ProcessMovement=void(__fastcall*)(void*,void*,void*,void*);
ProcessMovement original_process_movement{};
int rapid_first_command{},rapid_last_command{};
int rapid_generated_last{};
rapid_fire::PistolEdge pistol_edge{};
// Per-frame caches. Every rapidfire extra replays the same input frame, so
// config, focus, entity resolutions, target selection and status publishing
// only need to happen once per engine tick: reusing them removes the
// per-extra hook cost without changing extra moves, corrections or shots.
unsigned frame_generation{};
unsigned config_generation{};
bool frame_focus_valid{};
bool frame_focus{};
unsigned local_cache_generation{};
Entity local_cache{};
unsigned weapon_cache_generation{};
unsigned weapon_cache_handle{};
Entity weapon_cache{};
struct AimSelection { unsigned generation{}; bool valid{},found{}; aimbot::Result result{}; };
AimSelection aim_selection{};
thread_local bool inside_tick_move{};
movement_batch::SpeedBudget speed_budget{};
void* tick_move_fn{};
void* run_prediction_fn{};
using TickMove=void(__cdecl*)(float,bool);
TickMove original_tick_move{};
using RunCommand=void(__fastcall*)(void*,void*,void*,game::Command*,void*);
RunCommand original_run_command{};
// CRender::RenderView has four stack arguments on the L4D2 x86 build:
// view, HUD view, clear flags and draw flags. Omitting hudview shifts the
// remaining arguments and corrupts the caller's stack when the hook returns.
using RenderView=void(__thiscall*)(void*,game::View*,game::View*,int,int);
using DrawLocal=bool(__thiscall*)(void*);
// CAM_IsThirdPerson returns a 32-bit value in this client. OverrideView uses
// test eax,eax; a bool hook can clear AL while leaving the return address in
// the upper bytes of EAX, so a false result is still treated as true.
using ThirdPerson=int(__thiscall*)(void*,int);
using Viewmodel=void(__thiscall*)(void*,void*,Vec3*,Vec3*);
using WeaponFov=float(__thiscall*)(void*);
Move original_move{};
RenderView original_view{};
DrawLocal original_draw_local{};
ThirdPerson original_thirdperson{};
Viewmodel original_viewmodel{};
WeaponFov original_weapon_fov{};
struct ModelInfo {
    Vec3 origin,angles; void* renderable; const void* model;
    const game::Matrix3* transform; const game::Matrix3* lighting_offset; const Vec3* lighting_origin;
    int flags,index,skin,body,hitbox; unsigned short instance;
};
static_assert(offsetof(ModelInfo,index)==48 && sizeof(ModelInfo)==68);
using ModelDraw=void(__thiscall*)(void*,const void*,const ModelInfo*,game::Matrix3*);
using Override=void(__thiscall*)(void*,void*,int);
ModelDraw original_model{};
Override original_override{};
void* model_renderer{};
void* material_system{};
void* flat_material{};
ULONGLONG next_material_attempt{};
std::mutex material_mutex;
thread_local void* engine_override{};
thread_local int engine_override_type{};
thread_local bool model_inside{},view_inside{};
struct InsideGuard { bool& flag; explicit InsideGuard(bool& value):flag(value){flag=true;} ~InsideGuard(){flag=false;} };
using Overlay=snapshot::Overlay;
Overlay overlay{},published_overlay{};
std::array<bool,8192> model_selected{};
std::array<unsigned,8192> model_tint{};
std::array<unsigned,8192> model_handle{};
struct NameCache {
    unsigned handle{}; int peak{},sequence{-1}; ULONGLONG next{},friend_next{};
    bool idle{},burning{},steam_friend{}; char name[64]{};
};
std::array<NameCache,8192> names{};
struct GlowBackup {
    unsigned handle{}; const game::Fields* fields{};
    int old_type{},old_color{},old_range{},old_min{};
    int written_color{},written_range{};
    bool touched{};
};
std::array<GlowBackup,8192> glow_backup{};

void Message(const char* value) { snprintf(status.message,sizeof(status.message),"%s",value); }
void Publish(bool visuals=false) {
    AcquireSRWLockExclusive(&shared_lock);
    published_status=status;
    if(visuals) {snapshot::Copy(published_overlay,overlay);++overlay_revision;}
    else {published_overlay.aim_point=overlay.aim_point;published_overlay.has_target=overlay.has_target;}
    ReleaseSRWLockExclusive(&shared_lock);
}
void ReadConfig() {
    // Config, weapon profile and netvar layout cannot change inside one engine
    // tick: the rapidfire extras reuse the base command's resolution.
    const bool scoped=inside_tick_move;
    if(scoped && config_generation==frame_generation) return;
    bool changed=false;
    AcquireSRWLockShared(&shared_lock);
    if(game_config_revision!=pending_revision) {
        base_config=pending;game_config_revision=pending_revision;changed=true;
    }
    ReleaseSRWLockShared(&shared_lock);
    status.weapon_index=-1;
    Entity local{},weapon{};
    if(api.Local(local,false) && api.ActiveWeapon(local,weapon)) {
        status.weapon_index=api.WeaponIndex(weapon);
        // Seed Ready's per-frame resolution with the same active weapon.
        if(inside_tick_move) {
            weapon_cache=weapon;weapon_cache_handle=weapon.handle;weapon_cache_generation=frame_generation;
        }
    }
    if(changed || profile_index!=status.weapon_index) {
        cfg=base_config;profile_index=status.weapon_index;ApplyWeaponProfile(cfg,profile_index);
    }
    ApplyCapturePolicy(cfg);
    status.aim_enabled=cfg.aim;status.aim_auto_fire=AutomaticFireRequested(cfg);status.aim_mode=cfg.aim_mode;
    status.aim_activation=cfg.activation;status.aim_key=cfg.aim_key;
    status.aim_weapon_override=profile_index>=0 && static_cast<size_t>(profile_index)<cfg.weapons.size() &&
        cfg.weapons[profile_index].combat && weapons::Catalog[profile_index].group>=0;
    if(!cfg.bhop && !cfg.strafe) {
        status.strafe_active=status.bhop_active=false;
        std::snprintf(status.movement_message,sizeof(status.movement_message),"Movement assistance off");
    }
    if(scoped) config_generation=frame_generation;
}
bool FocusedUncached() {
    DWORD pid{}; GetWindowThreadProcessId(GetForegroundWindow(),&pid);
    return pid==GetCurrentProcessId() && !hooks::MenuOpen();
}
bool Focused() {
    // Focus and menu state cannot change inside one engine tick; the extras
    // reuse the value resolved by the first query of the frame.
    if(!inside_tick_move) return FocusedUncached();
    if(!frame_focus_valid) { frame_focus=FocusedUncached(); frame_focus_valid=true; }
    return frame_focus;
}
bool Key(int key) { return key>0 && key<256 && (GetAsyncKeyState(key)&0x8000)!=0; }
bool Mobile(const Entity& e) {
    bool incap{};
    if(e.fields->incap>=0 && (!memory::Field(e.ptr,e.fields->incap,incap) || incap)) return false;
    for(int offset:e.fields->pinned) {
        unsigned handle{};
        if(offset>=0 && memory::Field(e.ptr,offset,handle) && api.Handle(handle)) return false;
    }
    return true;
}
bool MovementType(const Entity& local) {
    unsigned char type{},water{};
    // Missing movement netvars disable movement assistance; no guessed offsets.
    const bool known=memory::Field(local.ptr,local.fields->move_type,type) && memory::Field(local.ptr,local.fields->water,water);
    if(known) status.move_type=type;
    return known && air_movement::WalkPhysics(type,water);
}
bool ManualLagHeld() {return cfg.lag_exploit && (cfg.lag_key<=0 || Key(cfg.lag_key));}
bool LagPlayerAlive(const Entity& local) {
    unsigned char life{};
    return local.ptr && local.fields && local.health>0 && !local.ghost && (local.team==2 || local.team==3) &&
        memory::Field(local.ptr,local.fields->life,life) && life==0;
}
bool AutomaticFallRequested(const Entity& local) {
    if(!cfg.no_fall_damage || local.team!=2 || !LagPlayerAlive(local) || !Mobile(local)) return false;
    api.ResolveMovement(local);
    if(!MovementType(local)) return false;
    int flags{};float fall_speed{};game::Globals clock{};
    return api.Clock(clock) && memory::Field(local.ptr,local.fields->flags,flags) &&
        memory::Field(local.ptr,local.fields->fall_velocity,fall_speed) &&
        storm_fall::Requested(fall_speed,clock.interval,(flags&1)!=0);
}
bool Eligible(const Entity& e,const Entity& local) {
    if(e.ptr==local.ptr || e.kind==Kind::None || e.team==local.team || e.ghost) return false;
    switch(e.kind) {
        case Kind::Survivor: if(!cfg.target_survivor) return false; break;
        case Kind::Common: if(!cfg.target_common) return false; break;
        case Kind::Tank: if(!cfg.target_tank) return false; break;
        case Kind::Witch: {
            if(!cfg.target_witch) return false;
            float rage{};
            if(cfg.ignore_calm_witch && (!memory::Field(e.ptr,e.fields->rage,rage) || !std::isfinite(rage) || rage<1.f)) return false;
            break;
        }
        case Kind::Player: return false;
        default: if(!cfg.target_special) return false; break;
    }
    if(cfg.ignore_burning && e.fields->burning>=0) {
        bool burning{}; if(!memory::Field(e.ptr,e.fields->burning,burning) || burning) return false;
    }
    return true;
}
bool VisualFilter(const Entity& e) {
    switch(e.kind) {
        case Kind::None: return false;
        case Kind::Survivor: return cfg.esp_survivor;
        case Kind::Common: return cfg.esp_common;
        case Kind::Witch: return cfg.esp_witch;
        default: return cfg.esp_special;
    }
}
bool ObjectFilter(game::Object o) {
    switch(o) {
        case game::Object::Weapon:return cfg.world_weapons;
        case game::Object::Health:return cfg.world_health;
        case game::Object::Ammo:return cfg.world_ammo;
        case game::Object::Throwable:return cfg.world_throwables;
        case game::Object::Projectile:return cfg.world_projectiles;
        case game::Object::Objective:return cfg.world_objectives;
        default:return false;
    }
}
bool Scan() {
    const auto now=GetTickCount64();
    if(now<next_scan) return true;
    int highest{},capacity{};
    if(!memory::Invoke(api.base.highest_entity,highest,api.base.entities) ||
       !memory::Invoke(api.base.entity_capacity,capacity,api.base.entities) || capacity<2 || capacity>8192 || highest<0 || highest>=capacity) return false;
    indices.clear();
    for(int i=1;i<=highest;++i) {
        Entity e{};
        if(api.Read(i,e,false) && (e.kind!=Kind::None || e.object!=game::Object::None)) indices.push_back(i);
    }
    next_scan=now+250; return true;
}
bool Ready(const Entity& local,Entity& weapon,float time) {
    unsigned handle{};
    if(!memory::Field(local.ptr,local.fields->active_weapon,handle)) return false;
    // The active weapon cannot change inside one tick: resolve it once and
    // re-read the firing fields per command (the burst advances the clock).
    if(inside_tick_move && weapon_cache_generation==frame_generation && weapon_cache_handle==handle && weapon_cache.ptr) {
        weapon=weapon_cache;
    } else {
        if(!api.ReadPointer(api.Handle(handle),weapon,false,false,true) || weapon.handle!=handle) return false;
        if(inside_tick_move) {
            weapon_cache=weapon;weapon_cache_handle=handle;weapon_cache_generation=frame_generation;
        }
    }
    status.weapon_group=weapon.fields->weapon_group;
    int clip{}; float next{};
    float player_next{};
    if(local.fields->next_attack>=0 && (!memory::Field(local.ptr,local.fields->next_attack,player_next) ||
       !std::isfinite(player_next) || player_next>time)) return false;
    if(weapon.fields->weapon_group<0 || !memory::Field(weapon.ptr,weapon.fields->clip,clip) || clip<=0 ||
       !memory::Field(weapon.ptr,weapon.fields->next_primary,next) || !std::isfinite(next) || next>time) return false;
    // RijiN's can_fire uses ammunition and the primary attack cooldown. In
    // particular, a loaded shotgun may interrupt reload without clearing its
    // reload flag first; requiring that flag to be false blocks valid shots.
    return true;
}
struct Candidate { Entity entity{}; float score{}; };
bool Select(const Entity& local,const Entity& weapon,Vec3 eye,Vec3 view,float time,Vec3& point,int& target,bool shove=false) {
    if(!Scan()) return false;
    std::vector<Candidate> shortlist;
    shortlist.reserve(32);
    status.candidates=status.traces=0;
    for(int index:indices) {
        Entity e{};
        if(!api.Read(index,e) || e.ptr==local.ptr) continue;
        if(shove) {
            if(e.team!=3 || e.kind==Kind::Tank || e.kind==Kind::Witch || e.kind==Kind::Charger ||
               e.kind==Kind::None || (e.kind==Kind::Common && !cfg.bash_common)) continue;
        } else if(!Eligible(e,local)) continue;
        const auto center=game::Add(e.origin,game::Mul(game::Add(e.mins,e.maxs),.5f));
        const float distance=game::Length(game::Sub(center,eye));
        const float fov=game::Fov(view,game::Angle(eye,center));
        if(distance>(shove?cfg.bash_range+48.f:cfg.aim_distance/.0254f)) continue;
        // Expand the coarse cone by the entity bounds, then check exact hitboxes.
        const float margin=std::atan2(game::Length(game::Sub(e.maxs,e.mins))*.5f,std::max(distance,1.f))*57.29578f;
        if(fov>(shove?cfg.bash_fov:cfg.fov)+margin) continue;
        ++status.candidates;
        float score=cfg.priority==1 ? distance : fov;
        if(cfg.priority==2) score=distance + ((e.kind==Kind::Common || e.kind==Kind::Witch)?10000.f:0.f);
        if(shove) score=distance;
        shortlist.push_back({e,score});
    }
    const size_t limit=std::min<size_t>(shortlist.size(),24);
    std::partial_sort(shortlist.begin(),shortlist.begin()+limit,shortlist.end(),[](const auto& a,const auto& b){return a.score<b.score;});
    game::Filter filter; filter.skip=local.ptr; filter.skip_weapon=weapon.ptr;
    std::vector<game::Point> points; points.reserve(32);
    float best=FLT_MAX;
    for(size_t i=0;i<limit && status.traces<48;++i) {
        const auto& e=shortlist[i].entity;
        if(!api.Hitboxes(e,points,time)) continue;
        // Auto tries the head first, then chest/stomach. Explicit head never
        // silently substitutes an origin/bounding-box estimate.
        std::stable_sort(points.begin(),points.end(),[&](auto& a,auto& b){
            const auto rank=[&](int group){return shove ? (group==2?0:group) : (group==1?0:group);};
            return rank(a.group)<rank(b.group);
        });
        for(const auto& p:points) {
            if(p.group<1 || p.group>7 || status.traces>=48) continue;
            if(shove && p.group!=2 && p.group!=3) continue;
            if(!shove && ((cfg.hitbox==0 && p.group>3) || (cfg.hitbox==1 && p.group!=1) || (cfg.hitbox==2 && p.group!=2 && p.group!=3))) continue;
            const float distance=game::Length(game::Sub(p.position,eye));
            const float fov=game::Fov(view,game::Angle(eye,p.position));
            if(fov>(shove?cfg.bash_fov:cfg.fov) || (shove && distance>cfg.bash_range)) continue;
            game::Trace trace{}; ++status.traces;
            if(!api.Cast(eye,p.position,filter,trace) || trace.start_solid || trace.entity!=e.ptr) continue;
            if(!shove && cfg.hitbox==1 && trace.hitgroup!=1) continue;
            const float score=shove || cfg.priority==1 ? distance : cfg.priority==2 ? shortlist[i].score : fov;
            if(score<best) { best=score; point=p.position; target=e.index; }
            break;
        }
    }
    return target>0;
}
air_movement::Controller air_control;
movement_assists::TickHistory freeze_ticks;
movement_assists::HunterClick hunter_click;
lag_exploit::CommandGate lag_commands;
lag_exploit::Result last_lag_result{};
lag_exploit::Method last_lag_method{};
int lag_origin{};
movement_assists::MeleeCycle melee_cycle;
ULONGLONG next_air_physics{};
void Movement(const Entity& local,game::Command& cmd,const game::Globals& clock) {
    status.strafe_active=status.bhop_active=false;
    if(!cfg.bhop && !cfg.strafe) return;
    const auto message=[](const char* text){std::snprintf(status.movement_message,sizeof(status.movement_message),"%s",text);};
    if(!MovementType(local)) {message("Paused: ladder / swimming / movement type");return;}
    int flags{};
    if(!memory::Field(local.ptr,local.fields->flags,flags)) {message("Movement flags unavailable");return;}
    Vec3 velocity{};
    const bool have_velocity=memory::Field(local.ptr,local.fields->velocity,velocity) && source::Finite(velocity);
    status.movement_speed=have_velocity?std::hypot(velocity.x,velocity.y):0.f;
    // Command assistance uses the validated tick interval. Render frame_time
    // can be zero during prediction and is not a reason to drop bunnyhop.
    if(GetTickCount64()>=next_air_physics) {
        float acceleration{};
        status.movement_physics_known=api.ConVarFloat("sv_airaccelerate",acceleration) && acceleration>=0.f && acceleration<=10000.f;
        status.movement_air_acceleration=status.movement_physics_known?acceleration:10.f;
        next_air_physics=GetTickCount64()+500;
    }
    float maximum=220.f,friction=1.f,value{};
    if(memory::Field(local.ptr,local.fields->max_speed,value) && std::isfinite(value) && value>0.f && value<=10000.f) maximum=value;
    if(memory::Field(local.ptr,local.fields->surface_friction,value) && std::isfinite(value) && value>=0.f && value<=10.f) friction=value;
    const air_movement::Settings settings{cfg.bhop,movement_assists::FastStrafe(cfg.bhop,cfg.bhop_fast,cfg.strafe),cfg.strafe_directional,clock.interval,
                                        status.movement_air_acceleration,maximum,friction};
    const float invalid=std::numeric_limits<float>::quiet_NaN();
    const air_movement::Player player{local.handle,true,(flags&1)!=0,0,have_velocity?velocity.x:invalid,have_velocity?velocity.y:invalid};
    const air_movement::Input input{cmd.number,cmd.seed,cmd.buttons,cmd.mouse_x,cmd.angles.y,cmd.forward,cmd.side};
    const auto assisted=air_control.Run(input,player,settings);
    if(assisted.hop_applied) {
        if((cmd.buttons&2) && !assisted.jump) ++status.bhop_filtered;
        cmd.buttons=(cmd.buttons&~2)|(assisted.jump?2:0);
        status.bhop_active=assisted.held;
    }
    if(assisted.strafed || assisted.restore_move) {
        cmd.forward=assisted.forward;cmd.side=assisted.side;
        game::FixMovement(cmd,assisted.basis_yaw);
        if(assisted.strafed) {++status.strafe_applied;status.strafe_active=!player.grounded;}
    }
    if(!assisted.held) message("Hold jump for bunnyhop / air strafe");
    else if(player.grounded) message("Jump requested: ready for takeoff");
    else if(assisted.strafed) message("Air strafe active: accelerating toward your heading");
    else if(cfg.strafe) message("Air strafe waiting: velocity / acceleration unavailable");
    else message("Bunnyhop active: jump re-arms on landing");
}
struct CommandAim {
    bool applied{},update_view{},shot{},spread_corrected{},force_send{},infected{};
    int shotgun_wire_number{};
    Vec3 visible_angles{};
};
// The burst replays one input frame, so the replicated world cannot change
// between the base command and its extras: the target selection is reused for
// every generated command, keeping the same point for every shot. The reuse
// covers the whole burst: a per-extra sweep (SetupBones plus up to 192
// traces) is what dropped the frame rate to 10-20 FPS when the aimbot and
// the rapidfire ran together.
bool SelectAim(const Entity& local,const Entity& weapon,const game::Command& original,
               const game::Globals& clock,Vec3 eye,aimbot::Result& out) {
    if(rapid_fire::ReuseSelection(inside_tick_move,extra_index,aim_selection.valid,
                                   aim_selection.generation,frame_generation)) {
        out=aim_selection.result;
        return aim_selection.found;
    }
    const bool found=aimbot::Select(api,cfg,local,weapon,original,clock,eye,indices,out);
    if(inside_tick_move) {
        aim_selection.result=out;aim_selection.valid=true;aim_selection.found=found;
        aim_selection.generation=frame_generation;
    }
    return found;
}
void PauseLag(const char* reason) {
    lag_origin=0;status.lag_active=status.fall_lag_active=false;
    std::snprintf(status.lag_message,sizeof(status.lag_message),"%s",reason);
    std::snprintf(status.fall_message,sizeof(status.fall_message),"%s",reason);
}
void StormFailure(const char* reason) {
    if(!storm_failed) app::Log("Storm fall paused: %s",reason);
    storm_failed=true;status.fall_lag_active=false;
    std::snprintf(status.fall_message,sizeof(status.fall_message),"Storm: %s",reason);
}
void StormAssistance(const Entity& local,const game::Command& cmd,bool packet_available,CommandAim& aim) {
    const auto message=[](const char* text){std::snprintf(status.fall_message,sizeof(status.fall_message),"%s",text);};
    status.fall_lag_active=false;
    if(!cfg.no_fall_damage) {message(storm_history.Empty()?"Storm automatic off":"Storm: finishing acknowledged prediction");return;}
    if(storm_failed) {message("Storm unavailable: prediction error; restart the game");return;}
    if(!status.run_command_hook || !storm_update_hook || !storm_movement_hook || !storm_prediction_return || !packet_available ||
       storm_player!=local.ptr || storm_history.Owner()!=local.handle || storm_tickbase<0 || storm_ducked<0) {
        message("Storm unavailable: prediction / movement / send context");return;
    }
    if(!AutomaticFallRequested(local)) {
        lag_origin=0;message(storm_history.Empty()?"Storm: waiting for fall speed 560":"Storm: waiting for command ACK");return;
    }
    if((inside_tick_move && extra_index>0) || rapid_frame_ready.load(std::memory_order_relaxed) || rapid_recovery.pending ||
       !rapid_ring.Empty() || speed_extras>0) {message("Storm paused: draining rapidfire / speed batch");return;}
    const int recorded=storm_history.Get(local.handle,cmd.number,cmd.seed);
    if(recorded) {status.fall_lag_active=true;aim.force_send=true;return;}
    int tickbase{};game::CommandState commands{};
    if(!api.Commands(commands) || !memory::Field(local.ptr,storm_tickbase,tickbase)) {message("Storm: command / tickbase unavailable");return;}
    storm_history.Retire(commands.acknowledged);
    const int shift=storm_fall::SequenceShift(tickbase);
    if(!shift || !storm_history.CanStore(local.handle,cmd.number)) {message("Storm: waiting for a free prediction slot");return;}
    if(!lag_commands.Take(local.handle,cmd.number)) {message("Storm: waiting for a fresh native command");return;}
    if(!api.StormSequence(shift)) {message("Storm: sequence write unavailable");return;}
    if(!storm_history.Store(local.handle,cmd.number,cmd.seed,shift)) {StormFailure("prediction history could not store the shift");return;}
    last_lag_result={};
    ++status.fall_lag_pulses;status.fall_lag_active=true;aim.force_send=true;
    if(lag_origin!=2) app::Log("Storm automatic fall: command=%d tickbase=%d sequence_shift=%d pending=%u",cmd.number,tickbase,shift,storm_history.Count());
    lag_origin=2;
    // Use the engine's normal prediction entry and its RunCommand/MoveHelper
    // lifecycle. No extra CL_Move or second manual player simulation is created.
    if(!api.UpdatePrediction()) {StormFailure("native Prediction::Update failed");return;}
    std::snprintf(status.fall_message,sizeof(status.fall_message),"Storm active | shift %d | pending %u",shift,storm_history.Count());
}
void LagAssistance(const Entity& local,const game::Command& cmd,bool packet_available,CommandAim& aim) {
    const auto message=[](char* target,size_t size,const char* text){std::snprintf(target,size,"%s",text);};
    const bool manual=ManualLagHeld();
    if(!LagPlayerAlive(local)) {PauseLag("Lag waiting for a living player");return;}
    if(!manual && !cfg.no_fall_damage) {
        PauseLag("Lag off");
        if(cfg.lag_exploit) message(status.lag_message,sizeof(status.lag_message),"Hold the lag key");
        return;
    }
    // Network exploits have one owner. Synthetic extra moves must never
    // multiply lag pushes or mix their sequence declarations with rapidfire.
    if((inside_tick_move && extra_index>0) || rapid_frame_ready.load(std::memory_order_relaxed) || rapid_recovery.pending ||
       speed_extras>0) {
        PauseLag("Lag paused: another network exploit is active");return;
    }
    if(!manual) {
        message(status.lag_message,sizeof(status.lag_message),cfg.lag_exploit?"Hold the lag key":"Manual lag off");
        StormAssistance(local,cmd,packet_available,aim);return;
    }
    if(!storm_history.Empty()) {PauseLag("Lag paused: waiting for Storm prediction ACK");return;}
    const auto method=lag_exploit::FromIndex(cfg.lag_method);
    const char* label=method==lag_exploit::Method::V1?"Raze V1":"Raze V2";
    const int amount=method==lag_exploit::Method::V1?cfg.lag_amount:cfg.lag_amount_v2;
    if(lag_exploit::RequiresSendContext(method) && !packet_available) {PauseLag("Raze V1 unavailable: native send context missing");return;}
    const bool fresh=lag_commands.Take(local.handle,cmd.number);
    if(fresh) {
        last_lag_method=method;last_lag_result=api.RazeLag(method,amount);
        if(last_lag_result.shifted) ++status.lag_updates;
    } else if(!lag_commands.Same(local.handle,cmd.number) || last_lag_method!=method) {
        PauseLag("Lag waiting for a fresh native command");return;
    }
    if(last_lag_result.force_send && packet_available) aim.force_send=true;
    if(!last_lag_result.available || !last_lag_result.shifted) {
        PauseLag(last_lag_result.available?"Lag: net channel or sequence unavailable":"Lag: native engine route unavailable");return;
    }
    if(lag_origin!=1) app::Log("Manual lag active: method=%s command=%d sequence_push=%d",label,cmd.number,amount);
    lag_origin=1;status.lag_active=true;status.fall_lag_active=false;
    std::snprintf(status.lag_message,sizeof(status.lag_message),"Manual lag: %s active | push %d",label,amount);
    message(status.fall_message,sizeof(status.fall_message),cfg.no_fall_damage?"Storm paused: manual lag owns the channel":"Storm automatic off");
}
bool MeleeAssistance(const Entity& local,game::Command& cmd,const game::Globals& clock,float time) {
    const auto paused=[&](const char* text) {melee_cycle.Reset();std::snprintf(status.melee_message,sizeof(status.melee_message),"%s",text);return false;};
    if(!cfg.fast_melee) return paused("Fast melee desactivado");
    if(local.team!=2 || local.health<=0 || local.ghost) return paused("Fast melee: requiere superviviente vivo");
    const bool hold=Key(cfg.fast_melee_key);
    if(!hold && !melee_cycle.Active()) return paused(cfg.fast_melee_key>0?"Fast melee: manten su tecla y ataque":"Fast melee: asigna la tecla (mantener)");
    Entity active_weapon{},melee{},primary{};
    if(!api.ActiveWeapon(local,active_weapon)) return paused("Fast melee: arma activa no disponible");
    if(active_weapon.fields->melee) melee=active_weapon;
    // Resolve the real inventory handles. Never assume entity indexes or use
    // console slot commands that could select a medkit after a manual switch.
    if(local.fields->inventory>0 && local.fields->inventory_count>0 && local.fields->inventory_count<=64) {
        for(int i=0;i<local.fields->inventory_count;++i) {
            unsigned handle{};Entity item{};
            if(!memory::Field(local.ptr,local.fields->inventory+i*4,handle) || !handle || handle==UINT_MAX ||
               !api.ReadPointer(api.Handle(handle),item,false,false,true) || item.handle!=handle || item.index<=0) continue;
            if(item.fields->melee) melee=item;
            if(item.fields->primary && !primary.ptr) primary=item;
        }
    }
    if(!melee.ptr || !primary.ptr) return paused("Fast melee: equipa melee y arma primaria");
    bool in_swing{};
    if(melee.fields->melee_swing<0 || !memory::Field(melee.ptr,melee.fields->melee_swing,in_swing))
        return paused("Fast melee: estado nativo del swing no disponible");
    float next{},player_next{};
    const bool ready=active_weapon.fields->melee && memory::Field(active_weapon.ptr,active_weapon.fields->next_primary,next) &&
        memory::Field(local.ptr,local.fields->next_attack,player_next) && std::isfinite(next) && std::isfinite(player_next) && next<=time && player_next<=time;
    const auto cycle=melee_cycle.Run({local.handle,active_weapon.handle,melee.handle,primary.handle,cmd.number,
        melee.index,primary.index,clock.interval,true,hold && (cmd.buttons&1)!=0,ready,cmd.weapon!=0,
        true,in_swing,(cmd.buttons&(32|8192|2048))!=0,cmd.seed,cmd.weapon});
    if(cycle.suppress) cmd.buttons&=~1;
    if(cycle.select>0) {cmd.weapon=cycle.select;cmd.subtype=0;}
    status.fast_melee_active=cycle.cycling;
    std::snprintf(status.melee_message,sizeof(status.melee_message),"%s",cycle.select>0?"Fast melee: cambiando y regresando al melee":
        cycle.cycling?"Fast melee: esperando el swing nativo":hold?"Fast melee: esperando ataque o cooldown":"Fast melee: tecla liberada");
    return active_weapon.fields->melee || cycle.suppress || cycle.select>0;
}
bool Process(game::Command& cmd,bool packet_available,bool flush_tick,CommandAim& aim) {
    aim={};status.aim_applied=false;
    if(!cfg.no_fall_damage && !cfg.lag_exploit) PauseLag("Lag off");
    status.airstuck_active=status.ceiling_active=status.hunter_click_active=status.fast_melee_active=status.fall_lag_active=status.lag_active=false;
    struct TrackingAttempt {CommandAim& aim;~TrackingAttempt(){if(!aim.update_view) normal_tracking.Reset();}} tracking_attempt{aim};
    status.strafe_active=status.bhop_active=false;
    status.target=-1; status.weapon_group=-1;status.weapon_ready=false;
    overlay.has_target=false;
    const bool focused=Focused();
    const bool requested=cfg.aim && !flush_tick && (cfg.activation==2 || (cfg.activation==0 && Key(cfg.aim_key)) || (cfg.activation==1 && (cmd.buttons&1)));
    const bool infected_hold=InfectedAimRequested(cfg,Key(cfg.infected_aim_key),focused,flush_tick);
    const char* activation_reason{};
    if(!cfg.aim) activation_reason=base_config.aim?"Aim desactivado en el perfil del arma":"Aim desactivado: activa enabled";
    else if(!focused) activation_reason=hooks::MenuOpen()?"Pausado: cierra el menu":"Pausado: da foco al juego";
    else if(flush_tick) activation_reason="Silent servidor: enviando vista original";
    else if(!requested && !infected_hold) activation_reason=cfg.activation==1?"Esperando ataque primario":
        cfg.aim_key>0?"Esperando la tecla de aim":"Asigna aim key o selecciona always";
    if((!requested && !infected_hold) || !focused) {aimbot::ReleaseTarget();aim_camera.Reset();}
    // Record failures before target selection too (movement, weapon, clock...).
    // Previously these returned without updating the last-attempt diagnostic.
    struct Attempt {
        bool enabled;const char*& activation_reason;
        ~Attempt() {
            const char* message=activation_reason?activation_reason:status.message;
            snprintf(status.aim_blocked,sizeof(status.aim_blocked),"%s",activation_reason?activation_reason:"");
            if(!activation_reason) snprintf(status.aim_message,sizeof(status.aim_message),"%s",message);
            if(!enabled) return;
            static char previous[128]{}; static ULONGLONG next{};
            static int previous_mode=-1,previous_weapon=-1,previous_activation=-1;
            const auto now=GetTickCount64();
            if(now>=next && (strcmp(previous,message) || previous_mode!=cfg.aim_mode ||
                            previous_weapon!=status.weapon_index || previous_activation!=cfg.activation)) {
                snprintf(previous,sizeof(previous),"%s",message);next=now+2000;
                previous_mode=cfg.aim_mode;previous_weapon=status.weapon_index;previous_activation=cfg.activation;
                app::Log("Aim: %s | modo=%d perfil=%d activacion=%d auto=%d tecla=%d movimiento=%d arma=%d lista=%d candidatos=%u hitboxes=%u trazas=%u packet=%d | %s",
                    message,cfg.aim_mode,status.aim_weapon_override,cfg.activation,AutomaticFireRequested(cfg),cfg.aim_key,status.move_type,status.weapon_group,status.weapon_ready,
                    status.candidates,status.hitbox_targets,status.traces,status.server_silent,status.aim_filters);
            }
        }
    } attempt{base_config.aim || cfg.aim,activation_reason};
    if((requested || infected_hold) && focused) {
        status.candidates=status.traces=status.hitbox_targets=status.history_records=0;
        status.aim_applied=false;status.move_type=-1;
        status.aim_filters[0]=0;
    }
    if(!cfg.aim && !cfg.recoil && !cfg.no_spread && !cfg.trigger && !cfg.auto_bash && !cfg.auto_pistol && !cfg.bhop && !cfg.strafe && !cfg.rapidfire && !cfg.pistol_rapid && !cfg.airstuck && !cfg.auto_ceiling && !cfg.hunter_click && !cfg.fast_melee && !cfg.no_fall_damage && !cfg.lag_exploit) {
        Message("Funciones de comandos desactivadas"); return false;
    }
    if(!focused) {
        hunter_click.Reset();PauseLag("Lag paused: close menu / focus game");melee_cycle.Reset();
        if(cfg.bhop || cfg.strafe) std::snprintf(status.movement_message,sizeof(status.movement_message),"Paused: close menu / focus the game");
        Message("Pausado: menu abierto o juego sin foco");return false;
    }
    Entity local{}; game::Globals clock{};
    if(inside_tick_move) {
        // One entity resolution per engine tick; the burst advances the
        // player, so the fields the aim reads (origin) refresh per command.
        if(local_cache_generation!=frame_generation) {
            local_cache_generation=frame_generation;
            if(!api.Local(local_cache,false,true)) local_cache={};
        }
        local=local_cache;
        if(local.ptr) {
            memory::Field(local.ptr,local.fields->origin,local.origin);
            memory::Field(local.ptr,local.fields->health,local.health);
        }
    } else if(!api.Local(local,false,true)) local={};
    if(!local.ptr || !api.Clock(clock)) {
        hunter_click.Reset();PauseLag("Lag waiting for local player and clock");melee_cycle.Reset();
        if(cfg.bhop || cfg.strafe) std::snprintf(status.movement_message,sizeof(status.movement_message),"Paused: waiting for the local player");
        aimbot::ReleaseTarget();aim_camera.Reset();Message("Esperando jugador local y reloj del motor");return false;
    }
    // An incapped survivor still fires the pistol: keep the press edge alive
    // for pistol rapid. Aim and movement assistance stay paused. The rapidfire
    // arm itself is gated in RapidReady, which now also accepts incap, ghost
    // and dead states while the engine keeps generating commands.
    if(!air_movement::MobileForAssistance(Mobile(local),local.ghost,local.team)) {
        hunter_click.Reset();PauseLag("Lag paused: incap / pinned player");melee_cycle.Reset();
        if(cfg.pistol_rapid && !flush_tick) {
            Entity weapon{};
            const bool semi=api.ActiveWeapon(local,weapon) && weapon.fields && weapon.fields->semi;
            cmd.buttons=combat_input::PausedPrimary(cmd.buttons,true,semi,pistol_edge);
        } else pistol_edge.Next(false);
        if(cfg.bhop || cfg.strafe) std::snprintf(status.movement_message,sizeof(status.movement_message),"Paused: waiting for an active mobile player");
        aimbot::ReleaseTarget();aim_camera.Reset();
        Message(cfg.pistol_rapid?"Incap/pinned: pistol rapid sigue activo; asistencia en pausa":"Incap/pinned: asistencia de comandos en pausa");
        return false;
    }
    float weapon_time=clock.time;
    int tickbase{};
    if(memory::Field(local.ptr,local.fields->tickbase,tickbase) && tickbase>0 && tickbase<100000000)
        weapon_time=static_cast<float>(tickbase)*clock.interval;
    api.ResolveMovement(local);
    status.movement=local.fields->move_type>=0 && local.fields->water>=0 && local.fields->flags>=0 && local.fields->velocity>=0;
    const bool living=local.health>0 && !local.ghost && (local.team==2 || local.team==3);
    if(living && MovementType(local) && cfg.airstuck && cfg.airstuck_key>0 && Key(cfg.airstuck_key)) {
        cmd.tick=movement_assists::FreezeTick;cmd.forward=cmd.side=cmd.up=0.f;
        cmd.buttons&=~(1|2|4|8|16|32|512|1024|2048|8192);
        status.airstuck_active=true;hunter_click.Reset();PauseLag("Lag paused: airstuck active");melee_cycle.Reset();
        return false;
    }
    Movement(local,cmd,clock);
    bool melee_owned=false;
    if(living) {
        unsigned char movement{},water{};
        const bool hunter_motion=memory::Field(local.ptr,local.fields->move_type,movement) && (movement==2 || movement==5) &&
                                 memory::Field(local.ptr,local.fields->water,water) && water<2;
        status.ceiling_active=cfg.auto_ceiling && cfg.ceiling_key>0 && Key(cfg.ceiling_key) && hunter_motion;
        const bool click=cfg.hunter_click && local.team==3 && local.kind==Kind::Hunter && hunter_motion &&
                         cfg.hunter_click_key>0 && Key(cfg.hunter_click_key) && !(cmd.buttons&(32|8192|2048));
        const bool attack=hunter_click.Run(local.handle,cmd.number,cmd.seed,clock.interval,click);
        if(click) {cmd.buttons=(cmd.buttons&~1)|(attack?1:0);status.hunter_click_active=true;}
        melee_owned=MeleeAssistance(local,cmd,clock,weapon_time);
        LagAssistance(local,cmd,packet_available,aim);
    } else {hunter_click.Reset();melee_cycle.Reset();PauseLag("Lag waiting for a living mobile player");}
    if(melee_owned || status.ceiling_active || status.hunter_click_active) return false;
    if(cfg.bhop || cfg.strafe) {
        static ULONGLONG next_move_log{};
        const auto now=GetTickCount64();
        if(now>=next_move_log) {
            next_move_log=now+4000;
            app::Log("Mov: %s | tipo=%d fantasma=%d bhop=%d strafe=%d strafes=%u",status.movement_message,
                status.move_type,local.ghost,cfg.bhop,cfg.strafe,status.strafe_applied);
        }
    }
    // Movement works while waiting to spawn; automatic combat must not press
    // attack in ghost mode and accidentally force a spawn.
    if(local.ghost) {
        aimbot::ReleaseTarget();aim_camera.Reset();
        Message("Modo fantasma: movimiento disponible; combate en pausa");return false;
    }
    unsigned char move_type{};
    const bool known_move=local.fields->move_type>0 && memory::Field(local.ptr,local.fields->move_type,move_type);
    if(known_move) status.move_type=move_type;
    if(known_move && move_type!=2) {
        aimbot::ReleaseTarget();aim_camera.Reset();
        Message("Asistencia de comandos pausada: escalera / movimiento especial"); return false;
    }
    if(local.team==3) {
        status.aim_mode=0;status.aim_auto_fire=false;
        status.aim_activation=0;status.aim_key=cfg.infected_aim_key;
        activation_reason=nullptr;
        const auto release=[&](const char* message) {
            aimbot::ReleaseTarget();aim_camera.Reset();Message(message);return false;
        };
        if(!cfg.infected_aim) return release("Aim de infectado desactivado");
        if(!infected_hold) return release(cfg.infected_aim_key>0?
            "Infectado: manten su tecla de aim":"Infectado: asigna su tecla de aim (mantener)");
        if(local.health<=0 || (cmd.buttons&(32|8192))) return release("Infectado: combate en pausa");
        Vec3 eye{};auto input=cmd;
        if(!api.Eye(local,eye)) return release("Posicion de vista no disponible");
        Vec3 input_view{};if(api.Angles(input_view,false)) input.angles=input_view;
        aimbot::Result selected{};
        const bool found=Scan() && aimbot::SelectInfected(api,cfg,local,input,clock,eye,indices,selected);
        status.candidates=selected.candidates;status.traces=selected.traces;
        status.hitbox_targets=selected.hitboxes;status.history_records=0;
        status.friend_filter=selected.friend_api;
        if(!found) return release("Infectado: sin supervivientes visibles dentro de rango/FOV");
        const auto angle=normal_tracking.Update(local.handle,0,selected.entity.handle,cmd.number,
            selected.hitbox,selected.sample,input.angles,game::Angle(eye,selected.point),cfg.smoothing,clock.interval);
        cmd.angles=angle;
        aim.applied=aim.update_view=aim.infected=true;aim.visible_angles=angle;
        status.target=selected.entity.index;overlay.aim_point=selected.point;overlay.has_target=true;
        Message("Aim infectado: seguimiento visible; ataques manuales");
        return true;
    }
    if(!requested) {aimbot::ReleaseTarget();aim_camera.Reset();}
    Entity weapon{};
    const bool ready=Ready(local,weapon,weapon_time);
    status.weapon_ready=ready;
    if(weapon.ptr && weapon.fields->weapon_group>=0)
        cmd.buttons=aimbot::logic::ShotButtons(cmd.buttons,cfg.aim_mode,false,flush_tick && cfg.aim,0.f,0.f);
    if(cfg.auto_pistol && weapon.ptr && weapon.fields->semi && !ready) cmd.buttons&=~1;
    if(cmd.buttons&(32|8192)) {aimbot::ReleaseTarget();Message("Usando objeto o recargando");return false;}
    Vec3 eye{}; if(!api.Eye(local,eye)) { Message("Posicion de disparo no disponible"); return false; }
    const bool aim_active=requested;
    bool changed=false;
    Vec3 point{}; int target=-1;
    aimbot::Result selected{};
    auto untouched=cmd;
    // Selection/smoothing must use the mouse view, never the compensated shot
    // left in a prediction command (especially recoil/spread during a burst).
    Vec3 input_view{};
    if(aim_active && api.Angles(input_view,false)) untouched.angles=input_view;
    if(aim_active && cfg.aim_mode==2 && !packet_available) Message("Silent servidor no disponible: contexto CL_Move no validado");
    else if(aim_active && !weapon.ptr) Message("Arma activa no disponible");
    else if(aim_active && weapon.fields->weapon_group<0) Message("Aimbot de rayos: el arma no es hitscan");
    // All modes prepare an aimed command throughout the cooldown too: the
    // native firing clock may expose a shot before our readiness estimate.
    else if(aim_active && Scan() &&
            SelectAim(local,weapon,untouched,clock,eye,selected)) {
        status.target=selected.entity.index; overlay.aim_point=selected.point; overlay.has_target=true;
        auto angle=game::Angle(eye,selected.point);
        if(cfg.aim_mode==0) {
            angle=normal_tracking.Update(local.handle,weapon.handle,selected.entity.handle,cmd.number,
                selected.hitbox,selected.sample,untouched.angles,angle,cfg.smoothing,clock.interval);
        }
        // Smoothing must reach the target before an automatic shot.
        const float tolerance=aimbot::logic::ShotTolerance(game::Length(game::Sub(selected.point,eye)),selected.radius);
        const float alignment=game::Fov(angle,game::Angle(eye,selected.point));
        cmd.buttons=aimbot::logic::ShotButtons(cmd.buttons,cfg.aim_mode,true,flush_tick,alignment,tolerance);
        if(ShouldAutoShoot(cfg,ready,alignment,tolerance)) cmd.buttons|=1;
        // Server silent carries its angle only on attack commands, including
        // potential shots during an estimated cooldown. Each such command is
        // held below; idle commands keep the visible direction on the wire.
        if(cfg.aim_mode!=2 || (cmd.buttons&1)) {
            cmd.angles=angle;changed=true;
            aim.applied=true;aim.update_view=cfg.aim_mode==0;aim.visible_angles=angle;
        }
        Message(changed ? "RijiN aim: objetivo visible, angulo aplicado" : "Objetivo visible: esperando disparo");
    } else if(flush_tick) Message("Silent servidor: enviando comando con vista original");
    else if(aim_active && !ready && cfg.aim_mode!=0) Message("Silent: esperando cooldown / municion");
    else if(!aim_active) Message("Listo: cierra el menu y activa la tecla de aim");
    else if(!selected.candidates) Message("Sin entidades elegibles dentro de filtros/rango/FOV");
    else if(!selected.hitboxes) Message("Candidatos encontrados; SetupBones/StudioModel sin hitboxes validas");
    else if(!selected.traces) Message("Hitboxes disponibles; ninguna zona coincide con mascara/FOV");
    else Message("Hitboxes obstruidas; no hay un punto visible");
    if(aim_active) {
        status.candidates=selected.candidates; status.traces=selected.traces;
        status.hitbox_targets=selected.hitboxes; status.history_records=selected.records;
        status.friend_filter=selected.friend_api;
        std::snprintf(status.aim_filters,sizeof(status.aim_filters),
            "Filtered: dead %u / friends %u / types %u / ghosts %u / FOV %u / range %u / unreadable %u",
            selected.filtered_dead,selected.filtered_friends,selected.filtered_types,selected.filtered_ghosts,selected.outside_fov,selected.outside_range,selected.unreadable);
    }
    if(cfg.trigger && !flush_tick && Key(cfg.trigger_key) && ready) {
        game::Filter filter; filter.skip=local.ptr; filter.skip_weapon=weapon.ptr;
        game::Trace tr{}; Entity target_entity{};
        Vec3 shot=cmd.angles,punch{};
        if(!(cfg.recoil && aim.applied) && memory::Field(local.ptr,local.fields->punch,punch) && source::Finite(punch) && game::Length(punch)<90.f) shot=game::Add(shot,punch);
        if(api.Cast(eye,game::Add(eye,game::Mul(game::Forward(shot),8192.f)),filter,tr) &&
           api.ReadPointer(tr.entity,target_entity) && aimbot::Eligible(api,cfg,target_entity,local) && (!cfg.trigger_head || tr.hitgroup==1)) {
            const auto now=GetTickCount64();
            if(trigger_handle!=target_entity.handle) { trigger_handle=target_entity.handle; trigger_since=now; }
            if(now-trigger_since>=static_cast<ULONGLONG>(cfg.trigger_delay)) cmd.buttons|=1;
        } else { trigger_handle=0; trigger_since=0; }
    } else { trigger_handle=0; trigger_since=0; }
    // Pistol rapid: pistols and other semi-autos only fire on a fresh press
    // edge. While attack is held, expose the button on alternating commands so
    // the weapon re-triggers without repeated clicks; each rapidfire extra
    // advances the weapon clock, so the presses turn into fast shots.
    const bool attack_held=(cmd.buttons&1)!=0;
    if(cfg.pistol_rapid && !flush_tick && weapon.ptr && weapon.fields->semi) {
        const bool expose=pistol_edge.Next(attack_held);
        if((cmd.buttons&1)!=expose) {
            if(expose) cmd.buttons|=1; else cmd.buttons&=~1;
            changed=true;
        }
    } else pistol_edge.Next(false);
    // Spread/recoil compensation belongs to the SHOT, not to aimbot lock: it
    // must also work when spraying manually without a target (RijiN applies
    // no-spread this way). Camera feedback is safe: view angles only receive
    // aim.visible_angles (assigned above, before these corrections).
    //
    // The engine fires with its own clock, so any command the aimbot aimed
    // while attack was held may be the one that fires: those commands always
    // carry the historical tick, the punch correction and the spread
    // correction, with or without the per-weapon toggles.
    const bool attacking=!flush_tick && (cmd.buttons&1)!=0;
    bool firing=ready && !flush_tick && (cmd.buttons&1);
    bool aimed_shot=aim.applied && attacking;
    if(aimed_shot && cfg.history && selected.historical) {
        // Live bones already correspond to the engine's interpolated view.
        // Replacing that tick with the latest network simulation time aims
        // at one pose while asking the server to evaluate another.
        int historical_tick{};
        const float lerp=api.Interpolation(clock.interval);
        if(aimbot::logic::HistoricalTick(selected.historical,selected.simulation_time,lerp,
            clock.interval,untouched.tick,clock.time,std::clamp(lerp+api.NetworkLatency(),0.f,1.f),
            cfg.history_ms*.001f,historical_tick)) cmd.tick=historical_tick;
        else {
            // A reused rapidfire selection can outlive its rewind window.
            // Never shoot an old pose with an unrelated current command tick.
            cmd.buttons&=~1;firing=aimed_shot=false;
            if(cfg.aim_mode==2) {cmd.angles=untouched.angles;aim.applied=false;}
            Message("Historical aim expired before firing; waiting for a current solution");
        }
    }
    aim.shot=aim.applied && firing;
    if(aim.shot) {
        // Throttled shot diagnostics: helps confirm whether misses come
        // from lag compensation (tick delta) or plain spread/recoil.
        {
            static ULONGLONG next_shot_log{};
            const auto now=GetTickCount64();
            if(now>=next_shot_log) {
                next_shot_log=now+1500;
                app::Log("Aim shot: modo=%d grupo=%d historico=%d tick %d->%d sim=%.3f ahora=%.3f lat=%.0fms",
                    cfg.aim_mode,selected.hitgroup,selected.historical?1:0,untouched.tick,cmd.tick,
                    selected.simulation_time,clock.time,api.NetworkLatency()*1000.f);
            }
        }
        aimbot::OnShot(selected,clock,api.NetworkLatency());
    }
    if((cfg.recoil && firing) || aimed_shot) {
        Vec3 punch{};
        if(memory::Field(local.ptr,local.fields->punch,punch) && source::Finite(punch) && game::Length(punch)<90.f) {
            cmd.angles=game::Sub(cmd.angles,punch); changed=true; ++status.recoil_applied;
        }
    }
    const bool shotgun=weapon.fields->weapon_group==3;
    const float correction_strength=SpreadCorrectionStrength(cfg,shotgun,firing,aimed_shot);
    if(correction_strength>0.f) {
        const int native_seed=cmd.seed;
        const auto* pattern=shotgun && status.shotgun_grouping && (firing || aimed_shot)?
            shotgun_pattern::Select(native_seed,shotgun_pattern::Pellets(status.weapon_index),correction_strength):nullptr;
        if(pattern)cmd.seed=pattern->seed;
        Vec3 spread{};
        {
            const spread_logic::Identity identity{cmd.number,cmd.tick,cmd.seed,local.handle,weapon.handle,tickbase,clock.interval};
            auto& sample=shotgun_samples[static_cast<size_t>(cmd.number)%shotgun_samples.size()];
            const bool cached=std::isfinite(clock.frame_time) && clock.frame_time>0.f && sample.Matches(identity);
            Vec3 corrected{};
            if(cached) spread=sample.offset;
            if(shotgun) {status.shotgun_sample_weapon=status.weapon_index;status.shotgun_sample_ok=false;}
            if((cached || api.SpreadAtTick(local,weapon,cmd,spread)) &&
               spread_logic::Correct(cached?&sample:nullptr,identity,cmd.angles,spread,corrected,correction_strength)) {
                cmd.angles=corrected;changed=true;aim.spread_corrected=true;
                sample={identity,spread,corrected,true,correction_strength};++status.spread_applied;
                if(shotgun) {status.shotgun_sample_ok=true;status.shotgun_sample_offset=spread;}
                if(pattern)aim.shotgun_wire_number=pattern->number;
            }
        }
        // A failed axis sample must never send a seed whose compensation was
        // not applied. The serializer mapping is committed after the write.
        if(pattern && !aim.shotgun_wire_number)cmd.seed=native_seed;
    }
    if(cfg.auto_bash && !flush_tick && local.team==2 && !attack_held && weapon.ptr) {
        float shove{},secondary{};
        if(memory::Field(local.ptr,local.fields->next_shove,shove) && std::isfinite(shove) && shove<=weapon_time &&
           memory::Field(weapon.ptr,weapon.fields->next_secondary,secondary) && std::isfinite(secondary) && secondary<=weapon_time) {
            target=-1;
            if(Select(local,weapon,eye,cmd.angles,clock.time,point,target,true)) {
                cmd.angles=game::Angle(eye,point); cmd.buttons|=2048; changed=true;
                status.target=target; Message("Auto shove: objetivo a alcance y visible");
            }
        }
    }
    return changed;
}
bool* PacketFromFrame(uintptr_t frame) {
    if(!packet_return || !packet_displacement) return nullptr;
    const auto tib=reinterpret_cast<const NT_TIB*>(NtCurrentTeb());
    const uintptr_t low=reinterpret_cast<uintptr_t>(tib->StackLimit),high=reinterpret_cast<uintptr_t>(tib->StackBase);
    for(int i=0;i<6;++i) {
        struct StackFrame {uintptr_t parent,return_address;} current{};
        if(frame<low || frame>high-sizeof(current) || (frame&3) || !memory::Read(reinterpret_cast<void*>(frame),current)) return nullptr;
        if(current.parent<=frame || current.parent>=high || current.parent-frame>0x10000) return nullptr;
        if(current.return_address==packet_return) {
            const auto location=static_cast<int64_t>(current.parent)+packet_displacement;
            unsigned char value{};
            if(location<static_cast<int64_t>(low) || location>=static_cast<int64_t>(high) ||
               !memory::Read(reinterpret_cast<void*>(location),value) || value>1) return nullptr;
            return reinterpret_cast<bool*>(location);
        }
        frame=current.parent;
    }
    return nullptr;
}
void ResolvePacketContext() {
    const auto cl_move=static_cast<unsigned char*>(game::Signature(L"engine.dll","55 8B EC 81 EC ? ? ? ? A1 ? ? ? ? 33 C5 89 45 ? 56 E8 ? ? ? ? 8B F0 83 7E"));
    const auto call=static_cast<unsigned char*>(game::Signature(L"engine.dll","F3 0F 11 04 24 51 8B 0D ? ? ? ? FF D2"));
    // Same function as the Storm "Move"/CopyCommand chain (verified per-build);
    // hooked for rapidfire batching at Initialize.
    tick_move_fn=cl_move;
    run_prediction_fn=api.run_prediction;
    if(!run_prediction_fn) app::Log("L4D2: CL_RunPrediction no validado; extras sin reset de prediccion");
    if(!cl_move || !call || call<cl_move || call-cl_move>0x600) return;
    unsigned char bytes[0x600]{};
    if(!memory::Copy(bytes,cl_move,sizeof(bytes))) return;
    int displacement{};
    for(size_t i=0;i<0x200;++i) {
        if(bytes[i]==0xC6 && bytes[i+1]==0x45 && bytes[i+3]==1 && bytes[i+4]==0xFF && bytes[i+5]==0xD2) {
            if(displacement) return;
            displacement=static_cast<signed char>(bytes[i+2]);
        }
    }
    if(displacement>=0 || displacement<-128) return;
    bool tested=false;
    for(size_t i=0;i<sizeof(bytes)-4;++i)
        if(bytes[i]==0x80 && bytes[i+1]==0x7D && static_cast<signed char>(bytes[i+2])==displacement && bytes[i+3]==0) tested=true;
    if(!tested) return;
    packet_return=reinterpret_cast<uintptr_t>(call+14); packet_displacement=displacement;
    app::Log("L4D2: silent servidor contexto validado: retorno=%p local EBP%+d",reinterpret_cast<void*>(packet_return),packet_displacement);
}
bool SpeedhackReady(Entity& local) {
    const char* reason{};
    if(!cfg.speedhack) reason="Speedhack off";
    else if(!status.command_hook || !status.tick_move_hook) reason="Movement hook unavailable";
    else if(!Focused()) reason="Paused: close menu / focus game";
    else if(cfg.speedhack_key>0 && !Key(cfg.speedhack_key)) reason="Hold the speed key";
    else if(cfg.airstuck && cfg.airstuck_key>0 && Key(cfg.airstuck_key)) reason="Paused while airstuck is active";
    else if(cfg.speedhack_scale<=1.f) reason="Choose a multiplier above 1x";
    else if(!api.Local(local) || local.ghost || !Mobile(local)) reason="Waiting for an active, living player";
    else if(ManualLagHeld() || AutomaticFallRequested(local) || !storm_history.Empty()) reason="Paused while lag owns the network channel";
    if(reason) {
        status.speedhack_active=false;status.speedhack_extra=0;
        std::snprintf(status.speedhack_message,sizeof(status.speedhack_message),"%s",reason);
        return false;
    }
    std::snprintf(status.speedhack_message,sizeof(status.speedhack_message),"Command speed %.2fx | +%u ticks",
                  cfg.speedhack_scale,status.speedhack_extra);
    return true;
}
const char* RapidMethodName(int index) {
    return index==0?"Methot 1 (Storm)":index==1?"Methot 2 (choke)":"Methot 3 (flush)";
}
int RapidSpeed() {
    int speed{};
    switch(rapid_fire::FromIndex(cfg.rapidfire_method)) {
    case rapid_fire::Method::Storm: speed=cfg.rapidfire_storm_speed;break;
    case rapid_fire::Method::Choked: speed=cfg.rapidfire_m1_speed;break;
    case rapid_fire::Method::Flush: speed=cfg.rapidfire_m2_speed;break;
    }
    return std::clamp(speed,rapid_fire::SpeedMinimum,rapid_fire::SpeedMaximum);
}
bool RapidKeyHeld() {return cfg.rapidfire_key<=0 || Key(cfg.rapidfire_key);}
// Per-frame gate. Raze checks the local entity and the key inside CreateMove;
// this runs once before the original CL_Move so the command callback can arm
// without skipping or delaying a real engine tick.
bool RapidReady() {
    std::lock_guard lock(game_mutex);ReadConfig();
    const bool previously_ready=rapid_frame_ready.exchange(false,std::memory_order_acq_rel);
    rapid_armed=false;
    rapid_channel_failed=false;
    rapid_prediction_failed=false;
    rapid_extra_moves=-1;
    status.rapidfire_active=false;
    status.rapidfire_extra=0;
    status.rapidfire_transport=api.prediction && api.delta_tick>=0 && api.outgoing_command>=0 && packet_return &&
        status.command_hook && status.tick_move_hook && status.run_command_hook;
    Entity local{};game::Globals clock{};
    // Raze resolves the local player in RunCommand independently of the key.
    // Retain its correction history on release; reset only for a lost/changed
    // owner (or ClearLevel), not on every inactive input frame.
    const bool have_local=(cfg.rapidfire || cfg.no_fall_damage || !storm_history.Empty() || !rapid_ring.Empty() || rapid_recovery.pending) && api.Local(local,false,true);
    if(!have_local || storm_history.Owner()!=local.handle || (storm_player && storm_player!=local.ptr))
        storm_history.Reset(have_local?local.handle:0);
    storm_player=have_local?local.ptr:nullptr;
    storm_tickbase=have_local?local.fields->tickbase:-1;
    storm_ducked=have_local?local.fields->ducked:-1;
    if(!have_local || rapid_owner_handle!=local.handle) {
        rapid_ring=rapid_fire::Ring{};
        rapid_recovery.Cancel();
        rapid_first_command=rapid_last_command=rapid_generated_last=0;
        status.rapidfire_acked=status.rapidfire_sent=0;
    }
    rapid_owner_handle=have_local?local.handle:0;
    rapid_tickbase_entity=have_local?local.ptr:nullptr;
    rapid_tickbase_offset=have_local?local.fields->tickbase:-1;
    game::CommandState commands{};
    if(api.Commands(commands)) {rapid_ring.Retire(commands.acknowledged);storm_history.Retire(commands.acknowledged);}
    const char* reason{};
    if(!cfg.rapidfire) reason="Rapidfire off";
    else if(!status.rapidfire_transport) reason="Rapidfire unavailable: RunCommand / Prediction::Update / command transport";
    else if(!Focused()) reason="Paused: close menu / focus game";
    else if(!RapidKeyHeld()) reason="Hold the rapidfire key";
    else if(ManualLagHeld()) reason="Paused while manual lag is held";
    else if(!storm_history.Empty() || (have_local && AutomaticFallRequested(local))) reason="Paused while Storm fall owns the channel";
    else if(cfg.airstuck && Key(cfg.airstuck_key)) reason="Paused while airstuck is active";
    else if(!have_local) reason="Waiting for the local player";
    else if(!api.Clock(clock)) reason="Paused / waiting for the game clock";
    else if(local.fields->tickbase<0) reason="m_nTickBase unavailable";
    if(reason) {
        if(previously_ready) {
            force_next_send=true;
            rapid_recovery.Begin(rapid_last_command);
            if(api.Commands(commands)) {
                int tickbase{};
                if(rapid_tickbase_entity&&rapid_tickbase_offset>=0)
                    memory::Field(rapid_tickbase_entity,rapid_tickbase_offset,tickbase);
                double delay{};const bool pacing=rapid_send_budget.Delay(api,delay);
                app::Log("Rapidfire release: outgoing=%d ack=%d queued=%d tickbase=%d rtt_ms=%.1f recovery_until=%d pacing_wait_ms=%.1f pacing_removed_ms=%.1f pacing_peak_ms=%.1f",
                    commands.outgoing,commands.acknowledged,commands.choked,tickbase,1000.f*api.NetworkLatency(),rapid_recovery.last,
                    pacing?1000.*delay:-1.,1000.*rapid_pacing_removed,1000.*rapid_pacing_peak);
            }
        }
        std::snprintf(status.rapidfire_message,sizeof(status.rapidfire_message),"%s",reason);
        if(rapid_ring.Empty()&&api.Commands(commands)&&rapid_recovery.Take(commands.acknowledged,commands.choked)) {
            const bool reset=api.ResetPredictionHistory();
            app::Log("Rapidfire recovery: last=%d ack=%d native_history_reset=%d rtt_ms=%.1f",
                rapid_recovery.last,commands.acknowledged,reset,1000.f*api.NetworkLatency());
            if(!reset)std::snprintf(status.rapidfire_message,sizeof(status.rapidfire_message),"%s | prediction recovery unavailable",reason);
        }
        Publish();
        return false;
    }
    rapid_interval=clock.interval;
    if(!previously_ready)rapid_pacing_removed=rapid_pacing_peak=0.;
    rapid_recovery.Cancel();
    // Seed the per-frame local cache with the resolution just validated.
    local_cache=local;local_cache_generation=frame_generation;
    status.rapidfire_active=true;
    rapid_frame_ready.store(true,std::memory_order_release);
    status.rapidfire_acked=status.rapidfire_sent=0;
    std::snprintf(status.rapidfire_message,sizeof(status.rapidfire_message),"%s | hold the key to repeat",
                  RapidMethodName(cfg.rapidfire_method));
    return true;
}
void NoteRapidFailure(const char* what) {
    rapid_channel_failed=true;
    static ULONGLONG next{};
    const auto now=GetTickCount64();
    if(now<next) return;
    next=now+2000;
    app::Log("Rapidfire: %s failed (method %d)",what,cfg.rapidfire_method);
}
// Raze ApplyNetChannelMode: every command generated while the key is held
// declares its method's channel state. Storm consults the previous command's
// correction; Methot 2 only declares the retained-packet count.
void ApplyRapidChannel(int command_number) {
    const bool held=rapid_frame_ready.load(std::memory_order_relaxed) && RapidKeyHeld();
    int acknowledged{},choked{};
    switch(rapid_fire::FromIndex(cfg.rapidfire_method)) {
    case rapid_fire::Method::Storm:
        if(rapid_fire::StormWrite(held,cfg.rapidfire_storm_ticks,rapid_ring.Get(command_number-1))) {
            if(!rapid_channel.Apply(api,true,-1,255)) NoteRapidFailure("Storm channel write");
        }
        break;
    case rapid_fire::Method::Choked:
        if(rapid_fire::ChokedWrite(held,api.Channel(acknowledged,choked))) {
            if(!rapid_channel.Apply(api,false,acknowledged,cfg.rapidfire_m1_choke)) NoteRapidFailure("Methot 2 choke write");
            force_next_send=true;
        }
        break;
    case rapid_fire::Method::Flush:
        if(rapid_fire::FlushWrite(held)) {
            if(!rapid_channel.Apply(api,true,-1,255)) NoteRapidFailure("Methot 3 channel write");
            force_next_send=true;
        }
        break;
    }
}
void RapidTickbaseFailure() {
    static ULONGLONG next{};
    const auto now=GetTickCount64();
    if(now<next) return;
    next=now+2000;
    app::Log("Rapidfire: m_nTickBase correction failed (entity=%p offset=%X)",
        rapid_tickbase_entity,rapid_tickbase_offset);
}
bool RunPrediction() noexcept {
    return memory::InvokeVoid(reinterpret_cast<void(__cdecl*)()>(run_prediction_fn));
}
// VClientPrediction001 slot 18: RunCommand(player, command, move_helper).
// Corrections belong to this exact command and the actual local player.
// Remote RunCommand calls must never alter the local simulation clock.
void __fastcall RunCommandHook(void* self,void* reserved,void* player,game::Command* command,void* helper) {
    game::StormClock storm_clock;
    const bool previous_storm=inside_storm_prediction;
    // The return site is resolved from C_BasePlayer::PhysicsSimulate's slot-18
    // call, independently of the configured feature or render frame.
    if(Running() && reinterpret_cast<uintptr_t>(_ReturnAddress())==storm_prediction_return && player==storm_player && command) {
        game::Command identity{};
        if(memory::Read(command,identity)) {
            const int shift=storm_history.Get(storm_history.Owner(),identity.number,identity.seed);
            if(shift) {
                if(storm_clock.Begin(player,storm_tickbase,api.globals,shift)) inside_storm_prediction=true;
                else StormFailure("RunCommand clock correction failed");
            }
        }
    }
    if(Running() && command && rapid_tickbase_entity && rapid_tickbase_offset>=0) {
        int number{};
        if(memory::Field(command,offsetof(game::Command,number),number) && number>0) {
            const int shift=rapid_fire::LocalCorrection(rapid_ring,number,player,rapid_tickbase_entity);
            if(shift>0) {
                int tickbase{};
                if(memory::Field(rapid_tickbase_entity,rapid_tickbase_offset,tickbase) &&
                   tickbase>-100000000 && tickbase<100000000) {
                    const int corrected=tickbase-shift;
                    if(!memory::Copy(static_cast<unsigned char*>(rapid_tickbase_entity)+rapid_tickbase_offset,
                                     &corrected,sizeof(corrected))) RapidTickbaseFailure();
                    else if(inside_tick_move&&number==rapid_first_command)rapid_base_corrected=true;
                } else RapidTickbaseFailure();
            }
        }
    }
    if(original_run_command) original_run_command(self,reserved,player,command,helper);
    if(!storm_clock.Restore()) StormFailure("RunCommand clock restore failed");
    inside_storm_prediction=previous_storm;
}
void __fastcall PredictionUpdateHook(void* self,void* reserved,int frame,bool valid,int incoming,int outgoing) {
    const int end=storm_fall::PredictionEnd(incoming,outgoing,Running() && !storm_history.Empty());
    if(original_prediction_update) original_prediction_update(self,reserved,frame,valid,incoming,end);
}
void __fastcall ProcessMovementHook(void* self,void* reserved,void* player,void* move) {
    if(Running() && inside_storm_prediction && player==storm_player) {
        int tick{};bool ducked{};
        if(!memory::Field(player,storm_tickbase,tick) || !memory::Field(player,storm_ducked,ducked) ||
           (tick<0 && !game::StormMovementInput(move,ducked))) StormFailure("negative-clock movement correction failed");
    }
    if(original_process_movement) original_process_movement(self,reserved,player,move);
}
void PrepareRapidCommand(int command_number) {
    const bool ready=inside_tick_move && extra_index==0 && rapid_frame_ready.load(std::memory_order_relaxed) && RapidKeyHeld();
    // Each extra calls the native movement/send path. The 15-command limit of
    // a single movement message is not a cap on all calls in this frame, nor
    // a reason to wait for the preceding burst's ACK before arming again.
    const int moves=ready?rapid_fire::BurstMoves(RapidSpeed(),rapid_interval,cfg.rapidfire_boost):0;
    const bool armed=rapid_fire::PrepareCommand(rapid_ring,rapid_extra_moves,command_number,moves,ready);
    if(armed) {
        rapid_armed=true;
        force_next_send=true;
        rapid_extras=rapid_extra_moves;
        rapid_first_command=rapid_last_command=rapid_generated_last=command_number;
        status.rapidfire_active=true;
        status.rapidfire_extra=0;
        status.rapidfire_sent=1;
        status.rapidfire_acked=0;
        ++status.rapidfire_bursts;
        static ULONGLONG next_arm_log{};
        if(GetTickCount64()>=next_arm_log) {
            next_arm_log=GetTickCount64()+2000;
            app::Log("Rapidfire arm: method=%d num=%d extras=%d interval=%.5f | Prediction::Update after arm",
                cfg.rapidfire_method,command_number,rapid_extras,rapid_interval);
        }
    }
    // Raze CreateMove: clear slot -> arm -> Prediction::Update -> command
    // features -> netchannel -> send_packet. Calling CL_RunPrediction before
    // CL_Move left the current correction unavailable to this update.
    // Pending history is consumed by the engine's normal prediction after
    // release. A second manual update there reapplied old shifts for seconds.
    if(rapid_fire::ManualPrediction(inside_tick_move,rapid_armed,rapid_tickbase_entity) && !api.UpdatePrediction()) {
        if(armed) {
            rapid_ring.Clear(command_number);
            rapid_extra_moves=rapid_extras=0;
            rapid_armed=false;
            status.rapidfire_sent=0;
        }
        rapid_frame_ready.store(false,std::memory_order_release);
        rapid_prediction_failed=true;
        force_next_send=true;
        status.rapidfire_active=false;
        std::snprintf(status.rapidfire_message,sizeof(status.rapidfire_message),"Prediction::Update failed; rapidfire paused");
        static ULONGLONG next_prediction_log{};
        if(GetTickCount64()>=next_prediction_log) {
            next_prediction_log=GetTickCount64()+2000;
            app::Log("Rapidfire: Prediction::Update failed after preparing command %d",command_number);
        }
    }
}
bool PrepareChargerTeleport(game::Command& cmd) {
    status.charger_teleport_pulse=false;
    status.charger_teleport_available=false;
    const auto message=[&](const char* text) {
        std::snprintf(status.charger_teleport_message,sizeof(status.charger_teleport_message),"%s",text);
    };
    if(!cfg.charger_teleport && !teleport_pulse.Tracking()) {message("Charger teleport desactivado");return false;}
    Entity local{};
    charger_teleport::Input in{};
    in.enabled=cfg.charger_teleport;in.focused=Focused();in.in_game=api.InGame();
    in.alive=in.in_game && api.Local(local,true,false);
    in.ghost=local.ghost;in.charger=local.kind==Kind::Charger;in.team=local.team;
    in.owner=local.handle;in.command=cmd.number;in.frozen=cmd.tick==movement_assists::FreezeTick;
    if(in.alive && in.charger && local.fields) {
        status.charger_teleport_available=local.fields->carry_victim>=0;
        in.carry_known=api.CarryVictim(local,in.victim);
        in.channel=api.NetworkChannelReady();
    }
    const bool pulse=teleport_pulse.Prepare(in);
    message(!in.enabled?"Charger teleport desactivado":!in.alive || !in.charger || in.team!=3?
        "Charger teleport: esperando Charger vivo":!in.carry_known?"Charger teleport: estado de transporte no disponible":
        !in.focused?"Charger teleport: cierra el menu / da foco al juego":!in.channel?
        "Charger teleport: esperando conexion":!in.victim?"Charger teleport: esperando un agarre":
        "Charger teleport: transporte detectado");
    if(pulse) charger_teleport::Apply(cmd.angles);
    return pulse;
}
void __cdecl CommandDispatch(void* self,game::Command* command,uintptr_t caller_frame,uintptr_t return_address) {
    Vec3 restore_view{};bool restore_visible_view=false;
    if(Running() && return_address==command_return) {
        try {
            std::lock_guard lock(game_mutex); ReadConfig();
            game::Command cmd{};
            if(memory::Read(command,cmd) && cmd.number>0 && source::Finite(cmd.angles) &&
               std::isfinite(cmd.forward) && std::isfinite(cmd.side) && std::isfinite(cmd.up)) {
                if(cmd.tick==movement_assists::FreezeTick) {
                    game::Globals native_clock{};
                    const int fallback=api.Clock(native_clock)?native_clock.tick:cmd.number;
                    cmd.tick=freeze_ticks.Native(cmd.number,cmd.seed,cmd.tick,fallback);
                } else freeze_ticks.Native(cmd.number,cmd.seed,cmd.tick,cmd.tick);
                const auto original=cmd;
                Vec3 mouse_view{};
                const bool have_mouse_view=(cfg.aim_mode!=0 || cfg.no_spread) && api.Angles(mouse_view,false);
                auto packet=PacketFromFrame(caller_frame);
                if(inside_tick_move&&!packet&&rapid_frame_ready.load(std::memory_order_relaxed)) {
                    rapid_frame_ready.store(false,std::memory_order_release);
                    rapid_prediction_failed=true;force_next_send=true;
                    status.rapidfire_active=false;
                    std::snprintf(status.rapidfire_message,sizeof(status.rapidfire_message),"Rapidfire paused: send context unavailable");
                }
                PrepareRapidCommand(cmd.number);
                status.server_silent=packet!=nullptr;
                bool flush=false;
                if(packet && force_next_send && !rapid_frame_ready.load(std::memory_order_relaxed)) {
                    const bool send=true;
                    if(memory::Copy(packet,&send,sizeof(send))) {force_next_send=false;flush=true;}
                }
                LARGE_INTEGER begin{},end{},frequency{};
                if(extra_index==0) {QueryPerformanceCounter(&begin);QueryPerformanceFrequency(&frequency);}
                CommandAim aim{};
                bool adjusted=Process(cmd,packet!=nullptr,flush && cfg.aim_mode==2,aim);
                if(status.ceiling_active) {
                    cmd.angles.x=movement_assists::CeilingPitch;
                    aim.visible_angles=cmd.angles;aim.update_view=true;adjusted=true;
                }
                if(status.airstuck_active && packet) {const bool send=true;memory::Copy(packet,&send,sizeof(send));}
                if(adjusted) {
                    cmd.angles={std::clamp(game::Normalize(cmd.angles.x),-89.f,89.f),game::Normalize(cmd.angles.y),0};
                    game::FixMovement(cmd,original.angles.y);
                }
                // Raze's carry pulse belongs only to the final command. Apply
                // after normal angle cleanup, before the engine's verified CRC;
                // never feed its roll into movement, aim or the visible camera.
                const bool teleport=PrepareChargerTeleport(cmd);
                // This callback runs immediately before CInput copies the
                // verified command and calculates its CRC. Keep the engine's
                // local number, unknown fields and prediction flags. Include
                // the chosen seed so local prediction agrees with the wire.
                const bool written=memory::Copy(reinterpret_cast<unsigned char*>(command)+offsetof(game::Command,tick),&cmd.tick,
                                               offsetof(game::Command,seed)+sizeof(cmd.seed)-offsetof(game::Command,tick));
                if(teleport && written) {
                    status.charger_teleport_pulse=true;
                    if(teleport_pulse.Commit()) {
                        ++status.charger_teleport_commands;
                        app::Log("Charger teleport: carry pulse committed to command %d",cmd.number);
                    }
                }
                if(written && aim.shotgun_wire_number)shotgun_history.Store(command,cmd,aim.shotgun_wire_number);
                // Render the selected/smoothed aim, never per-shot spread or
                // punch correction: feeding those back into input caused jitter.
                if(written && aim.update_view && (!(cmd.buttons&2048) || aim.infected)) {
                    restore_view={std::clamp(game::Normalize(aim.visible_angles.x),-89.f,89.f),game::Normalize(aim.visible_angles.y),0};
                    restore_visible_view=true;
                }
                if(written && aim.applied) status.aim_applied=true;
                if(written && aim.applied && (cfg.aim_mode!=2 || aim.infected) && (!(cmd.buttons&2048) || aim.infected)) {
                    Entity local{};
                    if(api.Local(local)) aim_camera.Update(local.handle,GetTickCount64());
                    if(!aim.infected && cfg.aim_mode==1 && have_mouse_view) {restore_view=mouse_view;restore_visible_view=true;}
                }
                if(written && aim.spread_corrected && !(cmd.buttons&2048)) {
                    Entity local{};
                    if(api.Local(local)) spread_camera.Update(local.handle,GetTickCount64());
                    if(!aim.update_view && have_mouse_view) {restore_view=mouse_view;restore_visible_view=true;}
                }
                // Only the base command can arm. Speed and rapidfire share the
                // same fresh commands; Raze's counter stays != -1 during extras.
                if(inside_tick_move && extra_index==0) {
                    Entity local{};
                    const bool rapid_ready=rapid_frame_ready.load(std::memory_order_relaxed);
                    const bool speed=!rapid_ready && !rapid_prediction_failed && !flush && written && SpeedhackReady(local);
                    if(rapid_ready && cfg.speedhack)
                        std::snprintf(status.speedhack_message,sizeof(status.speedhack_message),
                                      "Paused while rapidfire arms extra commands");
                    speed_extras=speed_budget.Next(cfg.speedhack_scale,speed,local.handle);
                    status.speedhack_active=speed;status.speedhack_extra=0;
                    const int total=std::max(speed_extras,rapid_extras);
                    static ULONGLONG next_batch_log{};
                    if(total>0 && GetTickCount64()>=next_batch_log) {
                        next_batch_log=GetTickCount64()+2000;
                        app::Log("Movement batch: speed=%.2fx extras=%d rapid=%d total=%d num=%d",
                            speed?cfg.speedhack_scale:1.f,speed_extras,rapid_extras,total,cmd.number);
                    }
                } else if(written && extra_index>0 && extra_index<=speed_extras) {
                    ++status.speedhack_extra;++status.speedhack_commands;
                }
                if(written && inside_tick_move && rapid_armed && packet)ApplyRapidChannel(cmd.number);
                if(written && packet) {
                    const auto action=aimbot::logic::SilentPacket(cfg.aim_mode,aim.applied && !aim.infected && (cmd.buttons&1),flush,
                        rapid_frame_ready.load(std::memory_order_relaxed),rapid_armed || force_next_send || aim.force_send);
                    if(action!=aimbot::logic::PacketAction::Keep) {
                        const bool send=action==aimbot::logic::PacketAction::Send;
                        force_next_send=memory::Copy(packet,&send,sizeof(send)) ? !send : true;
                    }
                }
                if(written && rapid_armed && extra_index>0) {
                    ++status.rapidfire_extra;
                    rapid_generated_last=cmd.number;
                }
                ++status.commands;
                // Status publishing and command timing belong to the frame:
                // extras already aggregate into the counters the frame-end
                // publish shows, and Publish takes an exclusive lock plus a
                // full status copy.
                if(extra_index==0) {
                    QueryPerformanceCounter(&end);
                    status.command_ms=frequency.QuadPart ? static_cast<float>(1000.*(end.QuadPart-begin.QuadPart)/frequency.QuadPart) : 0.f;
                    Publish();
                }
            }
        } catch(...) {
            std::lock_guard lock(game_mutex); Message("Error de comandos: funciones detenidas");
            active.store(false,std::memory_order_release); restore_requested.store(true,std::memory_order_release); Publish();
        }
    }
    original_move(self,command);
    // Publish visible aim only AFTER the native callback has consumed the shot:
    // normal gets its smooth angle, client silent keeps the mouse direction.
    // Later mouse samples remain free to update the engine's view normally.
    if(restore_visible_view) {
        std::lock_guard lock(game_mutex);
        if(Running() && Focused() && !api.Angles(restore_view,true)) {
            status.aim_applied=false;
            std::snprintf(status.aim_message,sizeof(status.aim_message),"No se pudo aplicar el angulo visible");
            Publish();
        }
    }
}
// Native CInput validates the original ring/CRC before entering this writer.
// Translate both sides of its delta: otherwise a backup, bind release or a
// following ordinary command would decode relative to the wrong number.
void __cdecl WriteUsercmdHook(void* buffer,const game::Command* to,const game::Command* from) {
    game::Command output{},baseline{};bool translated=false;
    if(memory::Read(to,output) && memory::Read(from,baseline)) {
        std::lock_guard lock(game_mutex);
        translated=shotgun_history.Translate(to,output);
        translated=shotgun_history.Translate(from,baseline)||translated;
    }
    // Keep pending mappings after a toggle/hold release. Only ClearLevel or
    // normal ring reuse retires them; this also covers packet retransmission.
    original_write_usercmd(buffer,translated?&output:to,translated?&baseline:from);
}
// Per-frame engine tick (CL_Move equivalent from the Storm reference, same
// build). Raze sets the shared counter to -1, calls the original, then repeats
// it while the previous counter was positive: one base call plus N extras.
void NativeTickMove(float samples,bool final) {
    struct RestoreChannel {~RestoreChannel(){rapid_channel.Restore(api);}} restore;
    original_tick_move(samples,final);
}
void __cdecl TickMoveHook(float samples,bool final) {
    if(inside_tick_move || !Running()) {original_tick_move(samples,final);return;}
    InsideGuard inside(inside_tick_move);
    ++frame_generation;frame_focus_valid=false;
    // The burst replays one input frame: the selection cache starts empty and
    // already resolved for this frame, so a generated command can never
    // trigger a fresh SetupBones + trace sweep. The base command overwrites
    // it with the real selection.
    aim_selection=AimSelection{};aim_selection.valid=true;aim_selection.generation=frame_generation;
    extra_index=0;speed_extras=rapid_extras=0;
    rapid_extra_moves=-1;rapid_armed=false;rapid_base_corrected=false;
    RapidReady();
    NativeTickMove(samples,final);
    const int extras=movement_batch::Combined(speed_extras,rapid_extras);
    {
    // Snapshot AFTER the ordinary move, then retire only the scheduling debt
    // from this frame's rapid extras. Every original call and method still runs.
    const bool budget=rapid_armed&&rapid_send_budget.Begin(api);
    if(rapid_armed&&!budget&&!rapid_pacing_warned) {
        rapid_pacing_warned=true;
        app::Log("Rapidfire pacing recovery unavailable: native SendDatagram / CanPacket layout not validated");
    }
    struct RestoreBudget {
        bool active;
        ~RestoreBudget() {
            if(active) {
                const double removed=rapid_send_budget.Restore(api);
                rapid_pacing_removed+=removed;rapid_pacing_peak=std::max(rapid_pacing_peak,removed);
            }
        }
    } restore_budget{budget};
    for(int index=1;index<=extras;++index) {
        if(!Running() || !Focused() || (rapid_armed && (!rapid_frame_ready.load(std::memory_order_relaxed)||!RapidKeyHeld()))) {
            force_next_send=true;
            break;
        }
        extra_index=index;
        // During extras CreateMove sees a non-negative counter, exactly as in
        // Raze, so it cannot arm a second sequence; the last value is zero.
        rapid_extra_moves=std::max(rapid_extras-index,0);
        // Rapidfire updates prediction in CreateMove AFTER storing the shift.
        // Raze repeats samples and final unchanged; restricting final to the
        // last extra changed how CL_Move decided to send intermediate packets.
        if(!rapid_armed && run_prediction_fn) RunPrediction();
        const auto args=rapid_fire::ReplayArguments(samples,final,rapid_armed,index,extras);
        NativeTickMove(args.samples,args.final);
    }
    }
    if(rapid_armed) {
        std::lock_guard lock(game_mutex);
        rapid_last_command=rapid_generated_last;
        status.rapidfire_sent=1+status.rapidfire_extra;
        if(status.rapidfire_extra<static_cast<unsigned>(rapid_extras)) {
            const int missing=rapid_fire::UnusedCorrection(rapid_extras,static_cast<int>(status.rapidfire_extra),rapid_base_corrected);
            // If the base prediction already borrowed the full planned shift,
            // return unused ticks immediately when release cancels the tail.
            int tickbase{};
            if(missing>0&&rapid_tickbase_entity&&rapid_tickbase_offset>=0&&
               memory::Field(rapid_tickbase_entity,rapid_tickbase_offset,tickbase)&&tickbase>-100000000&&tickbase<100000000) {
                tickbase+=missing;
                if(!memory::Copy(static_cast<unsigned char*>(rapid_tickbase_entity)+rapid_tickbase_offset,&tickbase,sizeof(tickbase)))
                    RapidTickbaseFailure();
            }
            rapid_ring.Set(rapid_first_command,static_cast<int>(status.rapidfire_extra));
            force_next_send=true;
        }
        std::snprintf(status.rapidfire_message,sizeof(status.rapidfire_message),
            "%s | %d extras armed, %u executed%s%s",RapidMethodName(cfg.rapidfire_method),
            rapid_extras,status.rapidfire_extra,rapid_channel_failed?" | channel write failed":"",
            rapid_prediction_failed?" | prediction update failed":"");
        Publish();
    } else if(speed_extras>0) Publish();
    extra_index=0;speed_extras=rapid_extras=0;
    rapid_extra_moves=-1;rapid_armed=false;
}
// Preserve the game's caller EBP independently of MSVC's /Oy setting. This
// shim follows the actual void(this,CUserCmd*) ABI used by RijiN's callback.
__declspec(naked) void __fastcall MoveHook(void*,void*,game::Command*) {
    __asm {
        push ebp
        mov ebp,esp
        push dword ptr [ebp+4]
        push dword ptr [ebp]
        push dword ptr [ebp+8]
        push ecx
        call CommandDispatch
        add esp,16
        mov esp,ebp
        pop ebp
        ret 4
    }
}
void ApplyCamera(game::View& view,int& draw) {
    third_active=false;
    third_owner.store(nullptr,std::memory_order_release);
    snprintf(status.camera_message,sizeof(status.camera_message),"Camara original / primera persona");
    Entity local{};
    if(!api.Local(local) || !source::Finite(view.origin) || !source::Finite(view.angles) ||
       !std::isfinite(view.fov) || view.fov<1.f || view.fov>179.f) return;
    bool isolated_view=false;
    const auto camera_time=GetTickCount64();
    const bool compensated_view=(cfg.aim && cfg.aim_mode!=2 && aim_camera.Active(local.handle,camera_time)) ||
        (cfg.shotgun_no_spread && spread_camera.Active(local.handle,camera_time));
    if(compensated_view && Focused() && Mobile(local)) {
        Vec3 input{},eye{},punch{};
        // A remote/scripted camera must retain the view supplied by the game.
        if(api.Eye(local,eye) && game::Length(game::Sub(eye,view.origin))<=40.f && api.Angles(input,false)) {
            memory::Field(local.ptr,local.fields->punch,punch);
            view.angles=aimbot::VisibleViewAngles(input,punch,cfg.no_visual_recoil);isolated_view=true;
        }
    }
    if(cfg.no_visual_recoil && !isolated_view) {
        Vec3 punch{};
        if(memory::Field(local.ptr,local.fields->punch,punch) && source::Finite(punch) && game::Length(punch)<90.f)
            view.angles=game::Sub(view.angles,punch);
    }
    // Respect scoped/cinematic views.
    if(cfg.override_fov && view.fov>=70.f) view.fov=cfg.camera_fov;
    overlay.fov=view.fov;
    if(!cfg.thirdperson || !status.local_model_hook || !Mobile(local)) return;
    if(!status.camera_isolation) {
        snprintf(status.camera_message,sizeof(status.camera_message),"Camara no disponible: falta aislar OverrideView");return;
    }
    Vec3 eye{}; if(!api.Eye(local,eye)) return;
    // OverrideView must leave the game's eye view intact. Do not stack another
    // offset on top of a spectator, cinematic or scripted third-person view.
    if(game::Length(game::Sub(eye,view.origin))>40.f) {
        snprintf(status.camera_message,sizeof(status.camera_message),"Camara especial del juego; desplazamiento omitido");return;
    }
    // Read the current mouse/view direction, never the actor's network angles
    // or a CInput camera offset left over from a native third-person view.
    Vec3 camera_angles{};
    if(!api.Angles(camera_angles,false)) {
        snprintf(status.camera_message,sizeof(status.camera_message),"Camara: direccion de la mira no disponible");return;
    }
    if(!cfg.no_visual_recoil) {
        Vec3 punch{};
        if(memory::Field(local.ptr,local.fields->punch,punch) && source::Finite(punch) && game::Length(punch)<90.f)
            camera_angles=game::Add(camera_angles,punch);
    }
    camera_angles.x=std::clamp(game::Normalize(camera_angles.x),-89.f,89.f);
    camera_angles.y=game::Normalize(camera_angles.y);
    camera_angles.z=0.f;
    const auto anchor=view.origin;
    const auto desired=game::CameraOrigin(anchor,camera_angles,cfg.camera_distance,
        cfg.camera_centered?0.f:cfg.camera_right,cfg.camera_centered?0.f:cfg.camera_up);
    auto position=desired;
    if(cfg.camera_collision) {
        // Follow RijiN's world-only hull trace. Entity filters are unnecessary
        // for camera clipping and can call into unstable entity state here.
        game::Filter filter; filter.world_only=true;
        game::Trace trace{};
        if(!api.Cast(view.origin,desired,filter,trace,0x200400B,2.f) || trace.start_solid || trace.all_solid) return;
        // Keep the hull-tested endpoint. A second lateral correction moves
        // the camera off the tested segment and can put it inside nearby cover.
        position=trace.end;
    }
    const float actual_distance=game::Length(game::Sub(position,anchor));
    if(actual_distance<=15.f) {
        snprintf(status.camera_message,sizeof(status.camera_message),"Sin espacio detras; usando primera persona");return;
    }
    // Aim convergence: the game draws the crosshair at screen center, but the
    // weapon ray starts at the player. Aim the camera at the same world point
    // the eye ray would hit, otherwise shots land off the reticle at range
    // ("mira desviada" in third person). Previously reported jitter.
    {
        game::Filter aim_filter; aim_filter.skip=local.ptr;
        game::Trace aim{};
        const Vec3 dir=game::Forward(camera_angles);
        if(api.Cast(anchor,game::Add(anchor,game::Mul(dir,4096.f)),aim_filter,aim) &&
           !aim.start_solid && !aim.all_solid && source::Finite(aim.end)) {
            Vec3 converged=game::Angle(position,aim.end);
            if(source::Finite(converged)) {
                converged.x=std::clamp(game::Normalize(converged.x),-89.f,89.f);
                converged.y=game::Normalize(converged.y); converged.z=0.f;
                camera_angles=converged;
            }
        }
    }
    view.origin=position;
    view.angles=camera_angles;
    third_active=true;
    third_owner.store(local.ptr,std::memory_order_release);
    draw&=~1; // RENDERVIEW_DRAWVIEWMODEL
    snprintf(status.camera_message,sizeof(status.camera_message),"Camara %s | distancia %.0f | pitch %.0f yaw %.0f",
        cfg.camera_centered?"centrada":"con desplazamiento",actual_distance,camera_angles.x,camera_angles.y);
}
void __fastcall ViewHook(void* self,void*,game::View* view,game::View* hudview,int clear,int draw) {
    if(!Running() || view_inside) { original_view(self,view,hudview,clear,draw); return; }
    InsideGuard guard(view_inside);
    std::unique_lock lock(game_mutex); ReadConfig();
    game::View saved{},adjusted{};
    const bool readable=memory::Read(view,saved);
    if(readable) {
        adjusted=saved; ApplyCamera(adjusted,draw);
        // Do not copy the unknown tail of CViewSetup.
        memory::Copy(reinterpret_cast<unsigned char*>(view)+offsetof(game::View,fov),&adjusted.fov,sizeof(game::View)-offsetof(game::View,fov));
    }
    lock.unlock();
    original_view(self,view,hudview,clear,draw);
    if(readable) memory::Copy(reinterpret_cast<unsigned char*>(view)+offsetof(game::View,fov),&saved.fov,sizeof(game::View)-offsetof(game::View,fov));
    // Do not clear third_active here: late render passes (attachments such as
    // the carried weapon, shadows, glow) can still query it after RenderView.
    // ApplyCamera recomputes it at the start of every frame.
}
bool __fastcall DrawLocalHook(void* self,void*) {
    if(Running() && third_active.load(std::memory_order_acquire)) {
        // RijiN semantics: while third person is up, ShouldDrawLocalPlayer
        // answers "yes" for every caller during the frame. The carried weapon
        // and late passes can query this with a different object/context;
        // narrowing to self==owner left the gun invisible in third person.
        if(self!=third_owner.load(std::memory_order_acquire)) {
            static void* seen[6]{}; static unsigned count{};
            bool known=false;
            for(unsigned i=0;i<count;++i) if(seen[i]==self) {known=true;break;}
            if(!known && count<std::size(seen)) {
                seen[count++]=self;
                app::Log("Cam3: ShouldDrawLocalPlayer consultado con objeto ajeno %p",self);
            }
        }
        return true;
    }
    return original_draw_local(self);
}
int __fastcall ThirdpersonHook(void* self,void*,int index) {
    if(Running() && third_requested.load(std::memory_order_acquire)) {
        // RijiN excludes this caller: ClientMode::OverrideView otherwise reads
        // CInput's native camera angles/distance and offsets the view again.
        // Keep third person available to animation/model queries elsewhere.
        if(reinterpret_cast<uintptr_t>(_ReturnAddress())==third_override_return) return 0;
        if(third_active.load(std::memory_order_acquire)) return 1;
    }
    return original_thirdperson(self,index);
}
void __fastcall ViewmodelHook(void* self,void*,void* owner,Vec3* position,Vec3* angles) {
    if(!Running()) { original_viewmodel(self,owner,position,angles); return; }
    std::unique_lock lock(game_mutex); ReadConfig();
    Vec3 p{},a{}; Entity local{};
    if(!memory::Read(position,p) || !memory::Read(angles,a) || !api.Local(local) || owner!=local.ptr || !source::Finite(p) || !source::Finite(a)) {
        lock.unlock(); original_viewmodel(self,owner,position,angles); return;
    }
    Vec3 mouse_view{};
    const auto camera_time=GetTickCount64();
    const bool compensated_view=(cfg.aim && cfg.aim_mode!=2 && aim_camera.Active(local.handle,camera_time)) ||
        (cfg.shotgun_no_spread && spread_camera.Active(local.handle,camera_time));
    const bool isolated_view=compensated_view && Focused() && Mobile(local) && api.Angles(mouse_view,false);
    if(isolated_view) {
        Vec3 punch{};memory::Field(local.ptr,local.fields->punch,punch);
        a=aimbot::VisibleViewAngles(mouse_view,punch,cfg.no_visual_recoil);
    } else if(cfg.no_visual_recoil) {
        Vec3 punch{}; if(memory::Field(local.ptr,local.fields->punch,punch) && source::Finite(punch) && game::Length(punch)<90.f) a=game::Sub(a,punch);
    }
    if(cfg.viewmodel) {
        const auto forward=game::Forward(a); const float yaw=a.y*.0174532925f;
        p=game::Add(game::Add(game::Add(p,game::Mul(forward,cfg.vm_forward)),{std::sin(yaw)*cfg.vm_right,-std::cos(yaw)*cfg.vm_right,0}),{0,0,cfg.vm_up});
        a=game::Add(a,{cfg.vm_pitch,cfg.vm_yaw,cfg.vm_roll});
    }
    // Give the original private copies: caller-owned eye position is preserved.
    lock.unlock(); original_viewmodel(self,owner,&p,&a);
}
float __fastcall WeaponFovHook(void* self,void*) {
    const float original=original_weapon_fov(self);
    std::lock_guard lock(game_mutex); ReadConfig();
    return Running() && cfg.vm_fov && api.InGame() ? cfg.viewmodel_fov : original;
}
template<class T> bool Write(void* entity,int offset,const T& value) {
    return entity && offset>=0 && offset<0x40000 && memory::Copy(static_cast<unsigned char*>(entity)+offset,&value,sizeof(value));
}
void __fastcall OverrideHook(void* self,void*,void* material,int type) {
    engine_override=material;
    engine_override_type=type;
    original_override(self,material,type);
}
void __fastcall ModelHook(void* self,void*,const void* draw_state,const ModelInfo* info,game::Matrix3* bones) {
    if(!Running() || !models_enabled.load(std::memory_order_acquire) || engine_override || model_inside) { original_model(self,draw_state,info,bones); return; }
    InsideGuard guard(model_inside);
    ModelInfo model{}; bool selected{},hidden{}; unsigned tint{};
    if(memory::Read(info,model) && model.index>0 && model.index<8192) {
        std::unique_lock lock(game_mutex,std::try_to_lock);
        if(!lock.owns_lock()) { original_model(self,draw_state,info,bones); return; }
        selected=model_selected[model.index] && api.Handle(model_handle[model.index])!=nullptr;
        tint=model_tint[model.index]; hidden=cfg.chams_hidden;
    }
    std::unique_lock render_lock(material_mutex,std::try_to_lock);
    if(!selected || !render_lock.owns_lock()) { original_model(self,draw_state,info,bones); return; }
    if(!flat_material && material_system) {
        const auto now=GetTickCount64();
        if(now<next_material_attempt) { original_model(self,draw_state,info,bones); return; }
        next_material_attempt=now+1000;
        using Find=void*(__thiscall*)(void*,const char*,const char*,bool,const char*);
        void* candidate{}; bool error=true;
        if(memory::Invoke(memory::Virtual<Find>(material_system,71),candidate,material_system,"debug/debugambientcube","Model textures",false,static_cast<const char*>(nullptr)) &&
           candidate && memory::Invoke(memory::Virtual<source::GetBool>(candidate,42),error,candidate) && !error &&
           memory::InvokeVoid(memory::Virtual<source::Cursor>(candidate,12),candidate)) flat_material=candidate;
    }
    using GetColor=void(__thiscall*)(void*,float*,float*,float*);
    using Modulate=void(__thiscall*)(void*,float,float,float);
    using GetFlag=bool(__thiscall*)(void*,int);
    using SetFlag=void(__thiscall*)(void*,int,bool);
    float old_r{},old_g{},old_b{}; bool old_z{};
    auto color=memory::Virtual<Modulate>(flat_material,28);
    auto flag=memory::Virtual<SetFlag>(flat_material,29);
    if(!color || !flag || !memory::InvokeVoid(memory::Virtual<GetColor>(flat_material,45),flat_material,&old_r,&old_g,&old_b) ||
       !memory::Invoke(memory::Virtual<GetFlag>(flat_material,30),old_z,flat_material,1<<15)) {
        original_model(self,draw_state,info,bones); return;
    }
    const float r=(tint&255)/255.f,g=((tint>>8)&255)/255.f,b=((tint>>16)&255)/255.f;
    // Each override is scoped to this model call. Never clear another engine pass.
    if(hidden) {
        memory::InvokeVoid(flag,flat_material,1<<15,true);
        memory::InvokeVoid(color,flat_material,r*.5f,g*.5f,b*.5f);
        original_override(self,flat_material,0);
        original_model(self,draw_state,info,bones);
    }
    memory::InvokeVoid(flag,flat_material,1<<15,false);
    memory::InvokeVoid(color,flat_material,r,g,b);
    original_override(self,flat_material,0);
    original_model(self,draw_state,info,bones);
    original_override(self,engine_override,engine_override_type);
    memory::InvokeVoid(color,flat_material,old_r,old_g,old_b);
    memory::InvokeVoid(flag,flat_material,1<<15,old_z);
}
void RestoreGlow(GlowBackup& b) {
    void* ptr=api.Handle(b.handle);
    if(ptr && b.fields) {
        int current{};
        // Restore only our last values; do not overwrite a server update.
        if(memory::Field(ptr,b.fields->glow_color,current) && current==b.written_color)
            source::SetInteger(api.base.client_image,ptr,b.fields->glow_color_property,b.old_color);
        if(memory::Field(ptr,b.fields->glow_range,current) && current==b.written_range) Write(ptr,b.fields->glow_range,b.old_range);
        if(memory::Field(ptr,b.fields->glow_min,current) && current==0) Write(ptr,b.fields->glow_min,b.old_min);
        if(memory::Field(ptr,b.fields->glow_type,current) && current==3)
            source::SetInteger(api.base.client_image,ptr,b.fields->glow_type_property,b.old_type);
    }
    b={};
}
void Glow(const Entity& e,float distance) {
    const auto& f=*e.fields;
    if(e.index<1 || e.index>=8192 || f.glow_type<0 || f.glow_color<0 || f.glow_range<0 || f.glow_min<0 ||
       !f.glow_type_property.prop || !f.glow_color_property.prop) {++status.glow_unsupported;return;}
    auto& b=glow_backup[e.index];
    if(b.handle && b.handle!=e.handle) RestoreGlow(b);
    if(!b.handle) {
        if(!memory::Field(e.ptr,f.glow_type,b.old_type) || !memory::Field(e.ptr,f.glow_color,b.old_color) ||
           !memory::Field(e.ptr,f.glow_range,b.old_range) || !memory::Field(e.ptr,f.glow_min,b.old_min)) {++status.glow_failed;return;}
        b.handle=e.handle; b.fields=&f;
    } else {
        int current{};
        if(memory::Field(e.ptr,f.glow_type,current) && current!=3) b.old_type=current;
        if(memory::Field(e.ptr,f.glow_color,current) && current!=b.written_color) b.old_color=current;
        if(memory::Field(e.ptr,f.glow_range,current) && current!=b.written_range) b.old_range=current;
        if(memory::Field(e.ptr,f.glow_min,current) && current!=0) b.old_min=current;
    }
    b.touched=true;
    // User-configurable glow tint (players tab); packing matches Color().
    const unsigned tint=e.kind==Kind::None ? esp::style::ObjectColor(cfg,e.object) :
        e.kind==Kind::Survivor ? cfg.glow_color_survivor : cfg.glow_color_infected;
    b.written_color=static_cast<int>(tint&0xffffff);
    b.written_range=static_cast<int>(distance/.0254f);
    // The type proxy registers/unregisters CGlowProperty with the native glow
    // manager. The color proxy also updates its cached channels. Raw writes
    // bypass both notifications and cannot enable a previously absent glow.
    const bool range_ok=Write(e.ptr,f.glow_range,b.written_range) && Write(e.ptr,f.glow_min,0);
    const bool color_ok=source::SetInteger(api.base.client_image,e.ptr,f.glow_color_property,b.written_color);
    const bool type_ok=range_ok && color_ok && source::SetInteger(api.base.client_image,e.ptr,f.glow_type_property,3);
    if(type_ok) ++status.glow_count;
    else {++status.glow_failed;RestoreGlow(b);}
}
esp::style::Info VisualInfo(const Entity& e,NameCache& cache) {
    esp::style::Info info{};info.object=e.object;
    if(e.ghost) info.flags|=esp::style::Ghost;
    if((cfg.esp_flags || cfg.esp_status_icons) && e.kind!=Kind::None) {
        if(e.kind==Kind::Common) {
            int sequence{};
            if(memory::Field(e.ptr,e.fields->sequence,sequence) && sequence!=cache.sequence) {
                cache.sequence=sequence;cache.idle=cache.burning=false;
                char name[160]{};
                if(api.Sequence(e,name,sizeof(name))) {
                    cache.idle=(strstr(name,"Idle") || strstr(name,"idle") || strstr(name,"Walk_Neutral")) && !strstr(name,"Upper_KNIFE");
                    cache.burning=strstr(name,"Burn") || strstr(name,"burn");
                }
            }
            if(cache.idle) info.flags|=esp::style::Idle;
        }
        bool burning{},incap{};
        if(cache.burning || (memory::Field(e.ptr,e.fields->burning,burning) && burning)) info.flags|=esp::style::OnFire;
        if(memory::Field(e.ptr,e.fields->incap,incap) && incap) info.flags|=esp::style::Incapacitated;
        if(e.kind==Kind::Survivor) {
            for(int offset:e.fields->pinned) {unsigned handle{};if(memory::Field(e.ptr,offset,handle)&&api.Handle(handle)){info.flags|=esp::style::Pinned;break;}}
            info.flags|=visuals::ActionFlags(e,GetTickCount64());
        }
        float rage{};
        if(e.kind==Kind::Witch && memory::Field(e.ptr,e.fields->rage,rage) && std::isfinite(rage))
            info.rage=static_cast<int>(std::lround(std::clamp(rage,0.f,1.f)*100));
    }
    if(cfg.esp_friends && e.kind!=Kind::None && e.index<=64) {
        const auto now=GetTickCount64();
        if(now>=cache.friend_next) {
            bool value{};cache.steam_friend=api.Friend(e,value) && value;cache.friend_next=now+2000;
        }
        if(cache.steam_friend) info.flags|=esp::style::Friend;
    }
    return info;
}
void CollectVisuals() {
    overlay.count=0; overlay.bone_count=0; overlay.valid=false; overlay.time=GetTickCount64();
    overlay.radar={};
    status.world_count=status.glow_count=status.glow_failed=status.glow_unsupported=0;
    model_selected.fill(false);
    models_enabled.store(cfg.chams || cfg.world_chams,std::memory_order_release);
    for(auto& b:glow_backup) b.touched=false;
    Entity local{};
    const bool legacy=cfg.radar || cfg.esp || cfg.world || cfg.glow || cfg.world_glow || cfg.chams || cfg.world_chams || (cfg.aim && (cfg.draw_fov || cfg.draw_target));
    const bool needed=legacy || visuals::Needed(cfg);
    if(needed && api.Local(local,false) && Scan()) {
        if(cfg.radar) {
            Vec3 view{};
            overlay.radar.valid=api.Angles(view);
            overlay.radar.origin=local.origin;overlay.radar.yaw=view.y;
            overlay.radar.captured=overlay.time;
        }
        const source::Matrix* matrix{};
        overlay.valid=memory::Invoke(api.base.matrix,matrix,api.base.engine) && memory::Read(matrix,overlay.matrix);
        float magnitude{};
        for(const auto& row:overlay.matrix.m) for(float value:row) {
            if(!std::isfinite(value)) overlay.valid=false;
            magnitude+=std::abs(value);
        }
        if(magnitude<.01f) overlay.valid=false;
        visuals::Capture(api,cfg,indices,local,overlay.matrix,overlay.valid);
        game::Globals clock{};
        const bool clock_valid=api.Clock(clock);
        static ULONGLONG next_health_cvars{};
        static float pills_decay_rate=.27f;
        static int incap_maximum=300;
        if(cfg.esp && overlay.time>=next_health_cvars) {
            float value{};
            if(api.ConVarFloat("pain_pills_decay_rate",value) && value>=0.f && value<=100.f)
                pills_decay_rate=value;
            if(api.ConVarFloat("survivor_incap_health",value) && value>=1.f && value<=1000000.f)
                incap_maximum=static_cast<int>(value);
            next_health_cvars=overlay.time+1000;
        }
        // Bones are captured for the skeleton overlay and also to tighten the
        // box to the animated body (they must exist even when skeletons stay
        // hidden so boxes stay snug to the model).
        const bool capture_skeleton=cfg.esp && (cfg.skeleton || cfg.box_precision) && overlay.valid && clock_valid;
        int max_clients{}; memory::Invoke(api.base.max_clients,max_clients,api.base.engine);
        for(int index:indices) {
            if(!legacy) break;
            Entity e{};
            if(index==local.index || !api.Read(index,e,true,cfg.esp_ghosts || (cfg.radar && cfg.radar_ghosts))) continue;
            const bool world=e.kind==Kind::None;
            if(cfg.radar && !world && (!e.ghost || cfg.radar_ghosts) && overlay.radar.count<overlay.radar.points.size()) {
                const bool show=e.kind==Kind::Survivor?cfg.radar_survivor:e.kind==Kind::Common?cfg.radar_common:
                    e.kind==Kind::Witch?cfg.radar_witch:cfg.radar_special;
                const auto delta=game::Sub(e.origin,local.origin);
                if(show && std::hypot(delta.x,delta.y)*.0254f<=cfg.radar_range)
                    overlay.radar.points[overlay.radar.count++]={e.origin,e.kind,e.ghost};
            }
            if(world) {
                if(!ObjectFilter(e.object)) continue;
                unsigned owner{};
                // Flying grenades retain their thrower's handle; they are not
                // held inventory and must remain visible (as in RijiN).
                if(e.object!=game::Object::Projectile && e.fields->owner>=0 &&
                   memory::Field(e.ptr,e.fields->owner,owner) && api.Handle(owner)) continue;
            } else if((e.ghost && !cfg.esp_ghosts) || !VisualFilter(e)) continue;
            const float distance=game::Length(game::Sub(local.origin,e.origin))*.0254f;
            const float range=world ? cfg.world_range : cfg.esp_distance;
            if(distance>range) continue;
            auto& cache=names[index];
            if(cache.handle!=e.handle) { cache={}; cache.handle=e.handle; }
            const auto info=VisualInfo(e,cache);
            if((world && cfg.world_glow) || (!world && cfg.glow)) Glow(e,range);
            model_selected[index]=world ? cfg.world_chams : cfg.chams;
            model_tint[index]=world?esp::style::ObjectColor(cfg,e.object):esp::style::PlayerColor(cfg,e.kind,info.flags);
            model_handle[index]=e.handle;
            if((world && !cfg.world) || (!world && !cfg.esp) || overlay.count>=overlay.targets.size()) continue;
            auto& target=overlay.targets[overlay.count]; target={};
            overlay.info[overlay.count]=info;
            auto& skeleton=overlay.skeletons[overlay.count]; skeleton={};
            target.origin=e.origin; target.mins=e.mins; target.maxs=e.maxs;
            if(world)target.bounds_source=api.RenderBounds(e,target.render_corners)?
                esp::Target::BoundsSource::Render:esp::Target::BoundsSource::Missing;
            target.kind=e.kind; target.health=e.health; target.meters=distance; target.has_distance=true;
            if(!world && capture_skeleton && overlay.bone_count<overlay.bones.size() && esp::IntersectsView(target,overlay.matrix)) {
                const auto available=std::min<size_t>(128,overlay.bones.size()-overlay.bone_count);
                const auto written=api.Skeleton(e,{overlay.bones.data()+overlay.bone_count,available},clock.time);
                skeleton.first=static_cast<unsigned short>(overlay.bone_count);
                skeleton.count=static_cast<unsigned short>(written);
                overlay.bone_count+=written;
            }
            cache.peak=std::max(cache.peak,e.health);
            int max_hp{};
            if(memory::Field(e.ptr,e.fields->max_health,max_hp) && max_hp>0 && max_hp<=1000000) target.max_health=max_hp;
            else target.max_health=e.kind==Kind::Survivor?std::max(100,e.health):cache.peak;
            if(e.kind==Kind::Survivor) {
                bool incapacitated{};
                float buffer{},buffer_time{};
                memory::Field(e.ptr,e.fields->incap,incapacitated);
                target.incapacitated=incapacitated;
                target.permanent_health=e.health;
                // Raze's ESP calls C_TerrorPlayer vtable slot 117 (+0x1D4).
                // The installed client.dll implementation adds its own decayed
                // health buffer to the base HP, avoiding a second clock/rounding
                // approximation in the overlay.
                using GetHealth=int(__thiscall*)(void*);
                int game_health{};
                if(memory::Invoke(memory::Virtual<GetHealth>(e.ptr,117,api.base.client_image),game_health,e.ptr) &&
                   game_health>0 && game_health<=1000000) {
                    target.health=game_health;
                    target.temporary_health=std::max(0,game_health-e.health);
                    if(incapacitated) target.max_health=std::max(incap_maximum,game_health);
                } else {
                    memory::Field(e.ptr,e.fields->health_buffer,buffer);
                    const bool has_buffer_time=memory::Field(e.ptr,e.fields->health_buffer_time,buffer_time);
                    const auto health=survivor_health::Calculate({
                        e.health,target.max_health>0?target.max_health:100,incap_maximum,incapacitated,
                        buffer,buffer_time,clock_valid&&has_buffer_time?clock.time:buffer_time,pills_decay_rate
                    });
                    target.health=health.current;
                    target.max_health=health.maximum;
                    target.temporary_health=health.temporary;
                }
            }
            if(index<=max_clients && max_clients<=64 && GetTickCount64()>=cache.next) {
                cache.next=GetTickCount64()+1000;
                source::PlayerInfo info{}; bool ok{};
                if(memory::Invoke(api.base.player_info,ok,api.base.engine,index,&info) && ok) {
                    player_name::Copy(cache.name,info.name);
                }
            }
            const char* label=world ? e.label : source::Label(e.kind);
            if(e.kind==Kind::Common) label="Infectado comun";
            if(cache.name[0]) label=cache.name;
            snprintf(target.name,sizeof(target.name),"%s",label);
            overlay.world[overlay.count++]=world;
            if(world) ++status.world_count;
        }
    }
    if(!needed || !overlay.valid) visuals::ClearLevel();
    for(auto& b:glow_backup) if(b.handle && !b.touched) RestoreGlow(b);
    snprintf(status.glow_message,sizeof(status.glow_message),"%s | aplicados %u, errores %u, sin soporte %u",
        cfg.glow || cfg.world_glow ? "Glow nativo solicitado" : "Glow desactivado",
        status.glow_count,status.glow_failed,status.glow_unsupported);
    static bool logged_enabled=false;static unsigned logged_failures{};static ULONGLONG next_glow_log{};
    const bool glow_enabled=cfg.glow || cfg.world_glow;
    const auto now=GetTickCount64();
    if(logged_enabled!=glow_enabled || (logged_failures!=status.glow_failed && now>=next_glow_log)) {
        app::Log("%s",status.glow_message);logged_enabled=glow_enabled;logged_failures=status.glow_failed;next_glow_log=now+2000;
    }
}
bool AddHook(void* target,void* detour,void** original,const char* label) {
    if(!target) { app::Log("L4D2: %s no disponible (firma/interfaz)",label); return false; }
    const auto created=MH_CreateHook(target,detour,original);
    if(created!=MH_OK) { app::Log("L4D2: %s: %s",label,MH_StatusToString(created)); return false; }
    const auto enabled=MH_EnableHook(target);
    if(enabled!=MH_OK) { app::Log("L4D2: %s: %s",label,MH_StatusToString(enabled)); return false; }
    app::Log("L4D2: %s instalado",label); return true;
}
}
void Initialize(const source::Interfaces& interfaces) noexcept {
    try {
        std::lock_guard lock(game_mutex);
        api.Bind(interfaces);
        chat_spy::Initialize(api);
        visuals::Initialize(api);
        indices.reserve(2048);
        status.classes=static_cast<unsigned>(api.classes.size());
        status.trace=api.trace!=nullptr; status.bones=api.model_info!=nullptr; status.clock=api.globals!=nullptr;
        ResolvePacketContext();
        status.server_silent=false; // Becomes available only after a live stack match.
        status.spread=api.shared_random && api.prediction_seed && api.spread_offset>=0;
        status.shotgun_spread=status.spread && api.update_spread && api.globals;
        const auto writer=game::Signature(L"client.dll","55 8B EC A1 ? ? ? ? 83 78 30 00 53 8B 5D 10 56 57 74 ? 8B 4D 0C 8B 51 04");
        const auto writer_call=static_cast<unsigned char*>(game::Signature(L"client.dll","8B 4D FC 8B 75 0C 51 57 56 E8 ? ? ? ? 83 C4 0C 38 5E 10"));
        const auto pellet_loop=game::Signature(L"client.dll","8B 7D 1C EB 08 EB 03 8D 49 00 0F 57 D2 8B 4D F0 F3 0F 10 41 04 4B 47");
        status.shotgun_grouping=status.shotgun_spread && pellet_loop && writer_call &&
            game::RelativeCall(writer_call+9)==writer &&
            AddHook(writer,reinterpret_cast<void*>(&WriteUsercmdHook),reinterpret_cast<void**>(&original_write_usercmd),"Shotgun pattern / native command serialization");
        const auto command_call=static_cast<unsigned char*>(game::Signature(L"client.dll","E8 ? ? ? ? 8B 7D FC 56"));
        const auto command_target=game::RelativeCall(command_call);
        if(command_call) command_return=reinterpret_cast<uintptr_t>(command_call+5);
        status.command_hook=AddHook(command_target,reinterpret_cast<void*>(&MoveHook),reinterpret_cast<void**>(&original_move),"RijiN final command / verified CRC");
        status.tick_move_hook=AddHook(tick_move_fn,reinterpret_cast<void*>(&TickMoveHook),reinterpret_cast<void**>(&original_tick_move),"TickMove (speed / rapidfire batching)");
        const auto run_command=memory::Virtual<RunCommand>(api.prediction,18,api.base.client_image);
        status.run_command_hook=AddHook(reinterpret_cast<void*>(run_command),reinterpret_cast<void*>(&RunCommandHook),
                                        reinterpret_cast<void**>(&original_run_command),"RunCommand (rapidfire tickbase correction)");
        const auto simulation=static_cast<unsigned char*>(game::Signature(L"client.dll","8B 0D ? ? ? ? 8B 11 8B 52 48 50 8D 86 ? ? ? ? 50 56 FF D2 8B 0D ? ? ? ? 8B 01 8B 50 04 6A 00 FF D2 5E C3"));
        void** prediction_slot{};void* simulation_prediction{};
        if(simulation && memory::Read(simulation+2,prediction_slot) &&
           api.base.client_image.Contains(prediction_slot,sizeof(void*)) && memory::Read(prediction_slot,simulation_prediction) &&
           simulation_prediction==api.prediction) storm_prediction_return=reinterpret_cast<uintptr_t>(simulation+22);
        const auto prediction_update=memory::Virtual<PredictionUpdate>(api.prediction,3,api.base.client_image);
        const auto expected_update=game::Signature(L"client.dll","55 8B EC 83 EC 48 A1 ? ? ? ? 33 C5 89 45 FC A1 ? ? ? ? 53 56 57 33 FF 89 7D C8 89 7D CC 8B D9");
        if(reinterpret_cast<void*>(prediction_update)==expected_update && expected_update)
            storm_update_hook=AddHook(expected_update,reinterpret_cast<void*>(&PredictionUpdateHook),reinterpret_cast<void**>(&original_prediction_update),"Storm bounded prediction update");
        const auto process_movement=game::Signature(L"client.dll","55 8B EC 56 57 8B 7D 08 8B F1 C7 86 80 07 00 00 00 00 00 00 85 FF 74 ? 8B 07 8B 90 3C 02 00 00");
        storm_movement_hook=AddHook(process_movement,reinterpret_cast<void*>(&ProcessMovementHook),reinterpret_cast<void**>(&original_process_movement),"Storm negative-clock movement");
        app::Log("Storm automatic fall routes: prediction_return=%p update=%d movement=%d",reinterpret_cast<void*>(storm_prediction_return),storm_update_hook,storm_movement_hook);
        status.camera_hook=AddHook(game::Signature(L"client.dll","55 8B EC 81 EC ? ? ? ? 53 56 57 8B D9"),reinterpret_cast<void*>(&ViewHook),reinterpret_cast<void**>(&original_view),"RenderView");
        const auto vomit_draw=static_cast<unsigned char*>(game::Signature(L"client.dll","55 8B EC 51 8B 81 ? ? ? ? 85 C0 74 ? 83 78 58 00 74 ? 8D 48 08 8D 55 FF C6 45 FF FF 8B 01 8B 40 24 52 6A 01 FF D0 8B E5 5D C3"));
        void* vomit_query{};unsigned char query_code[21]{};int query_field{},draw_field{};
        if(vomit_draw&&api.base.client_image.Contains(vomit_draw-32,sizeof(query_code))&&
           memory::Copy(query_code,vomit_draw-32,sizeof(query_code))&&query_code[0]==0x8b&&query_code[1]==0x81&&
           query_code[6]==0x85&&query_code[7]==0xc0&&query_code[8]==0x75&&query_code[9]==3&&query_code[10]==0x32&&
           query_code[11]==0xc0&&query_code[12]==0xc3&&query_code[13]==0x83&&query_code[14]==0x78&&
           query_code[15]==0x58&&query_code[16]==0&&query_code[17]==0x0f&&query_code[18]==0x95&&query_code[19]==0xc0&&query_code[20]==0xc3&&
           memory::Read(vomit_draw-30,query_field)&&memory::Read(vomit_draw+6,draw_field)&&query_field==draw_field&&query_field>0&&query_field<0x40000)
            vomit_query=vomit_draw-32;
        // Native infected / third-strike / ghost / intro weight submissions.
        // Suppress only the first and third return sites, preserving map and
        // third-strike corrections and the manager's normal queue lifecycle.
        auto weights=static_cast<unsigned char*>(game::Signature(L"client.dll","D9 46 30 8B 46 40 6A 00 51 8B 0D ? ? ? ? D9 1C 24 50 E8 ? ? ? ? D9 46 34 6A 00 51 8B 4E 44 D9 1C 24 51 8B 0D ? ? ? ? E8 ? ? ? ? D9 46 38 8B 56 48 6A 00 51 8B 0D ? ? ? ? D9 1C 24 52 E8 ? ? ? ? D9 46 3C"));
        void* color_weight=weights?game::RelativeCall(weights+19):nullptr;
        const unsigned char expected_weight[]{0x55,0x8b,0xec,0x8b,0x45,0x08,0x83,0xec,0x0c,0x56,0x8b,0xf1,0x85,0xc0};
        unsigned char weight_code[sizeof(expected_weight)]{};
        if(!color_weight || game::RelativeCall(weights+43)!=color_weight || game::RelativeCall(weights+67)!=color_weight ||
           !memory::Copy(weight_code,color_weight,sizeof(weight_code)) || std::memcmp(weight_code,expected_weight,sizeof(weight_code))) color_weight=nullptr;
        const auto removals=visual_removals::Install(api.mode,api.base.client_image,vomit_query,vomit_draw,&Running,
            color_weight,color_weight?weights+24:nullptr,color_weight?weights+72:nullptr);
        status.infected_vision_hook=removals.infected_vision;
        app::Log("Infected blue vision weight route: %s",removals.infected_vision?"ready":"unavailable");
        status.fog_hook=removals.fog;status.vomit_hook=removals.vomit;
        client_guard::Targets guard_targets{};
        guard_targets.send=game::Signature(L"engine.dll","55 8B EC 56 8B F1 8D 8E ? ? ? ? E8 ? ? ? ? 85 C0 75 07 B0 01 5E 5D C2 0C 00 53");
        guard_targets.settings=game::Signature(L"engine.dll","8B C1 8B 48 50 8B 11 50 8B 42 0C FF D0 C3");
        guard_targets.command=game::Signature(L"engine.dll","8B C1 8B 48 50 8B 11 50 8B 42 08 FF D0 C3");
        guard_targets.crosshair=game::Signature(L"engine.dll","8B C1 8B 48 50 8B 11 50 8B 42 48 FF D0 C3");
        guard_targets.sound=game::Signature(L"engine.dll","55 8B EC 81 EC 80 00 00 00 6A 00 68 BC 00 00 00 68 ? ? ? ? 8D 4D CC E8");
        const auto registration=game::Signature(L"client.dll","8B 0D ? ? ? ? 6A 01 68 ? ? ? ? E8 ? ? ? ? 8B 0D ? ? ? ? 6A 01 68 ? ? ? ? E8 ? ? ? ? 8B 0D ? ? ? ? 6A FF 68");
        using UserMessage=bool(__thiscall*)(void*,int,void*);
        if(client_guard::ResolveMessages(registration,api.base.client_image))
            guard_targets.dispatch=reinterpret_cast<void*>(memory::Virtual<UserMessage>(api.base.client,35,api.base.client_image));
        const auto guard=client_guard::Install(guard_targets,api.engine_image,api.base.client_image,&Running);
        app::Log("Client guard routes: send=%d ConVars=%d commands=%d crosshair=%d usermsg=%d sound=%d",guard.send,guard.settings,guard.command,guard.crosshair,guard.messages,guard.sound);
        app::Log("Visual removals: fog route=%s, vomit query+draw=%s",removals.fog?"ready":"unavailable",removals.vomit?"ready":"unavailable");
        status.charger_turn_available=charger_patch.Bind(api.base.client_image,game::Signature(L"client.dll","F6 86 ? ? ? ? 20 74 ? 80 BE ? ? ? ? 00 74 ? D9 86 ? ? ? ? 8D 47 0C D9 18 50 D9 86"));
        app::Log("Charger turn validated block: %s",status.charger_turn_available?"ready":"unavailable");
        status.local_model_hook=AddHook(game::Signature(L"client.dll","55 8B EC 83 EC ? A1 ? ? ? ? 33 C5 89 45 ? 56 8B B1"),reinterpret_cast<void*>(&DrawLocalHook),reinterpret_cast<void**>(&original_draw_local),"ShouldDrawLocalPlayer");
        // Unique return site from the L4D2 reference, additionally requiring
        // the immediately preceding push -1 / call eax instruction pair.
        auto override_return=static_cast<unsigned char*>(game::Signature(L"client.dll","8B 0D ? ? ? ? 85 C0 0F 84"));
        unsigned char call_bytes[4]{};
        if(override_return && memory::Copy(call_bytes,override_return-4,sizeof(call_bytes)) &&
           call_bytes[0]==0x6A && call_bytes[1]==0xFF && call_bytes[2]==0xFF && call_bytes[3]==0xD0)
            third_override_return=reinterpret_cast<uintptr_t>(override_return);
        const bool third_hook=AddHook(game::Signature(L"client.dll","55 8B EC 56 8B F1 E8 ? ? ? ? 84 C0 74 ? 33 C0"),reinterpret_cast<void*>(&ThirdpersonHook),reinterpret_cast<void**>(&original_thirdperson),"CamIsThirdPerson");
        status.camera_isolation=third_hook && third_override_return!=0;
        app::Log("Cam3: exclusion de OverrideView %s (retorno=%p)",status.camera_isolation?"resuelta":"no disponible",reinterpret_cast<void*>(third_override_return));
        status.viewmodel_hook=AddHook(game::Signature(L"client.dll","55 8B EC 83 EC ? A1 ? ? ? ? 33 C5 89 45 ? 8B 45 ? 8B 10 89 55 ? 8B 50 ? 89 55 ? 8B 50 ? 89 55 ? 8B 10 89 55"),reinterpret_cast<void*>(&ViewmodelHook),reinterpret_cast<void**>(&original_viewmodel),"CalcViewModelView");
        status.weapon_fov_hook=AddHook(reinterpret_cast<void*>(memory::Virtual<WeaponFov>(api.mode,40,api.base.client_image)),reinterpret_cast<void*>(&WeaponFovHook),reinterpret_cast<void**>(&original_weapon_fov),"GetViewModelFOV");
        model_renderer=game::Interface(L"engine.dll","VEngineModel016");
        material_system=game::Interface(L"materialsystem.dll","VMaterialSystem080");
        if(material_system && AddHook(reinterpret_cast<void*>(memory::Virtual<Override>(model_renderer,1)),reinterpret_cast<void*>(&OverrideHook),reinterpret_cast<void**>(&original_override),"ForcedMaterialOverride"))
            status.model_hook=AddHook(reinterpret_cast<void*>(memory::Virtual<ModelDraw>(model_renderer,19)),reinterpret_cast<void*>(&ModelHook),reinterpret_cast<void**>(&original_model),"DrawModelExecute");
        Message(status.command_hook ? "Hook instalado; esperando comandos del juego" : "Hook de comandos no disponible; revisar free_esp.log");
        Publish();
    } catch(...) { app::Log("L4D2: error al inicializar modulos opcionales"); }
}
void Activate() noexcept { active.store(true,std::memory_order_release); }
// Frame status for speedhack; lag mutations belong to native commands.
void ExploitSpeedhack() {
    if(rapid_frame_ready.load(std::memory_order_acquire)) {
        speed_budget.Reset();status.speedhack_active=false;
        if(cfg.speedhack) std::snprintf(status.speedhack_message,sizeof(status.speedhack_message),"Paused while rapidfire arms extra commands");
        return;
    }
    Entity local{};
    if(!SpeedhackReady(local)) speed_budget.Reset();
}
void Exploits() {
    ExploitSpeedhack();
    game::CommandState commands{};
    if(rapid_first_command>0 && rapid_last_command>=rapid_first_command && api.Commands(commands)) {
        const auto acknowledged=static_cast<unsigned>(std::clamp(commands.acknowledged-rapid_first_command+1,0,rapid_last_command-rapid_first_command+1));
        if(acknowledged!=status.rapidfire_acked) {
            status.rapidfire_acked=acknowledged;
            app::Log("Rapidfire command ACK: %u/%u | last_ack=%d expected=%d (not a damage confirmation)",
                status.rapidfire_acked,status.rapidfire_sent,commands.acknowledged,rapid_last_command);
        }
    }
}
void Frame(int stage) noexcept {
    if(restore_requested.exchange(false,std::memory_order_acq_rel)) ClearLevel();
    if((stage!=4 && stage!=6) || !Running()) return;
    try {
        std::lock_guard lock(game_mutex); ReadConfig();
        Entity charger{};
        const bool in_game=api.InGame();
        const bool known=cfg.charger_turn&&in_game&&api.Local(charger,true,false);
        charger_patch.Set(charger_turn::Eligible(cfg.charger_turn,Running(),in_game,known,charger.team,
            charger.kind==Kind::Charger,known,charger.ghost));
        status.charger_turn_available=charger_patch.Available();status.charger_turn_active=charger_patch.Active();
        status.fog_removed=cfg.no_fog&&status.fog_hook&&in_game;
        status.vomit_removed=cfg.no_vomit&&status.vomit_hook&&in_game;
        status.infected_vision_removed=cfg.no_infected_vision&&status.infected_vision_hook&&in_game;
        if(stage==6) {aimbot::Capture(api,cfg,indices);chat_spy::Frame();CollectVisuals();Exploits();Publish(true);}
    }
    catch(...) {
        std::lock_guard lock(game_mutex); overlay={}; Message("Error en recolector: funciones detenidas");
        active.store(false,std::memory_order_release); restore_requested.store(true,std::memory_order_release); Publish(true);
    }
}
void ClearLevel() noexcept {
    std::lock_guard lock(game_mutex);
    rapid_channel.Restore(api);
    rapid_send_budget.Clear();
    rapid_pacing_removed=rapid_pacing_peak=0.;rapid_pacing_warned=false;
    rapid_recovery.Cancel();
    charger_patch.Set(false);status.charger_turn_active=charger_patch.Active();
    teleport_pulse.Reset();status.charger_teleport_pulse=status.charger_teleport_available=false;
    status.charger_teleport_commands=0;
    aim_camera.Reset();
    normal_tracking.Reset();
    spread_camera.Reset();shotgun_samples.fill({});shotgun_history.Clear();
    status.aim_filters[0]=status.aim_blocked[0]=0;
    std::snprintf(status.aim_message,sizeof(status.aim_message),"Sin intentos en este mapa");
    status.candidates=status.traces=status.hitbox_targets=status.history_records=0;
    status.shotgun_sample_weapon=-1;status.shotgun_sample_ok=false;status.shotgun_sample_offset={};
    chat_spy::ClearLevel();visuals::ClearLevel();
    for(auto& b:glow_backup) if(b.handle) RestoreGlow(b);
    extra_index=0;speed_extras=rapid_extras=0;
    rapid_extra_moves=-1;rapid_ring=rapid_fire::Ring{};
    rapid_armed=false;rapid_channel_failed=false;rapid_prediction_failed=false;
    rapid_frame_ready.store(false,std::memory_order_release);
    rapid_interval=0.f;rapid_tickbase_entity=nullptr;rapid_tickbase_offset=-1;
    rapid_owner_handle=0;
    rapid_first_command=rapid_last_command=rapid_generated_last=0;
    pistol_edge=rapid_fire::PistolEdge{};
    config_generation=0;local_cache_generation=0;weapon_cache_generation=0;
    local_cache={};weapon_cache={};weapon_cache_handle=0;aim_selection=AimSelection{};
    frame_focus_valid=false;
    status.rapidfire_extra=status.rapidfire_bursts=status.rapidfire_acked=status.rapidfire_sent=0;
    status.rapidfire_active=false;
    speed_budget.Reset();status.speedhack_active=false;status.speedhack_extra=status.speedhack_commands=0;
    air_control.Reset();next_air_physics=0;
    status.strafe_active=status.bhop_active=false;status.movement_speed=0.f;
    status.movement_physics_known=false;status.movement_air_acceleration=10.f;
    freeze_ticks.Reset();hunter_click.Reset();melee_cycle.Reset();
    storm_history.Reset();storm_player=nullptr;storm_tickbase=storm_ducked=-1;storm_failed=inside_storm_prediction=false;
    status.airstuck_active=status.ceiling_active=status.hunter_click_active=status.fast_melee_active=status.fall_lag_active=status.lag_active=false;
    status.fall_lag_pulses=status.lag_updates=0;
    lag_commands.Reset();last_lag_result={};last_lag_method={};lag_origin=0;PauseLag("Lag off");
    indices.clear(); next_scan=trigger_since=0; trigger_handle=0; third_active=false;
    force_next_send=false;aimbot::Reset();status.aim_applied=false;status.server_silent=false;
    third_owner.store(nullptr,std::memory_order_release); names.fill({});
    api.model_cache.fill({});
    api.skeleton_cache.fill({});
    model_selected.fill(false); models_enabled.store(false,std::memory_order_release); overlay={}; status.target=-1; Publish(true);
}
void SetConfig(const Config& value) noexcept {
    auto c=value;
    const auto clamp=[](float& v,float lo,float hi,float fallback){v=std::isfinite(v)?std::clamp(v,lo,hi):fallback;};
    clamp(c.fov,.1f,180.f,180.f); clamp(c.smoothing,0.f,100.f,0.f); clamp(c.aim_distance,1.f,500.f,150.f);
    clamp(c.esp_distance,1.f,500.f,150.f); clamp(c.world_range,1.f,200.f,80.f); clamp(c.box_width,.5f,4.f,1.f);
    clamp(c.skeleton_width,.5f,4.f,1.f);
    clamp(c.radar_range,5.f,200.f,60.f);clamp(c.radar_size,160.f,400.f,230.f);
    clamp(c.radar_x,0.f,1.f,0.f);clamp(c.radar_y,0.f,1.f,.15f);
    clamp(c.chat_x,0.f,1.f,0.f);clamp(c.chat_y,0.f,1.f,.7f);
    clamp(c.damage_lifetime,.4f,2.f,1.f);clamp(c.hitmarker_size,4.f,16.f,8.f);
    clamp(c.offscreen_radius,.35f,.9f,.8f);clamp(c.offscreen_range,5.f,500.f,100.f);
    clamp(c.trajectory_seconds,.5f,4.f,3.f);clamp(c.hud_opacity,.2f,1.f,1.f);
    clamp(c.boss_x,0.f,1.f,.5f);clamp(c.boss_y,0.f,1.f,.02f);clamp(c.boss_width,230.f,560.f,310.f);clamp(c.boss_scale,.7f,1.5f,1.f);
    clamp(c.spectator_width,200.f,520.f,300.f);c.spectator_rows=std::clamp(c.spectator_rows,1,16);
    clamp(c.chat_width,280.f,800.f,460.f);clamp(c.chat_height,180.f,650.f,300.f);
    c.chat_clear_minutes=std::clamp(c.chat_clear_minutes,0,240);
    for(auto& p:c.weapons) {
        clamp(p.shotgun_accuracy,0.f,100.f,0.f);
        clamp(p.fov,.1f,180.f,180.f);clamp(p.smoothing,0.f,100.f,0.f);clamp(p.aim_distance,1.f,500.f,150.f);
        p.aim_mode=std::clamp(p.aim_mode,0,2);p.hitbox=std::clamp(p.hitbox,0,4);p.trigger_delay=std::clamp(p.trigger_delay,0,1000);
        clamp(p.vm_forward,-60.f,60.f,0.f);clamp(p.vm_right,-60.f,60.f,0.f);clamp(p.vm_up,-60.f,60.f,0.f);
        clamp(p.vm_pitch,-89.f,89.f,0.f);clamp(p.vm_yaw,-180.f,180.f,0.f);clamp(p.vm_roll,-180.f,180.f,0.f);
        clamp(p.viewmodel_fov,40.f,120.f,60.f);
    }
    c.health_position=esp::layout::NormalizeSide(c.health_position);
    c.name_position=esp::layout::NormalizeSide(c.name_position);
    c.distance_position=esp::layout::NormalizeSide(c.distance_position);
    clamp(c.bash_fov,1.f,180.f,60.f); clamp(c.bash_range,10.f,75.f,70.f);
    clamp(c.camera_fov,70.f,140.f,90.f); clamp(c.camera_distance,30.f,300.f,120.f);
    clamp(c.camera_right,-64.f,64.f,0.f); clamp(c.camera_up,-64.f,64.f,0.f);
    clamp(c.vm_forward,-60.f,60.f,0.f); clamp(c.vm_right,-60.f,60.f,0.f); clamp(c.vm_up,-60.f,60.f,0.f);
    clamp(c.vm_pitch,-89.f,89.f,0.f); clamp(c.vm_yaw,-180.f,180.f,0.f); clamp(c.vm_roll,-180.f,180.f,0.f);
    clamp(c.viewmodel_fov,40.f,120.f,60.f);
    c.aim_key=std::clamp(c.aim_key,0,255); c.trigger_key=std::clamp(c.trigger_key,0,255);
    c.infected_aim_key=std::clamp(c.infected_aim_key,0,255);
    c.third_key=std::clamp(c.third_key,0,255);
    c.ceiling_key=std::clamp(c.ceiling_key,0,255);c.hunter_click_key=std::clamp(c.hunter_click_key,0,255);
    c.fast_melee_key=std::clamp(c.fast_melee_key,0,255);
    c.speedhack_key=std::clamp(c.speedhack_key,0,255); c.airstuck_key=std::clamp(c.airstuck_key,0,255);
    c.lag_key=std::clamp(c.lag_key,0,255); c.lag_amount=std::clamp(c.lag_amount,10,1000);
    c.lag_amount_v2=std::clamp(c.lag_amount_v2,10,1000);
    c.lag_method=std::clamp(c.lag_method,0,1);c.fall_lag_method=std::clamp(c.fall_lag_method,0,1);
    c.rapidfire_key=std::clamp(c.rapidfire_key,0,255);
    c.rapidfire_method=std::clamp(c.rapidfire_method,0,2);
    c.rapidfire_storm_speed=std::clamp(c.rapidfire_storm_speed,rapid_fire::SpeedMinimum,rapid_fire::SpeedMaximum);
    c.rapidfire_storm_ticks=std::clamp(c.rapidfire_storm_ticks,rapid_fire::StormTicksMinimum,rapid_fire::StormTicksMaximum);
    c.rapidfire_m1_speed=std::clamp(c.rapidfire_m1_speed,rapid_fire::SpeedMinimum,rapid_fire::SpeedMaximum);
    c.rapidfire_m1_choke=std::clamp(c.rapidfire_m1_choke,rapid_fire::ChokeMinimum,rapid_fire::ChokeMaximum);
    c.rapidfire_m2_speed=std::clamp(c.rapidfire_m2_speed,rapid_fire::SpeedMinimum,rapid_fire::SpeedMaximum);
    clamp(c.speedhack_scale,1.f,movement_batch::MaxMultiplier,3.f);
    c.activation=std::clamp(c.activation,0,2); c.priority=std::clamp(c.priority,0,2); c.hitbox=std::clamp(c.hitbox,0,4);
    c.aim_mode=std::clamp(c.aim_mode,0,2);c.history_ms=std::clamp(c.history_ms,0,200);
    for(int* rank : {&c.rank_head,&c.rank_chest,&c.rank_stomach,&c.rank_left_arm,
                    &c.rank_right_arm,&c.rank_left_leg,&c.rank_right_leg}) *rank=std::clamp(*rank,1,4);
    c.hitbox_mask=(c.hit_head?1:0)|(c.hit_chest?2:0)|(c.hit_stomach?4:0)|(c.hit_left_arm?8:0)|
        (c.hit_right_arm?16:0)|(c.hit_left_leg?32:0)|(c.hit_right_leg?64:0);
    c.trigger_delay=std::clamp(c.trigger_delay,0,1000);
    AcquireSRWLockExclusive(&shared_lock);
    if(!(pending==c)) {pending=c;++pending_revision;}
    ReleaseSRWLockExclusive(&shared_lock);
    chat_spy::SetOptions({c.chat_spy,c.chat_panel,c.chat_hud&&!c.capture_privacy,c.chat_deduplicate,c.chat_team_prefix,c.chat_clear_minutes});
    third_requested.store(c.thirdperson,std::memory_order_release);
    visual_removals::SetOptions(c.no_fog,c.no_vomit,c.no_infected_vision);
    client_guard::SetOptions({c.guard_queries,c.guard_settings,c.guard_commands,c.guard_messages,c.guard_crosshair,
        c.guard_crc,c.guard_null_sound,c.guard_heartbeat,c.guard_logging});
}
Status GetStatus() noexcept { AcquireSRWLockShared(&shared_lock); auto s=published_status; ReleaseSRWLockShared(&shared_lock); return s; }
RadarSnapshot GetRadar() noexcept {
    RadarSnapshot frame{};
    AcquireSRWLockShared(&shared_lock);snapshot::CopyRadar(frame,published_overlay.radar);ReleaseSRWLockShared(&shared_lock);
    if(!frame.captured || GetTickCount64()-frame.captured>250) return {};
    return frame;
}
void Draw(ImDrawList* list,ImVec2 screen) noexcept {
    // This function only consumes POD snapshots. It never calls into entities.
    if(!list || screen.x<=0 || screen.y<=0) return;
    // The bounded bone pool is too large for a per-call game-thread stack copy.
    static thread_local Overlay frame{};
    static thread_local Config config{},raw_config{};
    static thread_local std::uint64_t seen_overlay=~std::uint64_t{},seen_config{};
    static thread_local int seen_weapon=-2;
    AcquireSRWLockShared(&shared_lock);
    const bool changed=seen_config!=pending_revision;
    if(changed) {raw_config=pending;seen_config=pending_revision;}
    const int weapon=published_status.weapon_index;
    if(changed || seen_weapon!=weapon) {config=raw_config;seen_weapon=weapon;ApplyWeaponProfile(config,weapon);}
    if(seen_overlay!=overlay_revision) {snapshot::Copy(frame,published_overlay);seen_overlay=overlay_revision;}
    frame.aim_point=published_overlay.aim_point;frame.has_target=published_overlay.has_target;
    ReleaseSRWLockShared(&shared_lock);
    if(!frame.valid || GetTickCount64()-frame.time>250) return;
    const auto label=[&](float x,float y,ImU32 color,const char* value){
        overlay_paint::Label(list,{x,y},color,value);
    };
    const auto text=[&](float x,float y,ImU32 color,const char* value){label(x-ImGui::CalcTextSize(value).x*.5f,y,color,value);};
    list->PushClipRect({0,0},screen,true);
    for(size_t i=0;i<frame.count;++i) {
        const auto& t=frame.targets[i]; const bool world=frame.world[i];
        if((world && !config.world) || (!world && !config.esp)) continue;
        if(!world && config.skeleton) {
            const auto& skeleton=frame.skeletons[i];
            const size_t end=static_cast<size_t>(skeleton.first)+skeleton.count;
            if(skeleton.count && end<=frame.bone_count && end<=frame.bones.size())
                esp::DrawSkeleton({frame.bones.data()+skeleton.first,skeleton.count},frame.matrix,list,screen,config.skeleton_width,
                    t.kind==Kind::Survivor ? static_cast<ImU32>(config.skeleton_color_survivor) : static_cast<ImU32>(config.skeleton_color_infected));
        }
        ImVec2 lo{},hi{};
        const bool bounded=esp::ProjectBounds(t,frame.matrix,screen,lo,hi);
        if(!bounded) {
            float x{},y{};
            if(!world || !game::Project(frame.matrix,t.origin,screen.x,screen.y,x,y)) continue;
            lo={x-2,y-2}; hi={x+2,y+2};
        } else if(!world && config.box_precision) {
            // Tighten the box to the animated skeleton. The AABB behind mins/maxs
            // overestimates thin poses; joints define the real hull. Bones sit
            // inside the mesh, so re-expand by a proportional margin.
            const auto& skeleton=frame.skeletons[i];
            const size_t seal=static_cast<size_t>(skeleton.first)+skeleton.count;
            if(skeleton.count && seal<=frame.bone_count && seal<=frame.bones.size()) {
                float sx0=FLT_MAX,sy0=FLT_MAX,sx1=-FLT_MAX,sy1=-FLT_MAX; unsigned joints{};
                for(size_t b=skeleton.first;b<seal;++b) {
                    for(const auto& p:{frame.bones[b].start,frame.bones[b].end}) {
                        float px{},py{};
                        if(!game::Project(frame.matrix,p,screen.x,screen.y,px,py)) continue;
                        if(px<-screen.x*.5f || px>screen.x*1.5f || py<-screen.y*.5f || py>screen.y*1.5f) continue;
                        sx0=std::min(sx0,px); sy0=std::min(sy0,py);
                        sx1=std::max(sx1,px); sy1=std::max(sy1,py); ++joints;
                    }
                }
                if(joints>=6 && sx1>sx0+4.f && sy1>sy0+8.f && sx1-sx0<screen.x*1.5f && sy1-sy0<screen.y*1.5f) {
                    const float pad_x=(sx1-sx0)*.16f,pad_top=(sy1-sy0)*.05f,pad_bottom=(sy1-sy0)*.02f;
                    lo={sx0-pad_x,sy0-pad_top}; hi={sx1+pad_x,sy1+pad_bottom};
                }
            }
        }
        const float left=lo.x,top=lo.y,right=hi.x,bottom=hi.y;
        const auto& info=frame.info[i];
        // Per-category palette (players tab): friend/ghost overrides win.
        const auto color=static_cast<ImU32>(world ? esp::style::ObjectColor(config,info.object)
                                                  : esp::style::PlayerColor(config,t.kind,info.flags));
        if(bounded && (world ? config.world_boxes : config.boxes)) {
            overlay_paint::Box(list,{left,top},{right,bottom},color,!world&&config.corners,config.box_width,config.box_outlines);
        }
        float name_y=top-ImGui::GetFontSize()-3,distance_y=bottom+3;
        esp::layout::HealthLayout reserved_health{};bool has_health=false;
        if(!world && config.health && t.health>0 && t.max_health>0) {
            const float fraction=std::clamp(static_cast<float>(t.health)/t.max_health,0.f,1.f);
            char hp[48]{};
            if(t.kind==Kind::Survivor && t.incapacitated)
                snprintf(hp,sizeof(hp),"%d/%d",t.health,t.max_health);
            else if(t.kind==Kind::Survivor && t.temporary_health>0 && t.permanent_health>=0)
                snprintf(hp,sizeof(hp),"%d (%d+%d)",t.health,t.permanent_health,t.temporary_health);
            else snprintf(hp,sizeof(hp),"%d",t.health);
            const float font_size=esp::layout::HealthTextSize;
            auto* hp_font=ImGui::GetFont();
            const auto ink=esp::layout::HealthTextInk(hp_font,hp);
            const auto hp_layout=esp::layout::Health({left,top,right-left,bottom-top},config.health_position,
                                                    fraction,ink.w,ink.h,ImGui::GetFontSize());
            const auto& bar=hp_layout.outline; const auto& fill=hp_layout.fill; const auto& label=hp_layout.text;
            overlay_paint::HealthTrack(list,bar);
            overlay_paint::HealthFill(list,fill,
                config.health_split && t.temporary_health>0 && !t.incapacitated ? IM_COL32(231,183,77,255) :
                static_cast<ImU32>(esp::style::HealthColor(fraction,config.health_bands)));
            if(config.health_split && t.temporary_health>0 && !t.incapacitated && t.permanent_health>=0) {
                const float permanent=std::clamp(float(t.permanent_health)/t.max_health,0.f,1.f);
                const auto part=esp::layout::Health({left,top,right-left,bottom-top},config.health_position,permanent,0,ink.h).fill;
                overlay_paint::HealthFill(list,part,static_cast<ImU32>(esp::style::HealthColor(fraction,config.health_bands)));
            }
            if(config.health_segments) {
                // Quarter marks, like RijiN's segmented bar.
                const bool vertical=bar.h>=bar.w;
                for(int seg=1;seg<4;++seg) {
                    const float f=static_cast<float>(seg)*.25f;
                    if(vertical) {
                        const float y=bar.y+bar.h*f;
                        if(y>=fill.y && y<=fill.bottom()) list->AddLine({fill.x,y},{fill.right(),y},IM_COL32(0,0,0,220),1.f);
                    } else {
                        const float x=bar.x+bar.w*f;
                        if(x>=fill.x && x<=fill.right()) list->AddLine({x,fill.y},{x,fill.bottom()},IM_COL32(0,0,0,220),1.f);
                    }
                }
            }
            const auto text_pos=esp::layout::HealthTextOrigin(label,ink);
            overlay_paint::Plate(list,{label.x-2,label.y-1,ink.w+4,ink.h+2});
            list->AddText(hp_font,font_size,{text_pos.x+1.f,text_pos.y+1.f},IM_COL32(0,0,0,230),hp);
            list->AddText(hp_font,font_size,text_pos,IM_COL32(240,240,245,255),hp);
            name_y=hp_layout.name_y; distance_y=hp_layout.distance_y;
            reserved_health=esp::layout::Health({left,top,right-left,bottom-top},config.health_position,
                                               1.f,ink.w,ink.h,ImGui::GetFontSize());
            has_health=true;
        }
        // Status labels down the right edge of the box (RijiN flags).
        float flags_bottom=top;
        if(!world && config.esp_flags && (info.flags||(t.kind==Kind::Witch&&info.rage>0))) {
            float fy=top;float fx=right+7.f;
            if(has_health&&config.health_position==esp::layout::Right)
                fx=std::max(fx,std::max(reserved_health.outline.right(),reserved_health.text.right())+6.f);
            const auto flag=[&](const char* s,ImU32 c){
                label(fx,fy,c,s);fy+=ImGui::GetFontSize()+5.f;
            };
            if(info.flags&esp::style::Friend) flag("FRIEND",static_cast<ImU32>(config.esp_color_friend));
            if(info.flags&esp::style::Ghost) flag("GHOST",static_cast<ImU32>(config.esp_color_ghost));
            if(info.flags&esp::style::OnFire) flag("ON FIRE",IM_COL32(255,100,32,255));
            if(info.flags&esp::style::Incapacitated) flag("INCAP",IM_COL32(255,120,80,255));
            if(info.flags&esp::style::Pinned) flag("PINNED",IM_COL32(255,153,128,255));
            if(info.flags&esp::style::Healing) flag("HEALING",IM_COL32(133,228,177,255));
            if(info.flags&esp::style::Reviving) flag("REVIVING",IM_COL32(153,201,255,255));
            if(info.flags&esp::style::Revived) flag("BEING REVIVED",IM_COL32(153,201,255,255));
            if(info.flags&esp::style::Idle) flag("IDLE",IM_COL32(225,225,255,255));
            if(t.kind==Kind::Witch && info.rage>0) {
                char rgl[32]{"RAGED"};
                if(info.rage<100) snprintf(rgl,sizeof(rgl),"ANGER %d%%",info.rage);
                flag(rgl,info.rage>=100?IM_COL32(245,48,0,255):IM_COL32(238,208,0,255));
            }
            flags_bottom=fy;
        }
        if(!world && config.esp_status_icons && info.flags&(esp::style::Incapacitated|esp::style::Pinned|esp::style::Healing|esp::style::Reviving|esp::style::Revived)) {
            const ImVec2 p{left+8,top+8};const bool danger=(info.flags&(esp::style::Incapacitated|esp::style::Pinned))!=0;
            const auto tint=danger?IM_COL32(255,149,119,245):IM_COL32(157,231,192,245);
            list->AddCircleFilled(p,8,IM_COL32(15,19,17,225),16);
            if(info.flags&esp::style::Pinned) {list->AddLine({p.x-3,p.y-3},{p.x+3,p.y+3},tint,2);list->AddLine({p.x+3,p.y-3},{p.x-3,p.y+3},tint,2);}
            else {list->AddLine({p.x-4,p.y},{p.x+4,p.y},tint,2);list->AddLine({p.x,p.y-4},{p.x,p.y+4},tint,2);}
            if(info.flags&(esp::style::Reviving|esp::style::Revived))list->AddCircle(p,10,tint,18,1.5f);
        }
        char extra[64]{};
        const bool show_distance=world ? config.world_distance : config.distance;
        if(show_distance) snprintf(extra,sizeof(extra),"%.0f m",t.meters);
        float name_center=(left+right)*.5f,distance_center=name_center;
        const bool class_label=!world&&config.esp_class&&t.kind!=Kind::Survivor&&t.kind!=Kind::Common;
        float class_y=name_y-ImGui::GetFontSize()-2.f;
        if(!world) {
            const auto name_size=ImGui::CalcTextSize(t.name),distance_size=ImGui::CalcTextSize(extra);
            const float line=ImGui::GetFontSize(),class_height=class_label?line+2.f:0.f;
            const float name_width=class_label?std::max(name_size.x,ImGui::CalcTextSize(source::Label(t.kind)).x):name_size.x;
            const auto positions=esp::layout::Labels({left,top,right-left,bottom-top},config.name_position,config.distance_position,
                {0,0,name_width,line+class_height},{0,0,distance_size.x,line},has_health?&reserved_health:nullptr,
                config.health_position,config.names||class_label,show_distance,flags_bottom-top);
            name_center=positions.name.x+positions.name.w*.5f;name_y=positions.name.y+class_height;class_y=positions.name.y;
            distance_center=positions.distance.x+positions.distance.w*.5f;distance_y=positions.distance.y;
        }
        if(world ? config.world_names : config.names) text(name_center,name_y,color,t.name);
        // Optional class label above the name for special infected (or named players).
        if(class_label) text(name_center,class_y,IM_COL32(240,240,245,255),source::Label(t.kind));
        if(extra[0]) text(distance_center,distance_y,IM_COL32(240,240,245,255),extra);
    }
    if(config.aim && config.draw_fov && config.fov<89.f && frame.fov>0 && frame.fov<179) {
        // World-to-screen projection contains the actual horizontal focal scale.
        const auto& m=frame.matrix.m;
        const float focal=std::sqrt(m[0][0]*m[0][0]+m[0][1]*m[0][1]+m[0][2]*m[0][2]);
        const float radius=std::tan(config.fov*.0174532925f)*screen.x*.5f*focal;
        if(std::isfinite(radius) && radius>0 && radius<screen.x*2) list->AddCircle({screen.x*.5f,screen.y*.5f},radius,IM_COL32(194,155,231,180),96,1.f);
    }
    if(config.aim && config.draw_target && frame.has_target) {
        float x{},y{};
        if(game::Project(frame.matrix,frame.aim_point,screen.x,screen.y,x,y)) list->AddCircle({x,y},5.f,IM_COL32(255,220,90,255),20,2.f);
    }
    list->PopClipRect();
}
void ShutdownRendering() noexcept {
    client_guard::Suspend();
    chat_spy::Suspend();
    visuals::Suspend();
    active.store(false,std::memory_order_release); restore_requested.store(true,std::memory_order_release);
}
}
