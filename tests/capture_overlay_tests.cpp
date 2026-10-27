#include "capture_overlay.h"
#include "features.h"
#include "menu_input.h"
#include "overlay_callsite.h"
#include <cstdio>
#include <cstring>
using Microsoft::WRL::ComPtr;
int main(){unsigned checks{},failed{};const auto expect=[&](bool ok,const char* text){++checks;if(!ok){++failed;std::printf("FAIL: %s\n",text);}};
    using capture_overlay::Route;
    expect(capture_overlay::SelectRoute(false,1)==Route::Game,"privacy off cannot leave the Steam route active");
    expect(capture_overlay::SelectRoute(true,0)==Route::PrivateWindow&&capture_overlay::SelectRoute(true,99)==Route::PrivateWindow,
           "existing and invalid profiles retain the capture-excluded window");
    expect(capture_overlay::SelectRoute(true,1)==Route::SteamOverlay,"the lightweight route requires an explicit game-capture selection");
    capture_overlay::OverlayCallsite site;
    expect(!site.Accept(0x1100,0),"a missing Steam module cannot select a game EndScene");
    site.Module(0x1000,0x1000);
    expect(!site.Accept(0xfff,0)&&!site.Accept(0x2000,0),"module bounds reject foreign callers on both edges");
    expect(site.Accept(0x1100,0),"the first validated Steam call may draw on presentation zero");
    expect(!site.Accept(0x1100,0)&&!site.Accept(0x1200,1),"duplicate scenes and other Steam sites do not rebuild the UI");
    expect(site.Accept(0x1100,1),"the selected site resumes on the next game presentation");
    site.Module(0x1000,0x1000);
    expect(!site.Accept(0x1200,2),"refreshing an unchanged module does not forget the selected call site");
    site.Module(0x3000,0x800);
    expect(!site.Accept(0x1100,2)&&site.Accept(0x3200,2),"module reload discards the old address and accepts the new site");
    site.Module(0,0);expect(!site.Accept(0x3200,3),"an unloaded overlay fails closed");
    site.Reset();site.Module(0x1000,0x1000);unsigned accepted{};
    for(unsigned frame=0;frame<1000;++frame)for(unsigned repeat=0;repeat<5;++repeat)if(site.Accept(0x1100,frame))++accepted;
    expect(accepted==1000,"five thousand EndScene calls generate exactly one thousand lightweight frames");
    capture_overlay::FrameBudget budget;budget.Reset(144);expect(budget.Rate()==90,"private rendering is capped independently of a fast game display");
    unsigned updates{};for(std::uint64_t t=0;t<1000000;t+=1000)if(budget.Due(t)){++updates;budget.Submitted(t);}
    expect(updates>=89&&updates<=91,"a thousand game frames generate about ninety private frames");
    budget.Reset(60);updates=0;for(std::uint64_t t=0;t<1000000;t+=1000)if(budget.Due(t)){++updates;budget.Submitted(t);}
    expect(updates>=59&&updates<=61,"a sixty-Hz monitor does not receive high-rate private submissions");
    budget.Submitted(5000000);expect(!budget.Due(5000000)&&budget.Due(5020000),"a long GPU stall never creates a catch-up burst");
    budget.Reset(0);expect(budget.Rate()==60&&budget.Due(1),"a missing display rate falls back to sixty and draws immediately");
    expect(!menu_input::Captures(false,true,true,1100,1000),"closed menu always passes game controls even after a rendered frame");
    expect(menu_input::Captures(true,true,true,1100,1000),"only a visible, active and focused menu captures controls");
    expect(!menu_input::Captures(true,true,true,1400,1000),"stalled or hidden menu frames stop capturing controls");
    expect(!menu_input::Captures(true,true,true,1100,0),"a menu that never appeared cannot block the game");
    expect(!menu_input::Captures(true,false,true,1100,1000)&&!menu_input::Captures(true,true,false,1100,1000),"inactive renderer and focus loss restore controls");
    expect(!menu_input::Captures(true,true,true,900,1000),"an invalid presentation timestamp cannot capture input");
    features::Config stored;stored.glow=stored.world_glow=stored.chams=stored.world_chams=stored.chat_hud=true;
    stored.esp=stored.world=stored.capture_privacy=true;auto effective=stored;features::ApplyCapturePolicy(effective);
    expect(!effective.glow&&!effective.world_glow&&!effective.chams&&!effective.world_chams&&!effective.chat_hud,"privacy pauses effects that are part of the captured game scene");
    expect(effective.esp&&effective.world&&stored.glow&&stored.chat_hud,"2D ESP and saved scene preferences remain available");
    stored.capture_privacy=false;effective=stored;features::ApplyCapturePolicy(effective);
    expect(effective.glow&&effective.world_glow&&effective.chams&&effective.chat_hud,"turning privacy off restores the saved feature policy");
    HWND game=CreateWindowExW(0,L"STATIC",L"VS3 render test",WS_OVERLAPPED,0,0,128,128,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);
    const HWND focus=GetFocus(),active=GetActiveWindow(),foreground=GetForegroundWindow();
    HWND overlay=capture_overlay::CreatePassiveWindow(game,{0,0,64,64});
    expect(game&&overlay,"isolated hidden test windows can be created");
    expect(!IsWindowEnabled(overlay)&&IsWindowEnabled(game),"the real private overlay cannot receive user input and leaves its owner enabled");
    expect(GetWindow(overlay,GW_OWNER)==game&&GetAncestor(overlay,GA_ROOT)==overlay,"the private overlay belongs to the game while remaining a top-level window");
    expect(SendMessageW(overlay,WM_NCHITTEST,0,0)==HTTRANSPARENT&&SendMessageW(overlay,WM_MOUSEACTIVATE,0,0)==MA_NOACTIVATE,"the private window rejects hit testing and mouse activation");
    SetWindowPos(overlay,nullptr,2,2,64,64,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOOWNERZORDER);
    SetFocus(overlay);
    expect(GetFocus()==focus&&GetActiveWindow()==active&&GetForegroundWindow()==foreground,"overlay creation, positioning and focus rejection leave input with its previous owner");
    DWORD affinity{};
    const bool excluded=overlay&&SetLayeredWindowAttributes(overlay,RGB(0,0,0),255,LWA_COLORKEY)&&
        SetWindowDisplayAffinity(overlay,0x11)&&GetWindowDisplayAffinity(overlay,&affinity)&&affinity==0x11;
    expect(excluded,"Windows accepts and confirms capture exclusion on the private window");
    ComPtr<IDirect3D9> d3d;d3d.Attach(Direct3DCreate9(D3D_SDK_VERSION));ComPtr<IDirect3DDevice9> device;
    D3DPRESENT_PARAMETERS pp{};pp.Windowed=TRUE;pp.hDeviceWindow=game;pp.SwapEffect=D3DSWAPEFFECT_DISCARD;pp.BackBufferWidth=64;pp.BackBufferHeight=64;pp.BackBufferFormat=D3DFMT_UNKNOWN;
    expect(d3d&&SUCCEEDED(d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,game,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&pp,&device)),"native DX9 device initializes for render isolation regression");
    if(device){
        ComPtr<IDirect3DSwapChain9> private_chain;pp.hDeviceWindow=overlay;
        expect(SUCCEEDED(device->CreateAdditionalSwapChain(&pp,&private_chain)),"private visuals have an independent swap chain on the same device");
        if(private_chain){
            ComPtr<IDirect3DSurface9> main;device->GetRenderTarget(0,&main);
            device->Clear(0,nullptr,D3DCLEAR_TARGET,D3DCOLOR_XRGB(211,17,29),1,0);
            D3DVIEWPORT9 before{};device->GetViewport(&before);expect(SUCCEEDED(device->BeginScene()),"test scene begins normally");
            {capture_overlay::Scene borrowed(device.Get(),true);expect(bool(borrowed),"Steam rendering borrows the caller's active scene without nesting BeginScene");}
            {capture_overlay::Target target(device.Get(),private_chain.Get());expect(bool(target),"private drawing binds an independent render target");
             device->Clear(0,nullptr,D3DCLEAR_TARGET,D3DCOLOR_XRGB(13,23,229),1,0);}
            expect(SUCCEEDED(device->EndScene()),"borrowed rendering leaves EndScene to its original caller");
            {capture_overlay::Scene owned(device.Get(),false);expect(bool(owned),"Present rendering owns a separate scene only when necessary");}
            expect(SUCCEEDED(device->BeginScene())&&SUCCEEDED(device->EndScene()),"owned rendering balances the scene before returning to the game");
            ComPtr<IDirect3DSurface9> restored;device->GetRenderTarget(0,&restored);
            D3DVIEWPORT9 after{};device->GetViewport(&after);
            expect(main.Get()==restored.Get()&&!std::memcmp(&before,&after,sizeof(before)),"private rendering restores the game's target and viewport");
            ComPtr<IDirect3DSurface9> pixels;D3DSURFACE_DESC desc{};main->GetDesc(&desc);
            bool copied=SUCCEEDED(device->CreateOffscreenPlainSurface(desc.Width,desc.Height,desc.Format,D3DPOOL_SYSTEMMEM,&pixels,nullptr))&&SUCCEEDED(device->GetRenderTargetData(main.Get(),pixels.Get()));
            D3DLOCKED_RECT lock{};bool read=copied&&SUCCEEDED(pixels->LockRect(&lock,nullptr,D3DLOCK_READONLY));
            expect(read&&((*static_cast<const DWORD*>(lock.pBits)&0xffffff)==0xd3111d),"the game's captured backbuffer contains none of the private overlay pixels");
            if(read)pixels->UnlockRect();
            const HRESULT presented=private_chain->Present(nullptr,nullptr,overlay,nullptr,D3DPRESENT_DONOTWAIT);
            expect(SUCCEEDED(presented)||presented==D3DERR_WASSTILLDRAWING,"the additional swap chain accepts nonblocking private presentation");
        }
        capture_overlay::Surface surface;expect(!surface.Prepare(device.Get(),game,false),"privacy off never creates a visible overlay");surface.Reset();
    }
    if(game)DestroyWindow(game);if(overlay)DestroyWindow(overlay);
    std::printf("%u capture overlay checks, %u failed\n",checks,failed);return failed?1:0;
}
