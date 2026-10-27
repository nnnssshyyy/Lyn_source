#include "radar_window.hpp"
#include "features_ui.hpp"
#include "hud_widgets.hpp"
#include "hunter_preview.hpp"
#include "spectators_window.hpp"
#include "../radar_logic.h"
#include "../esp_style.h"
#include <imgui.h>
#include <optional>
#include <cstdio>

namespace radar_window {
namespace {
constexpr float Header=34.f;
xui::rect bounds{};
bool captured{},dragging{},close_hovered{};
float grab_x{},grab_y{};
std::optional<xui::input_state> saved_input;
ImU32 Color(xdraw::color c) {return hud_widgets::Color(c);}
bool Layout() {
    const auto screen=ImGui::GetIO().DisplaySize;
    if(!std::isfinite(screen.x) || !std::isfinite(screen.y) || screen.x<180 || screen.y<220) return false;
    auto& s=feature_ui::settings;
    const float size=std::isfinite(s.radar_size.value)?std::clamp(s.radar_size.value,160.f,400.f):230.f;
    bounds.w=std::min(size,std::min(screen.x-16,screen.y-Header-16));bounds.h=bounds.w+Header;
    if(!std::isfinite(s.radar_x.value)) s.radar_x=0.f;
    if(!std::isfinite(s.radar_y.value)) s.radar_y=.15f;
    s.radar_x=std::clamp(s.radar_x.value,0.f,1.f);s.radar_y=std::clamp(s.radar_y.value,0.f,1.f);
    bounds.x=8+s.radar_x.value*std::max(0.f,screen.x-bounds.w-16);
    bounds.y=8+s.radar_y.value*std::max(0.f,screen.y-bounds.h-16);
    return true;
}
xui::rect Close() {return {bounds.right()-30,bounds.y+6,22,22};}
}
void CancelInput() {captured=dragging=close_hovered=false;saved_input.reset();}
bool OwnsPointer() {return captured;}
void BeginMenuInput(bool open) {
    close_hovered=false;
    if(!open || !feature_ui::settings.radar.value || !Layout()) {CancelInput();return;}
    auto& c=xui::ctx();auto& input=c.input;
    auto& settings=feature_ui::settings;
    if(hud_widgets::Resize(&bounds,bounds,settings.radar_size.value,nullptr,settings.radar_x.value,settings.radar_y.value,160,400,100,700,true)) {
        saved_input=input;hud_widgets::Mask(input);Layout();return;
    }
    if(!captured && (spectators_window::OwnsPointer() || hunter_preview::OwnsPointer() || c.overlay_blocking() ||
       c.active_window || c.active_resize || c.active_slider || c.active_child_scroll)) return;
    const bool hovered=input.in_rect(bounds);
    close_hovered=hovered && input.in_rect(Close());
    const bool owns=captured || hovered;
    if(hovered && (input.mouse_clicked || input.rmb_clicked)) {
        captured=true;dragging=input.mouse_clicked && !close_hovered && input.mouse_y<bounds.y+Header;
        grab_x=input.mouse_x-bounds.x;grab_y=input.mouse_y-bounds.y;
        if(input.mouse_clicked && close_hovered) {feature_ui::settings.radar.value=false;dragging=false;}
    }
    if(captured && dragging && input.mouse_down) {
        const auto screen=ImGui::GetIO().DisplaySize;
        auto& s=feature_ui::settings;
        const float rx=screen.x-bounds.w-16,ry=screen.y-bounds.h-16;
        s.radar_x=rx>0?std::clamp((input.mouse_x-grab_x-8)/rx,0.f,1.f):0.f;
        s.radar_y=ry>0?std::clamp((input.mouse_y-grab_y-8)/ry,0.f,1.f):0.f;
    }
    if(captured && !input.mouse_down && !input.rmb_down) captured=dragging=false;
    if(owns) {
        saved_input=input;
        input.mouse_x=input.mouse_y=input.prev_mouse_x=input.prev_mouse_y=-100000.f;
        input.mouse_clicked=input.mouse_released=input.mouse_double_clicked=input.mouse_down=false;
        input.rmb_clicked=input.rmb_released=input.rmb_down=false;input.scroll_delta=0;
    }
    Layout();
}
void EndMenuInput() {
    if(!saved_input) return;
    xui::ctx().input=std::move(*saved_input);saved_input.reset();xui::ctx().input.clear_frame();
}
void Draw(bool open) {
    const auto& s=feature_ui::settings;
    if(!s.radar.value || !Layout()) return;
    const auto frame=features::GetRadar();
    if(!open && !frame.valid) return;
    const auto& style=xui::ctx().style;auto* dl=ImGui::GetForegroundDrawList();
    const auto b=bounds;const ImVec2 center{b.x+b.w*.5f,b.y+Header+b.w*.5f};
    const float radius=b.w*.5f-16;
    const auto accent=Color(style.accent),muted=Color(style.text_dim);
    dl->PushClipRect({b.x,b.y},{b.right(),b.bottom()},true);
    dl->AddRectFilled({b.x,b.y},{b.right(),b.bottom()},Color(style.window_bg),8);
    dl->AddRect({b.x,b.y},{b.right(),b.bottom()},Color(style.accent.alpha(90)),8);
    dl->AddText({b.x+12,b.y+9},Color(style.text),"Radar");
    char range[32]{};std::snprintf(range,sizeof(range),"%.0f m",s.radar_range.value);
    dl->AddText({b.right()-(open?44.f:12.f)-ImGui::CalcTextSize(range).x,b.y+9},muted,range);
    if(open) {
        const auto r=Close();const auto color=close_hovered?accent:muted;
        dl->AddLine({r.x+7,r.y+7},{r.right()-7,r.bottom()-7},color,1.5f);
        dl->AddLine({r.right()-7,r.y+7},{r.x+7,r.bottom()-7},color,1.5f);
    }
    const auto grid=Color(style.text_dim.alpha(45));
    dl->AddCircle(center,radius,grid,64);dl->AddCircle(center,radius*.5f,grid,48);
    dl->AddLine({center.x-radius,center.y},{center.x+radius,center.y},grid);
    dl->AddLine({center.x,center.y-radius},{center.x,center.y+radius},grid);
    features::Config colors{};
#define F_BOOL(name,value,category,label)
#define F_INT(name,value,category,label)
#define F_FLOAT(name,value,category,label)
#define F_COLOR(name,r,g,b,a,category,label) colors.name=s.name.value.val;
#include "../features_fields.inc"
#undef F_BOOL
#undef F_INT
#undef F_FLOAT
#undef F_COLOR
    const float yaw=s.radar_rotate.value?frame.yaw:90.f;
    for(size_t i=0;i<std::min(frame.count,frame.points.size());++i) {
        const auto& p=frame.points[i];
        const auto projected=radar::Project(p.position.x-frame.origin.x,p.position.y-frame.origin.y,yaw,
                                            std::clamp(s.radar_range.value,5.f,200.f)/.0254f);
        if(!projected.valid) continue;
        const ImVec2 pos{center.x+projected.x*(radius-5),center.y+projected.y*(radius-5)};
        const auto tint=esp::style::PlayerColor(colors,p.kind,p.ghost?esp::style::Ghost:0);
        const float dot=p.kind==source::Kind::Tank?5.f:p.kind==source::Kind::Common?2.5f:3.5f;
        dl->AddCircleFilled(pos,dot+1,IM_COL32(0,0,0,210),12);dl->AddCircleFilled(pos,dot,tint,12);
        const float height=p.position.z-frame.origin.z;
        if(std::abs(height)>80.f) {
            const float direction=height>0?-1.f:1.f;
            dl->AddLine({pos.x-3,pos.y+direction*8},{pos.x,pos.y+direction*11},tint,1.5f);
            dl->AddLine({pos.x,pos.y+direction*11},{pos.x+3,pos.y+direction*8},tint,1.5f);
        }
    }
    const float heading=(s.radar_rotate.value?90.f:frame.yaw)*.0174532925f;
    const ImVec2 f{std::cos(heading),-std::sin(heading)},r{-f.y,f.x};
    dl->AddTriangleFilled({center.x+f.x*7,center.y+f.y*7},
        {center.x-f.x*5+r.x*4,center.y-f.y*5+r.y*4},{center.x-f.x*5-r.x*4,center.y-f.y*5-r.y*4},accent);
    if(!frame.valid) dl->AddText({b.x+12,b.bottom()-21},muted,"Waiting for a match");
    hud_widgets::Grip(dl,b,open);
    dl->PopClipRect();
}
}
