#pragma once
#include "features_ui.hpp"
#include <imgui.h>

namespace hud_widgets {
inline const void* resizing{};
inline float start_mouse_x{},start_mouse_y{},start_w{},start_h{},anchor_x{},anchor_y{};
inline float Opacity() {const auto a=feature_ui::settings.hud_opacity.value;return std::isfinite(a)?std::clamp(a,.2f,1.f):1.f;}
inline ImU32 Color(xdraw::color c) {return IM_COL32(c.r,c.g,c.b,int(c.a*Opacity()));}
inline void ResetInput(bool open) {if(!open||!feature_ui::settings.hud_editor.value||(!xui::ctx().input.mouse_down&&!xui::ctx().input.mouse_clicked))resizing=nullptr;}
inline bool OwnsPointer() {return resizing!=nullptr;}
inline bool Resize(const void* id,const xui::rect& b,float& width,float* height,float& nx,float& ny,
                   float min_w,float max_w,float min_h=100,float max_h=700,bool square=false) {
    if(!feature_ui::settings.hud_editor.value) {if(resizing==id)resizing=nullptr;return false;}
    auto& c=xui::ctx();auto& i=c.input;
    if(resizing&&resizing!=id)return false;
    if(!resizing&&(c.overlay_blocking()||c.active_window||c.active_resize||c.active_slider||c.active_child_scroll))return false;
    const bool corner=i.in_rect({b.right()-18,b.bottom()-18,18,18});
    if(corner&&i.mouse_clicked) {
        resizing=id;start_mouse_x=i.mouse_x;start_mouse_y=i.mouse_y;start_w=width;start_h=height?*height:b.h;anchor_x=b.x;anchor_y=b.y;
    }
    if(resizing!=id)return false;
    const auto screen=ImGui::GetIO().DisplaySize;
    width=std::clamp(start_w+i.mouse_x-start_mouse_x,min_w,std::max(min_w,std::min(max_w,screen.x-anchor_x-8)));
    if(height)*height=std::clamp(start_h+i.mouse_y-start_mouse_y,min_h,std::max(min_h,std::min(max_h,screen.y-anchor_y-8)));
    const float h=square?width+34:height?*height:b.h;
    const float rx=screen.x-width-16,ry=screen.y-h-16;
    nx=rx>0?std::clamp((anchor_x-8)/rx,0.f,1.f):0;ny=ry>0?std::clamp((anchor_y-8)/ry,0.f,1.f):0;
    if(!i.mouse_down)resizing=nullptr;
    return true;
}
inline void Grip(ImDrawList* dl,const xui::rect& b,bool open) {
    if(!open||!feature_ui::settings.hud_editor.value)return;
    const auto c=Color(xui::ctx().style.accent);
    dl->AddRect({b.x,b.y},{b.right(),b.bottom()},c,8);
    dl->AddLine({b.right()-13,b.bottom()-4},{b.right()-4,b.bottom()-13},c,1.5f);
    dl->AddLine({b.right()-8,b.bottom()-4},{b.right()-4,b.bottom()-8},c,1.5f);
}
inline void Mask(xui::input_state& i) {
    i.mouse_x=i.mouse_y=i.prev_mouse_x=i.prev_mouse_y=-100000.f;
    i.mouse_clicked=i.mouse_released=i.mouse_double_clicked=i.mouse_down=false;
    i.rmb_clicked=i.rmb_released=i.rmb_down=false;i.scroll_delta=0;
}
}
