#include <windows.h>
#include <d3d9.h>
#include <wincodec.h>
#include <wrl/client.h>
#include <filesystem>
#include <cstdio>
#include "backends/imgui_impl_dx9.h"
#include "velocity_ui/visuals_window.hpp"
#include "velocity_ui/hud_widgets.hpp"
#include "visuals.h"
#include "esp_font.h"
#include "esp_health_text.h"
#include "esp_style.h"
#include "overlay_paint.h"
using Microsoft::WRL::ComPtr;
namespace {visuals::Snapshot frame;}
namespace licensing {bool Active() noexcept{return true;}}
namespace visuals {Snapshot ReadSnapshot() noexcept{return frame;}}
namespace hunter_preview {bool OwnsPointer(){return false;}}
namespace xui {
context& ctx(){static context c;return c;}
bool context::overlay_blocking()const{return false;}
void input_state::clear_frame(){mouse_clicked=mouse_released=mouse_double_clicked=rmb_clicked=rmb_released=false;scroll_delta=0;char_count=key_press_count=key_release_count=0;prev_mouse_x=mouse_x;prev_mouse_y=mouse_y;}
namespace binds {void register_setting(setting*){}}
}
bool Save(IDirect3DDevice9* device,const std::filesystem::path& path,int width,int height) {
    ComPtr<IDirect3DSurface9> gpu,cpu;D3DSURFACE_DESC desc;
    if(FAILED(device->GetRenderTarget(0,&gpu))||FAILED(gpu->GetDesc(&desc))||
       FAILED(device->CreateOffscreenPlainSurface(desc.Width,desc.Height,desc.Format,D3DPOOL_SYSTEMMEM,&cpu,nullptr))||
       FAILED(device->GetRenderTargetData(gpu.Get(),cpu.Get())))return false;
    D3DLOCKED_RECT pixels;if(FAILED(cpu->LockRect(&pixels,nullptr,D3DLOCK_READONLY)))return false;
    ComPtr<IWICImagingFactory> factory;ComPtr<IWICStream> stream;ComPtr<IWICBitmapEncoder> encoder;ComPtr<IWICBitmapFrameEncode> output;
    WICPixelFormatGUID format=GUID_WICPixelFormat32bppBGRA;
    const bool ok=SUCCEEDED(CoCreateInstance(CLSID_WICImagingFactory,nullptr,CLSCTX_INPROC_SERVER,IID_PPV_ARGS(&factory)))&&
        SUCCEEDED(factory->CreateStream(&stream))&&SUCCEEDED(stream->InitializeFromFilename(path.c_str(),GENERIC_WRITE))&&
        SUCCEEDED(factory->CreateEncoder(GUID_ContainerFormatPng,nullptr,&encoder))&&SUCCEEDED(encoder->Initialize(stream.Get(),WICBitmapEncoderNoCache))&&
        SUCCEEDED(encoder->CreateNewFrame(&output,nullptr))&&SUCCEEDED(output->Initialize(nullptr))&&SUCCEEDED(output->SetSize(width,height))&&
        SUCCEEDED(output->SetPixelFormat(&format))&&SUCCEEDED(output->WritePixels(height,pixels.Pitch,pixels.Pitch*height,static_cast<BYTE*>(pixels.pBits)))&&
        SUCCEEDED(output->Commit())&&SUCCEEDED(encoder->Commit());cpu->UnlockRect();return ok;
}
void Figure(ImDrawList* dl,esp::layout::Rect b,bool crouched) {
    const float x=b.x+b.w*.5f,y=b.y+20;const ImU32 tint=IM_COL32(83,98,118,255);
    dl->AddCircleFilled({x,y},12,tint,24);
    const ImVec2 torso[]{{x-19,y+17},{x+19,y+17},{x+14,y+b.h*.52f},{x-14,y+b.h*.52f}};
    dl->AddConvexPolyFilled(torso,4,tint);
    dl->AddLine({x-15,y+26},{b.x+4,y+b.h*.48f},tint,11);
    dl->AddLine({x+15,y+26},{b.right()-4,y+b.h*.48f},tint,11);
    dl->AddLine({x-8,y+b.h*.5f},{x-(crouched?35:19),b.bottom()-4},tint,13);
    dl->AddLine({x+8,y+b.h*.5f},{x+(crouched?35:19),b.bottom()-4},tint,13);
}
void Esp(ImDrawList* dl,esp::layout::Rect body,const char* name,ImU32 color,int health,int maximum,int temporary,const char* flag) {
    Figure(dl,body,maximum==250);
    overlay_paint::Box(dl,{body.x,body.y},{body.right(),body.bottom()},color,true,1.f,true);
    char value[40];if(temporary)std::snprintf(value,sizeof(value),"%d (%d+%d)",health,health-temporary,temporary);else std::snprintf(value,sizeof(value),"%d",health);
    const auto ink=esp::layout::HealthTextInk(ImGui::GetFont(),value);
    const auto bar=esp::layout::Health(body,esp::layout::Left,float(health)/maximum,ink.w,ink.h,ImGui::GetFontSize());
    overlay_paint::HealthTrack(dl,bar.outline);
    overlay_paint::HealthFill(dl,bar.fill,temporary?IM_COL32(231,183,77,255):esp::style::HealthColor(float(health)/maximum,false));
    if(temporary){auto part=esp::layout::Health(body,esp::layout::Left,float(health-temporary)/maximum,0,ink.h).fill;overlay_paint::HealthFill(dl,part,esp::style::HealthColor(float(health)/maximum,false));}
    overlay_paint::Plate(dl,{bar.text.x-2,bar.text.y-1,bar.text.w+4,bar.text.h+2});
    overlay_paint::Label(dl,esp::layout::HealthTextOrigin(bar.text,ink),overlay_paint::Text,value,nullptr,esp::layout::HealthTextSize,false);
    overlay_paint::Label(dl,{body.x+(body.w-ImGui::CalcTextSize(name).x)*.5f,body.y-31},color,name);
    overlay_paint::Label(dl,{body.x+body.w*.5f-14,body.bottom()+5},overlay_paint::Muted,"18 m");
    if(flag)overlay_paint::Label(dl,{body.right()+8,body.y+4},color,flag);
}
void Scene(int width,int height) {
    auto* dl=ImGui::GetBackgroundDrawList();
    dl->AddRectFilledMultiColor({0,0},{float(width),float(height)},IM_COL32(22,30,43,255),IM_COL32(175,184,194,255),IM_COL32(102,117,135,255),IM_COL32(12,19,31,255));
    dl->AddRectFilled({0,height*.69f},{float(width),float(height)},IM_COL32(11,18,28,140));
    for(int i=0;i<13;++i)dl->AddLine({width*.5f,height*.62f},{float(i)*width/12,float(height)},IM_COL32(112,145,177,24));
    for(int i=0;i<4;++i)dl->AddLine({0,height*(.7f+i*.085f)},{float(width),height*(.7f+i*.085f)},IM_COL32(128,150,174,25));
    overlay_paint::Label(dl,{30,27},overlay_paint::Text,"LYN / VISUALES");
    overlay_paint::Label(dl,{30,float(height)-33},overlay_paint::Muted,"Vista previa del renderizador | datos simulados");
    Esp(dl,{280,230,95,245},"Hunter",IM_COL32(255,133,151,255),175,250,0,"ON FIRE");
    Esp(dl,{870,245,100,250},"Nicolás ♡",IM_COL32(120,238,193,255),73,100,20,"HEALING");
}
int main(int argc,char** argv) {
    const std::filesystem::path folder=argc>1?argv[1]:"build/visuals-preview";std::filesystem::create_directories(folder);
    CoInitializeEx(nullptr,COINIT_MULTITHREADED);const auto module=GetModuleHandleW(nullptr);
    WNDCLASSW cls{};cls.hInstance=module;cls.lpfnWndProc=DefWindowProcW;cls.lpszClassName=L"LynVisualCapture";RegisterClassW(&cls);
    HWND window=CreateWindowExW(0,cls.lpszClassName,L"",WS_POPUP,0,0,1280,800,nullptr,nullptr,module,nullptr);
    ComPtr<IDirect3D9> d3d;d3d.Attach(Direct3DCreate9(D3D_SDK_VERSION));ComPtr<IDirect3DDevice9> device;
    D3DPRESENT_PARAMETERS pp{};pp.Windowed=TRUE;pp.hDeviceWindow=window;pp.SwapEffect=D3DSWAPEFFECT_DISCARD;pp.BackBufferWidth=1280;pp.BackBufferHeight=800;pp.BackBufferFormat=D3DFMT_A8R8G8B8;
    if(!d3d||FAILED(d3d->CreateDevice(0,D3DDEVTYPE_HAL,window,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&pp,&device)))return 1;
    ImGui::CreateContext();auto& io=ImGui::GetIO();io.IniFilename=nullptr;io.LogFilename=nullptr;io.DisplaySize={1280,800};io.DeltaTime=1.f/60;
    io.Fonts->AddFontDefault();if(!esp::font::Initialize())return 2;ImGui_ImplDX9_Init(device.Get());
    auto& s=feature_ui::settings;s.boss_hud.value=s.hitmarker.value=s.damage_numbers.value=s.offscreen.value=s.trajectory.value=true;
    s.boss_width=300;s.boss_x=.5f;s.boss_y=.03f;s.offscreen_radius=.72f;s.hitmarker_size=9.f;
    auto& style=xui::ctx().style;style.window_bg={13,22,36,240};style.accent={141,185,244};style.slider_track={34,48,66};style.text={236,242,250};style.text_dim={165,182,204};
    const int ages[]{40,110,200};bool success=true;
    for(int age:ages){
        ImGui_ImplDX9_NewFrame();ImGui::NewFrame();
        frame={};const auto now=GetTickCount64();frame.valid=true;frame.time=now;frame.matrix.m[0][0]=.002f;frame.matrix.m[1][2]=.002f;frame.matrix.m[3][3]=1;
        frame.boss_count=2;frame.bosses[0]={1,source::Kind::Tank,3200,4000,0,25,true};frame.bosses[1]={2,source::Kind::Witch,840,1000,65,18,false};
        frame.damage.Add(1,1,12,18,{180,0,125},true,false,now-age-110);
        frame.damage.Add(1,1,11,28,{180,0,125},true,false,now-age-70);
        frame.damage.Add(1,1,10,75,{-240,0,200},true,true,now-age);
        frame.threat_count=2;frame.threats[0]={{-1500,-1200,0},source::Kind::Smoker,23};frame.threats[1]={{-1500,1400,0},source::Kind::Tank,32};
        frame.path.valid=frame.path.landed=true;frame.path.count=6;frame.path.seconds=1.3f;
        for(unsigned i=0;i<6;++i)frame.path.points[i]={-30+float(i)*40,0,-180.f+float(i)*45-float(i*i)*10};
        Scene(1280,800);visuals_window::Draw(false);ImGui::Render();
        for(const auto* list:ImGui::GetDrawData()->CmdLists)for(const auto& vertex:list->VtxBuffer)success&=std::isfinite(vertex.pos.x)&&std::isfinite(vertex.pos.y);
        device->Clear(0,nullptr,D3DCLEAR_TARGET,0xff101722,1,0);if(SUCCEEDED(device->BeginScene())){ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());device->EndScene();}
        success=Save(device.Get(),folder/("visuals-"+std::to_string(age)+".png"),1280,800)&&success;
    }
    ImGui_ImplDX9_Shutdown();ImGui::DestroyContext();DestroyWindow(window);
    std::printf("%s: production visual primitives and HUD captured at 3 effect ages; all vertices finite.\n",success?"PASS":"FAIL");return success?0:3;
}
