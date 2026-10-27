#include "game.h"
#include "log.h"
#include "weapon_catalog.h"
#include "shot_clock.h"
#include "storm_fall.h"
#include "world_bounds.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unordered_set>

namespace game {
constexpr float Pi=3.14159265358979323846f;
void* Interface(const wchar_t* module,const char* version) noexcept {
    auto handle=GetModuleHandleW(module);
    using Factory=void*(__cdecl*)(const char*,int*);
    auto factory=handle ? reinterpret_cast<Factory>(GetProcAddress(handle,"CreateInterface")) : nullptr;
    void* result{};
    return memory::Invoke(factory,result,version,static_cast<int*>(nullptr)) ? result : nullptr;
}
void* Signature(const wchar_t* module,const char* pattern) {
    const auto image=memory::Module(GetModuleHandleW(module));
    if(!image.base) return nullptr;
    std::vector<int> bytes;
    for(auto p=pattern; *p;) {
        if(*p==' ') { ++p; continue; }
        if(*p=='?') { bytes.push_back(-1); while(*p=='?') ++p; }
        else { char* end{}; auto value=strtoul(p,&end,16); if(end==p || value>255) return nullptr; bytes.push_back(static_cast<int>(value)); p=end; }
    }
    if(bytes.empty()) return nullptr;
    IMAGE_DOS_HEADER dos{}; IMAGE_NT_HEADERS32 nt{};
    if(!memory::Read(reinterpret_cast<void*>(image.base),dos) || dos.e_lfanew<0 ||
       !image.Contains(reinterpret_cast<void*>(image.base+dos.e_lfanew),sizeof(nt)) ||
       !memory::Read(reinterpret_cast<void*>(image.base+dos.e_lfanew),nt) || nt.FileHeader.NumberOfSections>96) return nullptr;
    const uintptr_t sections=image.base+dos.e_lfanew+offsetof(IMAGE_NT_HEADERS32,OptionalHeader)+nt.FileHeader.SizeOfOptionalHeader;
    uintptr_t match{};
    for(unsigned s=0;s<nt.FileHeader.NumberOfSections;++s) {
        IMAGE_SECTION_HEADER section{};
        if(!image.Contains(reinterpret_cast<void*>(sections+s*sizeof(section)),sizeof(section)) ||
           !memory::Read(reinterpret_cast<void*>(sections+s*sizeof(section)),section)) return nullptr;
        if(!(section.Characteristics & IMAGE_SCN_MEM_EXECUTE)) continue;
        const uintptr_t start=image.base+section.VirtualAddress;
        const size_t size=section.Misc.VirtualSize;
        if(!image.Contains(reinterpret_cast<void*>(start),size) || size<bytes.size()) continue;
        std::vector<unsigned char> code(size);
        if(!memory::Copy(code.data(),reinterpret_cast<void*>(start),size)) return nullptr;
        for(size_t i=0;i<=size-bytes.size();++i) {
            bool same=true;
            for(size_t j=0;j<bytes.size();++j) if(bytes[j]>=0 && code[i+j]!=bytes[j]) { same=false; break; }
            if(same) { if(match) return nullptr; match=start+i; }
        }
    }
    return reinterpret_cast<void*>(match);
}
static void* GlobalPointer(const char* pattern,int offset) {
    const auto found=reinterpret_cast<uintptr_t>(Signature(L"client.dll",pattern));
    uintptr_t address{}; void* result{};
    if(found && memory::Read(reinterpret_cast<void*>(found+offset),address) && memory::Read(reinterpret_cast<void*>(address),result)) return result;
    return nullptr;
}
void* RelativeCall(void* instruction) noexcept {
    unsigned char opcode{}; int displacement{};
    const auto address=reinterpret_cast<uintptr_t>(instruction);
    if(!memory::Read(instruction,opcode) || opcode!=0xE8 ||
       !memory::Read(reinterpret_cast<void*>(address+1),displacement)) return nullptr;
    const auto destination=static_cast<int64_t>(address)+5+displacement;
    return destination>0 && destination<=UINTPTR_MAX && memory::Executable(reinterpret_cast<void*>(destination))
        ? reinterpret_cast<void*>(destination) : nullptr;
}
static void* NetChannel(const Api& api) noexcept {
    using State=void*(__cdecl*)();
    void* state{};void* channel{};
    if(!api.client_state_getter ||
       !memory::Invoke(reinterpret_cast<State>(api.client_state_getter),state) || !state ||
       !memory::Field(state,0x10,channel) || !channel) return nullptr;
    return channel;
}
bool Api::Bind(const source::Interfaces& interfaces) {
    base=interfaces;
    mode=GlobalPointer("89 04 B5 ? ? ? ? E8",3);
    trace=Interface(L"engine.dll","EngineTraceClient003");
    model_info=Interface(L"engine.dll","VModelInfoClient004");
    prediction=Interface(L"client.dll","VClientPrediction001");
    cvars=Interface(L"vstdlib.dll","VEngineCvar007");
    const auto move=static_cast<unsigned char*>(Signature(L"engine.dll","55 8B EC 81 EC ? ? ? ? A1 ? ? ? ? 33 C5 89 45 ? 56 E8 ? ? ? ? 8B F0 83 7E"));
    if(move) client_state_getter=RelativeCall(move+20);
    run_prediction=Signature(L"engine.dll","55 8B EC 83 EC 18 A1 ? ? ? ? 33 C9 89 4D F8 89 4D FC 8B 00 3B C1 74 ? 8B 50 50 68 ? ? ? ? 68 ? ? ? ? 68 ? ? ? ? 6A 2B");
    engine_image=memory::Module(GetModuleHandleW(L"engine.dll"));
    const char* prediction_label{};char prediction_name[48]{};
    if(!run_prediction || !memory::Read(static_cast<unsigned char*>(run_prediction)+29,prediction_label) ||
       !memory::String(engine_image,prediction_label,prediction_name,sizeof(prediction_name)) ||
       std::strcmp(prediction_name,"CL_RunPrediction")) run_prediction=nullptr;
    const auto queue=static_cast<unsigned char*>(Signature(L"engine.dll","8B 8E ? ? ? ? 8B 96 ? ? ? ? 8D 44 11 01 8B 0D ? ? ? ? 8B 11"));
    const auto prediction_queue=static_cast<unsigned char*>(Signature(L"engine.dll","8B 8F ? ? ? ? 03 8F ? ? ? ? 8B 87 CC 00 00 00 8B 15 ? ? ? ? 8B 12 8B 52 0C 51 8B 8F ? ? ? ?"));
    int predicted_choked{},predicted_outgoing{},ack{},delta{};
    if(move && queue && queue>=move && queue-move<0x600 &&
       memory::Read(queue+2,outgoing_command) && memory::Read(queue+8,choked_commands) &&
       prediction_queue && run_prediction && prediction_queue>=static_cast<unsigned char*>(run_prediction) &&
       prediction_queue-static_cast<unsigned char*>(run_prediction)<0x180 &&
       memory::Read(prediction_queue+2,predicted_choked) && memory::Read(prediction_queue+8,predicted_outgoing) &&
       memory::Read(prediction_queue+14,delta) && memory::Read(prediction_queue+32,ack) &&
       delta>=0x20 && delta<outgoing_command && outgoing_command>=0x100 && outgoing_command<0x10000 &&
       choked_commands==outgoing_command+4 && predicted_choked==choked_commands && predicted_outgoing==outgoing_command &&
       ack==outgoing_command+8) {acknowledged_command=ack;delta_tick=delta;}
    else outgoing_command=choked_commands=acknowledged_command=delta_tick=-1;
    app::Log("Command transport: prediction=%p outgoing=%X choked=%X ack=%X delta=%X",
        run_prediction,outgoing_command,choked_commands,acknowledged_command,delta_tick);
    sequence_name=RelativeCall(Signature(L"client.dll","E8 ? ? ? ? 89 45 EC 8B 45 D0"));
    auto spread_code=static_cast<unsigned char*>(Signature(L"client.dll","F3 0F 10 86 ? ? ? ? F3 0F 11 45 ? 0F 57 05 ? ? ? ? D9 45 ? 6A 00 83 EC 08"));
    if(spread_code) memory::Read(spread_code+4,spread_offset);
    update_spread=Signature(L"client.dll","53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 83 EC 28 56 57 8B F9 E8");
    // Require the native updater's first spread store to use exactly the
    // field read by FireBullet; no guessed weapon offset is writable here.
    unsigned char spread_store[2]{};int updated_offset{-1};
    if(!update_spread || !memory::Copy(spread_store,static_cast<unsigned char*>(update_spread)+0x3E,2) ||
       spread_store[0]!=0xD9 || spread_store[1]!=0x9F ||
       !memory::Read(static_cast<unsigned char*>(update_spread)+0x40,updated_offset) || updated_offset!=spread_offset)
        update_spread=nullptr;
    auto random_call=static_cast<unsigned char*>(Signature(L"client.dll","D9 45 ? D9 1C 24 68 ? ? ? ? E8 ? ? ? ? D9 5D ? D9 45 ? 83 C4 10 6A 00"));
    if(random_call) shared_random=RelativeCall(random_call+11);
    // The operand of SharedRandomFloat's A1 instruction is the seed variable,
    // not a pointer to another pointer. Require the L4D2 prologue first.
    unsigned char random_prefix[7]{};
    if(shared_random && memory::Copy(random_prefix,shared_random,sizeof(random_prefix)) &&
       random_prefix[0]==0x55 && random_prefix[1]==0x8B && random_prefix[2]==0xEC && random_prefix[6]==0xA1)
        memory::Read(static_cast<unsigned char*>(shared_random)+7,prediction_seed);
    if(spread_offset<0 || spread_offset>0x40000 || !base.client_image.Contains(prediction_seed,sizeof(int))) {
        spread_offset=-1; prediction_seed=nullptr;
    }
    globals=static_cast<Globals*>(GlobalPointer("A1 ? ? ? ? D9 40 0C 51 D9 1C 24 57",1));
    if(!globals) globals=static_cast<Globals*>(GlobalPointer("0F 8F ? ? ? ? 8B 0D ? ? ? ? F3 0F 10 41 ?",8));
    app::Log("L4D2 runtime: ClientMode=%p trace=%p model=%p globals=%p",mode,trace,model_info,globals);
    app::Log("L4D2 aim: sequence=%p SharedRandomFloat=%p seed=%p spread=%X",sequence_name,shared_random,prediction_seed,spread_offset);
    app::Log("Shotgun client spread: UpdateSpread=%p",update_spread);
    return Resolve();
}
bool Api::Resolve() {
    classes.clear();
    std::unordered_map<source::ClientClass*,source::ClassInfo> known;
    unsigned count{},skip{};
    source::ResolveClasses(base,known,count,skip);
    source::ClientClass* current{};
    if(!memory::Invoke(base.classes,current,base.client)) return false;
    std::unordered_set<source::ClientClass*> visited;
    while(current && visited.size()<4096) {
        if(!visited.insert(current).second) return false;
        source::ClientClass cc{};
        char name[128]{};
        if(!base.client_image.Contains(current,sizeof(cc)) || !memory::Read(current,cc) ||
           !memory::String(base.client_image,cc.name,name,sizeof(name))) return false;
        Fields f{};
        const auto it=known.find(current);
        if(it!=known.end()) f.kind=it->second.kind;
        if(!strcmp(name,"Infected") || !strcmp(name,"CInfected")) f.kind=source::Kind::Common;
        snprintf(f.name,sizeof(f.name),"%.63s",name);
        const auto is=[&](const char* value){return !strcmp(name,value);};
        if(const int weapon=weapons::Find(name); weapon>=0) {
            f.weapon_group=weapons::Catalog[weapon].group;
            f.semi=weapons::Catalog[weapon].semi;
        }
        f.melee=is("CTerrorMeleeWeapon");f.primary=f.weapon_group>=1 || is("CGrenadeLauncher");
        if(f.weapon_group>=0 || is("CWeaponSpawn") || is("CGrenadeLauncher") || is("CTerrorMeleeWeapon") || is("CChainsaw")) f.object=Object::Weapon;
        if(is("CFirstAidKit") || is("CPainPills") || is("CItem_Adrenaline") || is("CItemDefibrillator")) f.object=Object::Health;
        if(is("CWeaponAmmoSpawn") || is("CItemUpgradePackExplosive") || is("CItemUpgradePackIncendiary")) f.object=Object::Ammo;
        if(is("CPipeBomb") || is("CMolotov") || is("CItem_VomitJar")) f.object=Object::Throwable;
        if(is("CPipeBombProjectile") || is("CMolotovProjectile") || is("CVomitJarProjectile") || is("CSpitterProjectile") || is("CGrenadeLauncher_Projectile") || is("CInferno")) f.object=Object::Projectile;
        if(is("CGasCan") || is("CColaBottles") || is("CGnome") || is("CPropaneTank") || is("COxygenTank") || is("CFireworkCrate")) f.object=Object::Objective;
        f.model_candidate=is("CDynamicProp") || is("CPhysicsProp") || is("CPhysicsPropMultiplayer") || is("CWeaponSpawn");
        // Resolve only relevant classes; no netvar tree walks per frame.
        if(f.kind!=source::Kind::None || f.object!=Object::None || f.model_candidate || is("CBaseViewModel")) {
            const auto field=[&](const char* n){return source::FindField(base.client_image,cc.table,n);};
#define FIND(member,netvar) f.member=field(netvar)
            FIND(origin,"m_vecOrigin"); FIND(mins,"m_vecMins"); FIND(maxs,"m_vecMaxs"); FIND(team,"m_iTeamNum");
            FIND(health,"m_iHealth"); FIND(life,"m_lifeState"); FIND(zombie,"m_zombieClass"); FIND(view,"m_vecViewOffset[0]");
            FIND(max_health,"m_iMaxHealth");
            FIND(punch,"m_vecPunchAngle"); FIND(active_weapon,"m_hActiveWeapon"); FIND(next_attack,"m_flNextAttack");
            FIND(next_primary,"m_flNextPrimaryAttack"); FIND(next_secondary,"m_flNextSecondaryAttack"); FIND(next_shove,"m_flNextShoveTime");
            FIND(clip,"m_iClip1"); FIND(reload,"m_bInReload"); FIND(tickbase,"m_nTickBase"); FIND(flags,"m_fFlags");
            FIND(velocity,"m_vecVelocity[0]"); FIND(water,"m_nWaterLevel");
            FIND(fall_velocity,"m_flFallVelocity"); FIND(ducked,"m_bDucked");
            FIND(max_speed,"m_flMaxspeed");
            FIND(ghost,"m_isGhost"); FIND(incap,"m_isIncapacitated"); FIND(hitbox_set,"m_nHitboxSet"); FIND(scale,"m_flModelScale");
            FIND(health_buffer,"m_healthBuffer"); FIND(health_buffer_time,"m_healthBufferTime");
            FIND(owner,"m_hOwnerEntity"); FIND(solid_flags,"m_usSolidFlags"); FIND(solid_type,"m_nSolidType");
            FIND(burning,"m_bIsBurning"); FIND(rage,"m_rage"); FIND(simulation,"m_flSimulationTime");
            FIND(sequence,"m_nSequence");
            FIND(client_ragdoll,"m_bClientSideRagdoll");
            FIND(vomit_start,"m_vomitStart");FIND(vomit_fade,"m_vomitFadeStart");
            FIND(dual_weapons,"m_hasDualWeapons");
            FIND(melee_swing,"m_bInMeleeSwing");
            FIND(carry_victim,"m_carryVictim");
            const auto inventory=source::FindProperty(base.client_image,cc.table,"m_hMyWeapons");
            source::RecvProp weapons_prop{};
            if(inventory.offset>0 && base.client_image.Contains(inventory.prop,sizeof(weapons_prop)) && memory::Read(inventory.prop,weapons_prop)) {
                if(weapons_prop.type==5 && weapons_prop.stride==4 && weapons_prop.elements>0 && weapons_prop.elements<=64) {
                    f.inventory=inventory.offset;f.inventory_count=weapons_prop.elements;
                } else if(weapons_prop.type==6) {
                    source::RecvTable table{};
                    if(base.client_image.Contains(weapons_prop.table,sizeof(table)) && memory::Read(weapons_prop.table,table) &&
                       table.count>0 && table.count<=64 && base.client_image.Contains(table.props,sizeof(source::RecvProp)*table.count)) {
                        bool valid=true;
                        for(int slot=0;slot<table.count;++slot) {
                            source::RecvProp prop{};
                            if(!memory::Read(table.props+slot,prop) || prop.type!=0 || prop.offset!=slot*4) {valid=false;break;}
                        }
                        if(valid) {f.inventory=inventory.offset;f.inventory_count=table.count;}
                    }
                }
            }
            FIND(glow_type,"m_iGlowType"); FIND(glow_color,"m_glowColorOverride"); FIND(glow_range,"m_nGlowRange");
            FIND(glow_min,"m_nGlowRangeMin"); FIND(render_mode,"m_nRenderMode");
            // "movetype" has offset 0 and a custom receive proxy. It is not
            // storage at entity+0 (the vtable). Resolve m_MoveType via datamap.
            FIND(move_type,"m_MoveType");
            if(f.move_type<=0) f.move_type=-1;
            f.glow_type_property=source::FindProperty(base.client_image,cc.table,"m_iGlowType");
            f.glow_color_property=source::FindProperty(base.client_image,cc.table,"m_glowColorOverride");
            const char* pins[]{"m_tongueOwner","m_pummelAttacker","m_carryAttacker","m_pounceAttacker","m_jockeyAttacker"};
            for(int i=0;i<5;++i) f.pinned[i]=field(pins[i]);
#undef FIND
            classes.emplace(current,f);
        }
        current=cc.next;
    }
    return !current && !classes.empty();
}
bool Api::InGame() const noexcept { bool result{}; return memory::Invoke(base.in_game,result,base.engine) && result; }
void Api::ResolveMovement(const Entity& local) {
    auto it=classes.find(local.cc);
    if(it==classes.end() || it->second.data_resolved) return;
    auto& fields=it->second; fields.data_resolved=true;
    if(fields.move_type>0 && fields.surface_friction>0) return;
    // Data maps cover predicted fields absent from RecvTables. The SDK 2013
    // descriptor is 52 bytes; the installed L4D2 client has 60-byte descriptors.
    // Accept a layout only if every descriptor in the table validates.
    // Reference: ValveSoftware/source-sdk-2013/src/public/datamap.h
    struct Map { const unsigned char* descriptions; int count; const char* name; Map* base; };
    // C_BaseEntity vtable on the installed client.dll (RTTI-verified): slot 16
    // and slot 18 are "mov eax, imm32; ret" descriptor-map getters; 17 returns
    // null and 19 is unrelated. The block without a name-match at stride 60
    // holds m_MoveType at entity+0x144 (C_BaseEntity datamap tail).
    for(unsigned slot : {16u,18u}) {
        Map* current{};
        using GetMap=Map*(__thiscall*)(void*);
        if(!memory::Invoke(memory::Virtual<GetMap>(local.ptr,slot,base.client_image),current,local.ptr)) continue;
        std::array<Map*,32> seen{}; size_t depth{};
        while(current && depth<seen.size()) {
            if(std::find(seen.begin(),seen.begin()+depth,current)!=seen.begin()+depth) break;
            seen[depth++]=current;
            Map map{}; char class_name[128]{};
            if(!base.client_image.Contains(current,sizeof(map)) || !memory::Read(current,map) || map.count<0 || map.count>2048 ||
               !memory::String(base.client_image,map.name,class_name,sizeof(class_name))) break;
            for(size_t stride : {60u,52u}) {
                if(!base.client_image.Contains(map.descriptions,stride*map.count)) continue;
                bool valid=true; int found=-1,friction=-1;
                for(int i=0;i<map.count;++i) {
                    struct Desc { int type; const char* name; int offset; } desc{};
                    char name[128]{};
                    if(!memory::Read(map.descriptions+i*stride,desc) || desc.type<0 || desc.type>32 ||
                       desc.offset<0 || desc.offset>0x40000) {valid=false;break;}
                    if(!desc.name && desc.type==0) continue;
                    if(!memory::String(base.client_image,desc.name,name,sizeof(name))) {valid=false;break;}
                    if(!strcmp(name,"m_MoveType") && desc.type==8 && desc.offset>0) found=desc.offset;
                    if(!strcmp(name,"m_surfaceFriction") && desc.type==1 && desc.offset>0) friction=desc.offset;
                }
                if(valid) {
                    if(found>=0) {
                        fields.move_type=found;
                        app::Log("L4D2: m_MoveType resuelto por datamap: %X",found);
                    }
                    if(friction>=0) fields.surface_friction=friction;
                    if(fields.move_type>0 && fields.surface_friction>0) return;
                }
            }
            current=map.base;
        }
    }
    app::Log("L4D2: m_MoveType no disponible; asistencia de movimiento inactiva");
}
bool Api::Clock(Globals& out) const noexcept {
    return memory::Read(globals,out) && std::isfinite(out.time) && out.time>=0.f &&
        std::isfinite(out.interval) && out.interval>=0.001f && out.interval<=0.1f && out.max_clients>0 && out.max_clients<=64;
}
void* Api::Handle(unsigned value) const noexcept {
    if(!value || value==UINT_MAX) return nullptr;
    using Get=void*(__thiscall*)(void*,unsigned);
    void* out{};
    return memory::Invoke(memory::Virtual<Get>(base.entities,4),out,base.entities,value) ? out : nullptr;
}
bool Api::Read(int index,Entity& out,bool alive,bool allow_ghost) const noexcept {
    void* ptr{};
    if(index<1 || index>=8192 || !memory::Invoke(base.entity,ptr,base.entities,index)) return false;
    if(!ReadPointer(ptr,out,alive,allow_ghost)) return false;
    out.index=index; return true;
}
bool Api::ReadPointer(void* ptr,Entity& out,bool alive,bool allow_ghost,bool inventory) const noexcept {
    out={}; if(!ptr) return false;
    void* network{}; source::ClientClass* cc{}; bool dormant{};
    if(!memory::Invoke(memory::Virtual<source::GetObject>(ptr,4,base.client_image),network,ptr) || !network ||
       !memory::Invoke(memory::Virtual<source::GetClass>(network,1,base.client_image),cc,network) ||
       !memory::Invoke(memory::Virtual<source::GetBool>(network,7,base.client_image),dormant,network) || (dormant && !inventory)) return false;
    const auto found=classes.find(cc); if(found==classes.end()) return false;
    out.ptr=ptr; out.cc=cc; out.fields=&found->second;
    memory::Invoke(memory::Virtual<source::GetInt>(network,8,base.client_image),out.index,network);
    const auto& f=*out.fields;
    out.kind=f.kind;
    using Ref=const unsigned*(__thiscall*)(void*);
    const unsigned* ref{};
    if(!memory::Invoke(memory::Virtual<Ref>(ptr,2,base.client_image),ref,ptr) || !memory::Read(ref,out.handle)) return false;
    memory::Field(ptr,f.team,out.team);
    if(inventory) {
        // A carried item can be dormant and have no usable world render pose.
        // Only inventory-class metadata is accepted through this path; combat
        // targets retain all dormancy, health and geometry checks below.
        if(f.kind!=source::Kind::None || f.object==Object::None) {out={};return false;}
        out.object=f.object;std::snprintf(out.label,sizeof(out.label),"%s",f.name);
        return true;
    }
    if(f.kind==source::Kind::Player) {
        int zombie{}; memory::Field(ptr,f.zombie,zombie);
        out.kind=out.team==2 ? source::Kind::Survivor : source::ZombieKind(zombie);
    }
    if(f.ghost>=0 && !memory::Field(ptr,f.ghost,out.ghost) && alive) return false;
    if(alive && f.kind!=source::Kind::None) {
        unsigned char life{};
        if(f.life>=0 && (!memory::Field(ptr,f.life,life) || life!=0)) return false;
        // Visuals may include replicated ghosts; combat callers keep the
        // default rejection. Dormancy, life and health checks still apply.
        if(out.ghost && !allow_ghost) return false;
        if(f.health>=0) { if(!memory::Field(ptr,f.health,out.health)) return false; }
        else if(f.kind!=source::Kind::Common && f.kind!=source::Kind::Witch) return false;
        // Common infected/Witch can lack replicated health. RijiN uses their
        // solidity to reject corpses; unknown health must not reject all NPCs
        // or be displayed as an invented value.
        if(f.health>=0 && (out.health<=0 || out.health>1000000)) return false;
        if(f.kind==source::Kind::Common || f.kind==source::Kind::Witch) {
            bool ragdoll{};
            if(f.client_ragdoll>=0 && (!memory::Field(ptr,f.client_ragdoll,ragdoll) || ragdoll)) return false;
            unsigned short flags{}; unsigned char solid{};
            if(!memory::Field(ptr,f.solid_flags,flags) || !memory::Field(ptr,f.solid_type,solid) || (flags&4) || solid==0) return false;
        }
    }
    memory::Invoke(memory::Virtual<source::GetObject>(ptr,5,base.client_image),out.renderable,ptr);
    const Vec3* position{};
    if(!out.renderable || !memory::Invoke(memory::Virtual<source::GetOrigin>(out.renderable,1,base.client_image),position,out.renderable) ||
       !memory::Read(position,out.origin)) {
        if(!memory::Field(ptr,f.origin,out.origin)) return false;
    }
    memory::Field(ptr,f.mins,out.mins); memory::Field(ptr,f.maxs,out.maxs);
    out.object=f.object;
    snprintf(out.label,sizeof(out.label),"%s",f.name);
    if(f.kind==source::Kind::None && (f.model_candidate || f.object!=Object::None)) ClassifyModel(out);
    if(!strcmp(out.label,"CInferno")) snprintf(out.label,sizeof(out.label),"Fuego");
    if(!strcmp(out.label,"CSpitterProjectile")) snprintf(out.label,sizeof(out.label),"Acido");
    if(!strcmp(out.label,"CWeaponAmmoSpawn") || !strcmp(out.label,"CItemAmmoPack")) snprintf(out.label,sizeof(out.label),"Municion");
    return source::Finite(out.origin) && source::Finite(out.mins) && source::Finite(out.maxs);
}
bool Api::RenderBounds(const Entity& e,std::array<Vec3,8>& corners) const noexcept {
    corners.fill({});
    if(!e.renderable||!source::Finite(e.origin))return false;
    using Model=const void*(__thiscall*)(void*);
    using Bounds=void(__thiscall*)(void*,Vec3*,Vec3*);
    using Transform=const Matrix3*(__thiscall*)(void*);
    const void* model{};const Matrix3* address{};Matrix3 transform{};
    const float unset=std::numeric_limits<float>::quiet_NaN();Vec3 lo{unset,unset,unset},hi=lo;
    // Installed L4D2 IClientRenderable: 17 = local GetRenderBounds (ret 8),
    // 32 = RenderableToWorldTransform (matrix reference in EAX). This differs
    // from SDK 2013's interface layout; both slots were checked against RTTI.
    if(!memory::Invoke(memory::Virtual<Model>(e.renderable,8,base.client_image),model,e.renderable)||!model||
       !memory::InvokeVoid(memory::Virtual<Bounds>(e.renderable,17,base.client_image),e.renderable,&lo,&hi)||
       !memory::Invoke(memory::Virtual<Transform>(e.renderable,32,base.client_image),address,e.renderable)||
       !memory::Read(address,transform))return false;
    return world_bounds::Build(lo,hi,transform,e.origin,corners);
}
void Api::ClassifyModel(Entity& e) const noexcept {
    using Model=const void*(__thiscall*)(void*);
    using Name=const char*(__thiscall*)(void*,const void*);
    const void* model{}; const char* address{};
    if(!memory::Invoke(memory::Virtual<Model>(e.renderable,8,base.client_image),model,e.renderable) || !model) return;
    auto& cache=model_cache[(reinterpret_cast<uintptr_t>(model)>>4)%model_cache.size()];
    if(cache.model==model) {
        if(cache.object!=Object::None && e.object!=Object::Projectile) e.object=cache.object;
        if(cache.label[0]) snprintf(e.label,sizeof(e.label),"%s",cache.label);
        return;
    }
    if(!memory::Invoke(memory::Virtual<Name>(model_info,3),address,model_info,model) || !address) return;
    char path[260]{}; bool ended{};
    for(size_t i=0;i<sizeof(path)-1;++i) {
        char c{}; if(!memory::Read(address+i,c)) return;
        if(!c) {ended=true;break;}
        path[i]=c>='A' && c<='Z' ? static_cast<char>(c-'A'+'a') : c;
    }
    if(!ended) return;
    cache={}; cache.model=model;
    const auto slash=strrchr(path,'/'); const char* file=slash?slash+1:path;
    struct Entry {const char* model; const char* label; Object object;};
    constexpr Entry entries[]{
        {"w_rifle_m16a2.mdl","M16",Object::Weapon},{"w_rifle_ak47.mdl","AK-47",Object::Weapon},
        {"w_desert_rifle.mdl","SCAR",Object::Weapon},{"w_rifle_sg552.mdl","SG552",Object::Weapon},
        {"w_smg_uzi.mdl","Uzi",Object::Weapon},{"w_smg_a.mdl","SMG silenciada",Object::Weapon},{"w_smg_mp5.mdl","MP5",Object::Weapon},
        {"w_pistol_b.mdl","Pistola",Object::Weapon},{"w_desert_eagle.mdl","Magnum",Object::Weapon},
        {"w_shotgun.mdl","Escopeta",Object::Weapon},{"w_pumpshotgun_a.mdl","Chrome",Object::Weapon},
        {"w_autoshot_m4super.mdl","Auto shotgun",Object::Weapon},{"w_shotgun_spas.mdl","SPAS",Object::Weapon},
        {"w_sniper_mini14.mdl","Hunting rifle",Object::Weapon},{"w_sniper_military.mdl","Military sniper",Object::Weapon},
        {"w_sniper_scout.mdl","Scout",Object::Weapon},{"w_sniper_awp.mdl","AWP",Object::Weapon},
        {"w_m60.mdl","M60",Object::Weapon},{"w_grenade_launcher.mdl","Lanzagranadas",Object::Weapon},
        {"w_eq_medkit.mdl","Botiquin",Object::Health},{"w_eq_defibrillator.mdl","Desfibrilador",Object::Health},
        {"w_eq_painpills.mdl","Pildoras",Object::Health},{"w_eq_adrenaline.mdl","Adrenalina",Object::Health},
        {"w_eq_explosive_ammopack.mdl","Municion explosiva",Object::Ammo},{"w_eq_incendiary_ammopack.mdl","Municion incendiaria",Object::Ammo},
        {"w_laser_sights.mdl","Mira laser",Object::Ammo},{"w_eq_pipebomb.mdl","Pipe bomb",Object::Throwable},
        {"w_eq_molotov.mdl","Molotov",Object::Throwable},{"w_eq_bile_flask.mdl","Bilis",Object::Throwable},
        {"gascan001a.mdl","Gasolina",Object::Objective},{"propanecanister001a.mdl","Propano",Object::Objective},
        {"oxygentank01.mdl","Oxigeno",Object::Objective},{"explosive_box001.mdl","Fuegos artificiales",Object::Objective},
        {"w_fireaxe.mdl","Hacha",Object::Weapon},{"w_cricket_bat.mdl","Bate de cricket",Object::Weapon},
        {"w_bat.mdl","Bate",Object::Weapon},{"w_katana.mdl","Katana",Object::Weapon},
        {"w_crowbar.mdl","Palanca",Object::Weapon},{"w_chainsaw.mdl","Motosierra",Object::Weapon},
        {"w_frying_pan.mdl","Sarten",Object::Weapon},{"w_machete.mdl","Machete",Object::Weapon},
        {"w_tonfa.mdl","Porra",Object::Weapon},{"w_electric_guitar.mdl","Guitarra",Object::Weapon},
        {"w_shovel.mdl","Pala",Object::Weapon},{"w_pitchfork.mdl","Horca",Object::Weapon},
        {"w_golfclub.mdl","Palo de golf",Object::Weapon},{"w_knife_t.mdl","Cuchillo",Object::Weapon}
    };
    for(const auto& item:entries) if(!strcmp(file,item.model)) {
        // Projectile category survives a model match with its held counterpart.
        if(e.object!=Object::Projectile) e.object=item.object;
        snprintf(e.label,sizeof(e.label),"%s",item.label);
        cache.object=item.object; snprintf(cache.label,sizeof(cache.label),"%s",item.label); return;
    }
}
bool Api::ActiveWeapon(const Entity& local,Entity& out) const noexcept {
    out={};unsigned handle{};
    if(!local.ptr || !local.fields || !memory::Field(local.ptr,local.fields->active_weapon,handle) ||
       !ReadPointer(Handle(handle),out,false,false,true) || out.handle!=handle) {out={};return false;}
    return true;
}
bool Api::Local(Entity& out,bool alive,bool allow_ghost) const noexcept {
    // The local player may be an infected ghost (versus respawn): features that
    // read the local player (movement, camera, exploits) must keep working.
    int index{}; return InGame() && memory::Invoke(base.local_player,index,base.engine) && Read(index,out,alive,allow_ghost);
}
bool Api::CarryVictim(const Entity& local,unsigned& victim) const noexcept {
    victim=0;unsigned handle{};
    if(!local.ptr || !local.fields || !memory::Field(local.ptr,local.fields->carry_victim,handle)) return false;
    if(!handle || handle==UINT_MAX) return true;
    using Get=void*(__thiscall*)(void*,unsigned);
    void* resolved{};
    // Preserve the distinction between an empty carry and a failed lookup.
    if(!memory::Invoke(memory::Virtual<Get>(base.entities,4,base.client_image),resolved,base.entities,handle)) return false;
    if(resolved) victim=handle; // GetClientEntityFromHandle checks the serial too.
    return true;
}
bool Api::NetworkChannelReady() const noexcept {
    // VEngineClient013::GetNetChannelInfo, matching Raze's +128h call.
    using Get=void*(__thiscall*)(void*);
    if(!engine_image.base) return false;
    void* channel{};
    return memory::Invoke(memory::Virtual<Get>(base.engine,74,engine_image),channel,base.engine) && channel;
}
bool Api::Eye(const Entity& e,Vec3& out) const noexcept {
    Vec3 offset{};
    if(!e.fields || !memory::Field(e.ptr,e.fields->view,offset) || !source::Finite(offset) || Length(offset)>200.f) return false;
    out=Add(e.origin,offset); return true;
}
int Api::WeaponIndex(const Entity& weapon) const noexcept {
    if(!weapon.fields) return -1;
    bool dual{};
    memory::Field(weapon.ptr,weapon.fields->dual_weapons,dual);
    const auto entity=std::string_view(weapon.fields->name);
    if(entity!="CTerrorMeleeWeapon") return weapons::Find(entity,{},dual);
    using Model=const void*(__thiscall*)(void*);
    using Name=const char*(__thiscall*)(void*,const void*);
    const void* model{}; const char* address{}; char name[260]{};
    if(memory::Invoke(memory::Virtual<Model>(weapon.renderable,8,base.client_image),model,weapon.renderable) && model &&
       memory::Invoke(memory::Virtual<Name>(model_info,3),address,model_info,model) && address) {
        for(size_t i=0;i<sizeof(name)-1;++i) {
            if(!memory::Read(address+i,name[i]) || !name[i]) { name[i]=0;break; }
            if(name[i]>='A' && name[i]<='Z') name[i]=static_cast<char>(name[i]-'A'+'a');
        }
    }
    return weapons::Find(entity,name);
}
bool Api::Hitboxes(const Entity& e,std::vector<Point>& out,float time) const {
    out.clear();
    using Model=const void*(__thiscall*)(void*);
    using Studio=const unsigned char*(__thiscall*)(void*,const void*);
    using Bones=bool(__thiscall*)(void*,Matrix3*,int,int,float);
    const void* model{}; const unsigned char* hdr{};
    if(!e.renderable || !memory::Invoke(memory::Virtual<Model>(e.renderable,8,base.client_image),model,e.renderable) || !model ||
       !memory::Invoke(memory::Virtual<Studio>(model_info,30),hdr,model_info,model) || !hdr) return false;
    int id{},length{},sets{},set_offset{},bone_count{},set_index{};
    if(!memory::Read(hdr,id) || id!=0x54534449 || !memory::Read(hdr+76,length) || length<180 || length>16*1024*1024 ||
       !memory::Read(hdr+156,bone_count) || bone_count<1 || bone_count>128 || !memory::Read(hdr+172,sets) || sets<1 || sets>32 ||
       !memory::Read(hdr+176,set_offset) || set_offset<180 || set_offset>length-sets*12) return false;
    if(e.fields->hitbox_set>=0 && !memory::Field(e.ptr,e.fields->hitbox_set,set_index)) return false;
    if(set_index<0 || set_index>=sets) return false;
    const auto set=hdr+set_offset+set_index*12;
    int count{},offset{};
    if(!memory::Read(set+4,count) || !memory::Read(set+8,offset) || count<1 || count>128 || offset<12 ||
       offset>length || (set-hdr)+offset+count*sizeof(Hitbox)>static_cast<size_t>(length)) return false;
    Matrix3 bones[128]{}; bool success{};
    if(!memory::Invoke(memory::Virtual<Bones>(e.renderable,13,base.client_image),success,e.renderable,bones,128,0x100,time) || !success) return false;
    float scale=1.f; if(e.fields->scale>=0) memory::Field(e.ptr,e.fields->scale,scale);
    if(!std::isfinite(scale) || scale<=0.f || scale>10.f) return false;
    for(int i=0;i<count;++i) {
        Hitbox box{};
        if(!memory::Read(set+offset+i*sizeof(box),box) || box.bone<0 || box.bone>=bone_count ||
           !source::Finite(box.min) || !source::Finite(box.max)) continue;
        const auto center=Mul(Add(box.min,box.max),0.5f*scale);
        const auto& m=bones[box.bone].m;
        const Vec3 p{m[0][0]*center.x+m[0][1]*center.y+m[0][2]*center.z+m[0][3],
            m[1][0]*center.x+m[1][1]*center.y+m[1][2]*center.z+m[1][3],
            m[2][0]*center.x+m[2][1]*center.y+m[2][2]*center.z+m[2][3]};
        // Stale bone caches (fresh spawns, teleports, ghosts) keep the pose at
        // the old position; real bones stay near the entity origin.
        const Vec3 half=Mul(Sub(box.max,box.min),0.5f*scale);
        const float radius=std::max(0.f,std::min({half.x,half.y,half.z}));
        if(source::Finite(p) && Length(Sub(p,e.origin))<256.f) out.push_back({p,box.group,box.bone,i,0,radius});
        // Multipoint: face centers inset 80%. A pill/head center peeking behind
        // cover may be blocked while an edge of the SAME hitbox is visible;
        // without these the whole priority group dies and the aim falls to a
        // worse zone (felt as "shots that should hit but don't").
        {
            for(int axis=0;axis<3;++axis) {
                const float extent=(axis==0?half.x:axis==1?half.y:half.z);
                if(extent<1.5f) continue;
                for(float sign:{-1.f,1.f}) {
                    const Vec3 q{p.x+m[0][axis]*extent*sign*.8f,
                                 p.y+m[1][axis]*extent*sign*.8f,
                                 p.z+m[2][axis]*extent*sign*.8f};
                    const int sample=1+axis*2+(sign>0.f?1:0);
                    const float inset=std::min(radius,extent*.2f);
                    if(source::Finite(q) && Length(Sub(q,e.origin))<256.f) out.push_back({q,box.group,box.bone,i,sample,inset});
                }
            }
        }
    }
    return !out.empty();
}
unsigned Api::Skeleton(const Entity& e,std::span<source::BoneSegment> out,float time) const noexcept {
    if(out.empty() || !e.renderable || !model_info || !source::Finite(e.origin) || !std::isfinite(time)) return 0;
    using Model=const void*(__thiscall*)(void*);
    using Studio=const unsigned char*(__thiscall*)(void*,const void*);
    using Bones=bool(__thiscall*)(void*,Matrix3*,int,int,float);
    const void* model{}; const unsigned char* hdr{};
    if(!memory::Invoke(memory::Virtual<Model>(e.renderable,8,base.client_image),model,e.renderable) || !model ||
       !memory::Invoke(memory::Virtual<Studio>(model_info,30),hdr,model_info,model) || !hdr) return 0;

    // L4D2 studiohdr/mstudiobone layout, checked against the local SDK.
    struct Header {
        int id,version,checksum; char name[64]; int length;
        unsigned char padding[76]; int bone_count,bone_offset;
    } header{};
    struct Bone {
        int name,parent; unsigned char transform[152]; int flags; unsigned char tail[52];
    };
    static_assert(sizeof(Header)==164 && offsetof(Header,bone_count)==0x9c);
    static_assert(sizeof(Bone)==0xd8 && offsetof(Bone,parent)==4 && offsetof(Bone,flags)==0xa0);
    if(!memory::Read(hdr,header) || header.id!=0x54534449 || header.length<180 || header.length>16*1024*1024 ||
       header.bone_count<1 || header.bone_count>128 || header.bone_offset<180 ||
       header.bone_offset>header.length-header.bone_count*static_cast<int>(sizeof(Bone)) ||
       reinterpret_cast<uintptr_t>(hdr)>UINTPTR_MAX-static_cast<unsigned>(header.length)) return 0;

    auto& cache=skeleton_cache[(reinterpret_cast<uintptr_t>(model)>>4)%skeleton_cache.size()];
    if(cache.model!=model || cache.header!=hdr || cache.checksum!=header.checksum || cache.length!=header.length ||
       cache.bone_count!=header.bone_count || cache.bone_offset!=header.bone_offset) {
        std::array<Bone,128> bones{};
        if(!memory::Copy(bones.data(),hdr+header.bone_offset,static_cast<size_t>(header.bone_count)*sizeof(Bone))) return 0;
        SkeletonCache next{};
        for(int i=0;i<header.bone_count;++i) {
            const int parent=bones[i].parent;
            if(parent<-1 || parent>=header.bone_count || parent==i) return 0;
        }
        bool has_child[128]{}; int siblings[128]{};
        for(int i=0;i<header.bone_count;++i) {
            const int parent=bones[i].parent;
            if(parent>=0) {has_child[parent]=true;++siblings[parent];}
        }
        for(int i=0;i<header.bone_count;++i) {
            // Reject cycles before trusting a model's hierarchy.
            int ancestor=i,steps=0;
            while(ancestor>=0 && steps<header.bone_count) {ancestor=bones[ancestor].parent;++steps;}
            if(ancestor>=0) return 0;
            int parent=bones[i].parent;
            if(parent<0) continue;
            // Charger/Spitter toe bones are bone-merge only (0x400), not hitbox
            // (0x100): the foot ended without its last segment. The toe is the
            // foot's sole child; twist helpers (Knee/Elbow/Ulna/Wrist) share their
            // parent with the real chain, and hands have five finger children.
            // Requiring a single child + a hitbox-parent + leaf selects only the
            // foot continuation.
            const bool hitbox=(bones[i].flags&0x100)!=0;
            const bool extension=(bones[i].flags&0x400)!=0 && (bones[parent].flags&0x100)!=0 &&
                                 !has_child[i] && siblings[parent]==1;
            // Never connect the pelvis to an unanimated scene/root bone.
            // Bridge unmarked helper bones to the nearest animated ancestor.
            while(parent>=0 && !(bones[parent].flags&0x100)) parent=bones[parent].parent;
            if(parent<0) continue;
            if(extension) next.mask|=0x400;
            if(hitbox || extension)
                next.links[next.count++]={static_cast<std::uint8_t>(i),static_cast<std::uint8_t>(parent)};
        }
        next.model=model; next.header=hdr; next.checksum=header.checksum; next.length=header.length;
        next.bone_count=header.bone_count; next.bone_offset=header.bone_offset;
        cache=next;
    }
    if(!cache.count) return 0;
    Matrix3 matrices[128]{}; bool success{};
    // Reuse the engine's current pose; never invalidate its bone cache.
    if(!memory::Invoke(memory::Virtual<Bones>(e.renderable,13,base.client_image),success,
                       e.renderable,matrices,128,cache.mask,time) || !success) return 0;
    Vec3 positions[128]{}; bool valid[128]{};
    for(int i=0;i<header.bone_count;++i) {
        const auto& m=matrices[i].m;
        bool finite=true;
        for(const auto& row:m) for(float value:row) finite=finite && std::isfinite(value);
        const float basis=m[0][0]*m[0][0]+m[0][1]*m[0][1]+m[0][2]*m[0][2];
        positions[i]={m[0][3],m[1][3],m[2][3]};
        // Stale poses (identity/stale cache on spawn/teleport) sit at the world
        // origin or an old spot: real bones stay within a few metres.
        valid[i]=finite && basis>.0001f && basis<1000.f && Length(Sub(positions[i],e.origin))<256.f;
    }
    unsigned written=0;
    for(unsigned i=0;i<cache.count && written<out.size();++i) {
        const auto link=cache.links[i];
        if(!valid[link.child] || !valid[link.parent]) continue;
        const auto a=positions[link.child],b=positions[link.parent];
        const float length=Length(Sub(a,b));
        // Parent->child offsets on human rigs stay far below one metre; longer
        // segments mean one endpoint came from a stale pose.
        if(length<.05f || length>96.f) continue;
        out[written++]={a,b};
    }
    return written;
}
bool Api::Cast(Vec3 start,Vec3 end,Filter& filter,Trace& out,unsigned mask,float hull) const noexcept {
    if(!source::Finite(start) || !source::Finite(end)) return false;
    Ray ray{}; const auto d=Sub(end,start);
    ray.start={start.x,start.y,start.z,0}; ray.delta={d.x,d.y,d.z,0};
    ray.swept=Length(d)>0.f; ray.is_ray=hull<=0.f; ray.extents={hull,hull,hull,0};
    using Fn=void(__thiscall*)(void*,const Ray*,unsigned,Filter*,Trace*);
    out={};
    return memory::InvokeVoid(memory::Virtual<Fn>(trace,5),trace,&ray,mask,&filter,&out) &&
        std::isfinite(out.fraction) && out.fraction>=0.f && out.fraction<=1.f && source::Finite(out.end);
}
bool Api::Angles(Vec3& angles,bool set) const noexcept {
    using Fn=void(__thiscall*)(void*,Vec3*);
    return memory::InvokeVoid(memory::Virtual<Fn>(base.engine,set ? 20 : 19),base.engine,&angles) && source::Finite(angles);
}
bool Api::Sequence(const Entity& e,char* name,size_t capacity) const noexcept {
    if(!name || capacity<2 || !e.fields) return false;
    name[0]=0;
    int sequence{}; const char* address{};
    using Fn=const char*(__thiscall*)(void*,int);
    if(!memory::Field(e.ptr,e.fields->sequence,sequence) || sequence<0 || sequence>65535 ||
       !memory::Invoke(reinterpret_cast<Fn>(sequence_name),address,e.ptr,sequence) || !address) return false;
    for(size_t i=0;i<capacity-1;++i) {
        if(!memory::Read(address+i,name[i])) {name[0]=0;return false;}
        if(!name[i]) return true;
    }
    name[0]=0; return false;
}
bool Api::Friend(const Entity& e,bool& result) const noexcept {
    result=false;
    if(e.index<1 || e.index>64) return true;
    const auto steam=GetModuleHandleW(L"steam_api.dll");
    using Get=void*(__cdecl*)();
    using Has=bool(__cdecl*)(void*,uint64_t,int);
    auto has=steam ? reinterpret_cast<Has>(GetProcAddress(steam,"SteamAPI_ISteamFriends_HasFriend")) : nullptr;
    auto get=steam ? reinterpret_cast<Get>(GetProcAddress(steam,"SteamAPI_SteamFriends_v017")) : nullptr;
    if(!get && steam) get=reinterpret_cast<Get>(GetProcAddress(steam,"SteamFriends"));
    void* friends{};
    source::PlayerInfo info{}; bool read{};
    unsigned account{};
    if(!has || !memory::Invoke(get,friends) || !friends ||
       !memory::Invoke(base.player_info,read,base.engine,e.index,&info) || !read ||
       !memory::Read(info.header,account)) return false;
    if(!account) return true; // Bot or an entity without a Steam account.
    return memory::Invoke(has,result,friends,UINT64_C(0x0110000100000000)|account,4);
}
bool Api::Spread(const Entity& weapon,const Command& command,Vec3& offset,bool refresh) const noexcept {
    offset={};
    // The catalog's six gun families share this angular FireBullet path.
    // Projectiles, melee and unknown families must never call its updater.
    if(!weapon.fields || weapon.fields->weapon_group<0 || weapon.fields->weapon_group>5 ||
       !shared_random || !prediction_seed || spread_offset<0 || command.number<=0 || command.seed<0) return false;
    float spread{}; int old_seed{};
    if(!memory::Field(weapon.ptr,spread_offset,spread) || !std::isfinite(spread) || spread<0.f || spread>90.f) return false;
    if(refresh) {
        if(!update_spread) return false;
        const float previous=spread;
        const bool updated=memory::InvokeVoid(reinterpret_cast<void(__thiscall*)(void*)>(update_spread),weapon.ptr);
        const bool sampled=memory::Field(weapon.ptr,spread_offset,spread);
        const bool restored=memory::Copy(static_cast<unsigned char*>(weapon.ptr)+spread_offset,&previous,sizeof(previous));
        if(!updated || !sampled || !restored || !std::isfinite(spread) || spread<0.f || spread>90.f) return false;
    }
    if(spread==0.f) return true;
    if(!memory::Read(prediction_seed,old_seed) || !memory::Copy(prediction_seed,&command.seed,sizeof(command.seed))) return false;
    using Fn=float(__cdecl*)(const char*,float,float,int);
    Vec3 sampled{};
    const bool horizontal=memory::Invoke(reinterpret_cast<Fn>(shared_random),sampled.x,"CTerrorGun::FireBullet HorizSpread",-spread,spread,0);
    const bool vertical=memory::Invoke(reinterpret_cast<Fn>(shared_random),sampled.y,"CTerrorGun::FireBullet VertSpread",-spread,spread,0);
    const bool restored=memory::Copy(prediction_seed,&old_seed,sizeof(old_seed));
    // A failed or malformed sample must not expose a partial correction. A
    // finite value outside the exact range requested is also an invalid RNG
    // result, even when it happens to fit the later angle safety limit.
    if(!horizontal || !vertical || !restored || !source::Finite(sampled) ||
       std::abs(sampled.x)>spread || std::abs(sampled.y)>spread) return false;
    offset=sampled;return true;
}
bool Api::SpreadAtTick(const Entity& local,const Entity& weapon,const Command& command,Vec3& offset) const noexcept {
    offset={};
    if(!local.fields || !weapon.fields || weapon.fields->weapon_group<0 || weapon.fields->weapon_group>5 ||
       !globals || !prediction_seed || !shared_random || !update_spread || spread_offset<0 || command.number<=0) return false;
    int tickbase{};
    if(!memory::Field(local.ptr,local.fields->tickbase,tickbase)) return false;
    ScopedShotClock context;
    if(!context.Begin(globals,prediction_seed,tickbase,command.seed)) return false;
    Vec3 sampled{};
    const bool valid=Spread(weapon,command,sampled,true);
    const bool restored=context.Restore();
    if(!valid || !restored) return false;
    offset=sampled;return true;
}
float Api::Interpolation(float interval) const noexcept {
    // RTTI on the installed vstdlib.dll: CCvar vtable has 5 IAppSystem slots,
    // then AllocateDLLIdentifier/Register/Unregister/FindCommandLineVar, the
    // FindCommandBase pair at 10/11 and FindVar at 12. Slot 16 is
    // InstallGlobalChangeCallback: it also takes one argument, so the old
    // miscall looked harmless but pushed the "cl_updaterate" name pointer into
    // the engine's global change-callback vector, once per frame. The next
    // ConVar change then dispatched into string bytes (crash dumps show EIP on
    // "cl_updaterate" in this module). Validate the resolved name to fail
    // closed if the build's CCvar layout ever shifts.
    const auto value=[&](const char* name,float fallback) {
        float out{};return ConVarFloat(name,out)?out:fallback;
    };
    if(!std::isfinite(interval) || interval<.001f || interval>.1f) interval=1.f/30.f;
    float update=std::clamp(value("cl_updaterate",1.f/interval),10.f,1000.f);
    const float min_rate=value("sv_minupdaterate",-1.f),max_rate=value("sv_maxupdaterate",-1.f);
    if(min_rate>=10.f && max_rate>=min_rate && max_rate<=1000.f) update=std::clamp(update,min_rate,max_rate);
    float ratio=std::max(1.f,value("cl_interp_ratio",1.f));
    const float minimum=value("sv_client_min_interp_ratio",-1.f),maximum=value("sv_client_max_interp_ratio",-1.f);
    if(minimum>=0.f && maximum>=minimum) ratio=std::clamp(ratio,minimum,maximum);
    return std::clamp(std::max(value("cl_interp",interval),ratio/update),0.f,1.f);
}
float Api::NetworkLatency() const noexcept {
    using State=void*(__cdecl*)();
    using Latency=float(__thiscall*)(void*,int);
    void* state{};void* channel{};float incoming{},outgoing{};
    // CL_Move's validated client-state accessor returns the L4D2 state. Its
    // netchannel lives at +0x10; INetChannelInfo::GetLatency is slot 9.
    if(!memory::Invoke(reinterpret_cast<State>(client_state_getter),state) ||
       !memory::Field(state,0x10,channel) || !channel) return 0.f;
    const auto latency=memory::Virtual<Latency>(channel,9);
    if(!memory::Invoke(latency,outgoing,channel,0) || !memory::Invoke(latency,incoming,channel,1) ||
       !std::isfinite(incoming) || !std::isfinite(outgoing) || incoming<0.f || outgoing<0.f) return 0.f;
    return std::clamp(incoming+outgoing,0.f,1.f);
}
bool Api::Commands(CommandState& out) const noexcept {
    out={};void* state{};using State=void*(__cdecl*)();
    if(outgoing_command<0 || choked_commands<0 || acknowledged_command<0 ||
       !memory::Invoke(reinterpret_cast<State>(client_state_getter),state) || !state ||
       !memory::Field(state,outgoing_command,out.outgoing) || !memory::Field(state,choked_commands,out.choked) ||
       !memory::Field(state,acknowledged_command,out.acknowledged) || out.outgoing<0 || out.choked<0 || out.choked>64 ||
       out.acknowledged<0 || out.acknowledged>out.outgoing || out.outgoing>0x3fffffff) {out={};return false;}
    return true;
}
bool Api::UpdatePrediction() const noexcept {
    CommandState commands{};void* state{};int delta{};
    using State=void*(__cdecl*)();
    using Update=void(__thiscall*)(void*,int,bool,int,int);
    // The engine's validated CL_RunPrediction instruction reads the delta
    // operand at queue+14; its state view is root+8 (delta 0xCC here, 0xD4
    // in Raze). Keep the offset bound to that signature rather than hardcoding it.
    if(delta_tick<0 || !Commands(commands) ||
       !memory::Invoke(reinterpret_cast<State>(client_state_getter),state) || !state ||
       !memory::Field(state,delta_tick,delta) || delta< -1 || delta>0x3fffffff) return false;
    return memory::InvokeVoid(memory::Virtual<Update>(prediction,3,base.client_image),
        prediction,delta,delta>0,commands.acknowledged,commands.outgoing+commands.choked);
}
bool Api::ResetPredictionHistory() const noexcept {
    using Reset=void(__thiscall*)(void*);
    const auto reset=memory::Virtual<Reset>(prediction,7,base.client_image);
    // Verified on the installed VClientPrediction001: OnReceivedUncompressedPacket
    // invalidates the previous frame and clears four prediction-history counters.
    // Call the native method, never write guessed offsets into the live object.
    constexpr unsigned char expected[]{0x33,0xc0,0xc7,0x41,0x0c,0xff,0xff,0xff,0xff,
        0x89,0x41,0x1c,0x89,0x41,0x20,0x89,0x41,0x30,0x89,0x41,0x34,0xc3};
    unsigned char actual[sizeof(expected)]{};
    if(!reset||!base.client_image.Contains(reinterpret_cast<void*>(reset),sizeof(actual))||
       !memory::Copy(actual,reinterpret_cast<void*>(reset),sizeof(actual))||
       std::memcmp(actual,expected,sizeof(actual)))return false;
    return memory::InvokeVoid(reset,prediction);
}
bool Api::Channel(int& acknowledged,int& choked) const noexcept {
    acknowledged=choked=0;
    void* channel=NetChannel(*this);
    // Storm and Methot 3 declare -1 in the outgoing-ACK field, so a negative
    // reading is expected; the retained-packet count stays a serialized byte.
    if(!channel || !memory::Field(channel,0x10,acknowledged) || !memory::Field(channel,0x1c,choked) ||
       acknowledged<-1 || acknowledged>0x3fffffff || choked<0 || choked>255) {
        acknowledged=choked=0;return false;
    }
    return true;
}
bool Api::SetChannelAck(int value) const noexcept {
    void* channel=NetChannel(*this);
    if(!channel || value<-1 || value>0x3fffffff) return false;
    return memory::Copy(reinterpret_cast<unsigned char*>(channel)+0x10,&value,sizeof(value));
}
bool Api::SetChannelChoked(int value) const noexcept {
    void* channel=NetChannel(*this);
    if(!channel || value<0 || value>255) return false;
    return memory::Copy(reinterpret_cast<unsigned char*>(channel)+0x1c,&value,sizeof(value));
}
bool ChannelPatch::Apply(const Api& api,bool write_ack,int acknowledged,int choked) noexcept {
    if(acknowledged< -1||acknowledged>0x3fffffff||choked<0||choked>255)return false;
    void* channel=NetChannel(api);int before_ack{},before_choked{},sequence{};
    if(!channel||!api.Channel(before_ack,before_choked)||!memory::Field(channel,0x08,sequence))return false;
    if(owner_!=channel) {
        Restore(api);owner_=channel;ack_=before_ack;choked_=before_choked;
    }
    bool ack_ok=true;
    if(write_ack) {
        ack_ok=memory::Copy(static_cast<unsigned char*>(channel)+0x10,&acknowledged,sizeof(acknowledged));
        if(ack_ok){wrote_ack_=true;applied_ack_=acknowledged;}
    }
    const bool choked_ok=memory::Copy(static_cast<unsigned char*>(channel)+0x1c,&choked,sizeof(choked));
    if(choked_ok){wrote_choked_=true;applied_choked_=choked;}
    sequence_=sequence;
    return ack_ok&&choked_ok;
}
void ChannelPatch::Restore(const Api& api) noexcept {
    if(owner_&&NetChannel(api)==owner_) {
        int current{};
        if(wrote_ack_&&memory::Field(owner_,0x10,current)&&current==applied_ack_)
            memory::Copy(static_cast<unsigned char*>(owner_)+0x10,&ack_,sizeof(ack_));
        if(wrote_choked_&&memory::Field(owner_,0x1c,current)) {
            int sequence{};
            if(current==applied_choked_)
                memory::Copy(static_cast<unsigned char*>(owner_)+0x1c,&choked_,sizeof(choked_));
            else if(current==applied_choked_+1&&memory::Field(owner_,0x08,sequence)&&
                    static_cast<long long>(sequence)-sequence_==1) {
                // SetChoked can increment our declaration instead of sending.
                // Preserve that one real packet without leaking 255 -> 256.
                const int real_choked=std::min(choked_+1,255);
                memory::Copy(static_cast<unsigned char*>(owner_)+0x1c,&real_choked,sizeof(real_choked));
            }
        }
    }
    *this={};
}
namespace {
template<size_t N> const unsigned char* UniqueInstruction(const unsigned char* code,size_t size,const short (&pattern)[N]) noexcept {
    const unsigned char* result{};
    for(size_t i=0;i+N<=size;++i) {
        bool same=true;
        for(size_t j=0;j<N;++j)if(pattern[j]>=0&&code[i+j]!=pattern[j]){same=false;break;}
        if(same){if(result)return nullptr;result=code+i;}
    }
    return result;
}
bool ReadSchedule(void* channel,int offset,const double* clock,double& deadline,double& now) noexcept {
    return channel&&clock&&memory::Field(channel,offset,deadline)&&memory::Read(clock,now)&&
        std::isfinite(deadline)&&std::isfinite(now)&&deadline>=0.&&now>=0.&&now<1.e12&&deadline<=now+60.;
}
}
bool ChannelMoveBudget::Resolve(const Api& api,void* channel) noexcept {
    void* table{};
    if(!channel||!api.engine_image.base||!memory::Read(channel,table)||!table)return false;
    if(table==table_&&image_.base==api.engine_image.base&&image_.size==api.engine_image.size)return deadline_offset_>=0;
    table_=table;image_=api.engine_image;deadline_offset_=-1;clock_=nullptr;
    using Method=void(__thiscall*)(void*);
    const auto send=reinterpret_cast<const unsigned char*>(memory::Virtual<Method>(channel,47,image_));
    const auto can=reinterpret_cast<const unsigned char*>(memory::Virtual<Method>(channel,57,image_));
    unsigned char send_code[0x700]{},can_code[0x80]{};
    if(!image_.Contains(table,58*sizeof(void*))||!send||!can||!memory::Executable(send)||!memory::Executable(can)||
       !image_.Contains(send,sizeof(send_code))||!image_.Contains(can,sizeof(can_code))||
       !memory::Copy(send_code,send,sizeof(send_code))||!memory::Copy(can_code,can,sizeof(can_code)))return false;
    // SendDatagram: max(clearTime,net_time) + bytes/rate. CanPacket compares
    // the SAME double against the SAME native clock. All four send operands
    // and the rate field must agree before the deadline becomes writable.
    constexpr short send_pattern[]{0xdd,0x86,-1,-1,-1,-1,0xdd,0x05,-1,-1,-1,-1,
        0xdb,0xf1,0xdd,0xd9,0x76,0x08,0xdd,0x9e,-1,-1,-1,-1,0xeb,0x02,0xdd,0xd8,
        0xdb,0x85,-1,-1,-1,-1,0xda,0xb6,-1,-1,-1,-1,0xdc,0x86,-1,-1,-1,-1,0xdd,0x96,-1,-1,-1,-1};
    constexpr short can_pattern[]{0xdd,0x86,-1,-1,-1,-1,0xdd,0x05,-1,-1,-1,-1,
        0xdf,0xf1,0xdd,0xd8,0x76,0x07,0xb8,0x01,0,0,0,0x5e,0xc3,0x33,0xc0};
    const auto a=UniqueInstruction(send_code,sizeof(send_code),send_pattern);
    const auto b=UniqueInstruction(can_code,sizeof(can_code),can_pattern);
    if(!a||!b)return false;
    int load{},store{},add{},finish{},rate{},gate{};const double* send_clock{};const double* gate_clock{};
    std::memcpy(&load,a+2,4);std::memcpy(&store,a+20,4);std::memcpy(&add,a+42,4);
    std::memcpy(&finish,a+48,4);std::memcpy(&rate,a+36,4);std::memcpy(&gate,b+2,4);
    std::memcpy(&send_clock,a+8,4);std::memcpy(&gate_clock,b+8,4);
    if(load<0x20||load>0x1000||load%alignof(double)||store!=load||add!=load||finish!=load||gate!=load||
       rate!=load-8||send_clock!=gate_clock||!image_.Contains(send_clock,sizeof(double)))return false;
    deadline_offset_=load;clock_=send_clock;return true;
}
bool ChannelMoveBudget::Begin(const Api& api) noexcept {
    owner_=nullptr;void* channel=NetChannel(api);
    if(!Resolve(api,channel)||!ReadSchedule(channel,deadline_offset_,clock_,before_,started_))return false;
    owner_=channel;return true;
}
double ChannelMoveBudget::Restore(const Api& api) noexcept {
    void* owner=owner_;owner_=nullptr;
    double deadline{},now{};void* table{};
    if(!owner||NetChannel(api)!=owner||!memory::Read(owner,table)||table!=table_||
       api.engine_image.base!=image_.base||api.engine_image.size!=image_.size||
       !ReadSchedule(owner,deadline_offset_,clock_,deadline,now)||now<started_)return 0.;
    // Preserve the base move's charge, elapsed real time, and any native
    // reduction/clear. Never restore the clock, sequence, receive or reliable data.
    const double baseline=std::max(before_,now);
    if(deadline<=baseline)return 0.;
    return memory::Copy(static_cast<unsigned char*>(owner)+deadline_offset_,&baseline,sizeof(baseline))?deadline-baseline:0.;
}
bool ChannelMoveBudget::Delay(const Api& api,double& seconds) noexcept {
    seconds=0.;void* channel=NetChannel(api);double deadline{},now{};
    if(!Resolve(api,channel)||!ReadSchedule(channel,deadline_offset_,clock_,deadline,now))return false;
    seconds=std::max(deadline-now,0.);return true;
}
bool Api::ConVarFloat(const char* name,float& value) const noexcept {
    if(!cvars || !name) return false;
    using Find=void*(__thiscall*)(void*,const char*);
    void* var{};void* parent{};const char* actual{};
    if(!memory::Invoke(memory::Virtual<Find>(cvars,12),var,cvars,name) || !var ||
       !memory::Field(var,12,actual) || !actual) return false;
    const memory::Range images[]{base.client_image,memory::Module(GetModuleHandleW(L"engine.dll")),
                                 memory::Module(GetModuleHandleW(L"vstdlib.dll")),memory::Module(GetModuleHandleW(L"server.dll"))};
    char resolved[80]{};bool named=false;
    for(const auto& image:images) {
        if(memory::String(image,actual,resolved,sizeof(resolved))) {named=!std::strcmp(resolved,name);break;}
    }
    return named && memory::Field(var,28,parent) && parent &&
        memory::Field(parent,44,value) && std::isfinite(value);
}
bool Api::ConVarNumber(const char* name,float& real,int& integer,bool set) const noexcept {
    if(!cvars||!name||(set&&!std::isfinite(real)))return false;
    using Find=void*(__thiscall*)(void*,const char*);
    void* var{};void* parent{};const char* actual{};
    if(!memory::Invoke(memory::Virtual<Find>(cvars,12),var,cvars,name)||!var||!memory::Field(var,12,actual)||!actual)return false;
    const memory::Range images[]{base.client_image,memory::Module(GetModuleHandleW(L"engine.dll")),memory::Module(GetModuleHandleW(L"vstdlib.dll"))};
    char resolved[80]{};bool named{};
    for(const auto& image:images)if(memory::String(image,actual,resolved,sizeof(resolved))){named=!std::strcmp(resolved,name);break;}
    struct Number {float real;int integer;};Number value{};
    if(!named||!memory::Field(var,28,parent)||!parent||!memory::Field(parent,44,value)||!std::isfinite(value.real))return false;
    if(set){value={real,integer};return memory::Copy(static_cast<unsigned char*>(parent)+44,&value,sizeof(value));}
    real=value.real;integer=value.integer;return true;
}
bool Api::SetConVarFloat(const char* name,float value,float* previous) const noexcept {
    if(!cvars || !name || !std::isfinite(value)) return false;
    using Find=void*(__thiscall*)(void*,const char*);
    // Same CCvar slot as Interpolation (12 = FindVar; 16 would be
    // InstallGlobalChangeCallback). ConVar layout: name +0xC, ConCommandBase
    // "real" parent +0x1C, fValue at parent+44.
    void* var{}; void* parent{};
    if(!memory::Invoke(memory::Virtual<Find>(cvars,12),var,cvars,name) || !var) return false;
    const char* actual{};
    if(!memory::Field(var,12,actual) || !actual) return false;
    // The name may live in client.dll, engine.dll or vstdlib.dll depending on
    // who registered the convar, so probe each image for a bounded read.
    char resolved[80]{}; bool named=false;
    const memory::Range images[]{base.client_image,memory::Module(GetModuleHandleW(L"engine.dll")),
                                 memory::Module(GetModuleHandleW(L"vstdlib.dll"))};
    for(const auto& image:images) {
        if(memory::String(image,actual,resolved,sizeof(resolved))) { named=!std::strcmp(resolved,name); break; }
    }
    if(!named || !memory::Field(var,28,parent) || !parent) return false;
    if(previous && !memory::Field(parent,44,*previous)) return false;
    // Direct value write: bypasses the replicated/cheat SetValue gates; the
    // engine keeps honoring the field until something re-registers a change.
    return memory::Copy(reinterpret_cast<unsigned char*>(parent)+44,&value,sizeof(value));
}
static bool PushSequence(void* channel,int amount,int limit=1000) noexcept {
    if(!channel || amount<1 || amount>limit) return false;
    int outbound{},inbound{},ack{};
    if(!memory::Field(channel,8,outbound) || !memory::Field(channel,12,inbound) || !memory::Field(channel,16,ack)) return false;
    if(outbound<0 || inbound<0 || ack<0 || outbound>0x3fffffff-amount || inbound>0x3fffffff || ack>0x3fffffff) return false;
    const int pushed=outbound+amount;
    return memory::Copy(static_cast<unsigned char*>(channel)+8,&pushed,sizeof(pushed));
}
bool Api::ShiftSequence(int amount) const noexcept {return PushSequence(NetChannel(*this),amount);}
bool Api::StormSequence(int amount) const noexcept {
    return PushSequence(NetChannel(*this),amount,storm_fall::MaxTickbase+450);
}
bool StormClock::Begin(void* player,int offset,Globals* globals,int shift) noexcept {
    if(Active() || !globals || offset<0 || offset>0x40000 || shift<=0 || shift>storm_fall::MaxTickbase+450) return false;
    int tick{};float time{},interval{};
    if(!memory::Field(player,offset,tick) || tick<=0 || tick>=storm_fall::MaxTickbase ||
       !memory::Field(globals,offsetof(Globals,time),time) || !std::isfinite(time) ||
       !memory::Field(globals,offsetof(Globals,interval),interval) || !std::isfinite(interval) || interval<.001f || interval>.1f) return false;
    const auto corrected=static_cast<int64_t>(tick)-shift;
    if(corrected<=-storm_fall::MaxTickbase || corrected>=storm_fall::MaxTickbase) return false;
    const int value=static_cast<int>(corrected);
    if(!memory::Copy(static_cast<unsigned char*>(player)+offset,&value,sizeof(value))) return false;
    player_=player;offset_=offset;shift_=shift;before_=tick;globals_=globals;time_=time;interval_=interval;return true;
}
bool StormClock::Restore() noexcept {
    if(!Active()) return true;
    auto player=player_;player_=nullptr;
    int tick{};
    const bool read=memory::Field(player,offset_,tick);
    const auto restored=static_cast<int64_t>(tick)+shift_;
    const bool valid=read && restored>0 && restored<storm_fall::MaxTickbase;
    const int value=valid?static_cast<int>(restored):before_;
    const float time=valid?(value-1)*interval_:time_;
    const bool wrote_tick=memory::Copy(static_cast<unsigned char*>(player)+offset_,&value,sizeof(value));
    const bool wrote_time=memory::Copy(&globals_->time,&time,sizeof(time));
    return valid && wrote_tick && wrote_time;
}
bool StormMovementInput(void* move,bool ducked) noexcept {
    // CMoveData layout from the supplied Storm source: buttons +36,
    // forward/side +44/+48. Copy before mutation, preserving all other bytes.
    struct Input {int buttons,old_buttons;float forward,side;};
    static_assert(sizeof(Input)==16);
    Input input{};
    if(!memory::Field(move,36,input) || !std::isfinite(input.forward) || !std::isfinite(input.side)) return false;
    input.buttons=storm_fall::MovementButtons(input.buttons,ducked);input.forward=input.side=0.f;
    return memory::Copy(static_cast<unsigned char*>(move)+36,&input,sizeof(input));
}
lag_exploit::Result Api::RazeLag(lag_exploit::Method method,int amount) const noexcept {
    if(amount<1 || amount>1000 || !engine_image.base || !client_state_getter) return {};
    using ChannelInfo=void*(__thiscall*)(void*);
    const auto getter=memory::Virtual<ChannelInfo>(base.engine,74,engine_image);
    unsigned char code[9]{};
    // Installed VEngineClient013::GetNetChannelInfo: call CL_GetLocalClient;
    // mov eax,[eax+10h]; ret. Verify the callee against CL_Move's accessor so
    // a changed interface cannot dispatch an unrelated slot as a pointer getter.
    if(!getter || !engine_image.Contains(reinterpret_cast<void*>(getter),sizeof(code)) ||
       !memory::Copy(code,reinterpret_cast<void*>(getter),sizeof(code)) || code[0]!=0xe8 ||
       code[5]!=0x8b || code[6]!=0x40 || code[7]!=0x10 || code[8]!=0xc3 ||
       RelativeCall(reinterpret_cast<void*>(getter))!=client_state_getter) return {};
    void* native_channel{};
    if(!memory::Invoke(getter,native_channel,base.engine)) return {};
    if(!native_channel) return {true,false,method==lag_exploit::Method::V2};
    // Raze V2 uses client-state root+18h. The validated getter returns root+8,
    // so the equivalent view here is state+10h, as in the normal transport.
    void* destination=method==lag_exploit::Method::V1?native_channel:NetChannel(*this);
    if(destination!=native_channel) return {};
    const bool shifted=PushSequence(destination,amount);
    return {true,shifted,shifted && method==lag_exploit::Method::V1};
}
Vec3 Add(Vec3 a,Vec3 b) noexcept { return {a.x+b.x,a.y+b.y,a.z+b.z}; }
Vec3 Sub(Vec3 a,Vec3 b) noexcept { return {a.x-b.x,a.y-b.y,a.z-b.z}; }
Vec3 Mul(Vec3 a,float b) noexcept { return {a.x*b,a.y*b,a.z*b}; }
float Length(Vec3 a) noexcept { return std::sqrt(a.x*a.x+a.y*a.y+a.z*a.z); }
float Normalize(float value) noexcept { return std::isfinite(value) ? std::remainder(value,360.f) : 0.f; }
Vec3 Angle(Vec3 from,Vec3 to) noexcept {
    const auto d=Sub(to,from);
    return {-std::atan2(d.z,std::hypot(d.x,d.y))*180.f/Pi,std::atan2(d.y,d.x)*180.f/Pi,0};
}
Vec3 Forward(Vec3 a) noexcept { const auto p=a.x*Pi/180.f,y=a.y*Pi/180.f; return {std::cos(p)*std::cos(y),std::cos(p)*std::sin(y),-std::sin(p)}; }
Vec3 CameraOrigin(Vec3 anchor,Vec3 angles,float distance,float shoulder,float height) noexcept {
    const float p=angles.x*Pi/180.f,y=angles.y*Pi/180.f,r=angles.z*Pi/180.f;
    const float sp=std::sin(p),cp=std::cos(p),sy=std::sin(y),cy=std::cos(y),sr=std::sin(r),cr=std::cos(r);
    const Vec3 forward{cp*cy,cp*sy,-sp};
    const Vec3 right{-sr*sp*cy+cr*sy,-sr*sp*sy-cr*cy,-sr*cp};
    const Vec3 up{cr*sp*cy+sr*sy,cr*sp*sy-sr*cy,cr*cp};
    return Add(Add(Add(anchor,Mul(forward,-distance)),Mul(right,shoulder)),Mul(up,height));
}
float Fov(Vec3 view,Vec3 aim) noexcept {
    const auto a=Forward(view),b=Forward(aim);
    return std::acos(std::clamp(a.x*b.x+a.y*b.y+a.z*b.z,-1.f,1.f))*180.f/Pi;
}
void FixMovement(Command& cmd,float original_yaw) noexcept {
    const float r=Normalize(cmd.angles.y-original_yaw)*Pi/180.f, c=std::cos(r),s=std::sin(r);
    const float forward=c*cmd.forward-s*cmd.side,side=s*cmd.forward+c*cmd.side;
    const float scale=std::max(1.f,std::max(std::abs(forward),std::abs(side))/450.f);
    cmd.forward=forward/scale; cmd.side=side/scale;
    cmd.buttons&=~(8|16|512|1024);
    if(cmd.forward>0.01f) cmd.buttons|=8; else if(cmd.forward<-.01f) cmd.buttons|=16;
    if(cmd.side>0.01f) cmd.buttons|=1024; else if(cmd.side<-.01f) cmd.buttons|=512;
}
bool Project(const source::Matrix& mat,Vec3 p,float width,float height,float& x,float& y) noexcept {
    const auto& m=mat.m;
    const float w=m[3][0]*p.x+m[3][1]*p.y+m[3][2]*p.z+m[3][3];
    if(!std::isfinite(w) || w<.01f) return false;
    x=width*.5f*(1.f+(m[0][0]*p.x+m[0][1]*p.y+m[0][2]*p.z+m[0][3])/w);
    y=height*.5f*(1.f-(m[1][0]*p.x+m[1][1]*p.y+m[1][2]*p.z+m[1][3])/w);
    return std::isfinite(x) && std::isfinite(y) && std::abs(x)<width*3 && std::abs(y)<height*3;
}
}
