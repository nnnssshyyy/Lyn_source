#include "chat_spy.h"
#include "license.h"
#include "log.h"
#include <MinHook.h>
#include <atomic>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <mutex>

namespace chat_spy {
namespace {
const game::Api* api{};
void* event_manager{};
memory::Range engine_image{};
// MSVC x86 member varargs: cdecl, with this explicitly on the stack.
using ChatPrintf=void(__cdecl*)(void*,int,int,const char*,...);
using GetHud=void*(__cdecl*)(int);
using FindHud=void*(__thiscall*)(void*,const char*);
ChatPrintf original_print{};
GetHud get_hud{};FindHud find_hud{};
bool print_hook{},registered{},was_enabled{};
std::atomic_bool accepting{false},clear_requested{false};
std::mutex shared_mutex;
Options options{};Snapshot published{};
unsigned generation{};
logic::History history;
struct RawEvent {int userid{};ULONGLONG time{};char text[512]{};};
struct RawHud {int index{};ULONGLONG time{};char text[2048]{};};
std::deque<RawEvent> events;
std::deque<RawHud> hud_messages;
unsigned queue_drops{};
thread_local bool inside_print{};

bool CopyString(const char* value,char* out,size_t capacity) noexcept {
    if(!value || !out || capacity<2) return false;
    for(size_t i=0;i<capacity-1;++i) {
        if(!memory::Read(value+i,out[i])) {out[0]=0;return false;}
        if(!out[i]) return true;
    }
    out[capacity-1]=0;return true;
}
bool Format(char* out,size_t capacity,const char* format,va_list args) noexcept {
    if(!format) return false;
    __try {_vsnprintf_s(out,capacity,_TRUNCATE,format,args);return true;}
    __except(memory::FaultFilter(GetExceptionCode())) {out[0]=0;return false;}
}
class Listener {
public:
    virtual ~Listener()=default;
    virtual void FireGameEvent(void* event) noexcept {
        if(!licensing::Active() || !accepting.load(std::memory_order_acquire) || !event) return;
        using Name=const char*(__thiscall*)(void*);
        using Int=int(__thiscall*)(void*,const char*,int);
        using Text=const char*(__thiscall*)(void*,const char*,const char*);
        const char* name{};char label[48]{};
        if(!memory::Invoke(memory::Virtual<Name>(event,1,engine_image),name,event) ||
           !CopyString(name,label,sizeof(label)) || std::strcmp(label,"player_say")) return;
        RawEvent raw{};const char* text{};
        if(!memory::Invoke(memory::Virtual<Int>(event,6,engine_image),raw.userid,event,"userid",0) || raw.userid<=0 ||
           !memory::Invoke(memory::Virtual<Text>(event,9,engine_image),text,event,"text","") ||
           !CopyString(text,raw.text,sizeof(raw.text)) || !raw.text[0]) return;
        raw.time=GetTickCount64();
        try {
            std::lock_guard lock(shared_mutex);
            if(!accepting.load(std::memory_order_relaxed)) return;
            if(events.size()==logic::HistoryLimit) {events.pop_front();++queue_drops;}
            events.push_back(raw);
        } catch(...) {}
    }
    virtual int GetEventDebugID() {return 42;}
} listener;

void __cdecl PrintHook(void* self,int index,int filter,const char* format,...) {
    char buffer[4096]{}; // Match the installed client's native formatting limit.
    va_list args;va_start(args,format);const bool formatted=Format(buffer,sizeof(buffer),format,args);va_end(args);
    if(licensing::Active() && !inside_print && formatted && accepting.load(std::memory_order_acquire)) {
        try {
            RawHud raw{};raw.index=index;raw.time=GetTickCount64();std::memcpy(raw.text,buffer,sizeof(raw.text)-1);
            std::lock_guard lock(shared_mutex);
            if(accepting.load(std::memory_order_relaxed)) {
                if(hud_messages.size()==logic::NormalLimit) {hud_messages.pop_front();++queue_drops;}
                hud_messages.push_back(raw);
            }
        } catch(...) {}
    }
    const bool previous=inside_print;inside_print=true;
    if(original_print) original_print(self,index,filter,"%s",buffer);
    inside_print=previous;
}
void Register(bool enable) noexcept {
    if(!event_manager || registered==enable) return;
    if(enable) {
        using Add=bool(__thiscall*)(void*,Listener*,const char*,bool);
        bool result{};
        registered=memory::Invoke(memory::Virtual<Add>(event_manager,3,engine_image),result,
                                  event_manager,&listener,"player_say",false) && result;
        if(registered) app::Log("Chat spy: listener player_say registrado");
    } else {
        using Remove=void(__thiscall*)(void*,Listener*);
        memory::InvokeVoid(memory::Virtual<Remove>(event_manager,5,engine_image),event_manager,&listener);
        registered=false;
    }
}
void* Hud() noexcept {
    void* manager{};void* hud{};
    if(get_hud && find_hud && memory::Invoke(get_hud,manager,-1) && manager)
        memory::Invoke(find_hud,hud,manager,"CHudChat");
    return hud; // Never cache a HUD pointer across a level transition.
}
bool Player(int index,source::PlayerInfo& info) noexcept {
    info={};bool found{};
    if(index<1 || index>64 || !memory::Invoke(api->base.player_info,found,api->base.engine,index,&info) || !found) return false;
    info.name[sizeof(info.name)-1]=0;
    return info.userid>0 && info.name[0];
}
void Publish(const Options& opt,void* hud) {
    Snapshot next{};next.listener=registered;next.hud_hook=print_hook;next.hud_ready=hud && original_print;
    next.received=history.received;next.suppressed=history.suppressed;next.dropped=history.dropped;
    next.generation=generation;
    for(const auto& m:history.Messages()) {
        auto& entry=next.entries[next.count++];entry.userid=m.userid;entry.team=m.team;entry.serial=m.serial;
        std::snprintf(entry.name,sizeof(entry.name),"%s",m.name.c_str());
        std::snprintf(entry.text,sizeof(entry.text),"%s",m.text.c_str());
    }
    const char* text=!opt.enabled?"Chat spy off":!event_manager?"GameEventManager no disponible":
        !api->InGame()?"Esperando partida":!registered?"No se pudo registrar player_say":
        !print_hook?"Eventos activos; filtro HUD no disponible":
        opt.hud && !next.hud_ready?"Panel activo; CHudChat no disponible":"Escuchando player_say";
    std::snprintf(next.status,sizeof(next.status),"%s | %u recibidos, %u filtrados",text,next.received,next.suppressed);
    std::lock_guard lock(shared_mutex);next.dropped+=queue_drops;published=next;
}
}
void Initialize(const game::Api& runtime) noexcept {
    try {
        api=&runtime;engine_image=memory::Module(GetModuleHandleW(L"engine.dll"));
        event_manager=game::Interface(L"engine.dll","GAMEEVENTSMANAGER002");
        auto target=game::Signature(L"client.dll","55 8B EC B8 ? ? ? ? E8 ? ? ? ? A1 ? ? ? ? 33 C5 89 45 ? 8B 4D ? 53");
        get_hud=reinterpret_cast<GetHud>(game::Signature(L"client.dll","55 8B EC 8B 45 ? 83 F8 ? 75 ? 8B 0D ? ? ? ? 8B 01 8B 90 ? ? ? ? FF D2 69 C0"));
        find_hud=reinterpret_cast<FindHud>(game::Signature(L"client.dll","55 8B EC 53 8B 5D ? 56 57 8B F1 33 FF 39 7E"));
        if(target && MH_CreateHook(target,reinterpret_cast<void*>(&PrintHook),reinterpret_cast<void**>(&original_print))==MH_OK)
            print_hook=MH_EnableHook(target)==MH_OK;
        app::Log("Chat spy: events=%p HUD hook=%d manager=%p finder=%p",event_manager,print_hook,get_hud,find_hud);
    } catch(...) {app::Log("Chat spy: error de inicializacion");}
}
void SetOptions(Options value) noexcept {
    value.clear_minutes=std::clamp(value.clear_minutes,0,240);
    std::lock_guard lock(shared_mutex);options=value;accepting.store(value.enabled,std::memory_order_release);
}
void RequestClear() noexcept {clear_requested.store(true,std::memory_order_release);}
void Suspend() noexcept {accepting.store(false,std::memory_order_release);SetOptions({});RequestClear();}
Snapshot ReadSnapshot() noexcept {std::lock_guard lock(shared_mutex);return published;}
void ClearLevel() noexcept {
    accepting.store(false,std::memory_order_release);Register(false);
    history.Reset();was_enabled=false;++generation;
    std::lock_guard lock(shared_mutex);events.clear();hud_messages.clear();queue_drops=0;published={};published.generation=generation;
}
void Frame() noexcept {
    if(!api) return;
    try {
        Options opt{};std::deque<RawEvent> incoming;std::deque<RawHud> normal;
        {
            std::lock_guard lock(shared_mutex);opt=options;incoming.swap(events);normal.swap(hud_messages);
        }
        const bool enabled=opt.enabled && api->InGame();
        accepting.store(enabled,std::memory_order_release);Register(enabled);
        if(!enabled || clear_requested.exchange(false,std::memory_order_acq_rel)) {
            if(was_enabled || enabled) {history.Reset();++generation;}
            incoming.clear();normal.clear();
        }
        was_enabled=enabled;
        void* hud=enabled && original_print?Hud():nullptr;
        if(enabled) {
            const auto now=GetTickCount64();
            for(const auto& raw:normal) {
                source::PlayerInfo info{};const bool known=Player(raw.index,info);
                const auto pair=logic::SplitHud(raw.text,known?info.name:"");
                if(!pair.name.empty()) history.Remember(known?info.userid:0,pair.name,pair.text,raw.time);
            }
            game::Entity local{};
            if(api->Local(local,false,true) && (local.team==2 || local.team==3)) {
                using FromUser=int(__thiscall*)(void*,int);
                const auto from_user=memory::Virtual<FromUser>(api->base.engine,9,engine_image);
                for(const auto& raw:incoming) {
                    int index{};game::Entity sender{};source::PlayerInfo info{};
                    if(memory::Invoke(from_user,index,api->base.engine,raw.userid) && Player(index,info) && info.userid==raw.userid &&
                       api->Read(index,sender,false,true) && sender.team!=local.team && (sender.team==2 || sender.team==3))
                        history.Receive(raw.userid,sender.team,info.name,raw.text,raw.time);
                }
            }
            const auto ready=history.Advance(now,opt.deduplicate && print_hook,opt.clear_minutes);
            if(opt.hud && hud && original_print) for(const auto& m:ready) {
                char line[768]{};
                std::snprintf(line,sizeof(line),"\x01[Chat] %s\x03%s\x01 : %s",
                    opt.team_prefix?(m.team==3?"(Infected) ":"(Survivor) "):"",m.name.c_str(),m.text.c_str());
                // Trampoline avoids feeding our own output into duplicate detection.
                memory::InvokeVoid(original_print,hud,0,0,"%s",line);
            }
        }
        Publish(opt,hud);
    } catch(...) {app::Log("Chat spy: fallo de procesamiento; historial descartado");history.Reset();}
}
}
