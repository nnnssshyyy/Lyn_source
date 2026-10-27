#include "menu.h"
#include "hooks.h"
#include "velocity_ui/rendering.hpp"
#include "velocity_ui/settings.hpp"
#include "velocity_ui/features_ui.hpp"
#include "velocity_ui/hunter_preview.hpp"
#include "velocity_ui/spectators_window.hpp"
#include "velocity_ui/radar_window.hpp"
#include "velocity_ui/chat_window.hpp"
#include "velocity_ui/visuals_window.hpp"
#include "velocity_ui/font_assets.hpp"
#include "log.h"
#include "esp_font.h"
#include "velocity_ui/ui_preferences.hpp"
#include "velocity_ui/steam_profile.hpp"
#include <cstdio>

namespace {
bool initialized{};
bool sync_pointer{};
ULONGLONG blur_until{},next_blur{};
std::span<const std::byte> bytes(const unsigned char* data,size_t size) {
    return {reinterpret_cast<const std::byte*>(data),size};
}
bool InitializeEspFont() {
    return esp::font::Initialize();
}
}
namespace rendering {
void fonts::initialize() {
    constexpr float sizes[]{12,15,18};
    constexpr float pixel_sizes[]{9,10.5f,14};
    for(size_t i=0;i<3;++i) {
        inter_medium.sizes[i]=i==1 ? xdraw::primary_font() : xdraw::load_font(
            bytes(velocity_assets::inter_medium,sizeof(velocity_assets::inter_medium)),sizes[i]);
        inter_bold.sizes[i]=xdraw::load_font(bytes(velocity_assets::inter_bold,sizeof(velocity_assets::inter_bold)),sizes[i]);
        smallest_pixel7.sizes[i]=xdraw::load_font(bytes(velocity_assets::pixel7,sizeof(velocity_assets::pixel7)),pixel_sizes[i]);
    }
    esp=inter_bold.sizes[0];
    if(esp)esp->fallback=inter_medium.sizes[0];
}
}
namespace menu {
void Style() { ImGui::StyleColorsDark(); }
bool Initialize(IDirect3DDevice9* device,HWND hwnd) {
    if(initialized) return true;
    if(!xdraw::initialize(device) || !xui::initialize(hwnd)) return false;
    if(!xdraw::load_font(bytes(velocity_assets::inter_medium,sizeof(velocity_assets::inter_medium)),15)) return false;
    xdraw::primary_font()->fallback=xdraw::load_font(bytes(velocity_assets::noto_math,sizeof(velocity_assets::noto_math)),15);
    rendering::g_fonts.initialize();
    rendering::g_fonts.menu_body=xdraw::load_font(bytes(velocity_assets::inter_medium,sizeof(velocity_assets::inter_medium)),16);
    rendering::g_fonts.menu_heading=xdraw::load_font(bytes(velocity_assets::inter_bold,sizeof(velocity_assets::inter_bold)),16);
    if(!rendering::g_fonts.menu_body||!rendering::g_fonts.menu_heading)return false;
    rendering::g_fonts.menu_body->fallback=xdraw::primary_font()->fallback;
    rendering::g_fonts.menu_heading->fallback=rendering::g_fonts.menu_body;
    // Rasterize body/bold fonts at each menu density; keep HUD fonts independent.
    for(int i=0;i<2;++i) {
        const float density=1.25f+.25f*i;
        auto* body=xdraw::load_font(bytes(velocity_assets::inter_medium,sizeof(velocity_assets::inter_medium)),16*density);
        auto* bold=xdraw::load_font(bytes(velocity_assets::inter_bold,sizeof(velocity_assets::inter_bold)),16*density);
        auto* fallback=xdraw::load_font(bytes(velocity_assets::noto_math,sizeof(velocity_assets::noto_math)),16*density);
        if(!body||!bold||!fallback)return false;
        body->density=bold->density=fallback->density=density;body->fallback=fallback;bold->fallback=body;
        rendering::g_fonts.menu_body->interface_variants[i]=body;
        rendering::g_fonts.menu_heading->interface_variants[i]=bold;
    }
    ui::load_preferences();
    if(!rendering::g_fonts.esp || !InitializeEspFont())return false;
    for(size_t i=0;i<3;++i)
        if(!rendering::g_fonts.inter_medium.sizes[i] || !rendering::g_fonts.inter_bold.sizes[i] ||
           !rendering::g_fonts.smallest_pixel7.sizes[i]) return false;
    config::initialize(&feature_ui::Defaults);
    settings::finalize_binds();
    rendering::g_menu.initialize_graphics_before_rpack();
    rendering::g_menu.finalize_textures_after_rpack();
    rendering::g_menu.set_open(false);
    initialized=true;
    app::Log("Velocity UI: XUI original, fuentes Inter/Pixel7, iconos SVG y adaptador DX9.");
    return true;
}
void ClearInput() {
    if(!initialized) return;
    auto& c=xui::ctx();
    c.input={};
    c.active_window=c.active_resize=c.active_slider=c.active_child_scroll=xui::null_id;
    c.active_text_input=c.active_keybind=c.active_slider_edit=xui::null_id;
    for(auto& item:c.child_scroll_cache) item.second.scrollbar_dragging=false;
    sync_pointer=true;
    xui::overlays::close_all();
    hunter_preview::CancelInput();
    spectators_window::CancelInput();
    radar_window::CancelInput();
    chat_window::CancelInput();
}
void Input(UINT msg,WPARAM wp,LPARAM lp) {
    if(!initialized) return;
    if(msg==WM_KILLFOCUS) { ClearInput(); return; }
    xui::wndproc(msg,wp,lp);
}
void PrepareFrame() {
    if(!initialized) return;
    // Privacy must not add a full-resolution game copy plus seven blur passes.
    // It also runs inside Steam's scene, where BeginScene cannot be nested.
    if(feature_ui::settings.capture_privacy.value){
        if(blur_until||next_blur)xdraw::invalidate_device_objects();
        blur_until=next_blur=0;return;
    }
    const auto now=GetTickCount64();
    if(hooks::MenuOpen())blur_until=now+200;
    if(now>=blur_until||now<next_blur)return;
    // The animated menu still runs at display rate; only its blurred scene
    // refresh is limited. A short time-based fade replaces 45 full-size copies.
    next_blur=now+40;xdraw::capture_blur();
}
void InvalidateDeviceObjects() { steam_profile::invalidate_device_objects(); hunter_preview::InvalidateDeviceObjects(); xdraw::invalidate_device_objects(); }
void Draw(esp::Options&,const esp::Frame&,unsigned) {
    if(!initialized) return;
    const bool open=hooks::MenuOpen();
    auto& input=xui::ctx().input;
    const auto pos=ImGui::GetIO().MousePos;
    if(open && ImGui::IsMousePosValid(&pos)) { input.mouse_x=pos.x; input.mouse_y=pos.y; }
    if(sync_pointer) {
        input.prev_mouse_x=input.mouse_x; input.prev_mouse_y=input.mouse_y;
        sync_pointer=false;
    }
    rendering::g_menu.set_open(open);
    if(!open) xui::binds::process(input);
    xdraw::begin_frame();
    visuals_window::BeginMenuInput(open);
    chat_window::BeginMenuInput(open);
    radar_window::BeginMenuInput(open);
    spectators_window::BeginMenuInput(open);
    rendering::g_menu.draw();
    rendering::g_menu.draw_watermark();
    spectators_window::EndMenuInput();
    radar_window::EndMenuInput();
    chat_window::EndMenuInput();
    visuals_window::EndMenuInput();
    xdraw::end_frame();
    feature_ui::Sync();
    spectators_window::Draw(open);
    radar_window::Draw(open);
    chat_window::Draw(open);
    visuals_window::Draw(open);
}
}
