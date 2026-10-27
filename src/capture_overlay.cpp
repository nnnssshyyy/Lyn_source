#include "capture_overlay.h"
#include <atomic>
#include <algorithm>

namespace capture_overlay {
namespace {
std::atomic<State> state{State::Off};
constexpr wchar_t ClassName[]=L"VS3.PrivateVisuals";
std::uint64_t Micros() noexcept {
    static const double scale=[] {LARGE_INTEGER f{};QueryPerformanceFrequency(&f);return f.QuadPart>0?1000000.0/static_cast<double>(f.QuadPart):0.0;}();
    LARGE_INTEGER now{};
    if(scale<=0.0||!QueryPerformanceCounter(&now))return GetTickCount64()*1000;
    return static_cast<std::uint64_t>(static_cast<double>(now.QuadPart)*scale);
}
LRESULT CALLBACK Proc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp) {
    if(msg==WM_NCHITTEST)return HTTRANSPARENT;
    if(msg==WM_MOUSEACTIVATE)return MA_NOACTIVATE;
    return DefWindowProcW(hwnd,msg,wp,lp);
}
}
HWND CreatePassiveWindow(HWND game,const RECT& bounds) noexcept {
    WNDCLASSEXW wc{sizeof(wc)};wc.hInstance=GetModuleHandleW(nullptr);wc.lpfnWndProc=Proc;wc.lpszClassName=ClassName;
    if(!RegisterClassExW(&wc)&&GetLastError()!=ERROR_CLASS_ALREADY_EXISTS)return nullptr;
    return CreateWindowExW(WS_EX_LAYERED|WS_EX_TRANSPARENT|WS_EX_TOOLWINDOW|WS_EX_NOACTIVATE,
        ClassName,L"VS 3 private visuals",WS_POPUP|WS_DISABLED,bounds.left,bounds.top,
        bounds.right-bounds.left,bounds.bottom-bounds.top,game,nullptr,wc.hInstance,nullptr);
}
const char* Status() noexcept {
    switch(state.load()) {
        case State::Ready:return "Privacy: separate overlay";
        case State::Hidden:return "Privacy: inactive window";
        case State::NeedsWindowed:return "Privacy: requires windowed / borderless";
        case State::Unavailable:return "Privacy: overlay unavailable (F8 to disable)";
        case State::WaitingOverlay:return "Privacy: waiting for Steam overlay (F8 to disable)";
        case State::GameCaptureReady:return "Privacy: OBS game capture / Steam";
        default:return "Privacy: off";
    }
}
void GameCaptureState(bool ready) noexcept {state.store(ready?State::GameCaptureReady:State::WaitingOverlay);}
void Surface::Reset() noexcept {
    pending_.Reset();gpu_pending_=false;
    chain_.Reset();width_=height_=target_count_=0;device_=nullptr;owner_=nullptr;retry_=next_geometry_=0;configured_=windowed_=false;budget_.Reset();
    if(window_){DestroyWindow(window_);window_=nullptr;}
    state.store(State::Off);
}
bool Surface::Prepare(IDirect3DDevice9* device,HWND game,bool enabled) noexcept {
    if(!enabled){if(configured_||window_||chain_)Reset();return false;}
    if(!device||!IsWindow(game)){Reset();state.store(State::Unavailable);return false;}
    if(device_&&(device_!=device||owner_!=game))Reset();
    if(!configured_) {
        Microsoft::WRL::ComPtr<IDirect3DSwapChain9> main;D3DPRESENT_PARAMETERS original{};D3DCAPS9 caps{};D3DDISPLAYMODE display{};
        if(FAILED(device->GetSwapChain(0,&main))||FAILED(main->GetPresentParameters(&original))||FAILED(device->GetDeviceCaps(&caps))) {
            state.store(State::Unavailable);return false;
        }
        windowed_=original.Windowed!=FALSE;target_count_=std::min(4u,static_cast<unsigned>(caps.NumSimultaneousRTs));
        device->GetDisplayMode(0,&display);budget_.Reset(display.RefreshRate?display.RefreshRate:60);
        device_=device;owner_=game;configured_=true;
        device->CreateQuery(D3DQUERYTYPE_EVENT,&pending_);
    }
    if(!windowed_) {
        if(window_)ShowWindow(window_,SW_HIDE);state.store(State::NeedsWindowed);return false;
    }
    DWORD pid{};GetWindowThreadProcessId(game,&pid);
    if(pid!=GetCurrentProcessId()){state.store(State::Unavailable);return false;}
    if(IsIconic(game)||GetForegroundWindow()!=game) {
        if(window_)ShowWindow(window_,SW_HIDE);state.store(State::Hidden);return false;
    }
    const auto now=GetTickCount64();
    if((!window_||!chain_)&&now<retry_)return false;
    if(window_&&chain_&&now<next_geometry_){state.store(State::Ready);return true;}
    next_geometry_=now+100;
    RECT rect{};POINT origin{};
    if(!GetClientRect(game,&rect)||!ClientToScreen(game,&origin)||rect.right<1||rect.bottom<1){Failed();return false;}
    if(!window_) {
        retry_=now+1000;
        window_=CreatePassiveWindow(game,{origin.x,origin.y,origin.x+rect.right,origin.y+rect.bottom});
        DWORD affinity{};
        // Fail closed. Never silently send private draw data to the game.
        if(!window_||IsWindowEnabled(window_)||!SetLayeredWindowAttributes(window_,RGB(0,0,0),255,LWA_COLORKEY)||
           !SetWindowDisplayAffinity(window_,0x11)||!GetWindowDisplayAffinity(window_,&affinity)||affinity!=0x11) {
            if(window_){DestroyWindow(window_);window_=nullptr;}state.store(State::Unavailable);return false;
        }
    }
    const auto width=static_cast<unsigned>(rect.right),height=static_cast<unsigned>(rect.bottom);
    if(!chain_||width_!=width||height_!=height) {
        retry_=now+1000;
        chain_.Reset();ShowWindow(window_,SW_HIDE);
        D3DPRESENT_PARAMETERS pp{};pp.Windowed=TRUE;pp.hDeviceWindow=window_;
        pp.BackBufferWidth=width;pp.BackBufferHeight=height;pp.BackBufferCount=1;
        pp.SwapEffect=D3DSWAPEFFECT_DISCARD;pp.BackBufferFormat=D3DFMT_A8R8G8B8;pp.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;
        if(FAILED(device->CreateAdditionalSwapChain(&pp,&chain_))) {
            pp.BackBufferFormat=D3DFMT_UNKNOWN;
            if(FAILED(device->CreateAdditionalSwapChain(&pp,&chain_))){state.store(State::Unavailable);return false;}
        }
        width_=width;height_=height;retry_=0;budget_.Reset(budget_.Rate());
    }
    RECT previous{};GetWindowRect(window_,&previous);
    if(previous.left!=origin.x||previous.top!=origin.y||previous.right-previous.left!=static_cast<LONG>(width)||
       previous.bottom-previous.top!=static_cast<LONG>(height))
        SetWindowPos(window_,nullptr,origin.x,origin.y,width,height,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOOWNERZORDER);
    MSG message{};
    for(unsigned i=0;i<32 && PeekMessageW(&message,window_,0,0,PM_REMOVE);++i){TranslateMessage(&message);DispatchMessageW(&message);}
    state.store(State::Ready);return true;
}
bool Surface::Due() const noexcept {return budget_.Due(Micros());}
bool Surface::Visible() const noexcept {return window_&&IsWindowVisible(window_);}
bool Surface::CanRender() noexcept {
    if(!pending_||!gpu_pending_)return true;
    // Poll once, without FLUSH or a wait loop. The game's Present submits work.
    const HRESULT result=pending_->GetData(nullptr,0,0);
    if(result==S_FALSE){budget_.Submitted(Micros());return false;}
    gpu_pending_=false;
    if(FAILED(result)){Failed();return false;}
    return true;
}
void Surface::Empty() noexcept {
    budget_.Submitted(Micros());
    if(Visible())ShowWindow(window_,SW_HIDE);
}
bool Surface::Present() noexcept {
    if(!chain_||!window_)return false;
    budget_.Submitted(Micros());
    gpu_pending_=pending_&&SUCCEEDED(pending_->Issue(D3DISSUE_END));
    const HRESULT result=chain_->Present(nullptr,nullptr,window_,nullptr,D3DPRESENT_DONOTWAIT);
    if(SUCCEEDED(result)){if(!IsWindowVisible(window_))ShowWindow(window_,SW_SHOWNOACTIVATE);}
    else if(result!=D3DERR_WASSTILLDRAWING){ShowWindow(window_,SW_HIDE);state.store(State::Unavailable);}
    return SUCCEEDED(result);
}
void Surface::Failed() noexcept {if(window_)ShowWindow(window_,SW_HIDE);state.store(State::Unavailable);}
Target::Target(IDirect3DDevice9* device,IDirect3DSwapChain9* chain,unsigned target_count) noexcept:device_(device) {
    if(!device||!chain)return;
    D3DCAPS9 caps{};Microsoft::WRL::ComPtr<IDirect3DSurface9> destination;
    if((!target_count&&FAILED(device->GetDeviceCaps(&caps)))||FAILED(device->GetViewport(&viewport_))||
       FAILED(chain->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO,&destination)))return;
    count_=std::min(4u,target_count?target_count:static_cast<unsigned>(caps.NumSimultaneousRTs));
    for(unsigned i=0;i<count_;++i)device->GetRenderTarget(i,&targets_[i]);
    device->GetDepthStencilSurface(&depth_);
    if(!targets_[0])return;
    saved_=true;
    for(unsigned i=1;i<count_;++i)device->SetRenderTarget(i,nullptr);
    device->SetDepthStencilSurface(nullptr);
    D3DSURFACE_DESC desc{};
    if(FAILED(destination->GetDesc(&desc))||FAILED(device->SetRenderTarget(0,destination.Get())))return;
    D3DVIEWPORT9 viewport{0,0,desc.Width,desc.Height,0.f,1.f};
    bound_=SUCCEEDED(device->SetViewport(&viewport))&&SUCCEEDED(device->Clear(0,nullptr,D3DCLEAR_TARGET,0,1.f,0));
}
Target::~Target() {
    if(!saved_)return;
    device_->SetDepthStencilSurface(nullptr);
    for(unsigned i=0;i<count_;++i)device_->SetRenderTarget(i,targets_[i].Get());
    device_->SetDepthStencilSurface(depth_.Get());device_->SetViewport(&viewport_);
}
}
