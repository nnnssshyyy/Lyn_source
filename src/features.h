#pragma once
#include "game.h"
#include "esp.h"
#include "weapon_catalog.h"
#include <algorithm>
#include <cmath>

namespace features {
struct WeaponProfile {
    bool operator==(const WeaponProfile&) const = default;
#define W_BOOL(name,value,label) bool name{value};
#define W_INT(name,value,label) int name{value};
#define W_FLOAT(name,value,label) float name{value};
#include "weapon_fields.inc"
#undef W_BOOL
#undef W_INT
#undef W_FLOAT
};
struct Config {
    bool operator==(const Config&) const = default;
    std::array<WeaponProfile,weapons::Count> weapons{};
    bool shotgun_no_spread{}; // Effective per-weapon switch, derived below.
    float shotgun_accuracy{100.f};
    int hitbox_mask{127};
#define F_BOOL(name,value,category,label) bool name{value};
#define F_INT(name,value,category,label) int name{value};
#define F_FLOAT(name,value,category,label) float name{value};
#define F_COLOR(name,r,g,b,a,category,label) unsigned name{static_cast<unsigned>((r)|((g)<<8)|((b)<<16)|((a)<<24))};
#include "features_fields.inc"
#undef F_BOOL
#undef F_INT
#undef F_FLOAT
#undef F_COLOR
};
inline unsigned SpecialPriorityBit(source::Kind kind) noexcept {
    switch(kind) {
        case source::Kind::Smoker:return 1u;
        case source::Kind::Boomer:return 2u;
        case source::Kind::Hunter:return 4u;
        case source::Kind::Spitter:return 8u;
        case source::Kind::Jockey:return 16u;
        case source::Kind::Charger:return 32u;
        case source::Kind::Tank:return 64u;
        default:return 0u;
    }
}
inline unsigned SpecialPriorityMask(const Config& c) noexcept {
    if(!c.prioritize_classes) return 0u;
    return (c.priority_smoker?1u:0u)|(c.priority_boomer?2u:0u)|(c.priority_hunter?4u:0u)|
        (c.priority_spitter?8u:0u)|(c.priority_jockey?16u:0u)|(c.priority_charger?32u:0u)|(c.priority_tank?64u:0u);
}
inline void ApplyCapturePolicy(Config& c) noexcept {
    if(c.capture_privacy){c.glow=c.world_glow=c.chams=c.world_chams=false;c.chat_hud=false;}
}
inline bool AutomaticFireRequested(const Config& c) noexcept {
    // Always is the fully automatic activation mode. The saved auto-shoot
    // preference still controls the hold-key and on-attack modes.
    return c.activation==2 || c.auto_fire;
}
inline bool InfectedAimRequested(const Config& c,bool key_held,bool focused,bool flush_tick) noexcept {
    return c.aim && c.infected_aim && c.infected_aim_key>0 && c.infected_aim_key<256 &&
        key_held && focused && !flush_tick;
}
inline float SpreadCorrectionStrength(const Config& c,bool shotgun,bool firing,bool aimed_shot) noexcept {
    if(shotgun) {
        if(!c.shotgun_no_spread || (!firing && !aimed_shot) || !std::isfinite(c.shotgun_accuracy)) return 0.f;
        return std::clamp(c.shotgun_accuracy,0.f,100.f)*.01f;
    }
    return (c.no_spread && firing) || aimed_shot ? 1.f : 0.f;
}
inline bool ShouldAutoShoot(const Config& c,bool ready,float alignment_error,float tolerance=.5f) noexcept {
    return AutomaticFireRequested(c) && ready && (c.aim_mode!=0 || (alignment_error>=0.f && alignment_error<=tolerance));
}
struct Status {
    bool fog_removed{},vomit_removed{},infected_vision_removed{},infected_vision_hook{};
    bool airstuck_active{},ceiling_active{},hunter_click_active{},fast_melee_active{},fall_lag_active{};
    unsigned fall_lag_pulses{};
    bool lag_active{};
    unsigned lag_updates{};
    char lag_message[128]{"Manual lag off"};
    char fall_message[128]{"Automatic fall lag off"};
    bool fog_hook{},vomit_hook{},charger_turn_available{},charger_turn_active{};
    bool charger_teleport_available{},charger_teleport_pulse{};
    unsigned charger_teleport_commands{};
    char charger_teleport_message[128]{"Charger teleport desactivado"};
    bool camera_isolation{};
    bool tick_move_hook{},run_command_hook{},speedhack_active{};
    unsigned speedhack_extra{},speedhack_commands{};
    bool rapidfire_active{},rapidfire_transport{};
    unsigned rapidfire_extra{},rapidfire_bursts{},rapidfire_acked{},rapidfire_sent{};
    bool command_hook{},camera_hook{},local_model_hook{},viewmodel_hook{},model_hook{},trace{},bones{},clock{},movement{},weapon_fov_hook{};
    unsigned classes{},candidates{},traces{},world_count{},glow_count{},hitbox_targets{},history_records{},commands{};
    unsigned strafe_applied{},spread_applied{},recoil_applied{};
    unsigned bhop_filtered{};
    float movement_speed{},movement_air_acceleration{10.f};
    bool strafe_active{},bhop_active{},movement_physics_known{};
    bool server_silent{},spread{},friend_filter{};
    bool shotgun_spread{};
    bool shotgun_grouping{}; // Native serializer + pellet layout validated.
    int shotgun_sample_weapon{-1};
    bool shotgun_sample_ok{};
    source::Vec3 shotgun_sample_offset{};
    bool weapon_ready{};
    bool aim_applied{};
    bool aim_enabled{},aim_auto_fire{},aim_weapon_override{};
    int aim_mode{-1},aim_activation{},aim_key{};
    unsigned glow_failed{},glow_unsupported{};
    int move_type{-1};
    int target{-1},weapon_group{-1},weapon_index{-1}; float command_ms{};
    char message[128]{"Esperando inicializacion"};
    char aim_message[128]{"Sin intentos: activa enabled, cierra el menu y manten Mouse5"};
    char aim_blocked[128]{};
    char aim_filters[128]{};
    char camera_message[128]{"Camara original"};
    char glow_message[128]{"Glow desactivado"};
    char speedhack_message[128]{"Speedhack off"};
    char movement_message[128]{"Hold jump to use movement assistance"};
    char melee_message[160]{"Fast melee desactivado"};
    char rapidfire_message[160]{"Rapidfire off"};
};
inline void ApplyWeaponProfile(Config& c,int index) noexcept {
    c.shotgun_no_spread=false;
    c.shotgun_accuracy=100.f;
    if(index<0 || static_cast<size_t>(index)>=c.weapons.size()) return;
    const auto& p=c.weapons[index];
    if(p.combat && weapons::Catalog[index].group>=0) {
        c.aim=c.aim && p.aim;
        c.auto_fire=p.auto_fire;c.recoil=p.recoil;c.no_spread=p.no_spread;
        c.trigger=p.trigger;c.trigger_head=p.trigger_head;c.auto_pistol=p.auto_pistol;
        c.aim_mode=p.aim_mode;c.hitbox=p.hitbox;c.trigger_delay=p.trigger_delay;
        c.fov=p.fov;c.smoothing=p.smoothing;c.aim_distance=p.aim_distance;
    }
    c.shotgun_no_spread=weapons::IsShotgun(index) && p.shotgun_no_spread;
    if(weapons::IsShotgun(index)) c.shotgun_accuracy=p.shotgun_accuracy;
    if(c.shotgun_no_spread) c.no_spread=true;
    if(p.view) {
        c.viewmodel=c.vm_fov=true;
        c.vm_forward=p.vm_forward;c.vm_right=p.vm_right;c.vm_up=p.vm_up;
        c.vm_pitch=p.vm_pitch;c.vm_yaw=p.vm_yaw;c.vm_roll=p.vm_roll;
        c.viewmodel_fov=p.viewmodel_fov;
    }
}
struct RadarPoint { source::Vec3 position{}; source::Kind kind{}; bool ghost{}; };
struct RadarSnapshot {
    std::array<RadarPoint,512> points{};
    size_t count{};
    source::Vec3 origin{}; float yaw{};
    ULONGLONG captured{}; bool valid{};
};
RadarSnapshot GetRadar() noexcept;
// Called after MinHook initialization; optional hooks are isolated from ESP/DX9.
void Initialize(const source::Interfaces& api) noexcept;
void Activate() noexcept;
void Frame(int stage) noexcept;
void ClearLevel() noexcept;
void SetConfig(const Config& config) noexcept;
Status GetStatus() noexcept;
void Draw(ImDrawList* list,ImVec2 screen) noexcept;
void ShutdownRendering() noexcept;
}
