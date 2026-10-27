#include "game.h"
#include "features.h"
#include "radar_logic.h"
#include "spectators.h"
#include "rapid_fire.h"
#include "storm_fall.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <limits>

namespace {
struct Glow { int cookie{},type{},range{},minimum{},color{}; };
struct Object { unsigned char prefix[32]{}; Glow glow{}; };
source::RecvProp glow_props[2]{},entity_props[13]{},fall_props[2]{};
source::RecvTable glow_table{},entity_table{},weapon_table{},fall_table{};
source::RecvProp weapon_props[3]{};
source::ClientClass client_class{};
void* received_container{};void* received_destination{};
bool registered{};unsigned calls{};
void __cdecl GlowType(const source::RecvProxyData* data,void* parent,void* output) {
    received_container=parent;received_destination=output;++calls;
    *static_cast<int*>(output)=data->value.integer;
    registered=data->value.integer!=0;
}
void __cdecl GlowColor(const source::RecvProxyData* data,void*,void* output) {
    ++calls;*static_cast<int*>(output)=data->value.integer;
}
source::ClientClass* __fastcall Classes(void*) {return &client_class;}
struct Desc { int type; const char* name;int offset; unsigned char rest[48]; };
static_assert(sizeof(Desc)==60);
Desc descriptions[]{{8,"m_MoveType",0x144,{}},{1,"m_surfaceFriction",0x13c4,{}}};
struct DataMap {const Desc* descriptions;int count;const char* name;DataMap* base;};
DataMap datamap{descriptions,2,"CBaseEntity",nullptr};
DataMap* __fastcall GetMap(void*) {return &datamap;}
void* vtable[20]{};
struct MockEntity {void** vtable;} mock_entity{vtable};
// ConVar layout used by Api::SetConVarFloat: name +12, parent +28, fValue +44.
struct ConVarNode { void* pad0[3]{}; const char* name{}; void* pad1[3]{}; void* parent{}; };
struct ConVarValue { void* pad[11]{}; float value{}; int integer{}; };
ConVarNode node{}; ConVarValue host{};
bool interpolation_vars{};
struct InterpVar {const char* name;float value;};
InterpVar interp_vars[]{{"cl_updaterate",100.f},{"cl_interp",0.f},{"cl_interp_ratio",1.f},
    {"sv_minupdaterate",10.f},{"sv_maxupdaterate",100.f},
    {"sv_client_min_interp_ratio",1.f},{"sv_client_max_interp_ratio",2.f}};
void* __fastcall FindVar(void*,void*,const char* name) {
    if(interpolation_vars) {
        for(const auto& var:interp_vars) if(!std::strcmp(name,var.name)) {
            node.name=var.name;host.value=var.value;return &node;
        }
        return nullptr;
    }
    return !std::strcmp(name,node.name) ? &node : nullptr;
}
void* cvar_vtable[13]{};
struct MockCvars { void** vtable; } mock_cvars{cvar_vtable};
// Client-state layout: net channel +0x10; CNetChan sequences +8/+12/+16 and
// the retained-packet choke count at +0x1C (0x14/0x18 are unknown fields).
struct FakeNet {
    void* vtbl{}; int unk{}; int out_seq{},in_seq{},ack{}; int gap0{},gap1{}; int choke{};
    unsigned char padding[0xb0-0x20]{};int rate{};int spare{};double deadline{};
};
static_assert(offsetof(FakeNet,deadline)==0xb8);
double net_clock{};
void* net_vtable[58]{};
// Executable fixtures contain the installed x86 instruction sequences, with
// relocatable clock operands. They are parsed but never executed as functions.
#pragma section(".pace",read,write)
#pragma comment(linker,"/SECTION:.pace,ERW")
__declspec(allocate(".pace")) unsigned char send_schedule[0x700]{};
__declspec(allocate(".pace")) unsigned char can_schedule[0x80]{};
void ScheduleFixture() {
    constexpr unsigned char send[]{0xdd,0x86,0xb8,0,0,0,0xdd,0x05,0,0,0,0,
        0xdb,0xf1,0xdd,0xd9,0x76,0x08,0xdd,0x9e,0xb8,0,0,0,0xeb,0x02,0xdd,0xd8,
        0xdb,0x85,0x30,0xff,0xff,0xff,0xda,0xb6,0xb0,0,0,0,0xdc,0x86,0xb8,0,0,0,0xdd,0x96,0xb8,0,0,0};
    constexpr unsigned char can[]{0xdd,0x86,0xb8,0,0,0,0xdd,0x05,0,0,0,0,
        0xdf,0xf1,0xdd,0xd8,0x76,0x07,0xb8,0x01,0,0,0,0x5e,0xc3,0x33,0xc0};
    std::memset(send_schedule,0xcc,sizeof(send_schedule));std::memset(can_schedule,0xcc,sizeof(can_schedule));
    std::memcpy(send_schedule+0x64e,send,sizeof(send));std::memcpy(can_schedule+0x45,can,sizeof(can));
    const double* clock=&net_clock;
    std::memcpy(send_schedule+0x64e+8,&clock,4);std::memcpy(can_schedule+0x45+8,&clock,4);
    net_vtable[47]=send_schedule;net_vtable[57]=can_schedule;
}
struct FakeState { void* pad[4]{}; void* channel{};int outgoing{},choked{},acknowledged{},delta{}; };
FakeState state{}; FakeNet channel{};
void* __cdecl StateGetter() { return &state; }
// Exact VEngineClient013 slot-74 instruction route, using the live fixture
// accessor so the runtime parser and the native x86 calling convention run.
__declspec(naked) void* __fastcall EngineChannel(void*,void*) {
    __asm {call StateGetter}
    __asm {mov eax,dword ptr [eax+10h]}
    __asm {ret}
}
void* engine_vtable[75]{};
struct FakeEngine {void** table{engine_vtable};} engine_fixture;

void* prediction_vtable[8]{};
struct FakePrediction {void** vtable{prediction_vtable};unsigned char state[0x34]{};} prediction;
__declspec(naked) void __fastcall PredictionReset(void*,void*) {
    __asm {
        xor eax,eax
        mov dword ptr [ecx+0ch],-1
        mov dword ptr [ecx+1ch],eax
        mov dword ptr [ecx+20h],eax
        mov dword ptr [ecx+30h],eax
        mov dword ptr [ecx+34h],eax
        ret
    }
}
unsigned prediction_calls{};
void* prediction_self{};
int prediction_delta{},prediction_ack{},prediction_outgoing{},prediction_shift{};
bool prediction_valid{};
const rapid_fire::Ring* prediction_ring{};
int prediction_command{};
void __fastcall PredictionUpdate(void* self,void*,int delta,bool valid,int ack,int outgoing) {
    ++prediction_calls;prediction_self=self;prediction_delta=delta;prediction_valid=valid;
    prediction_ack=ack;prediction_outgoing=outgoing;
    prediction_shift=prediction_ring?prediction_ring->Get(prediction_command):0;
}

struct TestBone {int name{},parent{-1};unsigned char transform[152]{};int flags{};unsigned char tail[52]{};};
struct TestStudio {
    int id{0x54534449},version{48},checksum{1};char name[64]{};int length{sizeof(TestStudio)};
    unsigned char padding[76]{};int count{6},offset{180};unsigned char rest[16]{};
    TestBone bones[6]{};
} studio;
static_assert(offsetof(TestStudio,bones)==180 && sizeof(TestBone)==216);
int requested_mask{};
void* model_vtable[31]{},*render_vtable[33]{};
struct VirtualObject {void** vtable;};
VirtualObject model_info{model_vtable},renderable{render_vtable};
const void* active_model=&studio;
source::Vec3 visual_min{-40,-2,-3},visual_max{10,6,4};
game::Matrix3 visual_transform{{{0,-1,0,100},{1,0,0,20},{0,0,1,10}}};
bool write_visual_bounds=true,return_visual_transform=true;
void __fastcall VisualBounds(void*,void*,source::Vec3* lo,source::Vec3* hi) {
    if(write_visual_bounds){*lo=visual_min;*hi=visual_max;}
}
const game::Matrix3* __fastcall VisualTransform(void*) {return return_visual_transform?&visual_transform:nullptr;}
const void* __fastcall Model(void*) {return active_model;}
const unsigned char* __fastcall Studio(void*,void*,const void* model) {return static_cast<const unsigned char*>(model);}
bool __fastcall SetupBones(void*,void*,game::Matrix3* output,int count,int mask,float) {
    requested_mask=mask;
    if(count<6) return false;
    const source::Vec3 points[]{{0,0,0},{10,10,30},{10,10,10},{20,10,10},{10,10,40},{10,10,60}};
    for(int i=0;i<6;++i) if(studio.bones[i].flags&mask) {
        auto& m=output[i].m;m[0][0]=m[1][1]=m[2][2]=1.f;
        m[0][3]=points[i].x;m[1][3]=points[i].y;m[2][3]=points[i].z;
    }
    return true;
}
struct Observer {int cookie{},mode{};unsigned target{};};
Observer observers[6]{};
source::ClientClass observer_class{};
source::RecvProp observer_props[2]{};
source::RecvTable observer_table{};
void* observer_vtable[2]{},*entity_vtable[5]{};
VirtualObject observer_network{observer_vtable},entity_list{entity_vtable};
bool connected_game{true};
bool __fastcall InGame(void*) {return connected_game;}
int __fastcall LocalIndex(void*) {return 1;}
int __fastcall MaxClients(void*) {return 5;}
void* __fastcall GetEntity(void*,void*,int index) {return index>=1 && index<=5?&observers[index]:nullptr;}
void* __fastcall GetNetwork(void*,void*,int index) {return index>=1 && index<=5?&observer_network:nullptr;}
source::ClientClass* __fastcall ObserverClass(void*) {return &observer_class;}
void* __fastcall FromHandle(void*,void*,unsigned handle) {return handle==0x1001?&observers[1]:nullptr;}
bool __fastcall PlayerInfo(void*,void*,int index,source::PlayerInfo* info) {
    *info={};info->userid=index+100;std::snprintf(info->name,sizeof(info->name),"Same name");return true;
}
void* held_vtable[6]{},*held_network_vtable[9]{};
VirtualObject held_network{held_network_vtable};
struct HeldObject {void** vtable{held_vtable};unsigned handle{0x200a};game::Vec3 origin{std::numeric_limits<float>::quiet_NaN(),0,0};} held;
struct HeldOwner {unsigned active{0x200a};} held_owner;
source::ClientClass held_class{};bool held_dormant{true};
void* __fastcall HeldNetwork(void*) {return &held_network;}
source::ClientClass* __fastcall HeldClass(void*) {return &held_class;}
bool __fastcall HeldDormant(void*) {return held_dormant;}
int __fastcall HeldIndex(void*) {return 10;}
const unsigned* __fastcall HeldReference(void*) {return &held.handle;}
void* __fastcall HeldHandle(void*,void*,unsigned handle) {return handle==held_owner.active?&held:nullptr;}
void* npc_vtable[6]{},*npc_network_vtable[9]{},*npc_render_vtable[2]{};
VirtualObject npc_network{npc_network_vtable},npc_render{npc_render_vtable};
source::ClientClass npc_class{};
struct NpcObject {
    void** table{npc_vtable};unsigned handle{0x104d};int team{3},health{50};
    game::Vec3 origin{100,0,0},mins{-12,-12,0},maxs{12,12,72};
    unsigned short solid_flags{};unsigned char solid{2},life{};bool ragdoll{};
} npc;
void* __fastcall NpcNetwork(void*) {return &npc_network;}
void* __fastcall NpcRenderable(void*) {return &npc_render;}
source::ClientClass* __fastcall NpcClass(void*) {return &npc_class;}
bool __fastcall NpcDormant(void*) {return false;}
int __fastcall NpcIndex(void*) {return 77;}
const unsigned* __fastcall NpcReference(void*) {return &npc.handle;}
}
int main() {
    unsigned failures{},checks{};
    const auto expect=[&](bool ok,const char* name) {
        ++checks;if(!ok) {std::fprintf(stderr,"FAIL: %s\n",name);++failures;}
    };
    const auto image=memory::Module(GetModuleHandleW(nullptr));
    {
        game::Api numbers;numbers.base.client_image=image;numbers.cvars=&mock_cvars;
        cvar_vtable[12]=reinterpret_cast<void*>(&FindVar);node.name="fog_enable";node.parent=&host;
        host.value=1.f;host.integer=1;float real{};int integer{};
        expect(numbers.ConVarNumber("fog_enable",real,integer)&&real==1.f&&integer==1,"fog snapshots retain both native convar mirrors");
        real=0.f;integer=0;
        expect(numbers.ConVarNumber("fog_enable",real,integer,true)&&host.value==0.f&&host.integer==0,"fog writes update GetFloat and GetBool storage together");
        real=1.f;integer=1;numbers.ConVarNumber("fog_enable",real,integer,true);
        expect(host.value==1.f&&host.integer==1,"fog restoration preserves both original convar values");
        expect(!numbers.ConVarNumber("unknown",real,integer),"unresolved fog names cannot access guessed convar storage");
    }
    {
        game::Api inventory;inventory.base.client_image=image;inventory.base.entities=&entity_list;
        entity_vtable[4]=reinterpret_cast<void*>(&HeldHandle);
        held_vtable[2]=reinterpret_cast<void*>(&HeldReference);held_vtable[4]=reinterpret_cast<void*>(&HeldNetwork);
        held_network_vtable[1]=reinterpret_cast<void*>(&HeldClass);held_network_vtable[7]=reinterpret_cast<void*>(&HeldDormant);held_network_vtable[8]=reinterpret_cast<void*>(&HeldIndex);
        game::Fields owned_fields{};owned_fields.object=game::Object::Weapon;owned_fields.origin=offsetof(HeldObject,origin);inventory.classes.emplace(&held_class,owned_fields);
        game::Fields owner_fields{};owner_fields.active_weapon=offsetof(HeldOwner,active);
        game::Entity owner{},item{};owner.ptr=&held_owner;owner.fields=&owner_fields;
        expect(inventory.ActiveWeapon(owner,item) && item.ptr==&held && item.handle==0x200a,"equipped dormant weapon resolves without a world render pose");
        expect(!inventory.ReadPointer(&held,item,false),"ordinary entity reads still reject dormant objects");
        held.handle=0x300a;
        expect(!inventory.ActiveWeapon(owner,item) && !item.ptr,"reused active weapon index with another serial fails closed");held.handle=0x200a;
        inventory.classes.at(&held_class).kind=source::Kind::Hunter;
        expect(!inventory.ActiveWeapon(owner,item),"inventory metadata path cannot accept a combat actor");
        inventory.classes.at(&held_class).kind=source::Kind::None;held_dormant=false;
        expect(!inventory.ReadPointer(&held,item,false),"ordinary world entity still requires finite geometry");held_dormant=true;
        held_owner.active=0xffffffff;
        expect(!inventory.ActiveWeapon(owner,item),"invalid active handle cannot select an inventory object");held_owner.active=0x200a;
    }
    {
        game::Api actors;actors.base.client_image=image;
        npc_vtable[2]=reinterpret_cast<void*>(&NpcReference);npc_vtable[4]=reinterpret_cast<void*>(&NpcNetwork);
        npc_vtable[5]=reinterpret_cast<void*>(&NpcRenderable);npc_network_vtable[1]=reinterpret_cast<void*>(&NpcClass);
        npc_network_vtable[7]=reinterpret_cast<void*>(&NpcDormant);npc_network_vtable[8]=reinterpret_cast<void*>(&NpcIndex);
        game::Fields f{};f.kind=source::Kind::Common;f.team=offsetof(NpcObject,team);
        f.origin=offsetof(NpcObject,origin);f.mins=offsetof(NpcObject,mins);f.maxs=offsetof(NpcObject,maxs);
        f.solid_flags=offsetof(NpcObject,solid_flags);f.solid_type=offsetof(NpcObject,solid);f.client_ragdoll=offsetof(NpcObject,ragdoll);
        actors.classes.emplace(&npc_class,f);auto& fields=actors.classes.at(&npc_class);game::Entity entity{};
        expect(actors.ReadPointer(&npc,entity) && entity.health==-1,"a solid living common without replicated HP remains eligible");
        npc.ragdoll=true;
        expect(!actors.ReadPointer(&npc,entity),"a common ragdoll is dead even while collision remains solid");
        expect(actors.ReadPointer(&npc,entity,false),"explicit raw reads can still inspect a corpse");
        fields.kind=source::Kind::Witch;
        expect(!actors.ReadPointer(&npc,entity),"the replicated ragdoll check also rejects a dead Witch");
        fields.kind=source::Kind::Common;npc.ragdoll=false;fields.client_ragdoll=-1;
        expect(actors.ReadPointer(&npc,entity),"absence of the optional ragdoll netvar does not hide all commons");
        fields.client_ragdoll=0x40001;
        expect(!actors.ReadPointer(&npc,entity),"unreadable resolved ragdoll metadata fails closed");
        fields.client_ragdoll=offsetof(NpcObject,ragdoll);npc.solid_flags=4;
        expect(!actors.ReadPointer(&npc,entity),"non-solid corpses remain rejected independently of the ragdoll flag");
        npc.solid_flags=0;npc.solid=0;
        expect(!actors.ReadPointer(&npc,entity),"SOLID_NONE corpses remain rejected");
        npc.solid=2;fields.health=offsetof(NpcObject,health);npc.health=0;
        expect(!actors.ReadPointer(&npc,entity),"replicated zero HP rejects a common even before ragdoll activation");
        npc.health=50;fields.life=offsetof(NpcObject,life);npc.life=1;
        expect(!actors.ReadPointer(&npc,entity),"a replicated dying life state still overrides solidity");
        npc.life=0;
        expect(actors.ReadPointer(&npc,entity) && entity.health==50,"a live NPC can return after independent death flags clear");
    }
    glow_props[0].name="m_iGlowType";glow_props[0].offset=offsetof(Glow,type);
    glow_props[0].proxy=reinterpret_cast<void*>(&GlowType);
    glow_props[1].name="m_glowColorOverride";glow_props[1].offset=offsetof(Glow,color);
    glow_props[1].proxy=reinterpret_cast<void*>(&GlowColor);
    glow_table.props=glow_props;glow_table.count=2;
    entity_props[0].name="movetype";entity_props[0].offset=0;
    entity_props[1].name="m_Glow";entity_props[1].type=6;
    entity_props[1].offset=offsetof(Object,glow);entity_props[1].table=&glow_table;
    const char* required[]{"m_vecOrigin","m_vecMins","m_vecMaxs","m_iTeamNum","m_iHealth","m_lifeState"};
    for(int i=0;i<6;++i) {entity_props[2+i].name=required[i];entity_props[2+i].offset=0x100+i*16;}
    entity_props[8].name="m_bClientSideRagdoll";entity_props[8].offset=0x200;
    for(int i=0;i<3;++i) {weapon_props[i].name=i==0?"000":i==1?"001":"002";weapon_props[i].offset=i*4;}
    weapon_table.props=weapon_props;weapon_table.count=3;
    entity_props[9].name="m_hMyWeapons";entity_props[9].type=6;entity_props[9].offset=0x300;entity_props[9].table=&weapon_table;
    fall_props[0].name="m_flFallVelocity";fall_props[0].type=1;fall_props[0].offset=88;
    fall_props[1].name="m_bDucked";fall_props[1].type=0;fall_props[1].offset=120;
    fall_table.props=fall_props;fall_table.count=2;
    entity_props[10].name="m_Local";entity_props[10].type=6;entity_props[10].offset=4516;entity_props[10].table=&fall_table;
    entity_props[11].name="m_bInMeleeSwing";entity_props[11].type=0;entity_props[11].offset=0x340;
    entity_props[12].name="m_carryVictim";entity_props[12].type=0;entity_props[12].offset=0x344;
    entity_table.props=entity_props;entity_table.count=13;
    client_class.name="CTerrorPlayer";client_class.table=&entity_table;
    const auto type=source::FindProperty(image,&entity_table,"m_iGlowType");
    const auto color=source::FindProperty(image,&entity_table,"m_glowColorOverride");
    expect(type.prop==&glow_props[0] && type.container==32 && type.offset==36,"nested property retains container and destination");
    Object object{};
    object.glow.type=3;
    expect(!registered,"raw type write does not notify the glow manager");
    expect(source::SetInteger(image,&object,type,3) && registered && calls==1,"type proxy registers glow even when value already matches");
    expect(received_container==&object.glow && received_destination==&object.glow.type,"proxy receives subobject rather than entity base");
    expect(source::SetInteger(image,&object,color,0x804020) && object.glow.color==0x804020 && calls==2,"color goes through its receive proxy");
    expect(source::SetInteger(image,&object,type,0) && !registered,"restoring original type unregisters native glow");
    auto invalid=type;invalid.container=0;
    expect(!source::SetInteger(image,&object,invalid,3),"mismatched container is rejected before invoking proxy");
    invalid=type;invalid.offset=-1;
    expect(!source::SetInteger(image,&object,invalid,3),"missing destination rejected");
    expect(!source::SetInteger(image,nullptr,type,3),"null entity rejected");
    const auto missing=source::FindProperty(image,&entity_table,"missing");
    expect(!missing.prop && missing.offset==-1,"missing receive property stays unavailable");
    glow_props[0].proxy=nullptr;
    expect(!source::SetInteger(image,&object,type,3),"missing proxy cannot be reported as applied");
    glow_props[0].proxy=reinterpret_cast<void*>(&GlowType);

    game::Api api;api.base.client_image=image;
    api.base.classes=reinterpret_cast<source::GetClasses>(&Classes);api.base.client=&mock_entity;
    expect(api.Resolve(),"mock client classes resolve");
    if(!api.classes.contains(&client_class)) return 1;
    auto& fields=api.classes.at(&client_class);
    expect(fields.melee_swing==0x340,"native melee swing state resolves from its named receive property");
    expect(fields.carry_victim==0x344,"Charger carry victim resolves by name rather than a copied Raze offset");
    {
        game::Api carry;carry.base.client_image=image;
        void* handles[5]{};handles[4]=reinterpret_cast<void*>(&FromHandle);
        VirtualObject list{handles};carry.base.entities=&list;
        unsigned raw=0x1001,victim=999;
        game::Fields carry_fields;carry_fields.carry_victim=0;
        game::Entity local;local.ptr=&raw;local.fields=&carry_fields;
        expect(carry.CarryVictim(local,victim) && victim==raw,"carry lookup preserves the full valid entity handle");
        raw=0x2001;expect(carry.CarryVictim(local,victim) && victim==0,"stale serial cannot target the reused entity index");
        raw=UINT_MAX;expect(carry.CarryVictim(local,victim) && victim==0,"invalid native handle reports no carry");
        raw=0;expect(carry.CarryVictim(local,victim) && victim==0,"zero handle reports no carry");
        raw=0x1001;handles[4]=nullptr;
        expect(!carry.CarryVictim(local,victim) && victim==0,"missing handle resolver is unknown rather than an observed release");
        carry_fields.carry_victim=-1;
        expect(!carry.CarryVictim(local,victim) && victim==0,"missing carry netvar fails closed");
        local.ptr=nullptr;expect(!carry.CarryVictim(local,victim),"missing local cannot access carry memory");
        void* channel_methods[75]{};channel_methods[74]=reinterpret_cast<void*>(&EngineChannel);
        VirtualObject engine{channel_methods};carry.base.engine=&engine;carry.engine_image=image;
        const auto saved_channel=state.channel;state.channel=&channel;
        expect(carry.NetworkChannelReady(),"Raze channel gate uses native slot 74");
        state.channel=nullptr;expect(!carry.NetworkChannelReady(),"null channel blocks carry pulse");
        state.channel=saved_channel;channel_methods[74]=nullptr;
        expect(!carry.NetworkChannelReady(),"missing channel getter blocks carry pulse");
        channel_methods[74]=reinterpret_cast<void*>(&EngineChannel);carry.engine_image={};
        expect(!carry.NetworkChannelReady(),"unknown engine image blocks channel calls");
    }
    expect(fields.fall_velocity==4604 && fields.ducked==4636,"Storm fields resolve through the nested m_Local table without fixed entity offsets");
    expect(fields.inventory==0x300 && fields.inventory_count==3,"weapon inventory resolves from a validated indexed RecvTable");
    expect(fields.client_ragdoll==0x200,"client-side ragdoll storage resolves by property name rather than a guessed offset");
    expect(source::FindField(image,&entity_table,"movetype")==0 && fields.move_type==-1,
        "zero-offset movetype receive proxy must not become a storage offset");
    vtable[16]=reinterpret_cast<void*>(&GetMap);
    game::Entity local{};local.ptr=&mock_entity;local.cc=&client_class;local.fields=&fields;
    api.ResolveMovement(local);
    expect(fields.move_type==0x144,"movement storage resolves from the 60-byte datamap descriptor");
    expect(fields.surface_friction==0x13c4,"surface friction resolves independently from the same validated datamap");
    {
        weapon_props[1].offset=12;game::Api bad_inventory=api;bad_inventory.classes.clear();bad_inventory.Resolve();
        expect(bad_inventory.classes.at(&client_class).inventory<0,"irregular inventory offsets fail closed");weapon_props[1].offset=4;
        entity_props[9].type=5;entity_props[9].stride=4;entity_props[9].elements=3;
        game::Api array_inventory=api;array_inventory.classes.clear();array_inventory.Resolve();
        expect(array_inventory.classes.at(&client_class).inventory_count==3,"native array inventory uses its validated four-byte handle stride");
        entity_props[9].elements=65;array_inventory.classes.clear();array_inventory.Resolve();
        expect(array_inventory.classes.at(&client_class).inventory<0,"oversized inventory declarations cannot cause unbounded scans");
        entity_props[9].type=6;
    }
    descriptions[0].offset=0;fields.move_type=-1;fields.data_resolved=false;
    api.ResolveMovement(local);
    expect(fields.move_type==-1,"invalid zero datamap offset remains unavailable");
    node.name="host_timescale"; node.parent=&host; host.value=1.f;
    cvar_vtable[12]=reinterpret_cast<void*>(&FindVar);
    api.cvars=&mock_cvars;
    float previous{};
    expect(api.SetConVarFloat("host_timescale",3.f,&previous) && previous==1.f && host.value==3.f,
        "convar write applies through the parent and captures the previous value");
    expect(!api.SetConVarFloat("other_cvar",1.f),"unknown convar fails closed");
    float air_value{};
    expect(api.ConVarFloat("host_timescale",air_value) && air_value==3.f && host.value==3.f,
           "readonly convar lookup validates the name and does not modify the native value");
    expect(!api.ConVarFloat("other_cvar",air_value),"readonly lookup rejects missing convars");
    host.value=std::numeric_limits<float>::quiet_NaN();
    expect(!api.ConVarFloat("host_timescale",air_value),"invalid convar values cannot enter air physics");
    host.value=3.f;
    interpolation_vars=true;
    expect(std::abs(api.Interpolation(1.f/30.f)-.01f)<.00001f,"interpolation uses configured update rate instead of simulation-rate fallback");
    interp_vars[4].value=60.f;
    expect(std::abs(api.Interpolation(1.f/30.f)-1.f/60.f)<.00001f,"replicated maximum update rate limits the effective interpolation rate");
    interp_vars[2].value=5.f;
    expect(std::abs(api.Interpolation(1.f/30.f)-2.f/60.f)<.00001f,"server interpolation-ratio maximum is respected");
    interp_vars[1].value=.1f;
    expect(std::abs(api.Interpolation(1.f/30.f)-.1f)<.00001f,"explicit cl_interp remains a lower bound");
    expect(std::isfinite(api.Interpolation(0.f)),"invalid interval cannot create nonfinite interpolation");
    interpolation_vars=false;node.name="host_timescale";host.value=3.f;
    channel.out_seq=1000; channel.in_seq=500; channel.ack=500; state.channel=&channel;
    api.client_state_getter=reinterpret_cast<void*>(&StateGetter);
    api.outgoing_command=offsetof(FakeState,outgoing);api.choked_commands=offsetof(FakeState,choked);
    api.acknowledged_command=offsetof(FakeState,acknowledged);
    state.outgoing=100;state.choked=6;state.acknowledged=90;
    game::CommandState command_state{};
    expect(api.Commands(command_state) && command_state.outgoing==100 && command_state.choked==6 && command_state.acknowledged==90,
           "command transport reads server ACK and queued commands without changing sequences");
    state.choked=-1;expect(!api.Commands(command_state),"negative choked-command count disables charged bursts");
    state.choked=0;state.acknowledged=101;
    expect(!api.Commands(command_state),"ACK beyond last outgoing command is rejected");
    state.acknowledged=90;api.acknowledged_command=-1;
    expect(!api.Commands(command_state),"unresolved command layout cannot enable rapidfire");
    api.acknowledged_command=offsetof(FakeState,acknowledged);
    api.delta_tick=offsetof(FakeState,delta);api.prediction=&prediction;
    prediction_vtable[3]=reinterpret_cast<void*>(&PredictionUpdate);
    state.delta=17;state.choked=6;
    expect(api.UpdatePrediction() && prediction_calls==1 && prediction_self==&prediction &&
           prediction_delta==17 && prediction_valid && prediction_ack==90 && prediction_outgoing==106,
           "Prediction slot 3 receives delta, valid flag, ACK, and outgoing plus choked commands");
    state.delta=0;
    expect(api.UpdatePrediction() && prediction_delta==0 && !prediction_valid,
           "a zero delta is forwarded as a full prediction update with validframe false");
    state.delta=-1;
    expect(api.UpdatePrediction() && prediction_delta==-1 && !prediction_valid,
           "Raze forwards the full-update sentinel without inventing a valid frame");
    state.delta=-2;const auto calls_before_invalid=prediction_calls;
    expect(!api.UpdatePrediction() && prediction_calls==calls_before_invalid,
           "an invalid delta cannot invoke native prediction");
    state.delta=17;state.acknowledged=101;
    expect(!api.UpdatePrediction() && prediction_calls==calls_before_invalid,
           "invalid command transport cannot invoke native prediction");
    state.acknowledged=90;api.delta_tick=-1;
    expect(!api.UpdatePrediction() && prediction_calls==calls_before_invalid,
           "an unresolved delta offset blocks prediction instead of using a hardcoded layout");
    api.delta_tick=offsetof(FakeState,delta);prediction_vtable[3]=nullptr;
    expect(!api.UpdatePrediction() && prediction_calls==calls_before_invalid,
           "a missing Update slot is detected without calling through a null pointer");
    prediction_vtable[3]=reinterpret_cast<void*>(&PredictionUpdate);
    rapid_fire::Ring command_ring;int remaining=-1;
    prediction_ring=&command_ring;prediction_command=1000;
    expect(rapid_fire::PrepareCommand(command_ring,remaining,prediction_command,8,true) &&
           api.UpdatePrediction() && prediction_shift==8,
           "native prediction observes the correction after the base command is armed");
    remaining=7;++prediction_command;
    expect(!rapid_fire::PrepareCommand(command_ring,remaining,prediction_command,8,true) &&
           api.UpdatePrediction() && prediction_shift==0 && command_ring.Get(1000)==8,
           "an extra command updates prediction without clearing the base correction or rearming");
    remaining=-1;++prediction_command;
    const auto before_release_calls=prediction_calls;
    expect(!rapid_fire::PrepareCommand(command_ring,remaining,prediction_command,8,false) &&
           !rapid_fire::ManualPrediction(true,false,&mock_entity) && prediction_calls==before_release_calls && command_ring.Get(1000)==8,
           "release preserves unacknowledged history without a second manual prediction update");
    command_ring.Retire(1000);
    expect(command_ring.Empty()&&command_ring.Get(1000)==0,"server confirmation retires the last shift immediately after release");
    command_ring={};remaining=-1;prediction_command=1000;
    const int boosted=rapid_fire::BurstMoves(200,1.f/60.f,true);
    rapid_fire::PrepareCommand(command_ring,remaining,prediction_command,boosted,true);
    bool retained_during_replay=true;
    for(int extra=1;extra<=boosted;++extra) {
        rapid_fire::PrepareCommand(command_ring,remaining,1000+extra,boosted,true);
        retained_during_replay&=api.UpdatePrediction()&&prediction_shift==boosted;
    }
    expect(boosted==800&&retained_during_replay,
           "native prediction retains the 4x base correction through more than five command-ring wraps");
    const auto boosted_release_calls=prediction_calls;
    remaining=-1;
    rapid_fire::PrepareCommand(command_ring,remaining,1001+boosted,boosted,false);
    expect(command_ring.Get(1000)==boosted&&!rapid_fire::ManualPrediction(true,false,&mock_entity)&&prediction_calls==boosted_release_calls,
           "boosted release keeps needed history without invoking manual prediction again");
    command_ring.Retire(1000+boosted);
    expect(command_ring.Empty(),"the boosted native prediction history retires on server confirmation");
    std::memset(prediction.state,0x5a,sizeof(prediction.state));
    prediction_vtable[7]=reinterpret_cast<void*>(&PredictionReset);
    const auto before_reset_calls=prediction_calls;
    const bool reset_ok=api.ResetPredictionHistory();
    int previous_frame{},predicted_count{},server_acked{},history_a{},history_b{};
    memory::Field(&prediction,0x0c,previous_frame);memory::Field(&prediction,0x1c,predicted_count);
    memory::Field(&prediction,0x20,server_acked);memory::Field(&prediction,0x30,history_a);memory::Field(&prediction,0x34,history_b);
    expect(reset_ok&&previous_frame==-1&&predicted_count==0&&server_acked==0&&history_a==0&&history_b==0&&
           prediction_calls==before_reset_calls&&prediction.state[0x24]==0x5a,
           "verified native reset clears prediction history without invoking another Update or overwriting unrelated state");
    prediction_vtable[7]=reinterpret_cast<void*>(&PredictionUpdate);
    const auto untouched=prediction;
    expect(!api.ResetPredictionHistory()&&std::memcmp(&untouched,&prediction,sizeof(prediction))==0,
           "a different slot-seven implementation is rejected before invocation");
    prediction_vtable[7]=nullptr;
    expect(!api.ResetPredictionHistory(),"an unresolved native reset is rejected without guessing object offsets");
    prediction_ring=nullptr;api.prediction=nullptr;state.choked=0;
    expect(api.ShiftSequence(100) && channel.out_seq==1100,"sequence push advances out sequence");
    {
        channel.out_seq=1000;channel.in_seq=500;channel.ack=500;
        auto expected=channel;expected.out_seq+=30300;
        expect(api.StormSequence(storm_fall::SequenceShift(30000)) && !std::memcmp(&channel,&expected,sizeof(channel)),
            "Storm accepts its tickbase-derived shift and changes only the outgoing network sequence");
        expect(!api.StormSequence(0) && !api.StormSequence(-1) && !api.StormSequence(storm_fall::MaxTickbase+451),"invalid Storm shifts leave channel state untouched");
        channel.out_seq=0x3fffffff-10;
        expect(!api.StormSequence(450) && channel.out_seq==0x3fffffff-10,"large Storm shifts cannot overflow the validated sequence range");
        state.channel=nullptr;expect(!api.StormSequence(450),"Storm cannot write a missing network channel");state.channel=&channel;channel.out_seq=1000;
        struct PlayerClock {int cookie{77};int tick{30000};int tail{88};} player;
        game::Globals globals{1,2,.01f,1000.f,.02f,4,10,1.f/30.f,.1f};
        const auto initial_globals=globals;
        game::StormClock clock;
        expect(clock.Begin(&player,offsetof(PlayerClock,tick),&globals,30300) && player.tick==-300 && player.cookie==77 && player.tail==88,
            "Storm subtracts the command's recorded shift before native RunCommand");
        expect(!clock.Begin(&player,offsetof(PlayerClock,tick),&globals,450),"a nested begin cannot apply the same shift twice");
        ++player.tick;globals.time=player.tick*globals.interval;
        expect(clock.Restore() && player.tick==30001 && std::fabs(globals.time-1000.f)<.001f,
            "native tick advancement survives shift restoration and globals use the restored clock");
        auto wanted=initial_globals;wanted.time=globals.time;
        expect(!std::memcmp(&globals,&wanted,sizeof(globals)) && clock.Restore() && player.tick==30001,
            "restoring twice is harmless and leaves frame interval interpolation and unrelated clock fields unchanged");
        bool steady=true;
        for(int n=0;n<100;++n) {
            game::StormClock replay;const int before=player.tick;
            const int shift=storm_fall::SequenceShift(before);
            steady&=replay.Begin(&player,offsetof(PlayerClock,tick),&globals,shift);
            ++player.tick;steady&=replay.Restore() && player.tick==before+1;
        }
        expect(steady && player.tick==30101,"successive native predictions never accumulate Storm clock debt");
        const int before=player.tick;
        expect(!clock.Begin(nullptr,4,&globals,450) && !clock.Begin(&player,-1,&globals,450) &&
            !clock.Begin(&player,offsetof(PlayerClock,tick),&globals,0) && player.tick==before,"invalid correction contexts cannot alter player tickbase");
        globals.interval=0;
        expect(!clock.Begin(&player,offsetof(PlayerClock,tick),&globals,450),"paused clocks reject Storm simulation before writing tickbase");globals.interval=1.f/30.f;
        player.tick=-300;
        expect(!clock.Begin(&player,offsetof(PlayerClock,tick),&globals,450) && player.tick==-300,"an already borrowed negative clock cannot receive another Storm subtraction");player.tick=before;
        {game::StormClock scoped;scoped.Begin(&player,offsetof(PlayerClock,tick),&globals,storm_fall::SequenceShift(player.tick));++player.tick;}
        expect(player.tick==before+1,"scope exit restores a borrowed Storm tickbase");
        struct MoveInput {unsigned char head[36];int buttons;int preserved;float forward,side;int tail;} move{};
        std::memset(&move,0x5a,sizeof(move));move.buttons=0x7fffffff;move.forward=450.f;move.side=-450.f;
        auto expected_move=move;expected_move.buttons=storm_fall::MovementButtons(move.buttons,true);expected_move.forward=expected_move.side=0.f;
        expect(game::StormMovementInput(&move,true) && !std::memcmp(&move,&expected_move,sizeof(move)),
            "negative-clock movement clears the source mask and forward/side input while preserving every other byte");
        move.forward=std::numeric_limits<float>::quiet_NaN();const auto invalid_move=move;
        expect(!game::StormMovementInput(nullptr,false) && !game::StormMovementInput(&move,false) && !std::memcmp(&move,&invalid_move,sizeof(move)),
            "invalid native move buffers never receive partial movement edits");
    }
    {
        using lag_exploit::Method;
        api.engine_image=image;api.base.engine=&engine_fixture;engine_vtable[74]=reinterpret_cast<void*>(&EngineChannel);
        channel.out_seq=1000;channel.in_seq=500;channel.ack=500;
        const auto unchanged=channel;
        const auto v1=api.RazeLag(Method::V1,100);
        expect(v1.available && v1.shifted && v1.force_send && channel.out_seq==1100,
            "Raze V1 uses the verified native engine getter and requests current-packet sending");
        auto expected=unchanged;expected.out_seq=1100;
        expect(!std::memcmp(&expected,&channel,sizeof(channel)),"V1 changes only the outgoing sequence and preserves ACK choke and bandwidth deadline");
        channel=unchanged;
        const auto v2=api.RazeLag(Method::V2,200);
        expect(v2.available && v2.shifted && !v2.force_send && channel.out_seq==1200,
            "Raze V2 uses the matching client-state channel and leaves sendPacket to the native policy");
        channel=unchanged;
        expect(!api.RazeLag(Method::V1,0).shifted && !api.RazeLag(Method::V2,-100).shifted && !api.RazeLag(Method::V1,1001).shifted && channel.out_seq==1000,
            "invalid sequence factors fail before modifying the channel");
        state.channel=nullptr;
        const auto missing_v1=api.RazeLag(Method::V1,100),missing_v2=api.RazeLag(Method::V2,100);
        expect(missing_v1.available && !missing_v1.shifted && !missing_v1.force_send,
            "missing V1 channel cannot cause a null pointer write");
        expect(missing_v2.available && !missing_v2.shifted && missing_v2.force_send,
            "Raze V2 null-channel branch preserves its original force-send request");
        state.channel=&channel;
        engine_vtable[74]=reinterpret_cast<void*>(&StateGetter);
        expect(!api.RazeLag(Method::V1,100).available && channel.out_seq==1000,
            "an unrelated slot-74 method is rejected before invocation");
        engine_vtable[74]=reinterpret_cast<void*>(&EngineChannel);
        api.client_state_getter=nullptr;
        expect(!api.RazeLag(Method::V2,100).available && channel.out_seq==1000,
            "V2 requires the independently validated client-state accessor");
        api.client_state_getter=reinterpret_cast<void*>(&StateGetter);
        channel.out_seq=0x3fffffff-50;
        expect(!api.RazeLag(Method::V1,100).shifted && channel.out_seq==0x3fffffff-50,
            "sequence push cannot overflow the supported native sequence range");
        channel=unchanged;api.base.engine=nullptr;
        expect(!api.RazeLag(Method::V1,100).available,"missing native engine interface leaves Raze lag unavailable");
    }
    channel.out_seq=-1;
    expect(!api.ShiftSequence(100) && channel.out_seq==-1,"non-sequence layout is rejected");
    channel.out_seq=1000;
    channel.ack=123;channel.choke=7;
    int channel_ack{},channel_choked{};
    expect(api.Channel(channel_ack,channel_choked) && channel_ack==123 && channel_choked==7,
           "channel accessor reads the outgoing ACK and the retained-packet count");
    expect(api.SetChannelAck(-1) && channel.ack==-1,
           "Storm/Methot 3 set the outgoing ACK field to -1");
    expect(api.SetChannelChoked(255) && channel.choke==255,
           "Storm/Methot 3 declare 255 retained packets");
    expect(api.Channel(channel_ack,channel_choked) && channel_ack==-1 && channel_choked==255,
           "the -1 ACK and 255 retained packets stay readable after a Storm write");
    expect(!api.SetChannelAck(-2) && channel.ack==-1,
           "an ACK below -1 is rejected");
    expect(!api.SetChannelChoked(256) && !api.SetChannelChoked(-1) && channel.choke==255,
           "retained packets stay within the serialized byte");
    channel.choke=300;
    expect(!api.Channel(channel_ack,channel_choked),
           "an out-of-range retained count disables the channel view");
    channel.choke=7;
    state.channel=nullptr;
    expect(!api.Channel(channel_ack,channel_choked) && !api.SetChannelAck(-1) && !api.SetChannelChoked(255),
           "an unresolved channel fails closed");
    state.channel=&channel;
    game::ChannelPatch channel_patch;
    channel.ack=123;channel.choke=7;
    expect(channel_patch.Apply(api,true,-1,255)&&channel.ack==-1&&channel.choke==255,
           "native rapidfire channel declarations apply inside their movement call");
    channel_patch.Restore(api);
    expect(channel.ack==123&&channel.choke==7,"unused rapidfire channel declarations do not survive the movement call");
    expect(channel_patch.Apply(api,true,-1,255),"the next movement call snapshots a fresh channel state");
    channel.ack=124;channel.choke=0;channel_patch.Restore(api);
    expect(channel.ack==124&&channel.choke==0,"packet receipt and SendDatagram updates survive channel restoration");
    expect(channel_patch.Apply(api,false,0,15)&&channel.ack==124&&channel.choke==15,"the choke method never rewrites the outgoing ACK");
    channel_patch.Restore(api);
    expect(channel.ack==124&&channel.choke==0,"the choke method restores its retained-packet declaration on release");
    for(int method=0;method<3;++method) {
        channel.ack=124;channel.choke=7;
        const int before_sequence=channel.out_seq;
        channel_patch.Apply(api,method!=1,-1,method==1?15:255);
        ++channel.out_seq;++channel.choke; // Native SetChoked without a datagram.
        channel_patch.Restore(api);
        expect(channel.ack==124&&channel.choke==8&&channel.out_seq==before_sequence+1,
               "SetChoked retains one real queued packet without leaking the synthetic 255-to-256 declaration");
    }
    channel_patch.Apply(api,true,-1,255);
    FakeNet replacement{};replacement.ack=800;replacement.choke=3;state.channel=&replacement;
    channel_patch.Restore(api);
    expect(replacement.ack==800&&replacement.choke==3&&channel.ack==-1,"reconnecting never writes an old channel snapshot into a new owner");
    state.channel=&channel;channel.ack=123;channel.choke=0;
    for(int method=0;method<3;++method) {
        bool clean=true;
        for(int frame=0;frame<120;++frame) {
            const int expected_ack=channel.ack;
            clean&=channel_patch.Apply(api,method!=1,-1,method==1?15:255);
            if(frame%7)channel.choke=0; // Native datagram sent; some frames lose their send opportunity.
            channel_patch.Restore(api);
            clean&=channel.ack==expected_ack&&channel.choke==0;
            ++channel.ack;
        }
        expect(clean,"two-second holds of each network method leave no artificial ACK or choke state on release");
    }
    ScheduleFixture();channel.vtbl=net_vtable;api.engine_image=memory::Module(GetModuleHandleW(nullptr));
    game::ChannelMoveBudget move_budget;
    net_clock=100.;channel.deadline=100.005;double delay{};
    expect(move_budget.Delay(api,delay)&&std::abs(delay-.005)<1.e-9,
           "native send and CanPacket instructions resolve one shared pacing deadline and clock");
    expect(move_budget.Begin(api),"rapid extras snapshot the post-base bandwidth charge");
    channel.deadline=104.;const auto before_net=channel;
    expect(std::abs(move_budget.Restore(api)-3.995)<1.e-9&&std::abs(channel.deadline-100.005)<1.e-9,
           "a four-second synthetic send delay is removed while the ordinary move charge survives");
    FakeNet expected_net=before_net;expected_net.deadline=100.005;
    expect(std::memcmp(&channel,&expected_net,sizeof(channel))==0,
           "pacing restoration leaves every sequence, ACK, choke, rate and other channel byte intact");
    expect(move_budget.Restore(api)==0.&&channel.deadline==expected_net.deadline,"a budget snapshot is consumed once");
    channel.deadline=99.;move_budget.Begin(api);net_clock=100.25;channel.deadline=104.;move_budget.Restore(api);
    expect(channel.deadline==net_clock,"real elapsed time is preserved instead of restoring an old native clock");
    channel.deadline=100.3;move_budget.Begin(api);channel.deadline=0.;move_budget.Restore(api);
    expect(channel.deadline==0.,"a native channel clear or lower deadline is never undone");
    channel.deadline=101.;move_budget.Begin(api);move_budget.Restore(api);
    expect(channel.deadline==101.,"a move that adds no pacing charge stays unchanged");
    move_budget.Begin(api);channel.deadline=102.;net_clock=99.;move_budget.Restore(api);
    expect(channel.deadline==102.,"clock rollback cancels pacing restoration");
    net_clock=100.;channel.deadline=100.01;move_budget.Begin(api);
    replacement.vtbl=net_vtable;replacement.deadline=103.;state.channel=&replacement;move_budget.Restore(api);
    expect(replacement.deadline==103.&&channel.deadline==100.01,"channel replacement cannot receive an earlier owner's deadline");
    state.channel=&channel;move_budget.Begin(api);channel.deadline=103.;channel.vtbl=nullptr;move_budget.Restore(api);
    expect(channel.deadline==103.,"a changed channel vtable cancels an outstanding pacing write");
    channel.vtbl=net_vtable;channel.deadline=std::numeric_limits<double>::quiet_NaN();
    expect(!move_budget.Begin(api),"non-finite pacing fields fail closed");
    channel.deadline=std::numeric_limits<double>::infinity();
    expect(!move_budget.Begin(api),"infinite pacing fields fail closed");
    channel.deadline=-1.;expect(!move_budget.Begin(api),"negative deadlines are not writable");
    channel.deadline=161.;expect(!move_budget.Begin(api),"implausible future deadlines are not writable");
    channel.deadline=100.;net_clock=std::numeric_limits<double>::quiet_NaN();
    expect(!move_budget.Begin(api),"a non-finite native clock is not writable");net_clock=100.;
    state.channel=nullptr;expect(!move_budget.Begin(api),"an absent channel disables pacing restoration");state.channel=&channel;
    const auto engine_range=api.engine_image;api.engine_image={};
    expect(!move_budget.Begin(api),"unresolved engine image cannot authorize an offset write");api.engine_image=engine_range;
    for(int operand:{2,20,36,42,48}) {
        ScheduleFixture();send_schedule[0x64e+operand]^=8;move_budget.Clear();
        expect(!move_budget.Begin(api),"all send operands and the rate field must match the CanPacket deadline");
    }
    ScheduleFixture();can_schedule[0x45+2]^=8;move_budget.Clear();
    expect(!move_budget.Begin(api),"a different CanPacket deadline rejects the send layout");
    ScheduleFixture();can_schedule[0x45+8]^=8;move_budget.Clear();
    expect(!move_budget.Begin(api),"different native clock operands reject the layout");
    ScheduleFixture();const double* foreign_clock=reinterpret_cast<const double*>(1);
    std::memcpy(send_schedule+0x64e+8,&foreign_clock,4);std::memcpy(can_schedule+0x45+8,&foreign_clock,4);move_budget.Clear();
    expect(!move_budget.Begin(api),"matching clock operands outside the engine image are rejected");
    ScheduleFixture();send_schedule[0x64e+16]^=1;move_budget.Clear();
    expect(!move_budget.Begin(api),"a changed native instruction never enables a guessed pacing write");
    ScheduleFixture();std::memcpy(send_schedule,send_schedule+0x64e,52);move_budget.Clear();
    expect(!move_budget.Begin(api),"ambiguous native send instructions are rejected");
    ScheduleFixture();std::memcpy(can_schedule,can_schedule+0x45,27);move_budget.Clear();
    expect(!move_budget.Begin(api),"ambiguous native CanPacket instructions are rejected");
    ScheduleFixture();net_vtable[57]=nullptr;move_budget.Clear();
    expect(!move_budget.Begin(api),"both native methods are required to recover the send budget");
    ScheduleFixture();net_vtable[47]=reinterpret_cast<void*>(1);move_budget.Clear();
    expect(!move_budget.Begin(api),"foreign virtual methods cannot authorize a pacing write");
    ScheduleFixture();move_budget.Clear();
    for(int method=0;method<3;++method)for(int extras:{400,800}) {
        bool clean=true;net_clock=100.;channel.deadline=100.;
        for(int frame=0;frame<120;++frame) {
            net_clock+=1./60.;
            channel.deadline=std::max(channel.deadline,net_clock)+.005; // Real base packet.
            const double ordinary_charge=channel.deadline;
            clean&=move_budget.Begin(api);
            for(int extra=0;extra<extras;++extra) {
                channel_patch.Apply(api,method!=1,-1,method==1?15:255);
                channel.deadline=std::min(std::max(channel.deadline,net_clock)+.01,net_clock+4.);
                ++channel.out_seq;channel.choke=0;channel_patch.Restore(api);
            }
            move_budget.Restore(api);clean&=channel.deadline==ordinary_charge;
        }
        clean&=move_budget.Delay(api,delay)&&delay<.006;
        expect(clean,"two-second 400/800-extra holds preserve all three methods without a four-second pacing tail");
    }
    channel.vtbl=nullptr;
    node.name="changed"; host.value=1.f;
    expect(!api.SetConVarFloat("host_timescale",2.f),"mismatched convar name is rejected");

    model_vtable[30]=reinterpret_cast<void*>(&Studio);render_vtable[8]=reinterpret_cast<void*>(&Model);
    render_vtable[13]=reinterpret_cast<void*>(&SetupBones);api.model_info=&model_info;
    studio.bones[0].parent=-1;
    studio.bones[1].parent=0;studio.bones[1].flags=0x100;
    studio.bones[2].parent=1;studio.bones[2].flags=0x100;
    studio.bones[3].parent=2;studio.bones[3].flags=0x400;
    studio.bones[4].parent=1;
    studio.bones[5].parent=4;studio.bones[5].flags=0x100;
    game::Entity actor{};actor.renderable=&renderable;actor.origin={10,10,0};
    std::array<source::BoneSegment,128> segments{};
    const auto bone_count=api.Skeleton(actor,segments,1.f);
    expect(bone_count==3,"skeleton excludes scene root and bridges helper bones");
    expect(requested_mask==0x500,"toe extensions request bone-merge matrices from SetupBones");
    expect(bone_count>=2 && segments[1].start.x==20 && segments[1].end.z==10,"toe has real animated endpoints");
    expect(api.Skeleton(actor,segments,2.f)==3 && requested_mask==0x500,"cached topology retains the required mask");
    actor.origin={2000,2000,2000};
    expect(api.Skeleton(actor,segments,3.f)==0,"stale pose after teleport is discarded");
    actor.origin={10,10,0};studio.checksum++;studio.bones[1].parent=5;
    expect(api.Skeleton(actor,segments,4.f)==0,"cyclic skeleton topology fails closed");
    struct HitboxStudio {
        int id{0x54534449},version{48},checksum{1};char name[64]{};int length{sizeof(HitboxStudio)};
        unsigned char padding[76]{};int bones{6},bone_offset{},controllers{},controller_offset{},sets{1},set_offset{180};
        struct Set {int name{},count{1},offset{12};} set;
        game::Hitbox box{5,1,{-4,-5,-6},{4,5,6}};
    } hitbox_studio;
    static_assert(offsetof(HitboxStudio,set)==180 && offsetof(HitboxStudio,box)==192);
    active_model=&hitbox_studio;game::Fields actor_fields{};actor.fields=&actor_fields;
    std::vector<game::Point> aim_points;
    expect(api.Hitboxes(actor,aim_points,5.f) && aim_points.size()==7,"native hitbox extraction provides center and six inset face samples");
    expect(!aim_points.empty() && aim_points[0].radius==4.f && aim_points[0].position.z==60.f,
           "center firing geometry uses the actual smallest hitbox half extent");
    expect(aim_points.size()>1 && std::abs(aim_points[1].radius-.8f)<.00001f,
           "edge firing geometry reserves only the remaining inset radius");
    active_model=&studio;

    render_vtable[17]=reinterpret_cast<void*>(&VisualBounds);render_vtable[32]=reinterpret_cast<void*>(&VisualTransform);
    actor.origin={100,20,10};std::array<source::Vec3,8> visual_corners{};
    expect(api.RenderBounds(actor,visual_corners)&&visual_corners[0].x==102&&visual_corners[0].y==-20&&visual_corners[0].z==7,
           "world bounds use local render geometry and the full native model transform");
    expect(visual_corners[7].x==94&&visual_corners[7].y==30&&visual_corners[7].z==14,
           "asymmetric weapon bounds retain all rotated model corners");
    visual_transform.m[0][3]=105;
    expect(api.RenderBounds(actor,visual_corners)&&visual_corners[0].x==107,"moving models refresh their matrix instead of reusing old corners");
    visual_transform.m[0][3]=100;write_visual_bounds=false;
    expect(!api.RenderBounds(actor,visual_corners)&&visual_corners[0].x==0,"unwritten native bounds clear previous geometry");
    write_visual_bounds=true;return_visual_transform=false;
    expect(!api.RenderBounds(actor,visual_corners),"missing native render transform cannot produce an approximate collision box");
    return_visual_transform=true;visual_transform.m[0][0]=NAN;
    expect(!api.RenderBounds(actor,visual_corners),"nonfinite native transform is rejected");visual_transform.m[0][0]=0;
    active_model=nullptr;expect(!api.RenderBounds(actor,visual_corners),"effects without a model do not invent model bounds");active_model=&studio;
    render_vtable[17]=nullptr;expect(!api.RenderBounds(actor,visual_corners),"missing render-bounds slot fails closed");
    render_vtable[17]=reinterpret_cast<void*>(&VisualBounds);

    const auto approx=[](float a,float b){return std::abs(a-b)<.001f;};
    auto radar=radar::Project(100,0,0,200);
    expect(radar.valid && approx(radar.x,0) && approx(radar.y,-.5f),"radar puts forward targets above the player");
    radar=radar::Project(0,100,0,200);
    expect(approx(radar.x,-.5f) && approx(radar.y,0),"radar preserves Source left/right handedness");
    radar=radar::Project(100,0,90,200);
    expect(approx(radar.x,.5f) && approx(radar.y,0),"north-up radar places world +X to the right");
    radar=radar::Project(1000,1000,0,200);
    expect(approx(std::hypot(radar.x,radar.y),1.f),"radar clips markers to its circular boundary");
    expect(!radar::Project(0,0,0,0).valid && !radar::Project(std::numeric_limits<float>::quiet_NaN(),0,0,100).valid,
           "invalid radar coordinates never reach rendering");

    expect(weapons::Find("CAssaultRifle")>=0 && weapons::Catalog[weapons::Find("CAssaultRifle")].group==2,
           "M16 uses the actual installed client class");
    expect(weapons::Find("CPistol",{},true)!=weapons::Find("CPistol"),"dual and single pistols have separate profiles");
    expect(weapons::Find("CTerrorMeleeWeapon","models/weapons/melee/v_katana.mdl")==weapons::Find("CTerrorMeleeWeapon","katana"),
           "melee profiles resolve by model");
    expect(weapons::Find("unknown")==-1,"unknown weapons inherit global settings");
    expect(weapons::Find("CTerrorMeleeWeapon","models/weapons/melee/w_bat.mdl")==weapons::Find("CTerrorMeleeWeapon","baseball_bat") &&
           weapons::Find("CTerrorMeleeWeapon","w_bat.mdl")!=weapons::Find("CTerrorMeleeWeapon","w_cricket_bat.mdl"),
           "baseball bat model alias does not overlap the cricket bat profile");
    features::Config config{};const int ak=weapons::Find("CRifle_AK47");
    config.aim=true;config.weapons[ak].fov=10.f;
    features::ApplyWeaponProfile(config,ak);
    expect(config.fov==180.f,"disabled weapon override inherits global combat settings");
    config.weapons[ak].combat=true;features::ApplyWeaponProfile(config,ak);
    expect(config.fov==10.f && config.aim,"active weapon override applies combat settings");
    config.aim=false;features::ApplyWeaponProfile(config,ak);
    expect(!config.aim,"weapon profile cannot bypass the main aimbot switch");
    const int melee=weapons::Find("CTerrorMeleeWeapon","katana");
    config.weapons[melee].combat=true;config.weapons[melee].fov=2.f;
    config.weapons[melee].view=true;config.weapons[melee].vm_up=7.f;
    features::ApplyWeaponProfile(config,melee);
    expect(config.fov==10.f && config.vm_up==7.f && config.viewmodel && config.vm_fov,
           "melee supports viewmodel overrides without activating hitscan combat");

    observer_props[0].name="m_iObserverMode";observer_props[0].offset=offsetof(Observer,mode);
    observer_props[1].name="m_hObserverTarget";observer_props[1].offset=offsetof(Observer,target);
    observer_table.props=observer_props;observer_table.count=2;observer_class.table=&observer_table;
    observer_vtable[1]=reinterpret_cast<void*>(&ObserverClass);entity_vtable[4]=reinterpret_cast<void*>(&FromHandle);
    source::Interfaces observer_api{};observer_api.client_image=image;observer_api.engine=&mock_entity;observer_api.entities=&entity_list;
    observer_api.in_game=reinterpret_cast<source::GetBool>(&InGame);
    observer_api.local_player=reinterpret_cast<source::GetInt>(&LocalIndex);
    observer_api.max_clients=reinterpret_cast<source::GetInt>(&MaxClients);
    observer_api.entity=reinterpret_cast<source::GetEntity>(&GetEntity);
    observer_api.networkable=reinterpret_cast<source::GetEntity>(&GetNetwork);
    observer_api.player_info=reinterpret_cast<source::GetInfo>(&PlayerInfo);
    observers[2].mode=4;observers[2].target=0x1001;
    observers[3].mode=5;observers[3].target=0x1001;
    observers[4].mode=6;observers[4].target=0x1001;
    observers[5].mode=4;observers[5].target=0x2001;
    spectators::Initialize(observer_api);spectators::SetEnabled(true);spectators::SetChase(true);spectators::Frame(4);
    auto roster=spectators::ReadSnapshot();
    expect(roster.count==2,"spectators accepts first-person and chase, rejects free camera and stale handles");
    expect(roster.count==2 && roster.entries[0].userid!=roster.entries[1].userid,"duplicate spectator names retain distinct identities");
    spectators::SetChase(false);spectators::Frame(4);roster=spectators::ReadSnapshot();
    expect(roster.count==1 && roster.entries[0].mode==4,"first-person-only filter removes chase observers immediately");
    observers[2].target=0x2001;spectators::ClearLevel();spectators::Frame(4);
    expect(spectators::ReadSnapshot().count==0,"observer switching target is removed without a grace period");
    connected_game=false;spectators::ClearLevel();spectators::Frame(4);
    expect(spectators::ReadSnapshot().state==spectators::State::NotInGame,"spectator roster clears on leaving a match");
    std::printf("%u runtime API regression checks, %u failed\n",checks,failures);
    return failures?1:0;
}
