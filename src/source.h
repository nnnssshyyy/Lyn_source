#pragma once
#include "memory.h"
#include <array>
#include <unordered_map>

namespace source {
struct Vec3 { float x{}, y{}, z{}; };
struct BoneSegment { Vec3 start{},end{}; };
struct Matrix { float m[4][4]{}; };
struct RecvTable;
struct RecvProp {
    const char* name;
    int type, flags, string_size;
    bool inside_array;
    const void* extra;
    RecvProp* array_prop;
    void* array_length_proxy;
    void* proxy;
    void* table_proxy;
    RecvTable* table;
    int offset, stride, elements;
    const char* parent_name;
};
struct RecvTable {
    RecvProp* props;
    int count;
    void* decoder;
    const char* name;
    bool initialized, in_list;
};
struct ClientClass {
    void* create;
    void* create_event;
    const char* name;
    RecvTable* table;
    ClientClass* next;
    int id;
};
// The container is the parent RecvTable's base, which can differ from the
// entity and the destination field (for example CGlowProperty inside it).
struct Property {
    const RecvProp* prop{};
    int container{-1},offset{-1};
};
struct RecvProxyData {
    const RecvProp* prop{};
    union { int integer; float vector[3]; } value{};
    int type{},element{},object_id{};
};
static_assert(sizeof(RecvProxyData)==28 && offsetof(RecvProxyData,value)==4);
struct PlayerInfo {
    unsigned char header[8];
    char name[32];
    int userid;
    unsigned char data[0x150];
};
static_assert(sizeof(RecvProp) == 0x3c);
static_assert(offsetof(RecvProp, table) == 0x28);
static_assert(offsetof(RecvProp, offset) == 0x2c);
static_assert(sizeof(RecvTable) == 20);
static_assert(sizeof(ClientClass) == 24);
static_assert(offsetof(PlayerInfo, name) == 8 && sizeof(PlayerInfo) == 0x17c);

enum class Kind : unsigned char { None, Player, Survivor, Smoker, Boomer, Hunter, Spitter, Jockey, Charger, Witch, Tank, Common };
struct Fields {
    int origin{-1}, mins{-1}, maxs{-1}, team{-1}, health{-1}, life{-1}, zombie{-1}, max_health{-1};
};
struct ClassInfo { Kind kind{Kind::None}; Fields fields{}; };
using GetClasses = ClientClass*(__thiscall*)(void*);
using GetInt = int(__thiscall*)(void*);
using GetBool = bool(__thiscall*)(void*);
using GetEntity = void*(__thiscall*)(void*, int);
using GetInfo = bool(__thiscall*)(void*, int, PlayerInfo*);
// The engine returns a const reference: on x86 that is one pointer in EAX.
using GetMatrix = const Matrix*(__thiscall*)(void*);
using GetClass = ClientClass*(__thiscall*)(void*);
using GetObject = void*(__thiscall*)(void*);
using GetOrigin = const Vec3*(__thiscall*)(void*);
using FrameStage = void(__thiscall*)(void*, int);
using Cursor = void(__thiscall*)(void*);

struct Interfaces {
    void* client{};
    void* engine{};
    void* entities{};
    void* surface{};
    memory::Range client_image{};
    GetClasses classes{};
    GetInt local_player{}, max_clients{}, highest_entity{}, entity_capacity{};
    GetBool in_game{};
    GetEntity entity{}, networkable{};
    GetInfo player_info{};
    GetMatrix matrix{};
    FrameStage frame_stage{};
    Cursor unlock_cursor{}, lock_cursor{};
};

bool Bind(Interfaces& result);
bool ResolveClasses(const Interfaces& interfaces, std::unordered_map<ClientClass*, ClassInfo>& result,
                    unsigned& targets, unsigned& skipped);
int FindField(memory::Range image, RecvTable* table, const char* name) noexcept;
Property FindProperty(memory::Range image, RecvTable* table, const char* name) noexcept;
bool SetInteger(memory::Range image,void* object,Property field,int value) noexcept;
const char* Label(Kind kind) noexcept;
Kind ZombieKind(int zombie_class) noexcept;
bool Finite(Vec3 value) noexcept;
}

