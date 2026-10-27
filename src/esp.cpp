#include "esp.h"
#include "player_name.h"
#include "overlay_paint.h"
#include "log.h"
#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace esp {
namespace {
source::Interfaces api{};
std::unordered_map<source::ClientClass*,source::ClassInfo> classes;
std::vector<int> npc_indices;
std::mutex collector_mutex;
SRWLOCK frame_lock = SRWLOCK_INIT;
Frame published{}, working{};
std::atomic_bool enabled{false}, retry{true};
bool resolved{}, faulted{}, previously_enabled{}, previous_game{};
ULONGLONG next_scan{};
LARGE_INTEGER counter_frequency{};

struct SlotCache {
    void* entity{};
    unsigned handle{};
    source::ClientClass* client_class{};
    int peak_health{};
    ULONGLONG next_name{};
    char name[64]{};
};
std::array<SlotCache,8192> slot_cache{};

void Publish() noexcept {
    working.captured_at = GetTickCount64();
    AcquireSRWLockExclusive(&frame_lock);
    published = working;
    ReleaseSRWLockExclusive(&frame_lock);
}
void Status(const char* text) noexcept {
    snprintf(working.status, sizeof(working.status), "%s", text);
}
void Fault(const char* reason) noexcept {
    enabled.store(false, std::memory_order_release);
    faulted = true;
    working.count = 0;
    working.has_matrix = false;
    working.ready = false;
    Status(reason);
    app::Log("ESP detenido: %s", reason);
    Publish();
}
void ResetTracking() noexcept {
    npc_indices.clear();
    next_scan = 0;
    slot_cache.fill({});
}
bool Resolve() {
    ResetTracking();
    faulted = false;
    resolved = source::ResolveClasses(api, classes, working.resolved_classes, working.skipped_classes);
    working.ready = resolved;
    if (!resolved) { Fault("No se pudieron resolver las clases/netvars. Revisa free_esp.log."); return false; }
    app::Log("ESP propio listo: %u clases, %u omitidas", working.resolved_classes, working.skipped_classes);
    Status("Listo. Activa el ESP dentro de una partida.");
    return true;
}
bool ClassOf(void* networkable, source::ClientClass*& result) noexcept {
    return memory::Invoke(memory::Virtual<source::GetClass>(networkable,1,api.client_image), result, networkable);
}
bool RenderOrigin(void* entity, source::Vec3& result) noexcept {
    void* renderable{};
    const source::Vec3* origin{};
    return memory::Invoke(memory::Virtual<source::GetObject>(entity,5,api.client_image),renderable,entity) &&
        renderable && memory::Invoke(memory::Virtual<source::GetOrigin>(renderable,1,api.client_image),origin,renderable) &&
        memory::Read(origin,result) && source::Finite(result);
}
bool RefreshNpcs(int max_clients, ULONGLONG now) {
    if (now < next_scan) return true;
    int highest{}, capacity{};
    if (!memory::Invoke(api.highest_entity,highest,api.entities) ||
        !memory::Invoke(api.entity_capacity,capacity,api.entities) ||
        capacity < max_clients + 1 || capacity > 8192 || highest < 0 || highest >= capacity) {
        Fault("La lista de entidades devolvio limites invalidos."); return false;
    }
    npc_indices.clear();
    for (int i = max_clients + 1; i <= highest; ++i) {
        void* network{};
        source::ClientClass* cc{};
        if (!memory::Invoke(api.networkable,network,api.entities,i)) {
            Fault("Fallo al recorrer entidades del motor."); return false;
        }
        if (!network || !ClassOf(network,cc)) continue;
        const auto found = classes.find(cc);
        if (found == classes.end() || found->second.kind == source::Kind::None) continue;
        if (npc_indices.size() < MaxTargets) npc_indices.push_back(i);
    }
    next_scan = now + 250;
    return true;
}
bool Sample(int index, int max_clients, const source::Vec3& local_origin, bool local_valid,
            ULONGLONG now, Target& result) noexcept {
    void* network{};
    void* entity{};
    if (!memory::Invoke(api.networkable,network,api.entities,index) ||
        !memory::Invoke(api.entity,entity,api.entities,index)) { ++working.read_failures; return false; }
    if (!network || !entity) return false;
    bool dormant{};
    source::ClientClass* cc{};
    if (!memory::Invoke(memory::Virtual<source::GetBool>(network,7,api.client_image),dormant,network) ||
        !ClassOf(network,cc)) { ++working.read_failures; return false; }
    if (dormant) return false;
    const auto found = classes.find(cc);
    if (found == classes.end() || found->second.kind == source::Kind::None) return false;
    const auto& info = found->second;
    const auto& fields = info.fields;
    int team{};
    unsigned char life{};
    int hp = -1;
    if (!memory::Field(entity,fields.team,team)) { ++working.read_failures; return false; }
    if (team != 2 && team != 3) return false;
    if (fields.life >= 0) {
        if (!memory::Field(entity,fields.life,life)) { ++working.read_failures; return false; }
        if (life != 0) return false;
    }
    if (fields.health >= 0) {
        if (!memory::Field(entity,fields.health,hp)) { ++working.read_failures; return false; }
        if (hp <= 0 || hp > 1000000) return false;
    }
    result = {};
    result.kind = info.kind;
    if (info.kind == source::Kind::Player) {
        result.kind = team == 2 ? source::Kind::Survivor : source::Kind::Player;
        if (team == 3 && fields.zombie >= 0) {
            int zombie{};
            if (memory::Field(entity,fields.zombie,zombie)) result.kind = source::ZombieKind(zombie);
        }
    }
    // RenderOrigin is interpolated by the game. No function or entity pointer
    // is retained in the frame consumed by DirectX.
    if (!RenderOrigin(entity,result.origin) && !memory::Field(entity,fields.origin,result.origin)) {
        ++working.read_failures; return false;
    }
    if (!memory::Field(entity,fields.mins,result.mins) || !memory::Field(entity,fields.maxs,result.maxs) ||
        !source::Finite(result.origin) || !source::Finite(result.mins) || !source::Finite(result.maxs)) {
        ++working.read_failures; return false;
    }
    const auto& lo = result.mins;
    const auto& hi = result.maxs;
    if (hi.x <= lo.x || hi.y <= lo.y || hi.z - lo.z < 4.f || hi.z - lo.z > 512.f ||
        hi.x - lo.x > 512.f || hi.y - lo.y > 512.f) return false;
    result.health = hp;
    auto& cache = slot_cache[static_cast<size_t>(index)];
    unsigned handle{};
    using GetHandle = const unsigned*(__thiscall*)(void*);
    const unsigned* handle_address{};
    if (memory::Invoke(memory::Virtual<GetHandle>(entity,2,api.client_image),handle_address,entity))
        memory::Read(handle_address,handle);
    if (cache.entity != entity || cache.client_class != cc || cache.handle != handle) {
        cache = {}; cache.entity = entity; cache.client_class = cc; cache.handle = handle;
    }
    cache.peak_health = std::max(cache.peak_health,hp);
    int max_hp{};
    if (fields.max_health >= 0 && memory::Field(entity,fields.max_health,max_hp) && max_hp > 0 && max_hp <= 1000000)
        result.max_health = max_hp;
    else if (hp > 0)
        result.max_health = std::max(cache.peak_health,result.kind == source::Kind::Survivor ? 100 : hp);
    if (index <= max_clients && now >= cache.next_name) {
        cache.next_name = now + 1000;
        source::PlayerInfo player{};
        bool success{};
        if (memory::Invoke(api.player_info,success,api.engine,index,&player) && success) {
            player_name::Copy(cache.name,player.name);
        } else cache.name[0] = 0;
    }
    snprintf(result.name,sizeof(result.name),"%s",cache.name[0] ? cache.name : source::Label(result.kind));
    if (local_valid) {
        const float dx = result.origin.x - local_origin.x;
        const float dy = result.origin.y - local_origin.y;
        const float dz = result.origin.z - local_origin.z;
        result.meters = std::sqrt(dx*dx + dy*dy + dz*dz) * 0.01905f;
        result.has_distance = std::isfinite(result.meters);
    }
    return true;
}
void Collect() {
    LARGE_INTEGER begin{}, end{};
    QueryPerformanceCounter(&begin);
    working.count = 0;
    working.has_matrix = false;
    working.read_failures = 0;
    working.candidates = 0;
    working.collection_ms = 0.f;
    if (!resolved || faulted) { Publish(); return; }
    bool in_game{};
    if (!memory::Invoke(api.in_game,in_game,api.engine)) { Fault("Fallo al consultar el estado de la partida."); return; }
    working.in_game = in_game;
    const bool active = enabled.load(std::memory_order_acquire);
    if (!in_game || !active) {
        if (previously_enabled || previous_game != in_game) ResetTracking();
        previously_enabled = false; previous_game = in_game;
        Status(in_game ? "Listo. ESP desactivado." : "Esperando una partida.");
        Publish(); return;
    }
    if (!previously_enabled || !previous_game) ResetTracking();
    previously_enabled = true; previous_game = true;
    const ULONGLONG now = GetTickCount64();
    int local_index{}, max_clients{};
    if (!memory::Invoke(api.local_player,local_index,api.engine) ||
        !memory::Invoke(api.max_clients,max_clients,api.engine) || max_clients < 1 || max_clients > 64 ||
        local_index < 0 || local_index > max_clients) { Fault("Indices de jugadores incompatibles."); return; }
    const source::Matrix* matrix{};
    if (!memory::Invoke(api.matrix,matrix,api.engine) || !memory::Read(matrix,working.matrix)) {
        Fault("No se pudo copiar la matriz de la camara."); return;
    }
    bool matrix_valid = true;
    float magnitude{};
    for (const auto& row : working.matrix.m) for (const float value : row) {
        matrix_valid = matrix_valid && std::isfinite(value);
        magnitude += std::abs(value);
    }
    if (!matrix_valid || magnitude < 0.01f) { Status("Esperando una matriz de camara valida."); Publish(); return; }
    working.has_matrix = true;
    if (!RefreshNpcs(max_clients,now)) return;
    source::Vec3 local_origin{};
    void* local{};
    const bool local_valid = local_index > 0 && memory::Invoke(api.entity,local,api.entities,local_index) &&
        local && RenderOrigin(local,local_origin);
    auto sample = [&](int index) {
        if (index == local_index || working.count >= MaxTargets) return;
        ++working.candidates;
        if (Sample(index,max_clients,local_origin,local_valid,now,working.targets[working.count])) ++working.count;
    };
    for (int index = 1; index <= max_clients; ++index) sample(index);
    for (const int index : npc_indices) sample(index);
    QueryPerformanceCounter(&end);
    if (counter_frequency.QuadPart > 0)
        working.collection_ms = static_cast<float>(1000.0 * static_cast<double>(end.QuadPart-begin.QuadPart) /
                                                   static_cast<double>(counter_frequency.QuadPart));
    snprintf(working.status,sizeof(working.status),"ESP activo: %zu objetivos. %u lecturas omitidas.",
             working.count,working.read_failures);
    Publish();
}

struct Clip { float x,y,w; };
Clip Transform(const source::Matrix& matrix, source::Vec3 point) noexcept {
    const auto& m = matrix.m;
    return {m[0][0]*point.x+m[0][1]*point.y+m[0][2]*point.z+m[0][3],
            m[1][0]*point.x+m[1][1]*point.y+m[1][2]*point.z+m[1][3],
            m[3][0]*point.x+m[3][1]*point.y+m[3][2]*point.z+m[3][3]};
}
source::Vec3 Corner(const Target& target,unsigned i) noexcept {
    if(target.bounds_source==Target::BoundsSource::Render)return target.render_corners[i];
    return {target.origin.x+((i&1)?target.maxs.x:target.mins.x),
            target.origin.y+((i&2)?target.maxs.y:target.mins.y),
            target.origin.z+((i&4)?target.maxs.z:target.mins.z)};
}
bool Bounds(const Target& target, const source::Matrix& matrix, ImVec2 screen, ImVec2& min, ImVec2& max) noexcept {
    constexpr float near_w = 0.01f;
    if (screen.x <= 0 || screen.y <= 0 || target.bounds_source==Target::BoundsSource::Missing) return false;
    Clip corners[8]{};
    for (unsigned i = 0; i < 8; ++i) {
        corners[i] = Transform(matrix,Corner(target,i));
        if(!std::isfinite(corners[i].x)||!std::isfinite(corners[i].y)||!std::isfinite(corners[i].w))return false;
    }
    min = {FLT_MAX,FLT_MAX}; max = {-FLT_MAX,-FLT_MAX};
    unsigned points{};
    auto add = [&](const Clip& point) {
        if (point.w < near_w || !std::isfinite(point.w)) return;
        const ImVec2 pixel{screen.x * 0.5f*(1.f+point.x/point.w),screen.y * 0.5f*(1.f-point.y/point.w)};
        if (!std::isfinite(pixel.x) || !std::isfinite(pixel.y)) return;
        min.x = std::min(min.x,pixel.x); min.y = std::min(min.y,pixel.y);
        max.x = std::max(max.x,pixel.x); max.y = std::max(max.y,pixel.y);
        ++points;
    };
    for (const auto& point : corners) add(point);
    // Clip crossing edges instead of dropping the whole box as soon as one
    // corner crosses the near plane. This also removes that source of flicker.
    for (unsigned i = 0; i < 8; ++i) for (unsigned axis = 1; axis <= 4; axis <<= 1) {
        if (i & axis) continue;
        const auto& a = corners[i]; const auto& b = corners[i|axis];
        if ((a.w >= near_w) == (b.w >= near_w)) continue;
        const float t = (near_w-a.w)/(b.w-a.w);
        if (!std::isfinite(t)) continue;
        add({a.x+(b.x-a.x)*t,a.y+(b.y-a.y)*t,near_w});
    }
    if (!points || max.x < 0 || max.y < 0 || min.x > screen.x || min.y > screen.y ||
        max.x-min.x < 2.f || max.y-min.y < 4.f) return false;
    // Bound the submitted geometry; the viewport clip rect handles the rest.
    min.x = std::max(min.x,-screen.x); min.y = std::max(min.y,-screen.y);
    max.x = std::min(max.x,screen.x*2.f); max.y = std::min(max.y,screen.y*2.f);
    return true;
}
void Text(ImDrawList* list, ImVec2 center, ImU32 color, const char* value) {
    ImVec2 size = ImGui::CalcTextSize(value);
    ImVec2 pos{std::round(center.x-size.x*0.5f),std::round(center.y)};
    overlay_paint::Label(list,pos,color,value);
}
void Box(ImDrawList* list, ImVec2 lo, ImVec2 hi, ImU32 color, bool corners, float thickness) {
    overlay_paint::Box(list,lo,hi,color,corners,thickness);
}
}
void Initialize(const source::Interfaces& interfaces) {
    api = interfaces;
    npc_indices.reserve(MaxTargets);
    classes.reserve(512);
    QueryPerformanceFrequency(&counter_frequency);
}
bool ProjectBounds(const Target& target,const source::Matrix& matrix,ImVec2 screen,ImVec2& min,ImVec2& max) noexcept {
    return Bounds(target,matrix,screen,min,max);
}
bool IntersectsView(const Target& target,const source::Matrix& matrix) noexcept {
    // Conservative AABB/frustum rejection before asking the engine for bones.
    if(target.bounds_source==Target::BoundsSource::Missing)return false;
    unsigned outside=31;
    for(unsigned i=0;i<8;++i) {
        const auto p=Transform(matrix,Corner(target,i));
        if(!std::isfinite(p.x) || !std::isfinite(p.y) || !std::isfinite(p.w)) return false;
        unsigned mask=0;
        if(p.w<.01f) mask|=1;
        if(p.x<-p.w) mask|=2;
        if(p.x>p.w) mask|=4;
        if(p.y<-p.w) mask|=8;
        if(p.y>p.w) mask|=16;
        outside&=mask;
    }
    return outside==0;
}
bool ProjectSegment(source::Vec3 start,source::Vec3 end,const source::Matrix& matrix,ImVec2 screen,
                    ImVec2& from,ImVec2& to) noexcept {
    if(!source::Finite(start) || !source::Finite(end) || !std::isfinite(screen.x) ||
       !std::isfinite(screen.y) || screen.x<=0 || screen.y<=0) return false;
    const auto a=Transform(matrix,start),b=Transform(matrix,end);
    if(!std::isfinite(a.x) || !std::isfinite(a.y) || !std::isfinite(a.w) ||
       !std::isfinite(b.x) || !std::isfinite(b.y) || !std::isfinite(b.w)) return false;
    float enter=0.f,leave=1.f;
    const auto clip=[&](float fa,float fb) {
        if(fa<0.f && fb<0.f) return false;
        if(fa<0.f) enter=std::max(enter,fa/(fa-fb));
        else if(fb<0.f) leave=std::min(leave,fa/(fa-fb));
        return enter<=leave;
    };
    // Clip near-plane crossings and viewport edges before perspective division.
    if(!clip(a.w-.01f,b.w-.01f) || !clip(a.w+a.x,b.w+b.x) || !clip(a.w-a.x,b.w-b.x) ||
       !clip(a.w+a.y,b.w+b.y) || !clip(a.w-a.y,b.w-b.y)) return false;
    const auto project=[&](float t,ImVec2& p) {
        const float w=a.w+(b.w-a.w)*t,x=a.x+(b.x-a.x)*t,y=a.y+(b.y-a.y)*t;
        if(!std::isfinite(w) || w<=0.f) return false;
        p={screen.x*.5f*(1.f+x/w),screen.y*.5f*(1.f-y/w)};
        if(!std::isfinite(p.x) || !std::isfinite(p.y)) return false;
        p.x=std::clamp(p.x,0.f,screen.x); p.y=std::clamp(p.y,0.f,screen.y);
        return true;
    };
    return project(enter,from) && project(leave,to);
}
void DrawSkeleton(std::span<const source::BoneSegment> bones,const source::Matrix& matrix,
                  ImDrawList* list,ImVec2 screen,float thickness,ImU32 color) noexcept {
    if(!list || !std::isfinite(thickness)) return;
    struct Line {ImVec2 from,to;};
    std::array<Line,128> lines{};
    size_t count=0;
    for(const auto& bone:bones.first(std::min(bones.size(),lines.size()))) {
        if(ProjectSegment(bone.start,bone.end,matrix,screen,lines[count].from,lines[count].to)) ++count;
    }
    thickness=std::clamp(thickness,.5f,4.f);
    for(size_t i=0;i<count;++i) list->AddLine(lines[i].from,lines[i].to,IM_COL32(0,0,0,200),thickness+1.f);
    for(size_t i=0;i<count;++i) list->AddLine(lines[i].from,lines[i].to,color,thickness);
}
void OnGameFrame(int stage) noexcept {
    if (stage != 0 && stage != 6) return;
    try {
        std::lock_guard<std::mutex> lock(collector_mutex);
        if (stage == 0 && retry.exchange(false,std::memory_order_acq_rel)) { Resolve(); Publish(); }
        if (stage == 6) Collect();
    } catch (...) {
        // Allocation failures are confined to this module, never allowed into
        // the game's FrameStageNotify call stack.
        std::lock_guard<std::mutex> lock(collector_mutex);
        Fault("Error interno al actualizar el ESP. Consulta el registro.");
    }
}
void ClearLevel() noexcept {
    std::lock_guard<std::mutex> lock(collector_mutex);
    ResetTracking();
    previously_enabled = previous_game = false;
    working.count = 0; working.in_game = working.has_matrix = false;
    Status("Cambio de mapa. Esperando una partida."); Publish();
}
void SetEnabled(bool value) noexcept { enabled.store(value,std::memory_order_release); }
bool Enabled() noexcept { return enabled.load(std::memory_order_acquire); }
void Retry() noexcept { retry.store(true,std::memory_order_release); }
void CopyFrame(Frame& result) noexcept {
    AcquireSRWLockShared(&frame_lock); result = published; ReleaseSRWLockShared(&frame_lock);
}
unsigned Draw(const Frame& frame, const Options& options, ImDrawList* list, ImVec2 screen) {
    if (!options.enabled || !list || !frame.ready || !frame.in_game || !frame.has_matrix ||
        GetTickCount64() - frame.captured_at > 250) return 0;
    unsigned visible{};
    list->PushClipRect({0,0},screen,true);
    for (size_t i = 0; i < frame.count && i < frame.targets.size(); ++i) {
        const auto& target = frame.targets[i];
        const bool survivor = target.kind == source::Kind::Survivor;
        const bool witch = target.kind == source::Kind::Witch;
        if ((survivor && !options.survivors) || (witch && !options.witch) ||
            (!survivor && !witch && !options.infected) ||
            (target.has_distance && target.meters > options.max_distance)) continue;
        ImVec2 lo{},hi{};
        if (!Bounds(target,frame.matrix,screen,lo,hi)) continue;
        ++visible;
        const ImVec4 tint = survivor ? options.survivor_color : witch ? options.witch_color :
            target.kind == source::Kind::Tank ? options.tank_color : options.infected_color;
        const ImU32 color = ImGui::ColorConvertFloat4ToU32(tint);
        if (options.boxes) Box(list,lo,hi,color,options.corner_boxes,options.thickness);
        if (options.health && target.health > 0 && target.max_health > 0) {
            const float fraction = std::clamp(static_cast<float>(target.health)/target.max_health,0.f,1.f);
            const float top = hi.y-(hi.y-lo.y)*fraction;
            overlay_paint::HealthTrack(list,{lo.x-7.f,lo.y-1.f,5.f,hi.y-lo.y+2.f});
            list->AddRectFilled({lo.x-6.f,top},{lo.x-3.f,hi.y},
                IM_COL32(static_cast<int>(255.f*(1.f-fraction)),static_cast<int>(230.f*fraction),65,255));
        }
        const float center = (lo.x+hi.x)*0.5f;
        if (options.names) Text(list,{center,lo.y-ImGui::GetFontSize()-3.f},color,target.name);
        char extra[80]{};
        if (options.distance && target.has_distance && options.health && target.health > 0)
            snprintf(extra,sizeof(extra),"%.0f m | %d HP",target.meters,target.health);
        else if (options.distance && target.has_distance) snprintf(extra,sizeof(extra),"%.0f m",target.meters);
        else if (options.health && target.health > 0) snprintf(extra,sizeof(extra),"%d HP",target.health);
        if (extra[0]) Text(list,{center,hi.y+3.f},IM_COL32(240,240,244,255),extra);
    }
    list->PopClipRect();
    return visible;
}
}
