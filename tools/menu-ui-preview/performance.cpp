#include <Windows.h>
#include <d3d9.h>
#include <wrl/client.h>
#include "imgui.h"
#include "backends/imgui_impl_dx9.h"
#include "menu.h"
#include "velocity_ui/rendering.hpp"
#include "velocity_ui/features_ui.hpp"
#include "velocity_ui/hunter_model.hpp"
#include "velocity_ui/ui_preferences.hpp"
#include <chrono>
#include <cstdio>
#include <fstream>
#include <numeric>

// Offscreen UI benchmark only: no DLL entry point, injection or engine hooks.
using Microsoft::WRL::ComPtr;
using Clock=std::chrono::steady_clock;
double Milliseconds(Clock::time_point a,Clock::time_point b) {
    return std::chrono::duration<double,std::milli>(b-a).count();
}
struct Sample {double build{},submit{},backend{};int vertices{},indices{};size_t source_vertices{};};
nlohmann::json Summary(const std::vector<Sample>& samples) {
    double build{},submit{},backend{},vertices{},indices{},source{};std::vector<double> total;
    for(const auto& s:samples) {
        build+=s.build;submit+=s.submit;backend+=s.backend;vertices+=s.vertices;indices+=s.indices;source+=s.source_vertices;
        total.push_back(s.build+s.submit+s.backend);
    }
    const double n=static_cast<double>(samples.size());std::sort(total.begin(),total.end());
    return {{"samples",samples.size()},{"build_cpu_ms",build/n},{"mesh_submission_cpu_ms",submit/n},
            {"backend_cpu_ms",backend/n},{"total_median_ms",total[total.size()/2]},
            {"total_p95_ms",total[std::min(total.size()-1,total.size()*95/100)]},
            {"imgui_vertices",vertices/n},{"imgui_indices",indices/n},{"source_vertices",source/n}};
}
int main(int argc,char** argv) {
    if(argc!=2)return 1;
    std::setvbuf(stderr,nullptr,_IONBF,0);CoInitializeEx(nullptr,COINIT_MULTITHREADED);
    const auto instance=GetModuleHandleW(nullptr);
    WNDCLASSW cls{};cls.hInstance=instance;cls.lpfnWndProc=DefWindowProcW;cls.lpszClassName=L"LynUiPerformance";RegisterClassW(&cls);
    const auto window=CreateWindowExW(0,cls.lpszClassName,L"",WS_POPUP,0,0,1280,800,nullptr,nullptr,instance,nullptr);
    ComPtr<IDirect3D9> d3d;d3d.Attach(Direct3DCreate9(D3D_SDK_VERSION));ComPtr<IDirect3DDevice9> device;
    D3DPRESENT_PARAMETERS pp{};pp.Windowed=TRUE;pp.hDeviceWindow=window;pp.SwapEffect=D3DSWAPEFFECT_DISCARD;
    pp.BackBufferWidth=1280;pp.BackBufferHeight=800;pp.BackBufferFormat=D3DFMT_A8R8G8B8;
    if(!window||!d3d||FAILED(d3d->CreateDevice(0,D3DDEVTYPE_HAL,window,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&pp,&device)))return 2;
    ImGui::CreateContext();auto& io=ImGui::GetIO();io.IniFilename=nullptr;io.LogFilename=nullptr;io.DisplaySize={1280,800};
    io.Fonts->AddFontDefault();ImGui_ImplDX9_Init(device.Get());ui::set_persistence(false);
    if(!menu::Initialize(device.Get(),window))return 3;
    auto& settings=feature_ui::settings;settings.boxes.value=settings.health.value=settings.names.value=settings.distance.value=true;
    const auto frame=[&](bool open,float dt=1.f/240.f,float x=-1,float y=-1,bool click=false,bool held=false,bool release=false) {
        io.DeltaTime=dt;ImGui_ImplDX9_NewFrame();ImGui::NewFrame();
        auto& input=xui::ctx().input;input={};input.mouse_x=x;input.mouse_y=y;input.mouse_clicked=click;input.mouse_down=held;input.mouse_released=release;
        device->Clear(0,nullptr,D3DCLEAR_TARGET,D3DCOLOR_ARGB(255,36,42,53),1,0);
        const bool scene=SUCCEEDED(device->BeginScene());
        const auto a=Clock::now();xdraw::begin_frame();rendering::g_menu.set_open(open);rendering::g_menu.draw();rendering::g_menu.draw_watermark();
        const auto b=Clock::now();size_t source{};
        for(const auto layer:{xdraw::layer::bottom,xdraw::layer::middle,xdraw::layer::top})source+=xdraw::get(layer).vertices.size();
        xdraw::end_frame();const auto c=Clock::now();ImGui::Render();auto* data=ImGui::GetDrawData();
        if(scene){ImGui_ImplDX9_RenderDrawData(data);device->EndScene();}
        return Sample{Milliseconds(a,b),Milliseconds(b,c),Milliseconds(c,Clock::now()),data->TotalVtxCount,data->TotalIdxCount,source};
    };
    nlohmann::json result{{"scope","Offscreen CPU/driver submission; excludes scene blur and in-game runtime. Not an in-game FPS measurement."},
                           {"simulated_display_hz",240},{"menu_scale",100},{"resolution",{1280,800}}};
    const auto measure=[&](const char* name,bool open,int count) {
        std::fprintf(stderr,"Measuring %s...\n",name);std::vector<Sample> samples;
        for(int i=0;i<count;++i)samples.push_back(frame(open));
        result[name]=Summary(samples);std::fprintf(stderr,"%s: %s\n",name,result[name].dump().c_str());
    };
    for(int i=0;i<4;++i)frame(false,1.f/60.f);measure("menu_closed",false,96);
    for(int i=0;i<24;++i)frame(true,1.f/60.f);measure("menu_general",true,48);
    frame(true,1.f/60.f,160,265,true,true);frame(true,1.f/60.f,160,265,false,false,true);
    for(int i=0;i<16;++i)frame(true,1.f/60.f);measure("menu_players_animated",true,48);
    hunter_model::View paused{};paused.playing=false;
    device->BeginScene();const bool prepared=hunter_model::Render(paused,320,400);device->EndScene();
    if(!prepared)return 4;
    std::vector<Sample> paused_samples;
    for(int i=0;i<120;++i) {
        device->BeginScene();const auto start=Clock::now();const bool ok=hunter_model::Render(paused,320,400);
        const double cost=Milliseconds(start,Clock::now());device->EndScene();if(!ok)return 5;
        paused_samples.push_back({cost});
    }
    result["paused_model"]=Summary(paused_samples);hunter_model::Invalidate(paused);
    std::ofstream(argv[1])<<result.dump(2)<<'\n';
    std::fprintf(stderr,"Benchmark saved to %s\n",argv[1]);
    menu::InvalidateDeviceObjects();ImGui_ImplDX9_Shutdown();ImGui::DestroyContext();DestroyWindow(window);CoUninitialize();return 0;
}
