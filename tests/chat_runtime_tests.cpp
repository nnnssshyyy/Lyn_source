#include "chat_spy.h"
#include <MinHook.h>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <vector>

namespace licensing { bool Active() noexcept {return true;} }

namespace {
struct Object {void** vtable;};
void* manager_table[6]{},*engine_table[10]{},*event_table[10]{};
Object manager{manager_table},engine{engine_table};
struct Event {void** vtable{event_table};int userid{};const char* text{};};
void* registered_listener{};
unsigned registrations{},removals{};
int get_hud_slot{},hud_cookie{};
bool in_game=true,server_side{},debug_id_ok{};
std::vector<std::string> displayed;
bool __fastcall Add(void*,void*,void* target,const char* event,bool server) {
    ++registrations;registered_listener=target;server_side=server;
    using Debug=int(__thiscall*)(void*);
    int id{};debug_id_ok=memory::Invoke(memory::Virtual<Debug>(target,2),id,target) && id==42;
    return !std::strcmp(event,"player_say");
}
void __fastcall Remove(void*,void*,void* target) {
    if(target==registered_listener) {++removals;registered_listener=nullptr;}
}
int __fastcall FromUser(void*,void*,int userid) {return userid>=101 && userid<=103?userid-100:0;}
bool __fastcall Info(void*,void*,int index,source::PlayerInfo* info) {
    if(index<1 || index>3) return false;
    *info={};info->userid=100+index;std::snprintf(info->name,sizeof(info->name),"Alice");return true;
}
const char* __fastcall EventName(void*) {return "player_say";}
int __fastcall EventInt(Event* event,void*,const char* key,int fallback) {return !std::strcmp(key,"userid")?event->userid:fallback;}
const char* __fastcall EventText(Event* event,void*,const char* key,const char* fallback) {return !std::strcmp(key,"text")?event->text:fallback;}
__declspec(noinline) void __cdecl Print(void*,int,int,const char* format,...) {
    char buffer[4096]{};va_list args;va_start(args,format);
    _vsnprintf_s(buffer,sizeof(buffer),_TRUNCATE,format,args);va_end(args);
    displayed.emplace_back(buffer);
}
void* __cdecl GetHud(int slot) {get_hud_slot=slot;return &hud_cookie;}
void* __fastcall FindHud(void* hud,void*,const char* name) {return hud==&hud_cookie && !std::strcmp(name,"CHudChat")?hud:nullptr;}
void Send(int userid,const char* text) {
    if(!registered_listener) return;
    Event event{};event.userid=userid;event.text=text;
    using Fire=void(__thiscall*)(void*,void*);
    memory::InvokeVoid(memory::Virtual<Fire>(registered_listener,1),registered_listener,&event);
}
}
// Substitute only the engine boundary. Production listener, MinHook detour,
// queues, snapshots, formatting and HUD trampoline execute unchanged.
namespace game {
void* Interface(const wchar_t*,const char* version) noexcept {return !std::strcmp(version,"GAMEEVENTSMANAGER002")?&manager:nullptr;}
void* Signature(const wchar_t*,const char* pattern) {
    if(std::strstr(pattern,"55 8B EC B8")) return reinterpret_cast<void*>(&Print);
    if(std::strstr(pattern,"55 8B EC 8B 45")) return reinterpret_cast<void*>(&GetHud);
    if(std::strstr(pattern,"55 8B EC 53 8B 5D")) return reinterpret_cast<void*>(&FindHud);
    return nullptr;
}
bool Api::InGame() const noexcept {return in_game;}
bool Api::Local(Entity& out,bool,bool) const noexcept {out={};out.index=1;out.team=2;return in_game;}
bool Api::Read(int index,Entity& out,bool,bool) const noexcept {
    out={};out.index=index;out.team=index==1?2:3;return index>=1 && index<=3;
}
}
int main() {
    unsigned checks{},failures{};
    const auto expect=[&](bool ok,const char* name) {++checks;if(!ok){++failures;std::fprintf(stderr,"FAIL: %s\n",name);}};
    manager_table[3]=reinterpret_cast<void*>(&Add);manager_table[5]=reinterpret_cast<void*>(&Remove);
    engine_table[9]=reinterpret_cast<void*>(&FromUser);
    event_table[1]=reinterpret_cast<void*>(&EventName);event_table[6]=reinterpret_cast<void*>(&EventInt);event_table[9]=reinterpret_cast<void*>(&EventText);
    game::Api api{};api.base.engine=&engine;api.base.player_info=reinterpret_cast<source::GetInfo>(&Info);
    expect(MH_Initialize()==MH_OK,"MinHook initializes in the mock process");
    chat_spy::Initialize(api);chat_spy::SetOptions({true,true,true,true,true,0});chat_spy::Frame();
    auto state=chat_spy::ReadSnapshot();
    expect(state.listener && registrations==1 && !server_side && debug_id_ok,"listener vtable and client-side AddListener ABI");
    expect(state.hud_hook && state.hud_ready && get_hud_slot==-1,"ChatPrintf hook and split-screen-aware HUD lookup");
    chat_spy::Frame();expect(registrations==1,"frames do not register duplicate listeners");
    Print(&hud_cookie,2,0,"\x03" "Alice\x01 : %s","Hello");
    expect(displayed.size()==1 && displayed.back()=="\x03" "Alice\x01 : Hello","normal HUD varargs and color bytes pass through unchanged");
    Send(102,"Hello");Send(103,"Hello");Send(101,"Own team");Send(999,"Invalid identity");
    chat_spy::Frame();Sleep(180);chat_spy::Frame();state=chat_spy::ReadSnapshot();
    expect(state.received==2 && state.suppressed==1 && state.count==1 && state.entries[0].userid==103,
           "production event virtual slots, team filter and userid-aware duplicate detection");
    expect(displayed.size()==2 && displayed.back().find("[Chat]")!=std::string::npos,"one eligible event prints once via the HUD trampoline");
    chat_spy::Frame();expect(displayed.size()==2,"our own HUD output does not re-enter the captured queue");
    chat_spy::SetOptions({true,true,true,false,false,0});Send(102,"100% %s José");
    chat_spy::Frame();Sleep(180);chat_spy::Frame();state=chat_spy::ReadSnapshot();
    expect(state.count==2 && std::strcmp(state.entries[1].text,"100% %s José")==0 &&
           displayed.back().find("100% %s José")!=std::string::npos,"UTF-8 and literal percent sequences are safe in both outputs");
    Send(102,"Pending at clear");chat_spy::RequestClear();chat_spy::Frame();Sleep(180);chat_spy::Frame();
    expect(chat_spy::ReadSnapshot().count==0,"manual clear discards queued events before they can be displayed");
    in_game=false;chat_spy::Frame();expect(!chat_spy::ReadSnapshot().listener && removals==1,"disconnect removes the listener and clears state");
    in_game=true;chat_spy::Frame();expect(registrations==2,"reconnect re-registers the listener");
    chat_spy::ClearLevel();expect(!registered_listener && removals==2 && chat_spy::ReadSnapshot().count==0,"level shutdown unregisters without retaining entity or HUD pointers");
    chat_spy::SetOptions({});chat_spy::Frame();expect(!registered_listener,"disabled feature stays unregistered");
    const std::string long_line(3000,'x');Print(&hud_cookie,0,0,"%s",long_line.c_str());
    expect(displayed.back()==long_line,"normal HUD forwarding retains the native 4096-byte formatting limit");
    MH_DisableHook(MH_ALL_HOOKS);MH_Uninitialize();
    std::printf("%u chat runtime checks, %u failed\n",checks,failures);
    return failures?1:0;
}
