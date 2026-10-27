#pragma once
#include "source.h"
#include "lag_exploit.h"
#include <vector>
#include <unordered_map>
#include <span>

// L4D2 / Source 2018 x86 ABI. These structures are intentionally independent
// of the reference project's loader, allocator and packet manager.
namespace game {
using source::Vec3;
struct Matrix3 { float m[3][4]; };
struct Command {
    uintptr_t vtable;
    int number, tick;
    Vec3 angles;
    float forward, side, up;
    int buttons;
    unsigned char impulse, padding[3];
    int weapon, subtype, seed;
    short mouse_x, mouse_y;
    bool predicted;
    unsigned char tail[27];
};
static_assert(sizeof(Command)==88 && offsetof(Command,buttons)==36 && offsetof(Command,weapon)==44 && offsetof(Command,subtype)==48 && offsetof(Command,seed)==52);
struct Globals {
    float realtime; int frame; float absolute_frame_time, time, frame_time;
    int max_clients, tick; float interval, interpolation;
};
struct View {
    unsigned char prefix[0x34];
    float fov, viewmodel_fov;
    Vec3 origin, angles;
};
static_assert(offsetof(View,fov)==52 && offsetof(View,origin)==60);
struct alignas(16) AlignedVec { float x{},y{},z{},w{}; };
struct alignas(16) Ray {
    AlignedVec start{},delta{},offset{},extents{};
    const Matrix3* transform{}; bool is_ray{true}, swept{};
};
static_assert(sizeof(Ray)==80 && offsetof(Ray,is_ray)==68);
struct Trace {
    Vec3 start{},end{};
    Vec3 normal{}; float plane_distance{};
    unsigned char plane_type{},plane_signs{},plane_padding[2]{};
    float fraction{1.f}; int contents{};
    unsigned short displacement{}; bool all_solid{},start_solid{};
    float fraction_left{}; const char* surface{}; short surface_props{};
    unsigned short surface_flags{}; int hitgroup{};
    short bone{}; unsigned short world_surface{}; void* entity{}; int hitbox{};
};
static_assert(sizeof(Trace)==84 && offsetof(Trace,entity)==76);
struct Filter {
    void* skip{}; void* skip_weapon{}; bool world_only{};
    int skip_team{},team_offset{-1};
    memory::Range entity_image{};
    virtual bool ShouldHitEntity(void* entity,int) {
        if(entity==skip || entity==skip_weapon) return false;
        // Trace callbacks also receive static props, which have no entity netvars.
        using EntityMethod = void(__thiscall*)(void*);
        if(!skip_team || !entity_image.base ||
           !memory::Virtual<EntityMethod>(entity,4,entity_image)) return true;
        int team{};
        return !memory::Field(entity,team_offset,team) || team!=skip_team;
    }
    virtual int GetTraceType() const { return world_only ? 1 : 0; }
};
struct Hitbox { int bone,group; Vec3 min,max; int name; int unused[8]; };
static_assert(sizeof(Hitbox)==68);
struct Point { Vec3 position{}; int group{}, bone{}; int hitbox{-1},sample{}; float radius{}; };
enum class Object : unsigned char { None,Weapon,Health,Ammo,Throwable,Projectile,Objective };
struct Fields {
    source::Kind kind{}; Object object{}; int weapon_group{-1}; bool semi{},melee{},primary{};
    bool data_resolved{},model_candidate{};
    char name[64]{};
#define GAME_FIELD(name) int name{-1}
    GAME_FIELD(origin); GAME_FIELD(mins); GAME_FIELD(maxs); GAME_FIELD(team); GAME_FIELD(health);
    GAME_FIELD(max_health);
    GAME_FIELD(life); GAME_FIELD(zombie); GAME_FIELD(view); GAME_FIELD(punch); GAME_FIELD(active_weapon);
    GAME_FIELD(inventory); int inventory_count{};
    GAME_FIELD(next_attack); GAME_FIELD(next_primary); GAME_FIELD(next_secondary); GAME_FIELD(next_shove);
    GAME_FIELD(clip); GAME_FIELD(reload); GAME_FIELD(tickbase); GAME_FIELD(flags); GAME_FIELD(velocity);
    GAME_FIELD(move_type); GAME_FIELD(max_speed); GAME_FIELD(surface_friction);
    GAME_FIELD(fall_velocity); GAME_FIELD(ducked);
    GAME_FIELD(water); GAME_FIELD(ghost); GAME_FIELD(incap); GAME_FIELD(hitbox_set);
    GAME_FIELD(health_buffer); GAME_FIELD(health_buffer_time);
    GAME_FIELD(scale); GAME_FIELD(owner); GAME_FIELD(solid_flags); GAME_FIELD(solid_type);
    GAME_FIELD(burning); GAME_FIELD(rage); GAME_FIELD(simulation); GAME_FIELD(glow_type);
    GAME_FIELD(sequence); GAME_FIELD(client_ragdoll);
    GAME_FIELD(vomit_start); GAME_FIELD(vomit_fade);
    GAME_FIELD(dual_weapons);
    GAME_FIELD(melee_swing);
    GAME_FIELD(carry_victim);
    GAME_FIELD(glow_color); GAME_FIELD(glow_range); GAME_FIELD(glow_min); GAME_FIELD(render_mode);
    int pinned[5]{-1,-1,-1,-1,-1};
    source::Property glow_type_property{},glow_color_property{};
#undef GAME_FIELD
};
struct Entity {
    void* ptr{}; void* renderable{}; source::ClientClass* cc{};
    const Fields* fields{}; int index{},team{},health{-1}; unsigned handle{};
    source::Kind kind{}; Vec3 origin{},mins{},maxs{};
    Object object{}; char label[64]{}; bool ghost{};
};
struct CommandState {int outgoing{},choked{},acknowledged{};};
struct Api {
    struct ModelCache { const void* model{}; Object object{}; char label[64]{}; };
    struct SkeletonCache {
        const void* model{}; const void* header{};
        int checksum{},length{},bone_count{},bone_offset{};
        struct Link { std::uint8_t child{},parent{}; };
        std::array<Link,128> links{};
        unsigned count{}; int mask{0x100};
    };
    source::Interfaces base{};
    memory::Range engine_image{};
    void* mode{}; void* trace{}; void* model_info{}; void* prediction{};
    Globals* globals{};
    void* cvars{};
    void* sequence_name{};
    void* shared_random{};
    void* update_spread{};
    int* prediction_seed{};
    int spread_offset{-1};
    void* client_state_getter{};
    void* run_prediction{};
    int outgoing_command{-1},choked_commands{-1},acknowledged_command{-1},delta_tick{-1};
    std::unordered_map<source::ClientClass*,Fields> classes;
    mutable std::array<ModelCache,1024> model_cache{};
    mutable std::array<SkeletonCache,128> skeleton_cache{};
    bool Bind(const source::Interfaces& interfaces);
    bool Resolve();
    void ResolveMovement(const Entity& local);
    bool InGame() const noexcept;
    bool Clock(Globals& out) const noexcept;
    bool Read(int index,Entity& out,bool alive=true,bool allow_ghost=false) const noexcept;
    bool ReadPointer(void* ptr,Entity& out,bool alive=true,bool allow_ghost=false,bool inventory=false) const noexcept;
    bool ActiveWeapon(const Entity& local,Entity& out) const noexcept;
    bool CarryVictim(const Entity& local,unsigned& victim) const noexcept;
    bool NetworkChannelReady() const noexcept;
    void ClassifyModel(Entity& entity) const noexcept;
    bool RenderBounds(const Entity& entity,std::array<Vec3,8>& corners) const noexcept;
    bool Local(Entity& out,bool alive=true,bool allow_ghost=true) const noexcept;
    void* Handle(unsigned value) const noexcept;
    bool Eye(const Entity& entity,Vec3& out) const noexcept;
    int WeaponIndex(const Entity& weapon) const noexcept;
    bool Hitboxes(const Entity& entity,std::vector<Point>& out,float time) const;
    unsigned Skeleton(const Entity& entity,std::span<source::BoneSegment> out,float time) const noexcept;
    bool Cast(Vec3 start,Vec3 end,Filter& filter,Trace& out,unsigned mask=0x46004003,float hull=0.f) const noexcept;
    bool Angles(Vec3& angles,bool set=false) const noexcept;
    bool Sequence(const Entity& entity,char* name,size_t capacity) const noexcept;
    bool Friend(const Entity& entity,bool& result) const noexcept;
    bool Spread(const Entity& weapon,const Command& command,Vec3& offset,bool refresh=false) const noexcept;
    bool SpreadAtTick(const Entity& local,const Entity& weapon,const Command& command,Vec3& offset) const noexcept;
    float Interpolation(float interval) const noexcept;
    float NetworkLatency() const noexcept;
    bool Commands(CommandState& out) const noexcept;
    // VClientPrediction001::Update (slot 3), called after a rapidfire command
    // stores its tickbase correction, as in Raze's CreateMove hook.
    bool UpdatePrediction() const noexcept;
    bool ResetPredictionHistory() const noexcept;
    // CNetChan fields. The validated client-state accessor returns root+8, so
    // the channel lives at state+0x10 and its own fields are not adjusted again.
    // +0x10 is the outgoing acknowledged sequence; +0x1C is the retained-packet
    // choke count serialized as a byte. They are not clientstate command counters.
    bool Channel(int& acknowledged,int& choked) const noexcept;
    bool SetChannelAck(int value) const noexcept;
    bool SetChannelChoked(int value) const noexcept;
    bool SetConVarFloat(const char* name,float value,float* previous=nullptr) const noexcept;
    bool ConVarFloat(const char* name,float& value) const noexcept;
    bool ConVarNumber(const char* name,float& real,int& integer,bool set=false) const noexcept;
    bool ShiftSequence(int amount) const noexcept;
    bool StormSequence(int amount) const noexcept;
    lag_exploit::Result RazeLag(lag_exploit::Method method,int amount) const noexcept;
};
// Storm's RunCommand correction is scoped to a matching local command. Keep
// the native simulation's tick advance, then add back the recorded shift.
class StormClock {
    void* player_{};int offset_{-1},shift_{},before_{};Globals* globals_{};float time_{},interval_{};
public:
    StormClock()=default;
    StormClock(const StormClock&)=delete;
    StormClock& operator=(const StormClock&)=delete;
    ~StormClock() {Restore();}
    bool Begin(void* player,int offset,Globals* globals,int shift) noexcept;
    bool Restore() noexcept;
    bool Active() const noexcept {return player_!=nullptr;}
};
bool StormMovementInput(void* move,bool ducked) noexcept;
// Channel declarations belong only to the native CL_Move call that sends them.
// Restore fields still equal to our writes; preserve updates made by the engine.
class ChannelPatch {
    void* owner_{};
    int ack_{},choked_{},applied_ack_{},applied_choked_{},sequence_{};
    bool wrote_ack_{},wrote_choked_{};
public:
    bool Apply(const Api&,bool write_ack,int acknowledged,int choked) noexcept;
    void Restore(const Api&) noexcept;
};
// The ordinary CL_Move pays its normal bandwidth charge. Synthetic moves in
// that same frame must not leave a future CanPacket deadline after release.
// Resolve the shared deadline/clock from both native methods, never a guessed
// object offset. Unknown builds and replaced channels are left untouched.
class ChannelMoveBudget {
    void* table_{}; void* owner_{};
    memory::Range image_{};
    int deadline_offset_{-1}; const double* clock_{};
    double before_{},started_{};
    bool Resolve(const Api&,void*) noexcept;
public:
    bool Begin(const Api&) noexcept;
    double Restore(const Api&) noexcept;
    bool Delay(const Api&,double& seconds) noexcept;
    void Clear() noexcept {*this={};}
};
void* Interface(const wchar_t* module,const char* version) noexcept;
void* Signature(const wchar_t* module,const char* pattern);
void* RelativeCall(void* instruction) noexcept;
Vec3 Add(Vec3 a,Vec3 b) noexcept;
Vec3 Sub(Vec3 a,Vec3 b) noexcept;
Vec3 Mul(Vec3 a,float b) noexcept;
float Length(Vec3 a) noexcept;
float Normalize(float value) noexcept;
Vec3 Angle(Vec3 from,Vec3 to) noexcept;
Vec3 Forward(Vec3 angles) noexcept;
Vec3 CameraOrigin(Vec3 anchor,Vec3 angles,float distance,float shoulder,float height) noexcept;
float Fov(Vec3 view,Vec3 aim) noexcept;
void FixMovement(Command& cmd,float original_yaw) noexcept;
bool Project(const source::Matrix& matrix,Vec3 point,float width,float height,float& x,float& y) noexcept;
}
