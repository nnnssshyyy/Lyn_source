#include <Windows.h>
#include <d3d9.h>
#include <wincodec.h>
#include <wrl/client.h>
#include "imgui.h"
#include "backends/imgui_impl_dx9.h"
#include "menu.h"
#include "velocity_ui/rendering.hpp"
#include "velocity_ui/features_ui.hpp"
#include "esp_layout.h"
#include "esp_font.h"
#include "velocity_ui/ui_preferences.hpp"
#include "velocity_ui/steam_profile.hpp"
#include "velocity_ui/hunter_preview.hpp"
#include "velocity_ui/settings.hpp"
#include <cstdio>
#include <filesystem>
using Microsoft::WRL::ComPtr;

bool Save(IDirect3DDevice9* device,const wchar_t* path,int width,int height) {
    ComPtr<IDirect3DSurface9> gpu,cpu;
    D3DSURFACE_DESC desc{};
    if(FAILED(device->GetRenderTarget(0,&gpu))||FAILED(gpu->GetDesc(&desc))||
       width<=0||height<=0||static_cast<UINT>(width)>desc.Width||static_cast<UINT>(height)>desc.Height||
       FAILED(device->CreateOffscreenPlainSurface(desc.Width,desc.Height,desc.Format,D3DPOOL_SYSTEMMEM,&cpu,nullptr))||
       FAILED(device->GetRenderTargetData(gpu.Get(),cpu.Get())))return false;
    D3DLOCKED_RECT pixels{};if(FAILED(cpu->LockRect(&pixels,nullptr,D3DLOCK_READONLY)))return false;
    ComPtr<IWICImagingFactory> factory;ComPtr<IWICStream> stream;ComPtr<IWICBitmapEncoder> encoder;ComPtr<IWICBitmapFrameEncode> frame;
    WICPixelFormatGUID format=GUID_WICPixelFormat32bppBGRA;
    const bool ok=SUCCEEDED(CoCreateInstance(CLSID_WICImagingFactory,nullptr,CLSCTX_INPROC_SERVER,IID_PPV_ARGS(&factory)))&&
        SUCCEEDED(factory->CreateStream(&stream))&&SUCCEEDED(stream->InitializeFromFilename(path,GENERIC_WRITE))&&
        SUCCEEDED(factory->CreateEncoder(GUID_ContainerFormatPng,nullptr,&encoder))&&SUCCEEDED(encoder->Initialize(stream.Get(),WICBitmapEncoderNoCache))&&
        SUCCEEDED(encoder->CreateNewFrame(&frame,nullptr))&&SUCCEEDED(frame->Initialize(nullptr))&&SUCCEEDED(frame->SetSize(width,height))&&
        SUCCEEDED(frame->SetPixelFormat(&format))&&IsEqualGUID(format,GUID_WICPixelFormat32bppBGRA)&&
        SUCCEEDED(frame->WritePixels(height,pixels.Pitch,pixels.Pitch*height,static_cast<BYTE*>(pixels.pBits)))&&
        SUCCEEDED(frame->Commit())&&SUCCEEDED(encoder->Commit());
    cpu->UnlockRect();return ok;
}

namespace {
unsigned checks{},failures{};
void Expect(bool value,const char* message) {++checks;if(!value){++failures;std::fprintf(stderr,"FAIL: %s\n",message);}}
namespace steam_fixture {
std::uint64_t account=1;
const char* name="Nombre##á";
int handle=-1,decodes{};
std::uint32_t width=2,height=2;
bool decode_ok=true;
std::uint64_t __cdecl Id(void*) {return account;}
const char* __cdecl Name(void*) {return name;}
int __cdecl Avatar(void*,std::uint64_t) {return handle;}
bool __cdecl Size(void*,int,std::uint32_t* w,std::uint32_t* h) {*w=width;*h=height;return true;}
bool __cdecl Rgba(void*,int,std::uint8_t* dst,int size) {
    ++decodes;if(!decode_ok)return false;
    for(int i=0;i<size;i+=4) {dst[i]=240;dst[i+1]=40;dst[i+2]=80;dst[i+3]=255;}
    return true;
}
}
void SteamProfileChecks() {
    using namespace steam_fixture;
    steam_profile::Profile profile;
    profile.poll({},0);Expect(profile.name().empty()&&profile.rgba().empty(),"Steam unavailable uses fallback");
    void* instance=&account;
    const steam_profile::Api api{instance,instance,instance,Id,Name,Avatar,Size,Rgba};
    profile.poll(api,2000);
    Expect(profile.name()=="Nombre##á","Steam display name preserves Unicode and literal ##");
    Expect(profile.rgba().empty()&&decodes==0,"pending Steam avatar waits without decoding");
    handle=7;profile.poll(api,4000);
    Expect(profile.width()==2&&profile.height()==2&&profile.rgba().size()==16&&decodes==1,"ready Steam avatar has validated RGBA dimensions");
    Expect(profile.rgba()[0]==240&&profile.rgba()[3]==255,"Steam RGBA bytes and alpha are preserved");
    name="Renombrado";profile.poll(api,4100);
    Expect(profile.name()=="Nombre##á","Steam reads are throttled between polls");
    profile.poll(api,6000);
    Expect(profile.name()=="Renombrado"&&decodes==1,"name refresh reuses decoded avatar");
    account=2;handle=-1;profile.poll(api,8000);
    Expect(profile.rgba().empty(),"account change clears the previous account avatar");
    handle=8;width=513;profile.poll(api,10000);
    Expect(profile.rgba().empty()&&decodes==1,"oversized avatar is rejected before allocation and decoding");
    width=2;decode_ok=false;profile.poll(api,12000);
    Expect(profile.rgba().empty(),"failed image decode leaves fallback intact");
    decode_ok=true;profile.poll(api,14000);
    Expect(profile.rgba().size()==16&&decodes==3,"failed image decode is retried on a later poll");
    handle=0;profile.poll(api,16000);
    Expect(profile.rgba().empty(),"removing the Steam avatar restores fallback");
    profile.poll({},18000);
    Expect(profile.name().empty()&&profile.rgba().empty(),"lost Steam session clears account data");
}
void BehaviorChecks() {
    const auto baseline=config::to_json();
    const auto key=xui::parse_label("names##stable").second;
    ui::preferences().language=0;
    Expect(ui::translate("ESP settings")=="Ajustes de ESP","Spanish UI label");
    Expect(ui::translate("prioritize classes")=="Priorizar clases","Spanish class-filter checkbox label");
    Expect(ui::search_text("CÁMARA")=="camara","accent-insensitive search");
    const auto spanish=xui::parse_label("names##stable").first;
    ui::preferences().language=1;
    Expect(ui::translate("prioritize classes")=="prioritize classes","English class-filter checkbox label");
    Expect(xui::parse_label("names##stable").first!=spanish,"language switch updates visible labels");
    Expect(xui::parse_label("names##stable").second==key,"language switch preserves control IDs");
    Expect(config::to_json()==baseline,"language switch preserves all saved fields");
    auto& setting=feature_ui::settings.names;
    ui::profile_saved(L"QA##á");
    Expect(ui::active_profile()=="QA##á","profile name is literal UTF-8 user data");
    ui::poll_profile_state(true);Expect(!ui::profile_dirty(),"saved state starts clean");
    const bool old=setting.value;setting.value=!old;ui::poll_profile_state(true);
    Expect(ui::profile_dirty(),"editing a saved field marks profile dirty");
    setting.value=old;ui::poll_profile_state(true);Expect(!ui::profile_dirty(),"reverting fields restores clean state");
    const bool active=setting.bind.active;setting.bind.active=!active;ui::poll_profile_state(true);
    Expect(!ui::profile_dirty(),"transient bind activation is not a saved edit");setting.bind.active=active;
    ui::profile_removed(L"another profile");Expect(!ui::profile_dirty(),"deleting a different profile preserves active state");
    ui::profile_removed(L"QA##á");Expect(ui::profile_dirty()&&ui::active_profile().empty(),"removing active profile preserves unsaved edits");
    ui::profile_saved(L"");
    auto& io=ImGui::GetIO();io.DisplaySize={1920,1080};
    for(int scale=0;scale<3;++scale) {
        ui::preferences().scale=scale;
        float x=100,y=100;
        const auto* font=xdraw::current_font();
        auto& input=xui::ctx().input;input.mouse_x=400;input.mouse_y=300;input.prev_mouse_x=390;input.prev_mouse_y=290;
        {
            ui::FrameScale scope(x,y,960,640);
            Expect(std::abs(xdraw::interface_scale-(1.f+.25f*scale))<.001f,"requested menu scale fits full HD");
            Expect(std::abs(input.mouse_x*xdraw::interface_scale-400.f)<.01f,"pointer maps to scaled controls");
            auto* body=xdraw::interface_font(xdraw::current_font());
            Expect(std::abs(body->size-16.f*(1.f+.25f*scale))<.01f,"body font is rasterized at native density");
            xdraw::begin_frame(false);auto& list=xdraw::get();list.push_clip(10,20,100,80);list.rect_filled(10,20,100,80,{255,255,255});list.pop_clip();
            Expect(!list.commands.empty()&&list.commands[0].scissor.left==static_cast<LONG>(10*xdraw::interface_scale),"scissor matches menu scale");
            Expect(!list.vertices.empty()&&std::abs(list.vertices[0].pos[0]-10*xdraw::interface_scale)<.01f,"geometry matches menu scale");
        }
        Expect(xdraw::interface_scale==1.f&&xdraw::current_font()==font,"menu scale restores HUD drawing state");
        Expect(std::abs(input.mouse_x-400.f)<.01f,"menu scale restores raw mouse position");
    }
    io.DisplaySize={1280,720};ui::preferences().scale=2;float x=100,y=100;
    {
        ui::FrameScale scope(x,y,960,640);
        Expect((x+960)*xdraw::interface_scale<=1280.f&&(y+640)*xdraw::interface_scale<=720.f,"oversized menu remains inside smaller screen");
    }
    ui::preferences()={};io.DisplaySize={1280,800};
    {ui::FrameScale scope(x,y,960,640);}
    Expect(config::to_json()==baseline,"scale changes preserve profile values");
}
void SearchScrollCheck() {
    static std::array<xui::setting,18> rows;
    for(size_t i=0;i<rows.size();++i){rows[i].name="QA "+std::to_string(i);rows[i].category="qa";}
    xui::set_highlight_target(rows.back().name,2.5f,&rows.back());
    bool visible{};
    for(int frame=0;frame<2;++frame) {
        ImGui_ImplDX9_NewFrame();ImGui::NewFrame();xdraw::begin_frame();xui::begin();
        float x=0,y=0,w=400,h=200;
        if(xui::begin_window("##qa_search",x,y,w,h,false)) {
            if(xui::begin_child("##qa_scroll",300,120,true)) {
                for(auto& row:rows)xui::checkbox(row.name,row);
                const auto* panel=xui::layout::current_window();
                const auto last_y=panel->bounds.y+panel->last_item.y;
                if(frame==1)visible=last_y>=panel->bounds.y&&last_y+18<=panel->bounds.bottom();
                xui::end_child();
            }
            xui::end_window();
        }
        xui::end();xdraw::end_frame();ImGui::Render();
    }
    Expect(visible,"search scroll reveals the exact control below the fold");
    xui::set_highlight_target("",.1f);xui::ctx().child_scroll_cache.clear();
}
void LayoutAndCometChecks(IDirect3DDevice9* device,const std::filesystem::path& output) {
    const auto saved=config::to_json();
    auto& settings=feature_ui::settings;
    using namespace esp::layout;
    const Rect body{200,150,120,220};
    const auto overlap=[](Rect a,Rect b) {return a.x<b.right()&&a.right()>b.x&&a.y<b.bottom()&&a.bottom()>b.y;};
    bool clear=true;
    for(int hp=0;hp<SideCount;++hp)for(int name=0;name<SideCount;++name)for(int distance=0;distance<SideCount;++distance) {
        const auto health=Health(body,hp,1.f,22,9,12);
        const auto layout=Labels(body,name,distance,{0,0,40,12},{0,0,24,12},&health,hp);
        clear=clear&&!overlap(layout.name,layout.distance)&&!overlap(layout.name,health.outline)&&
            !overlap(layout.distance,health.outline)&&!overlap(layout.name,health.text)&&!overlap(layout.distance,health.text);
    }
    Expect(clear,"all 64 name/distance/health side combinations reserve separate lanes");
    const auto reserved=Labels(body,Right,Right,{0,0,40,26},{0,0,24,12},nullptr,Left,true,true,140);
    Expect(reserved.name.y>=body.y+140&&reserved.distance.y>=reserved.name.bottom()+3.f,
           "right-side labels reserve status flags and a class-name row");
    const auto invalid=Labels(body,-99,99,{0,0,40,12},{0,0,24,12});
    Expect(invalid.name.right()<body.x&&invalid.distance.y>=body.bottom(),"invalid label slots are normalized");
    menu_effects::CometField snow;
    const auto first=snow.particles()[0];
    snow.advance(960,640,0.f);
    Expect(first.x==snow.particles()[0].x&&first.y==snow.particles()[0].y,"paused comets keep their positions");
    menu_effects::CometField whole,split;
    whole.advance(960,640,1.f/60.f);split.advance(960,640,1.f/120.f);split.advance(960,640,1.f/120.f);
    Expect(std::abs(whole.particles()[0].x-split.particles()[0].x)<.00001f&&
           std::abs(whole.particles()[0].y-split.particles()[0].y)<.00001f,
           "comet motion follows elapsed time rather than the frame count");
    for(int i=0;i<2000;++i)snow.advance(960,640,1.f/60.f);
    Expect(std::all_of(snow.particles().begin(),snow.particles().end(),[](const auto& p) {
        return std::isfinite(p.x)&&std::isfinite(p.y)&&p.x>=0.f&&p.x<1.f&&p.y>=0.f&&p.y<1.f;
    }),"comets wrap inside fixed storage without drifting out of range");
    xdraw::begin_frame(false);auto& list=xdraw::get();
    snow.draw(list,100,100,300,200,1.f/60.f,0.f);
    Expect(list.vertices.empty(),"hidden comets submit no geometry");
    snow.draw(list,100,100,300,200,1.f/60.f,1.f);
    Expect(!list.vertices.empty()&&list.vertices.size()<=600&&list.clip_stack.empty()&&
           list.commands[0].has_scissor&&list.commands[0].scissor.left==108&&list.commands[0].scissor.right==392,
           "stars and comet trails stay within 600 vertices and restore the menu clip");
    list.clear();
    const std::array<float,4> menu_bounds{100,100,400,400};
    snow.draw(list,0,0,500,520,1.f/60.f,1.f,&menu_bounds);
    Expect(!list.vertices.empty()&&std::none_of(list.vertices.begin(),list.vertices.end(),[&](const auto& v) {
        return v.pos[0]>=menu_bounds[0]&&v.pos[0]<=menu_bounds[2]&&v.pos[1]>=menu_bounds[1]&&v.pos[1]<=menu_bounds[3];
    }),"comets, stars and complete trails stay outside the menu rectangle");
    list.clear();const std::array<float,4> full_screen{0,0,500,520};
    snow.draw(list,0,0,500,520,1.f/60.f,1.f,&full_screen);
    Expect(list.vertices.empty()&&list.clip_stack.empty(),"a menu covering the viewport excludes every background particle");

    settings.health.value=false;settings.names.value=settings.distance.value=true;
    settings.health_position=Left;settings.name_position=Top;settings.distance_position=Bottom;
    const xui::rect canvas{40,40,420,440};
    hunter_preview::CancelInput();
    auto* font=rendering::g_fonts.esp;
    const auto name_size=xdraw::measure_text("Hunter",font),distance_size=xdraw::measure_text("15 m",font);
    const auto placements=[&](int name_side,int distance_side) {
        return Labels(body,name_side,distance_side,{0,0,name_size.first,name_size.second},
                      {0,0,distance_size.first,distance_size.second});
    };
    const auto center=[](Rect r) {return std::pair{r.x+r.w*.5f,r.y+r.h*.5f};};
    const auto frame=[&](std::pair<float,float> pointer,bool click=false,bool down=false,bool released=false,bool escape=false,bool right=false) {
        ImGui_ImplDX9_NewFrame();ImGui::NewFrame();
        auto& input=xui::ctx().input;input={};input.mouse_x=pointer.first;input.mouse_y=pointer.second;
        input.prev_mouse_x=pointer.first;input.prev_mouse_y=pointer.second;
        input.mouse_clicked=click;input.mouse_down=down;input.mouse_released=released;input.rmb_clicked=right;
        if(escape)input.keys[VK_ESCAPE]=true;
        xdraw::begin_frame();xui::begin();float x=10,y=10,w=480,h=500;
        if(xui::begin_window("##layout_editor_qa",x,y,w,h,false)) {
            auto& draw=xui::draw::current();draw.push_clip(canvas.x,canvas.y,canvas.w,canvas.h);
            draw.rect(body.x,body.y,body.w,body.h,{255,89,107});
            hunter_preview::EditEspLayout(canvas,body);draw.pop_clip();xui::end_window();
        }
        xui::end();xdraw::end_frame();ImGui::Render();
        device->Clear(0,nullptr,D3DCLEAR_TARGET,D3DCOLOR_ARGB(255,36,42,53),1,0);
        if(SUCCEEDED(device->BeginScene())) {ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());device->EndScene();}
    };
    const auto name_top=center(placements(Top,Bottom).name),name_right=center(placements(Right,Bottom).name);
    frame(name_top,true,true);Expect(hunter_preview::OwnsPointer(),"dragging the name captures the preview pointer");
    frame(name_right,false,true);
    Expect(settings.name_position.value==Top,"name slot changes only when released");
    const auto& drawing=xdraw::get();
    Expect(std::any_of(drawing.vertices.begin(),drawing.vertices.end(),[](const auto& v) {
        return v.col.r==255&&v.col.g==199&&v.col.b==64;
    }),"available destinations use gold independently of the menu theme");
    Expect(Save(device,(output/L"name-golden-slots.png").c_str(),500,520),"name drag destination screenshot is saved");
    frame(name_right,false,false,true);
    Expect(settings.name_position.value==Right&&!hunter_preview::OwnsPointer(),"dropping the name applies its side and releases the pointer");
    const auto distance_bottom=center(placements(Right,Bottom).distance),distance_top=center(placements(Right,Top).distance);
    frame(distance_bottom,true,true);frame(distance_top,false,true);frame(distance_top,false,false,true);
    Expect(settings.distance_position.value==Top,"distance can be dragged independently to the upper slot");
    const auto distance_right=center(placements(Right,Right).distance);
    frame(distance_top,true,true);frame(distance_right,false,true);
    Expect(Save(device,(output/L"distance-golden-slots.png").c_str(),500,520),"distance drag destination screenshot is saved");
    frame(distance_right,false,false,true);
    Expect(settings.distance_position.value==Right,"name and distance can share a side without sharing a row");
    const auto name_now=center(placements(Right,Right).name),name_left=center(placements(Left,Right).name);
    frame(name_now,true,true);frame(name_left,false,true);frame(name_left,false,true,false,true);
    Expect(settings.name_position.value==Right&&!hunter_preview::OwnsPointer(),"Escape cancels a name move without changing its saved side");
    frame(name_now,true,true);frame(name_left,false,true,false,false,true);
    Expect(settings.name_position.value==Right&&!hunter_preview::OwnsPointer(),"right-click cancels a label move");
    frame(name_now,true,true);frame({900.f,700.f},false,true);frame({900.f,700.f},false,false,true);
    Expect(settings.name_position.value==Right,"dropping outside the preview preserves the previous slot");
    settings.health.value=true;
    frame({body.x-5.5f,body.y+body.h*.5f},true,true);
    frame({body.x+body.w*.5f,body.bottom()+5.f},false,true);
    frame({body.x+body.w*.5f,body.bottom()+5.f},false,false,true);
    Expect(settings.health_position.value==Bottom,"health bar dragging still works with movable labels");
    settings.health.value=settings.names.value=settings.distance.value=false;
    frame(name_top,true,true);
    Expect(!hunter_preview::OwnsPointer(),"hidden labels do not leave invisible drag targets");
    hunter_preview::CancelInput();config::from_json(saved);
    Expect(config::to_json()==saved,"layout QA restores all user settings after testing");
}
}
int main(int argc,char** argv) {
    const std::filesystem::path output=argc>1?argv[1]:"build/menu-ui-qa";
    std::filesystem::create_directories(output);
    std::setvbuf(stdout,nullptr,_IONBF,0);
    std::fputs("UI preview: creating Direct3D device.\n",stderr);
    CoInitializeEx(nullptr,COINIT_MULTITHREADED);
    const auto instance=GetModuleHandleW(nullptr);
    WNDCLASSW cls{};cls.hInstance=instance;cls.lpfnWndProc=DefWindowProcW;cls.lpszClassName=L"VS3MenuUiCapture";RegisterClassW(&cls);
    const auto window=CreateWindowExW(0,cls.lpszClassName,L"",WS_POPUP,0,0,1920,1080,nullptr,nullptr,instance,nullptr);
    ComPtr<IDirect3D9> d3d;d3d.Attach(Direct3DCreate9(D3D_SDK_VERSION));ComPtr<IDirect3DDevice9> device;
    D3DPRESENT_PARAMETERS pp{};pp.Windowed=TRUE;pp.hDeviceWindow=window;pp.SwapEffect=D3DSWAPEFFECT_DISCARD;
    pp.BackBufferWidth=1920;pp.BackBufferHeight=1080;pp.BackBufferFormat=D3DFMT_A8R8G8B8;
    if(!window||!d3d||FAILED(d3d->CreateDevice(0,D3DDEVTYPE_HAL,window,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&pp,&device)))return 1;
    ImGui::CreateContext();auto& io=ImGui::GetIO();io.IniFilename=nullptr;io.LogFilename=nullptr;io.DisplaySize={1280,800};io.DeltaTime=1.f/60.f;
    io.Fonts->AddFontDefault();ImGui_ImplDX9_Init(device.Get());
    ui::set_persistence(false);
    std::fputs("UI preview: initializing menu.\n",stderr);
    if(!menu::Initialize(device.Get(),window))return 2;
    if(argc>2 && (std::string_view(argv[2])=="--melee-only" || std::string_view(argv[2])=="--charger-only" || std::string_view(argv[2])=="--aim-only" || std::string_view(argv[2])=="--weapons-web" || std::string_view(argv[2])=="--guard-only")) {
        const bool charger=std::string_view(argv[2])=="--charger-only";
        const bool aim=std::string_view(argv[2])=="--aim-only";
        const bool weapons_web=std::string_view(argv[2])=="--weapons-web";
        const bool guard=std::string_view(argv[2])=="--guard-only";
        const auto render=[&](float mx=-1,float my=-1,bool pressed=false,bool held=false,bool right=false) {
            ImGui_ImplDX9_NewFrame();ImGui::NewFrame();
            auto& in=xui::ctx().input;in.prev_mouse_x=in.mouse_x;in.prev_mouse_y=in.mouse_y;
            in.mouse_x=mx;in.mouse_y=my;in.mouse_clicked=pressed;in.mouse_down=held;
            in.rmb_clicked=right;
            xdraw::begin_frame();rendering::g_menu.set_open(true);rendering::g_menu.draw();
            rendering::g_menu.draw_watermark();xdraw::end_frame();ImGui::Render();
            device->Clear(0,nullptr,D3DCLEAR_TARGET,D3DCOLOR_ARGB(255,36,42,53),1,0);
            if(SUCCEEDED(device->BeginScene())){ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());device->EndScene();}
        };
        const auto click=[&](float x,float y){render(x,y);render(x,y,true,true);render(x,y);for(int i=0;i<10;++i)render();};
        for(int i=0;i<40;++i)render();
        if(guard) {
            click(160,349);click(900,129);bool ok=true;
            for(int language=0;language<2;++language)for(int scale=0;scale<3;++scale) {
                ui::preferences().language=language;ui::preferences().scale=scale;
                io.DisplaySize=scale?ImVec2{1920,1080}:ImVec2{1280,800};for(int i=0;i<15;++i)render();
                const auto file=std::string("protection-")+(language?"en-":"es-")+std::to_string(100+scale*25)+".png";
                ok=Save(device.Get(),(output/file).c_str(),scale?1920:1280,scale?1080:800)&&ok;
            }
            ui::preferences().language=0;ui::preferences().scale=0;io.DisplaySize={1280,800};for(int i=0;i<10;++i)render();
            render(400,202,false,false,true);for(int i=0;i<10;++i)render(400,202);
            ok=Save(device.Get(),(output/L"protection-bind.png").c_str(),1280,800)&&ok;
            std::puts(ok?"PASS: protection controls captured in English and Spanish at 100, 125 and 150 percent with bind menu.":"FAIL: protection capture");
            menu::InvalidateDeviceObjects();ImGui_ImplDX9_Shutdown();ImGui::DestroyContext();DestroyWindow(window);return ok?0:3;
        }
        if(weapons_web) {
            feature_ui::selected_weapon=weapons::Find("CRifle_AK47");
            auto& profile=feature_ui::weapon_settings.items[feature_ui::selected_weapon];
            profile.combat.value=true;profile.view.value=true;
            click(160,181);click(880,129);
        } else if(aim) click(160,181);else if(charger) click(160,349);else {click(160,223);click(820,129);}
        bool ok=Save(device.Get(),(output/(weapons_web?L"weapons-web.png":aim?L"aim-100.png":charger?L"charger-100.png":L"fast-melee-100.png")).c_str(),1280,800);
        if(weapons_web) {
            menu::InvalidateDeviceObjects();ImGui_ImplDX9_Shutdown();ImGui::DestroyContext();DestroyWindow(window);
            std::puts(ok?"PASS: current weapon settings captured.":"FAIL: weapon settings capture");return ok?0:3;
        }
        io.DisplaySize={1920,1080};ui::preferences().scale=1;for(int i=0;i<10;++i)render();
        ok=Save(device.Get(),(output/(aim?L"aim-125.png":charger?L"charger-125.png":L"fast-melee-125.png")).c_str(),1920,1080)&&ok;
        ui::preferences().scale=2;for(int i=0;i<10;++i)render();
        ok=Save(device.Get(),(output/(aim?L"aim-150.png":charger?L"charger-150.png":L"fast-melee-150.png")).c_str(),1920,1080)&&ok;
        if(charger) {
            ui::preferences().scale=0;io.DisplaySize={1280,800};for(int i=0;i<10;++i)render();
            render(400,337,false,false,true);for(int i=0;i<10;++i)render(400,337);
            ok=Save(device.Get(),(output/L"charger-bind.png").c_str(),1280,800)&&ok;
        }
        std::puts(ok?"PASS: feature controls captured at 100, 125 and 150 percent.":"FAIL: feature capture");
        menu::InvalidateDeviceObjects();ImGui_ImplDX9_Shutdown();ImGui::DestroyContext();DestroyWindow(window);return ok?0:3;
    }
    if(argc>2 && std::string_view(argv[2])=="--accuracy-only") {
        const auto baseline=config::to_json();
        auto& bound=feature_ui::weapon_settings.items[6].shotgun_no_spread;
        auto& amount=feature_ui::weapon_settings.items[6].shotgun_accuracy;
        amount=50.f;bound.value=false;bound.bind={};bound.bind.key=VK_F6;bound.bind.mode=xui::bind_mode::hold_on;
        xui::input_state keys{};keys.keys[VK_F6]=true;xui::binds::process(keys);
        Expect(bound.value,"Accuracy hold activates while the assigned key is held");
        keys.keys[VK_F6]=false;xui::binds::process(keys);Expect(!bound.value,"Accuracy hold releases immediately");
        bound.bind.mode=xui::bind_mode::toggle;bound.bind.active=false;
        keys.keys[VK_F6]=true;xui::binds::process(keys);Expect(bound.value,"Accuracy toggle activates on a press");
        xui::binds::process(keys);Expect(bound.value,"Accuracy toggle does not repeat while held");
        keys.keys[VK_F6]=false;xui::binds::process(keys);Expect(bound.value,"Accuracy toggle retains state on release");
        keys.keys[VK_F6]=true;xui::binds::process(keys);Expect(!bound.value,"second Accuracy press disables compensation");
        Expect(amount.value==50.f,"bind activation preserves the chosen slider amount");
        keys.keys[VK_F6]=false;xui::binds::process(keys);config::from_json(baseline);
        const auto render=[&](float mx=-1,float my=-1,bool pressed=false,bool held=false) {
            ImGui_ImplDX9_NewFrame();ImGui::NewFrame();
            auto& in=xui::ctx().input;in.prev_mouse_x=in.mouse_x;in.prev_mouse_y=in.mouse_y;
            in.mouse_x=mx;in.mouse_y=my;in.mouse_clicked=pressed;in.mouse_down=held;
            xdraw::begin_frame();rendering::g_menu.set_open(true);rendering::g_menu.draw();
            rendering::g_menu.draw_watermark();xdraw::end_frame();ImGui::Render();
            device->Clear(0,nullptr,D3DCLEAR_TARGET,D3DCOLOR_ARGB(255,36,42,53),1,0);
            if(SUCCEEDED(device->BeginScene())){ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());device->EndScene();}
        };
        for(int i=0;i<40;++i)render();render(880,129);render(880,129,true,true);render(880,129);
        for(int i=0;i<12;++i)render();
        bool ok=true;
        const char* names[]{"CPumpShotgun","CShotgun_Chrome","CAutoShotgun","CShotgun_SPAS"};
        const wchar_t* files[]{L"accuracy-pump.png",L"accuracy-chrome.png",L"accuracy-auto.png",L"accuracy-spas.png"};
        for(int i=0;i<4;++i) {
            feature_ui::selected_weapon=weapons::Find(names[i]);
            auto& p=feature_ui::weapon_settings.items[feature_ui::selected_weapon];
            p.shotgun_no_spread.value=true;p.shotgun_accuracy=i==0?0.f:i==3?100.f:50.f;
            if(i==3) {p.shotgun_no_spread.bind.key=VK_F6;p.shotgun_no_spread.bind.mode=xui::bind_mode::toggle;p.shotgun_no_spread.bind.active=true;}
            for(int frame=0;frame<4;++frame)render();
            ok=Save(device.Get(),(output/files[i]).c_str(),1280,800)&&ok;
        }
        std::printf("%u Accuracy binding checks, %u failed.\n",checks,failures);
        std::puts(ok?"PASS: all four shotgun Accuracy panels captured.":"FAIL: Accuracy capture");
        menu::InvalidateDeviceObjects();ImGui_ImplDX9_Shutdown();ImGui::DestroyContext();DestroyWindow(window);return ok&&!failures?0:3;
    }
    if(argc>2 && std::string_view(argv[2])=="--watermark-only") {
        auto& s=feature_ui::settings;s.watermark.value=true;
        const auto render=[&] {
            ImGui_ImplDX9_NewFrame();ImGui::NewFrame();
            xdraw::begin_frame();rendering::g_menu.set_open(true);rendering::g_menu.draw();
            rendering::g_menu.draw_watermark();xdraw::end_frame();ImGui::Render();
            device->Clear(0,nullptr,D3DCLEAR_TARGET,D3DCOLOR_ARGB(255,36,42,53),1,0);
            if(SUCCEEDED(device->BeginScene())){ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());device->EndScene();}
        };
        for(int i=0;i<40;++i)render();
        bool ok=Save(device.Get(),(output/L"watermark-fps.png").c_str(),1280,800);
        s.watermark_fps.value=false;render();
        ok=Save(device.Get(),(output/L"watermark-no-fps.png").c_str(),1280,800)&&ok;
        std::puts(ok?"PASS: watermark captured with FPS enabled and disabled.":"FAIL: watermark capture");
        menu::InvalidateDeviceObjects();ImGui_ImplDX9_Shutdown();ImGui::DestroyContext();DestroyWindow(window);return ok?0:3;
    }
    std::fputs("UI preview: checking behavior and search scrolling.\n",stderr);
    BehaviorChecks();SearchScrollCheck();SteamProfileChecks();LayoutAndCometChecks(device.Get(),output);
    std::fputs("UI preview: rendering pages.\n",stderr);
    auto& s=feature_ui::settings;s.boxes.value=s.health.value=s.names.value=s.distance.value=s.skeleton.value=true;
    auto frame=[&](float x=-1,float y=-1,bool click=false,bool held=false,bool release=false) {
        static unsigned frame_number{};++frame_number;
        if(frame_number<=3||frame_number%20==0)std::fprintf(stderr,"UI preview: frame %u.\n",frame_number);
        ImGui_ImplDX9_NewFrame();ImGui::NewFrame();
        auto& in=xui::ctx().input;in.prev_mouse_x=in.mouse_x;in.prev_mouse_y=in.mouse_y;
        in.mouse_x=x;in.mouse_y=y;in.mouse_clicked=click;in.mouse_down=held;in.mouse_released=release;
        xdraw::begin_frame();rendering::g_menu.set_open(true);rendering::g_menu.draw();rendering::g_menu.draw_watermark();xdraw::end_frame();
        ImGui::Render();device->Clear(0,nullptr,D3DCLEAR_TARGET,D3DCOLOR_ARGB(255,36,42,53),1,0);
        if(SUCCEEDED(device->BeginScene())){ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());device->EndScene();}
    };
    auto click=[&](float x,float y){frame(x,y);frame(x,y,true,true);frame(x,y,false,false,true);for(int i=0;i<20;++i)frame(x,y);};
    const auto check_background=[&](float scale) {
        const auto& background=xdraw::get(xdraw::layer::bottom);
        Expect(!background.vertices.empty()&&std::none_of(background.vertices.begin(),background.vertices.end(),[&](const auto& v) {
            return v.pos[0]>=100.f&&v.pos[0]<=100.f+960.f*scale&&v.pos[1]>=100.f&&v.pos[1]<=100.f+640.f*scale;
        }),"triangular particles remain outside the menu at the current scale");
        Expect(xdraw::interface_scale==1.f&&background.clip_stack.empty(),"background triangles restore scale and clip state");
        const auto& interior=xdraw::get(xdraw::layer::middle);
        const auto comet_command=std::find_if(interior.commands.begin(),interior.commands.end(),[&](const auto& command) {
            return command.has_scissor&&std::abs(command.scissor.left-(100.f+8.f*scale))<=1.f&&
                   std::abs(command.scissor.top-(100.f+8.f*scale))<=1.f&&
                   std::abs(command.scissor.right-(100.f+952.f*scale))<=1.f&&
                   std::abs(command.scissor.bottom-(100.f+632.f*scale))<=1.f;
        });
        Expect(comet_command!=interior.commands.end(),"interior comet geometry is clipped to the menu at the current scale");
        auto* label_font=rendering::g_fonts.inter_bold[rendering::fonts::size::normal];
        if(scale>1.01f&&label_font->interface_variants[scale<1.375f?0:1])
            label_font=label_font->interface_variants[scale<1.375f?0:1];
        const auto first_text=std::find_if(interior.commands.begin(),interior.commands.end(),[&](const auto& command) {
            return command.texture&&(command.texture==label_font->atlas_srv.Get()||command.texture==label_font->atlas_tex.Get());
        });
        Expect(comet_command!=interior.commands.end()&&first_text!=interior.commands.end()&&comet_command<first_text,
               "comets draw behind the menu labels and controls");
    };
    for(int i=0;i<40;++i)frame();click(160,265);
    check_background(1.f);
    ui::preferences().snow=0;frame();
    Expect(xdraw::get(xdraw::layer::bottom).vertices.empty(),"turning animation off clears the exterior triangles");
    ui::preferences().snow=1;frame();
    std::fputs("UI preview: saving screenshots.\n",stderr);
    auto save=[&](const wchar_t* name,int width,int height) {return Save(device.Get(),(output/name).c_str(),width,height);};
    bool ok=save(L"general-es-100.png",1280,800);
    click(640,129);ok=save(L"appearance-es.png",1280,800)&&ok;
    click(880,129);ok=save(L"colors-es.png",1280,800)&&ok;
    ui::preferences().language=1;for(int i=0;i<10;++i)frame();
    ok=save(L"colors-en.png",1280,800)&&ok;
    ui::preferences().language=0;click(160,391);
    ok=save(L"profiles-es.png",1280,800)&&ok;
    click(160,265);click(390,129);
    io.DisplaySize={1920,1080};ui::preferences().scale=1;
    for(int i=0;i<10;++i)frame();check_background(1.25f);ok=save(L"general-es-125.png",1920,1080)&&ok;
    ui::preferences().scale=2;for(int i=0;i<10;++i)frame();check_background(1.5f);ok=save(L"general-es-150.png",1920,1080)&&ok;
    ui::preferences().scale=0;io.DisplaySize={1280,800};for(int i=0;i<10;++i)frame();
    click(160,223);s.prioritize_classes.value=false;for(int i=0;i<15;++i)frame();
    ok=save(L"classes-off-es.png",1280,800)&&ok;
    s.prioritize_classes.value=true;for(int i=0;i<15;++i)frame();
    ok=save(L"classes-empty-es.png",1280,800)&&ok;
    s.priority_smoker.value=s.priority_hunter.value=s.priority_charger.value=true;for(int i=0;i<15;++i)frame();
    ok=save(L"classes-selected-es.png",1280,800)&&ok;
    ui::preferences().language=1;for(int i=0;i<10;++i)frame();
    ok=save(L"classes-selected-en.png",1280,800)&&ok;
    ui::preferences().language=0;io.DisplaySize={1920,1080};ui::preferences().scale=1;
    for(int i=0;i<10;++i)frame();ok=save(L"classes-selected-125.png",1920,1080)&&ok;
    ui::preferences().scale=2;for(int i=0;i<10;++i)frame();ok=save(L"classes-selected-150.png",1920,1080)&&ok;
    ui::preferences().scale=0;io.DisplaySize={1280,800};for(int i=0;i<10;++i)frame();
    click(160,181);click(390,129);ok=save(L"aim-general-es.png",1280,800)&&ok;
    click(640,129);ok=save(L"combat-infected-es.png",1280,800)&&ok;
    io.DisplaySize={1920,1080};ui::preferences().scale=1;for(int i=0;i<10;++i)frame();
    ok=save(L"infected-hold-125.png",1920,1080)&&ok;
    ui::preferences().scale=2;for(int i=0;i<10;++i)frame();ok=save(L"infected-hold-150.png",1920,1080)&&ok;
    ui::preferences().scale=0;io.DisplaySize={1280,800};for(int i=0;i<10;++i)frame();
    click(880,129);ok=save(L"weapons-es.png",1280,800)&&ok;
    click(160,223);click(820,129);ok=save(L"automation-es.png",1280,800)&&ok;
    click(160,307);ok=save(L"world-es.png",1280,800)&&ok;
    click(817,129);ok=save(L"effects-es.png",1280,800)&&ok;
    click(971,129);ok=save(L"hud-es.png",1280,800)&&ok;
    click(160,349);click(640,129);ok=save(L"camera-es.png",1280,800)&&ok;
    io.DisplaySize={820,340};ImGui_ImplDX9_NewFrame();ImGui::NewFrame();
    auto* names=ImGui::GetForegroundDrawList();
    float name_y=20.f;
    for(const auto* sample:{"ESP Unicode / 18 px","♡ 𝓐lex ★ ⠿ Ω Ж","𝕯𝖊𝖛𝖎𝖑  ♛  ☠  ツ",
                           "Ångel  François  Игрок  Ωmega","☯  ♫  ✿  ★  →  ∞  ⣿  😀"}) {
        names->AddText(esp::font::At(18),18,{24,name_y},IM_COL32(230,230,240,255),sample);name_y+=48;
    }
    names->AddText(esp::font::At(12),12,{24,280},IM_COL32(230,230,240,255),"12 px: ♡ 𝓐lex ★ ⠿ Ω Ж 𝕯𝖊𝖛𝖎𝖑 ツ");
    ImGui::Render();device->Clear(0,nullptr,D3DCLEAR_TARGET,D3DCOLOR_ARGB(255,22,22,26),1,0);
    if(SUCCEEDED(device->BeginScene())){ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());device->EndScene();}
    ok=save(L"unicode-names.png",820,340)&&ok;
    std::printf("%u UI behavior checks, %u failed.\n",checks,failures);
    std::puts(ok?"PASS: bilingual pages and three native UI scales captured.":"FAIL: UI capture");
    menu::InvalidateDeviceObjects();ImGui_ImplDX9_Shutdown();ImGui::DestroyContext();DestroyWindow(window);return ok&&!failures?0:3;
}
