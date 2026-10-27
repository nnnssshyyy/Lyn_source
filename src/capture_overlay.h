#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <wrl/client.h>
#include <array>
#include "overlay_budget.h"

namespace capture_overlay {
enum class State {Off,Ready,Hidden,NeedsWindowed,Unavailable,WaitingOverlay,GameCaptureReady};
const char* Status() noexcept;
void GameCaptureState(bool ready) noexcept;
// An owned, disabled tool window: all interaction stays with the game HWND.
HWND CreatePassiveWindow(HWND game,const RECT& bounds) noexcept;
// Only the project's 2D draw data is sent to this additional swap chain.
// The game backbuffer is never copied to, cleared or drawn by this class.
class Surface {
    HWND window_{}; IDirect3DDevice9* device_{};
    Microsoft::WRL::ComPtr<IDirect3DSwapChain9> chain_;
    Microsoft::WRL::ComPtr<IDirect3DQuery9> pending_;
    unsigned width_{},height_{},target_count_{}; ULONGLONG retry_{},next_geometry_{};
    HWND owner_{};bool configured_{},windowed_{},gpu_pending_{};
    FrameBudget budget_;
public:
    bool Prepare(IDirect3DDevice9*,HWND game,bool enabled) noexcept;
    bool Due() const noexcept;
    bool Visible() const noexcept;
    bool Present() noexcept;
    bool CanRender() noexcept;
    void Empty() noexcept;
    void Reset() noexcept;
    void Failed() noexcept;
    IDirect3DSwapChain9* Chain() const noexcept {return chain_.Get();}
    unsigned TargetCount() const noexcept {return target_count_;}
};
// The Steam route borrows an existing scene; it must neither begin nor end it.
class Scene {
    IDirect3DDevice9* device_{};bool owned_{},valid_{};
public:
    Scene(IDirect3DDevice9* device,bool existing) noexcept:device_(device),
        owned_(device&&!existing&&SUCCEEDED(device->BeginScene())),valid_(device&&(existing||owned_)) {}
    ~Scene(){if(owned_)device_->EndScene();}
    explicit operator bool() const noexcept {return valid_;}
    Scene(const Scene&)=delete;Scene& operator=(const Scene&)=delete;
};
// D3D state blocks do not preserve render/depth targets. Restore those
// explicitly, including secondary MRT slots used by the game's renderer.
class Target {
    IDirect3DDevice9* device_{};
    std::array<Microsoft::WRL::ComPtr<IDirect3DSurface9>,4> targets_;
    Microsoft::WRL::ComPtr<IDirect3DSurface9> depth_;
    D3DVIEWPORT9 viewport_{}; unsigned count_{};bool saved_{},bound_{};
public:
    Target(IDirect3DDevice9*,IDirect3DSwapChain9*,unsigned target_count=0) noexcept;
    ~Target();
    explicit operator bool() const noexcept {return bound_;}
    Target(const Target&)=delete;Target& operator=(const Target&)=delete;
};
}
