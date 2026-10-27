#include "hooks.h"
#include "license.h"
#include "esp.h"
#include "log.h"
#include "menu.h"
#include "features.h"
#include "capture_overlay.h"
#include "overlay_callsite.h"
#include "menu_input.h"
#include "spectators.h"
#include "velocity_ui/features_ui.hpp"
#include <MinHook.h>
#include <d3d9.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <windowsx.h>
#include <algorithm>
#include <array>
#include <atomic>
#include <cmath>
#include <cwchar>
#include <iterator>
#include <mutex>
#include <intrin.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND,UINT,WPARAM,LPARAM);

namespace hooks {
namespace {
using Present = HRESULT(WINAPI*)(IDirect3DDevice9*,const RECT*,const RECT*,HWND,const RGNDATA*);
using Reset = HRESULT(WINAPI*)(IDirect3DDevice9*,D3DPRESENT_PARAMETERS*);
using EndScene = HRESULT(WINAPI*)(IDirect3DDevice9*);
using ShutdownLevel = void(__thiscall*)(void*);
Present original_present{};
Reset original_reset{};
EndScene original_end_scene{};
source::FrameStage original_stage{};
source::Cursor original_lock_cursor{};
ShutdownLevel original_level_shutdown{};
source::Interfaces api{};
std::atomic_bool running{false}, menu_open{false}, renderer_failed{false};
std::atomic<WNDPROC> original_window_proc{nullptr};
std::atomic<HWND> game_window{nullptr};
std::recursive_mutex render_mutex;
IDirect3DDevice9* rendering_device{};
ImGuiContext* context{};
bool resetting{}, last_open{};
esp::Options options{};
esp::Frame render_frame{};
capture_overlay::Surface private_surface;
capture_overlay::OverlayCallsite steam_callsite;
HMODULE steam_module{};
ULONGLONG next_steam_lookup{},steam_seen{};
std::uint64_t presentation_epoch{};
std::atomic_bool steam_requested{},steam_menu_drawn{};
std::atomic<ULONGLONG> menu_presented{};
bool menu_mouse_capture{}; // Used only by the game window's thread.

bool CapturingInput() noexcept {
    return menu_input::Captures(menu_open.load(std::memory_order_acquire),
        running.load(std::memory_order_acquire)&&licensing::Active()&&!renderer_failed.load(std::memory_order_acquire),
        GetForegroundWindow()==game_window.load(std::memory_order_acquire),GetTickCount64(),
        menu_presented.load(std::memory_order_acquire));
}
void CloseMenu() noexcept {
    const bool was_open=menu_open.exchange(false,std::memory_order_acq_rel);
    const auto presented=menu_presented.exchange(0,std::memory_order_acq_rel);
    // Capture must be released on its owning window thread, including when
    // a renderer failure closes the menu on another thread.
    if(was_open||presented)if(const HWND hwnd=game_window.load(std::memory_order_acquire))PostMessageW(hwnd,WM_NULL,0,0);
}

struct Message { HWND hwnd{}; UINT message{}; WPARAM wparam{}; LPARAM lparam{}; };
std::array<Message,512> messages{};
size_t message_count{};
bool messages_lost{};
std::mutex input_mutex;

bool InputMessage(UINT message) noexcept {
    return (message >= WM_KEYFIRST && message <= WM_KEYLAST) ||
        (message >= WM_MOUSEFIRST && message <= WM_MOUSELAST) || message == WM_INPUT ||
        message == WM_MOUSELEAVE || message == WM_SETFOCUS || message == WM_KILLFOCUS ||
        message == WM_INPUTLANGCHANGE;
}
void Queue(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp) {
    if (msg == WM_INPUT) return; // HRAWINPUT has already expired by render time.
    std::lock_guard<std::mutex> lock(input_mutex);
    if (msg == WM_MOUSEMOVE && message_count && messages[message_count-1].message == WM_MOUSEMOVE) {
        messages[message_count-1] = {hwnd,msg,wp,lp}; return;
    }
    if (message_count < messages.size()) messages[message_count++] = {hwnd,msg,wp,lp};
    else messages_lost = true;
}
void DrainInput() {
    std::array<Message,512> pending{};
    size_t count{};
    bool lost{};
    {
        std::lock_guard<std::mutex> lock(input_mutex);
        count = message_count; lost = messages_lost;
        std::copy_n(messages.begin(),count,pending.begin());
        message_count = 0; messages_lost = false;
    }
    if (lost) { ImGui::GetIO().ClearInputKeys(); ImGui::GetIO().ClearInputMouse(); menu::ClearInput(); }
    for (size_t i = 0; i < count; ++i) {
        const auto& m = pending[i];
        ImGui_ImplWin32_WndProcHandler(m.hwnd,m.message,m.wparam,m.lparam);
        menu::Input(m.message,m.wparam,m.lparam);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp) {
    auto original = original_window_proc.load(std::memory_order_acquire);
    if(msg==WM_KILLFOCUS || (msg==WM_ACTIVATEAPP && !wp))CloseMenu();
    if(!CapturingInput()&&menu_mouse_capture){if(GetCapture()==hwnd)ReleaseCapture();menu_mouse_capture=false;}
    if (running.load(std::memory_order_acquire) && licensing::Active()) {
        if ((msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN) && !(lp & (1LL << 30))) {
            if (wp == VK_INSERT) {
                const bool open = !menu_open.load(std::memory_order_acquire);
                if(open)menu_open.store(true,std::memory_order_release);else CloseMenu();
                return 0;
            }
            if(wp==VK_ESCAPE&&menu_open.load(std::memory_order_acquire)){CloseMenu();return 0;}
            const int third_key=feature_ui::settings.third_key.value;
            if (third_key>0 && wp==third_key && !(wp==VK_INSERT) && !menu_open.load(std::memory_order_acquire)) {
                feature_ui::ToggleThirdPerson(); return 0;
            }
        }
        // Mouse-button binds for the third-person toggle while in game.
        if (running.load(std::memory_order_acquire) && !menu_open.load(std::memory_order_acquire)) {
            const int third_key=feature_ui::settings.third_key.value;
            if (third_key>0) {
                bool hit=false;
                if (msg==WM_MBUTTONDOWN) hit=(third_key==VK_MBUTTON);
                else if (msg==WM_XBUTTONDOWN) hit=(third_key==VK_XBUTTON1 && HIWORD(lp)==XBUTTON1) ||
                                                 (third_key==VK_XBUTTON2 && HIWORD(lp)==XBUTTON2);
                if (hit) { feature_ui::ToggleThirdPerson(); return 0; }
            }
        }
        if ((msg == WM_KEYUP || msg == WM_SYSKEYUP) && wp == VK_INSERT) return 0;
        if (InputMessage(msg)) Queue(hwnd,msg,wp,lp);
        if (msg == WM_KILLFOCUS && GetCapture() == hwnd) ReleaseCapture();
        if (CapturingInput()) {
            // Capture belongs to the window's thread, not the DirectX thread.
            if (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_XBUTTONDOWN ||
                msg == WM_LBUTTONDBLCLK || msg == WM_RBUTTONDBLCLK || msg == WM_MBUTTONDBLCLK || msg == WM_XBUTTONDBLCLK)
                {SetCapture(hwnd);menu_mouse_capture=GetCapture()==hwnd;}
            if ((msg == WM_LBUTTONUP || msg == WM_RBUTTONUP || msg == WM_MBUTTONUP || msg == WM_XBUTTONUP) &&
                !(wp & (MK_LBUTTON|MK_RBUTTON|MK_MBUTTON|MK_XBUTTON1|MK_XBUTTON2)) && GetCapture() == hwnd)
                {ReleaseCapture();menu_mouse_capture=false;}
            if (msg == WM_SETCURSOR && LOWORD(lp) == HTCLIENT) { SetCursor(nullptr); return TRUE; }
            if (msg == WM_INPUT) return DefWindowProcW(hwnd,msg,wp,lp);
            if ((msg >= WM_KEYFIRST && msg <= WM_KEYLAST) || (msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST)) return 0;
        }
    }
    return original ? CallWindowProcW(original,hwnd,msg,wp,lp) : DefWindowProcW(hwnd,msg,wp,lp);
}
BOOL CALLBACK FindWindowCallback(HWND hwnd,LPARAM result_address) {
    DWORD process{};
    GetWindowThreadProcessId(hwnd,&process);
    if (process != GetCurrentProcessId() || !IsWindowVisible(hwnd)) return TRUE;
    wchar_t name[64]{};
    GetClassNameW(hwnd,name,static_cast<int>(std::size(name)));
    if (wcscmp(name,L"Valve001") && wcscmp(name,L"SDL_app")) return TRUE;
    *reinterpret_cast<HWND*>(result_address) = hwnd;
    return FALSE;
}
bool DeviceTargets(void*& present,void*& reset,void*& end_scene) {
    HWND window = CreateWindowExW(0,L"STATIC",L"lyn DX9",WS_OVERLAPPED,0,0,64,64,nullptr,nullptr,
                                  GetModuleHandleW(nullptr),nullptr);
    if (!window) return false;
    IDirect3D9* d3d = Direct3DCreate9(D3D_SDK_VERSION);
    IDirect3DDevice9* device{};
    D3DPRESENT_PARAMETERS parameters{};
    parameters.Windowed = TRUE; parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    parameters.hDeviceWindow = window; parameters.BackBufferFormat = D3DFMT_UNKNOWN;
    HRESULT result = d3d ? d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,window,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,&parameters,&device) : E_FAIL;
    if (FAILED(result) && d3d)
        result = d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_NULLREF,window,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING,&parameters,&device);
    if (SUCCEEDED(result) && device) {
        auto table = *reinterpret_cast<void***>(device);
        present = table[17]; reset = table[16]; end_scene=table[42];
        device->Release();
    }
    if (d3d) d3d->Release();
    DestroyWindow(window);
    return present && reset && end_scene && memory::Executable(present) && memory::Executable(reset) && memory::Executable(end_scene);
}
bool InitializeRenderer(IDirect3DDevice9* device) {
    if (rendering_device) return rendering_device == device;
    D3DDEVICE_CREATION_PARAMETERS parameters{};
    if (!device || FAILED(device->GetCreationParameters(&parameters))) return false;
    HWND hwnd = game_window.load(std::memory_order_acquire);
    if (!hwnd || !IsWindow(hwnd) || parameters.hFocusWindow != hwnd) return false;
    IMGUI_CHECKVERSION();
    context = ImGui::CreateContext();
    if (!context) return false;
    ImGui::SetCurrentContext(context);
    auto& io = ImGui::GetIO();
    io.IniFilename = nullptr; io.LogFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    ImFontConfig font{}; font.SizePixels = 15.f;
    io.Fonts->AddFontDefault(&font);
    menu::Style();
    const bool win32 = ImGui_ImplWin32_Init(hwnd);
    const bool dx9 = win32 && ImGui_ImplDX9_Init(device);
    if (!dx9) {
        if (win32) ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext(context); context = nullptr; return false;
    }
    if (!menu::Initialize(device,hwnd)) {
        ImGui_ImplDX9_Shutdown(); ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext(context); context = nullptr;
        renderer_failed.store(true,std::memory_order_release);
        app::Log("No se pudo inicializar Velocity UI.");
        return false;
    }
    auto before = reinterpret_cast<WNDPROC>(GetWindowLongPtrW(hwnd,GWLP_WNDPROC));
    original_window_proc.store(before,std::memory_order_release);
    SetLastError(0);
    auto previous = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(hwnd,GWLP_WNDPROC,reinterpret_cast<LONG_PTR>(&WindowProc)));
    if (!previous) {
        app::Log("No se pudo instalar WndProc: %lu",GetLastError());
        ImGui_ImplDX9_Shutdown(); ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext(context); context = nullptr; return false;
    }
    original_window_proc.store(previous,std::memory_order_release);
    rendering_device = device; // ImGui's DX9 backend owns its AddRef.
    app::Log("ImGui/DX9 inicializado. Insert abre el menu.");
    return true;
}
capture_overlay::Route CaptureRoute() noexcept {
    return capture_overlay::SelectRoute(feature_ui::settings.capture_privacy.value,feature_ui::settings.capture_method.value);
}
bool DrawFrame(IDirect3DDevice9* device,bool steam=false) {
    std::lock_guard<std::recursive_mutex> lock(render_mutex);
    if (!licensing::Active() || !running.load(std::memory_order_acquire) || renderer_failed.load(std::memory_order_acquire)) {CloseMenu();private_surface.Reset();steam_requested.store(false);return false;}
    if (resetting || !InitializeRenderer(device) || !IsWindow(game_window.load(std::memory_order_acquire))) {CloseMenu();return false;}
    ImGui::SetCurrentContext(context);
    // Apply the queued third-person toggle between private frames.
    feature_ui::PollHotkeys();
    const auto route=CaptureRoute();
    const bool private_window=route==capture_overlay::Route::PrivateWindow;
    const bool steam_route=route==capture_overlay::Route::SteamOverlay;
    steam_requested.store(steam_route,std::memory_order_release);
    if(!steam)++presentation_epoch;
    if(steam&&!steam_route)return false;
    if(!steam_route)steam_menu_drawn.store(false);
    const bool prepared_private=private_surface.Prepare(device,game_window.load(),private_window);
    if(GetForegroundWindow()!=game_window.load()) {CloseMenu();return false;}
    if(steam_route) {
        const auto now=GetTickCount64();
        capture_overlay::GameCaptureState(steam_seen&&now>=steam_seen&&now-steam_seen<1000);
        // Discovery/drawing belongs only to Steam's EndScene. No second window,
        // BeginScene, clear, backbuffer switch or private Present in this mode.
        if(!steam)return false;
    }
    if(private_window) {
        if(!prepared_private){CloseMenu();return false;}
        // Keep the last presented private frame; do not rebuild or submit it
        // at the game's potentially much higher frame rate.
        if(menu_open.load(std::memory_order_acquire)==last_open&&!private_surface.Due())return false;
        if(!private_surface.CanRender())return false;
    }
    bool rendered_private=false;
    bool rendered_game=false;
    menu::PrepareFrame();
    {
    capture_overlay::Scene scene(device,steam);
    if(!scene){CloseMenu();return false;}
    try {
        const bool open = menu_open.load(std::memory_order_acquire);
        if (open != last_open) {
            ImGui::GetIO().ClearInputKeys(); ImGui::GetIO().ClearInputMouse();
            menu::ClearInput();
            last_open = open;
        }
        DrainInput();
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::GetIO().MouseDrawCursor = open;
        ImGui::NewFrame();
        menu::Draw(options,render_frame,0);
        features::Draw(ImGui::GetBackgroundDrawList(),ImGui::GetIO().DisplaySize);
        ImGui::Render();
        // A checkbox/profile may change the method during this UI frame.
        // Drop it rather than send it to the destination selected earlier.
        if(CaptureRoute()!=route){menu_presented.store(0);return false;}
        auto* draw_data=ImGui::GetDrawData();
        if(!draw_data||draw_data->TotalIdxCount==0) {
            if(private_window)private_surface.Empty();
            return false;
        }
        if(private_window) {
            if(prepared_private) {
                capture_overlay::Target target(device,private_surface.Chain(),private_surface.TargetCount());
                if(target){ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());rendered_private=true;}
                else private_surface.Failed();
            }
        } else {ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());rendered_game=true;}
    } catch (...) {
        renderer_failed.store(true,std::memory_order_release);
        esp::SetEnabled(false); CloseMenu();
        features::ShutdownRendering();
        private_surface.Reset();
        app::Log("Interfaz detenida tras un error interno de renderizado.");
    }
    }
    const bool submitted=private_window?(rendered_private&&private_surface.Present()):rendered_game;
    const bool visible=private_window?private_surface.Visible():rendered_game;
    if(!visible)CloseMenu();
    // A busy private swap chain may retain a visible older frame. Only an
    // actual successful submission renews the menu's input lease.
    return submitted&&menu_open.load(std::memory_order_acquire);
}
bool SteamCaller(IDirect3DDevice9* device,void* caller) {
    const auto now=GetTickCount64();
    if(now>=next_steam_lookup) {
        next_steam_lookup=now+1000;
        const auto module=GetModuleHandleW(L"gameoverlayrenderer.dll");
        const auto range=memory::Module(module);
        if(module!=steam_module)steam_seen=0;
        steam_module=module;steam_callsite.Module(range.base,range.size);
    }
    if(!steam_callsite.Candidate(reinterpret_cast<std::uintptr_t>(caller),presentation_epoch))return false;
    MEMORY_BASIC_INFORMATION page{};
    if(!VirtualQuery(caller,&page,sizeof(page))||page.AllocationBase!=steam_module||page.Type!=MEM_IMAGE||!memory::Executable(caller)) {
        steam_callsite.Reset();steam_seen=0;next_steam_lookup=0;return false;
    }
    // Steam also renders offscreen textures. Select only its game-backbuffer
    // call, before caching the site or consuming this presentation's slot.
    Microsoft::WRL::ComPtr<IDirect3DSurface9> target,backbuffer;
    if(FAILED(device->GetRenderTarget(0,&target))||FAILED(device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&backbuffer))||
       target.Get()!=backbuffer.Get())return false;
    return steam_callsite.Accept(reinterpret_cast<std::uintptr_t>(caller),presentation_epoch);
}
HRESULT WINAPI EndSceneHook(IDirect3DDevice9* device) {
    const auto caller=_ReturnAddress();
    static thread_local bool inside{};
    bool menu_visible{};
    if(!inside&&steam_requested.load(std::memory_order_acquire)&&running.load(std::memory_order_acquire)&&licensing::Active()) {
        std::lock_guard<std::recursive_mutex> lock(render_mutex);
        if(!resetting&&!renderer_failed.load()&&device==rendering_device&&GetForegroundWindow()==game_window.load()&&SteamCaller(device,caller)) {
            inside=true;steam_seen=GetTickCount64();
            try {menu_visible=DrawFrame(device,true);}
            catch(...) {
                renderer_failed.store(true);esp::SetEnabled(false);CloseMenu();private_surface.Reset();
                features::ShutdownRendering();app::Log("Interfaz detenida tras un error del overlay de Steam.");
            }
            inside=false;
        }
    }
    const HRESULT result=original_end_scene(device);
    if(menu_visible&&SUCCEEDED(result))steam_menu_drawn.store(true,std::memory_order_release);
    else if(menu_visible&&FAILED(result))CloseMenu();
    return result;
}
HRESULT WINAPI PresentHook(IDirect3DDevice9* device,const RECT* source_rect,const RECT* destination_rect,
                           HWND override_window,const RGNDATA* dirty) {
    static thread_local bool inside{};
    bool menu_visible{};
    if (!inside) {
        inside = true;
        try { menu_visible=DrawFrame(device); }
        catch (...) {
            renderer_failed.store(true,std::memory_order_release);
            esp::SetEnabled(false); CloseMenu();private_surface.Reset();
            features::ShutdownRendering();
            app::Log("Interfaz detenida tras un error de inicializacion.");
        }
        inside = false;
    }
    const HRESULT result=original_present(device,source_rect,destination_rect,override_window,dirty);
    const bool steam_visible=steam_menu_drawn.exchange(false,std::memory_order_acq_rel);
    if((menu_visible||steam_visible)&&SUCCEEDED(result))menu_presented.store(GetTickCount64(),std::memory_order_release);
    else if(FAILED(result))CloseMenu();
    return result;
}
HRESULT WINAPI ResetHook(IDirect3DDevice9* device,D3DPRESENT_PARAMETERS* parameters) {
    bool owner{};
    {
        std::lock_guard<std::recursive_mutex> lock(render_mutex);
        owner = rendering_device && rendering_device == device;
        if (owner) {
            resetting = true;
            CloseMenu();
            private_surface.Reset();
            steam_callsite.Reset();steam_module=nullptr;next_steam_lookup=steam_seen=0;
            steam_menu_drawn.store(false);++presentation_epoch;
            ImGui::SetCurrentContext(context);
            menu::InvalidateDeviceObjects();
            ImGui_ImplDX9_InvalidateDeviceObjects();
        }
    }
    const HRESULT result = original_reset(device,parameters);
    if (owner) {
        std::lock_guard<std::recursive_mutex> lock(render_mutex);
        ImGui::SetCurrentContext(context);
        if (SUCCEEDED(result)) ImGui_ImplDX9_CreateDeviceObjects();
        resetting = false;
    }
    return result;
}
void __fastcall StageHook(void* self,void*,int stage) {
    original_stage(self,stage);
    if (!running.load(std::memory_order_acquire)) return;
    const bool licensed=licensing::Active();
    static bool previously_licensed=true;
    if(!licensed) {
        CloseMenu();
        if(previously_licensed) {spectators::ClearLevel();features::ClearLevel();esp::ClearLevel();}
    }
    previously_licensed=licensed;
    if (stage == 0 && api.unlock_cursor && api.lock_cursor) {
        static bool previous{};
        const bool open = CapturingInput();
        if (open != previous) {
            memory::InvokeVoid(open ? api.unlock_cursor : api.lock_cursor,api.surface);
            previous = open;
        }
    }
    if(!licensed) return;
    spectators::Frame(stage);
    features::Frame(stage);
}
void __fastcall LevelShutdownHook(void* self,void*) {
    if (running.load(std::memory_order_acquire)) { spectators::ClearLevel(); features::ClearLevel(); esp::ClearLevel(); }
    original_level_shutdown(self);
}
void __fastcall CursorHook(void* self,void*) {
    if (CapturingInput())
        memory::InvokeVoid(api.unlock_cursor,self);
    else original_lock_cursor(self);
}
}
bool Install(const source::Interfaces& interfaces) {
    api = interfaces;
    HWND window{};
    EnumWindows(&FindWindowCallback,reinterpret_cast<LPARAM>(&window));
    if (!window) { app::Log("No se encontro la ventana de L4D2."); return false; }
    game_window.store(window,std::memory_order_release);
    void* present{}; void* reset{};void* end_scene{};
    if (!DeviceTargets(present,reset,end_scene)) { app::Log("No se pudieron obtener los metodos DX9."); return false; }
    if (MH_Initialize() != MH_OK) { app::Log("MinHook no pudo inicializarse."); return false; }
    if (MH_SetRemoteMode(TRUE) != MH_OK) {app::Log("No se pudo preparar la activacion desde el servidor.");return false;}
    struct Hook { void* target; void* detour; void** original; };
    std::array<Hook,6> entries{{
        {present,reinterpret_cast<void*>(&PresentHook),reinterpret_cast<void**>(&original_present)},
        {reset,reinterpret_cast<void*>(&ResetHook),reinterpret_cast<void**>(&original_reset)},
        {end_scene,reinterpret_cast<void*>(&EndSceneHook),reinterpret_cast<void**>(&original_end_scene)},
        {reinterpret_cast<void*>(api.frame_stage),reinterpret_cast<void*>(&StageHook),reinterpret_cast<void**>(&original_stage)},
        {reinterpret_cast<void*>(memory::Virtual<ShutdownLevel>(api.client,6)),reinterpret_cast<void*>(&LevelShutdownHook),reinterpret_cast<void**>(&original_level_shutdown)},
        {reinterpret_cast<void*>(api.lock_cursor),reinterpret_cast<void*>(&CursorHook),reinterpret_cast<void**>(&original_lock_cursor)}
    }};
    const size_t count = api.unlock_cursor && api.lock_cursor ? entries.size() : entries.size()-1;
    size_t created{};
    for (; created < count; ++created) {
        const auto& hook = entries[created];
        const MH_STATUS result = hook.target ? MH_CreateHook(hook.target,hook.detour,hook.original) : MH_ERROR_NOT_EXECUTABLE;
        if (result != MH_OK) {
            app::Log("No se pudo crear hook %zu: %s",created,MH_StatusToString(result));
            for (size_t i = 0; i < created; ++i) MH_RemoveHook(entries[i].target);
            MH_Uninitialize(); return false;
        }
    }
    for (size_t i = 0; i < count; ++i) {
        const MH_STATUS result = MH_QueueEnableHook(entries[i].target);
        if (result != MH_OK) {
            app::Log("No se pudo preparar hook %zu: %s",i,MH_StatusToString(result));
            for (size_t j = 0; j < count; ++j) MH_RemoveHook(entries[j].target);
            MH_Uninitialize(); return false;
        }
    }
    const MH_STATUS result = MH_ApplyQueued();
    if (result != MH_OK) {
        // Keep the DLL/trampolines resident if enablement was only partial.
        // The detours just forward while running=false.
        app::Log("No se pudieron activar hooks: %s. Reinicia el juego.",MH_StatusToString(result));
        return false;
    }
    spectators::Initialize(api);
    features::Initialize(api);
    features::Activate();
    running.store(true,std::memory_order_release);
    app::Log("Hooks preparados; esperando parches firmados: Present, Reset, EndScene, FrameStageNotify, LevelShutdown%s",
             count == 6 ? ", LockCursor" : "");
    return true;
}
bool MenuOpen() noexcept { return licensing::Active() && menu_open.load(std::memory_order_acquire); }
void SetMenuOpen(bool open) noexcept { if(open&&licensing::Active())menu_open.store(true,std::memory_order_release);else CloseMenu(); }
}

