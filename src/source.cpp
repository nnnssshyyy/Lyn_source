#include "source.h"
#include "log.h"
#include <cmath>
#include <cstring>
#include <unordered_set>

namespace source {
namespace {
using Factory = void*(__cdecl*)(const char*, int*);
void* Interface(const wchar_t* module_name, const char* version) noexcept {
    HMODULE module = GetModuleHandleW(module_name);
    if (!module) return nullptr;
    auto factory = reinterpret_cast<Factory>(GetProcAddress(module, "CreateInterface"));
    void* result{};
    return memory::Invoke(factory, result, version, static_cast<int*>(nullptr)) ? result : nullptr;
}
int Walk(memory::Range image, RecvTable* table, const char* name, int base,
         std::array<RecvTable*, 32>& ancestors, unsigned depth, unsigned& budget, Property* binding) noexcept {
    if (depth >= ancestors.size() || !budget || !image.Contains(table, sizeof(RecvTable))) return -1;
    for (unsigned i = 0; i < depth; ++i) if (ancestors[i] == table) return -1;
    ancestors[depth] = table;
    RecvTable data{};
    if (!memory::Read(table, data) || data.count <= 0 || data.count > 4096 ||
        !image.Contains(data.props, sizeof(RecvProp) * static_cast<size_t>(data.count))) return -1;
    for (int i = 0; i < data.count && budget; ++i) {
        --budget;
        RecvProp prop{};
        char prop_name[128]{};
        if (!memory::Read(data.props + i, prop) || prop.inside_array ||
            prop.offset < 0 || prop.offset > 0x40000 - base) continue;
        if (memory::String(image, prop.name, prop_name, sizeof(prop_name)) && std::strcmp(prop_name, name) == 0) {
            if(binding) *binding={data.props+i,base,base+prop.offset};
            return base + prop.offset;
        }
        if (prop.type == 6 && prop.table) {
            const int found = Walk(image, prop.table, name, base + prop.offset, ancestors, depth + 1, budget,binding);
            if (found >= 0) return found;
        }
    }
    return -1;
}
Kind ClassKind(const char* name) noexcept {
    if (!std::strcmp(name, "CTerrorPlayer") || !std::strcmp(name, "SurvivorBot") || !std::strcmp(name, "CSurvivorBot"))
        return Kind::Player;
    struct Mapping { const char* name; Kind kind; };
    constexpr Mapping types[]{
        {"Smoker",Kind::Smoker},{"Boomer",Kind::Boomer},{"Hunter",Kind::Hunter},
        {"Spitter",Kind::Spitter},{"Jockey",Kind::Jockey},{"Charger",Kind::Charger},
        {"Witch",Kind::Witch},{"Tank",Kind::Tank}};
    for (const auto& type : types)
        if (!std::strcmp(name, type.name) || (name[0] == 'C' && !std::strcmp(name + 1, type.name))) return type.kind;
    return Kind::None;
}
}
bool Bind(Interfaces& out) {
    out = {};
    out.client = Interface(L"client.dll", "VClient016");
    out.engine = Interface(L"engine.dll", "VEngineClient013");
    out.entities = Interface(L"client.dll", "VClientEntityList003");
    out.client_image = memory::Module(GetModuleHandleW(L"client.dll"));
    if (!out.client || !out.engine || !out.entities || !out.client_image.base) {
        app::Log("Interfaces requeridas no disponibles: VClient016 / VEngineClient013 / VClientEntityList003");
        return false;
    }
    out.classes = memory::Virtual<GetClasses>(out.client, 7);
    out.frame_stage = memory::Virtual<FrameStage>(out.client, 34);
    out.local_player = memory::Virtual<GetInt>(out.engine, 12);
    out.max_clients = memory::Virtual<GetInt>(out.engine, 21);
    out.in_game = memory::Virtual<GetBool>(out.engine, 26);
    out.matrix = memory::Virtual<GetMatrix>(out.engine, 37);
    out.player_info = memory::Virtual<GetInfo>(out.engine, 8);
    out.entity = memory::Virtual<GetEntity>(out.entities, 3);
    out.networkable = memory::Virtual<GetEntity>(out.entities, 0);
    out.highest_entity = memory::Virtual<GetInt>(out.entities, 6);
    out.entity_capacity = memory::Virtual<GetInt>(out.entities, 8);
    out.surface = Interface(L"vguimatsurface.dll", "VGUI_Surface031");
    if (out.surface) {
        // Confirmed against this branch's IVGuiSurface/IAppSystem declaration.
        out.unlock_cursor = memory::Virtual<Cursor>(out.surface, 58);
        out.lock_cursor = memory::Virtual<Cursor>(out.surface, 59);
    }
    const bool ok = out.classes && out.frame_stage && out.local_player && out.max_clients && out.in_game &&
        out.matrix && out.player_info && out.entity && out.networkable && out.highest_entity && out.entity_capacity;
    app::Log("Interfaces %s; cursor %s", ok ? "resueltas" : "incompatibles",
             out.unlock_cursor && out.lock_cursor ? "resuelto" : "no disponible");
    return ok;
}
int FindField(memory::Range image, RecvTable* table, const char* name) noexcept {
    std::array<RecvTable*,32> ancestors{};
    unsigned budget = 65536;
    return Walk(image, table, name, 0, ancestors, 0, budget,nullptr);
}
Property FindProperty(memory::Range image,RecvTable* table,const char* name) noexcept {
    Property field{}; std::array<RecvTable*,32> ancestors{}; unsigned budget=65536;
    Walk(image,table,name,0,ancestors,0,budget,&field); return field;
}
bool SetInteger(memory::Range image,void* object,Property field,int value) noexcept {
    RecvProp prop{};
    const auto base=reinterpret_cast<uintptr_t>(object);
    if(!base || field.container<0 || field.offset<field.container || field.offset>0x40000 ||
       base>UINTPTR_MAX-static_cast<unsigned>(field.offset)-sizeof(int) ||
       !image.Contains(field.prop,sizeof(prop)) || !memory::Read(field.prop,prop) || prop.type!=0 ||
       prop.offset!=field.offset-field.container || !image.Contains(prop.proxy) || !memory::Executable(prop.proxy)) return false;
    RecvProxyData data{};data.prop=field.prop;data.value.integer=value;
    using Proxy=void(__cdecl*)(const RecvProxyData*,void*,void*);
    if(!memory::InvokeVoid(reinterpret_cast<Proxy>(prop.proxy),&data,
        reinterpret_cast<void*>(base+field.container),reinterpret_cast<void*>(base+field.offset))) return false;
    int actual{};
    return memory::Field(object,field.offset,actual) && actual==value;
}
bool ResolveClasses(const Interfaces& api, std::unordered_map<ClientClass*, ClassInfo>& out,
                    unsigned& targets, unsigned& skipped) {
    out.clear(); targets = skipped = 0;
    ClientClass* current{};
    if (!memory::Invoke(api.classes, current, api.client) || !current) return false;
    std::unordered_set<ClientClass*> visited;
    while (current && visited.size() < 4096) {
        if (!visited.insert(current).second) return false;
        ClientClass cc{};
        char name[128]{};
        if (!api.client_image.Contains(current, sizeof(cc)) || !memory::Read(current, cc) ||
            !memory::String(api.client_image, cc.name, name, sizeof(name))) return false;
        ClassInfo info{};
        info.kind = ClassKind(name);
        if (info.kind != Kind::None) {
            auto find = [&](const char* field) { return FindField(api.client_image, cc.table, field); };
            info.fields = {find("m_vecOrigin"),find("m_vecMins"),find("m_vecMaxs"),find("m_iTeamNum"),
                find("m_iHealth"),find("m_lifeState"),find("m_zombieClass"),find("m_iMaxHealth")};
            const auto& f = info.fields;
            bool valid = f.origin >= 0 && f.mins >= 0 && f.maxs >= 0 && f.team >= 0;
            if (info.kind != Kind::Witch) valid = valid && f.health >= 0 && f.life >= 0;
            app::Log("%s: origin=%X mins=%X maxs=%X team=%X health=%X life=%X zombie=%X maxhp=%X (%s)",
                name, f.origin, f.mins, f.maxs, f.team, f.health, f.life, f.zombie, f.max_health,
                valid ? "lista" : "campos ausentes");
            if (valid) ++targets;
            else { ++skipped; info.kind = Kind::None; }
        }
        out.emplace(current, info);
        current = cc.next;
    }
    return !current && targets > 0;
}
Kind ZombieKind(int value) noexcept {
    switch (value) {
        case 1: return Kind::Smoker; case 2: return Kind::Boomer; case 3: return Kind::Hunter;
        case 4: return Kind::Spitter; case 5: return Kind::Jockey; case 6: return Kind::Charger;
        case 7: return Kind::Witch; case 8: return Kind::Tank; default: return Kind::Player;
    }
}
const char* Label(Kind kind) noexcept {
    switch (kind) {
        case Kind::Survivor: return "Superviviente"; case Kind::Smoker: return "Smoker";
        case Kind::Boomer: return "Boomer"; case Kind::Hunter: return "Hunter"; case Kind::Spitter: return "Spitter";
        case Kind::Jockey: return "Jockey"; case Kind::Charger: return "Charger"; case Kind::Witch: return "Witch";
        case Kind::Tank: return "Tank"; case Kind::Common: return "Infectado comun"; default: return "Infectado";
    }
}
bool Finite(Vec3 value) noexcept {
    return std::isfinite(value.x) && std::isfinite(value.y) && std::isfinite(value.z) &&
        std::abs(value.x) < 1.e7f && std::abs(value.y) < 1.e7f && std::abs(value.z) < 1.e7f;
}
}
