#include "chat_window.hpp"
#include "features_ui.hpp"
#include "hud_widgets.hpp"
#include "hunter_preview.hpp"
#include "spectators_window.hpp"
#include "radar_window.hpp"
#include "../chat_spy.h"
#include <imgui.h>
#include <optional>
#include <cstdio>

namespace chat_window {
namespace {
constexpr float Header=34,Padding=10;
xui::rect bounds{};
chat_spy::Snapshot snapshot{};
std::array<float,chat_spy::logic::HistoryLimit> row_heights{};
float scroll{},content_height{},grab_x{},grab_y{};
bool captured{},dragging{},clear_hovered{},close_hovered{};
std::optional<xui::input_state> saved_input;
std::uint64_t last_serial{};unsigned generation{};
int layout_frame{-1};
ImU32 Color(xdraw::color c) {return hud_widgets::Color(c);}
bool Layout() {
    const auto screen=ImGui::GetIO().DisplaySize;
    if(!std::isfinite(screen.x) || !std::isfinite(screen.y) || screen.x<280 || screen.y<220) return false;
    auto& s=feature_ui::settings;
    if(!std::isfinite(s.chat_width.value))s.chat_width=460.f;
    if(!std::isfinite(s.chat_height.value))s.chat_height=300.f;
    bounds.w=std::min(std::clamp(s.chat_width.value,280.f,800.f),screen.x-16);bounds.h=std::min(std::clamp(s.chat_height.value,180.f,650.f),screen.y-16);
    if(!std::isfinite(s.chat_x.value)) s.chat_x=0.f;
    if(!std::isfinite(s.chat_y.value)) s.chat_y=.7f;
    s.chat_x=std::clamp(s.chat_x.value,0.f,1.f);s.chat_y=std::clamp(s.chat_y.value,0.f,1.f);
    bounds.x=8+s.chat_x.value*std::max(0.f,screen.x-bounds.w-16);
    bounds.y=8+s.chat_y.value*std::max(0.f,screen.y-bounds.h-16);
    if(layout_frame!=ImGui::GetFrameCount()) {
        layout_frame=ImGui::GetFrameCount();snapshot=chat_spy::ReadSnapshot();
        snapshot.count=std::min(snapshot.count,snapshot.entries.size());content_height=0;
        for(size_t i=0;i<snapshot.count;++i) {
            row_heights[i]=ImGui::GetFontSize()+4+
                ImGui::CalcTextSize(snapshot.entries[i].text,nullptr,false,bounds.w-2*Padding-8).y+10;
            content_height+=row_heights[i];
        }
        const auto latest=snapshot.count?snapshot.entries[snapshot.count-1].serial:0;
        if(generation!=snapshot.generation) scroll=0;
        if(s.chat_auto_scroll.value && (latest!=last_serial || generation!=snapshot.generation))
            scroll=std::max(0.f,content_height-(bounds.h-Header-2*Padding));
        last_serial=latest;generation=snapshot.generation;
    }
    scroll=std::clamp(scroll,0.f,std::max(0.f,content_height-(bounds.h-Header-2*Padding)));
    return true;
}
xui::rect Close() {return {bounds.right()-30,bounds.y+6,22,22};}
xui::rect Clear() {return {bounds.right()-83,bounds.y+6,48,22};}
}
void CancelInput() {captured=dragging=clear_hovered=close_hovered=false;saved_input.reset();}
void BeginMenuInput(bool open) {
    clear_hovered=close_hovered=false;
    const auto& s=feature_ui::settings;
    if(!open || !s.chat_spy.value || !s.chat_panel.value || !Layout()) {CancelInput();return;}
    auto& c=xui::ctx();auto& input=c.input;
    auto& settings=feature_ui::settings;
    if(hud_widgets::Resize(&bounds,bounds,settings.chat_width.value,&settings.chat_height.value,settings.chat_x.value,settings.chat_y.value,280,800,180,650)) {
        saved_input=input;hud_widgets::Mask(input);Layout();return;
    }
    if(!captured && (hunter_preview::OwnsPointer() || spectators_window::OwnsPointer() || radar_window::OwnsPointer() || c.overlay_blocking() ||
       c.active_window || c.active_resize || c.active_slider || c.active_child_scroll || c.active_text_input || c.active_keybind)) return;
    const bool hovered=input.in_rect(bounds),owns=captured || hovered;
    clear_hovered=hovered && input.in_rect(Clear());close_hovered=hovered && input.in_rect(Close());
    if(hovered && (input.mouse_clicked || input.rmb_clicked)) {
        captured=true;dragging=input.mouse_clicked && !clear_hovered && !close_hovered && input.mouse_y<bounds.y+Header;
        grab_x=input.mouse_x-bounds.x;grab_y=input.mouse_y-bounds.y;
        if(input.mouse_clicked && close_hovered) feature_ui::settings.chat_panel.value=false;
        if(input.mouse_clicked && clear_hovered) {chat_spy::RequestClear();scroll=0;}
    }
    if(captured && dragging && input.mouse_down) {
        const auto screen=ImGui::GetIO().DisplaySize;
        const float rx=screen.x-bounds.w-16,ry=screen.y-bounds.h-16;
        settings.chat_x=rx>0?std::clamp((input.mouse_x-grab_x-8)/rx,0.f,1.f):0.f;
        settings.chat_y=ry>0?std::clamp((input.mouse_y-grab_y-8)/ry,0.f,1.f):0.f;
    }
    if(hovered && input.scroll_delta) scroll-=input.scroll_delta*48.f;
    if(captured && !input.mouse_down && !input.rmb_down) captured=dragging=false;
    if(owns) {
        saved_input=input;input.mouse_x=input.mouse_y=input.prev_mouse_x=input.prev_mouse_y=-100000.f;
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
    if(!s.chat_spy.value || !s.chat_panel.value || !Layout() || (!open && !snapshot.count && s.chat_hide_empty.value)) return;
    const auto& b=bounds;const auto& style=xui::ctx().style;
    const auto accent=Color(style.accent),muted=Color(style.text_dim);auto* dl=ImGui::GetForegroundDrawList();
    dl->PushClipRect({b.x,b.y},{b.right(),b.bottom()},true);
    dl->AddRectFilled({b.x,b.y},{b.right(),b.bottom()},Color(style.window_bg),8);
    dl->AddRect({b.x,b.y},{b.right(),b.bottom()},Color(style.accent.alpha(90)),8);
    dl->AddRectFilled({b.x+1,b.y+1},{b.right()-1,b.y+Header},Color(style.window_bg.alpha(28)),8,ImDrawFlags_RoundCornersTop);
    char title[64]{};std::snprintf(title,sizeof(title),"Enemy chat (%zu)",snapshot.count);
    dl->AddText({b.x+12,b.y+9},Color(style.text),title);
    if(open) {
        const auto clear=Clear(),close=Close();
        dl->AddText({clear.x+5,clear.y+3},clear_hovered?accent:muted,"Clear");
        const auto color=close_hovered?accent:muted;
        dl->AddLine({close.x+7,close.y+7},{close.right()-7,close.bottom()-7},color,1.5f);
        dl->AddLine({close.right()-7,close.y+7},{close.x+7,close.bottom()-7},color,1.5f);
    }
    const float top=b.y+Header+Padding,bottom=b.bottom()-Padding,body=bottom-top;
    dl->PushClipRect({b.x+Padding,top},{b.right()-Padding,bottom},true);
    if(!snapshot.count) {
        dl->AddText({b.x+Padding,top},muted,"Waiting for received enemy chat...");
        dl->AddText(nullptr,0,{b.x+Padding,top+26},muted,snapshot.status,nullptr,b.w-2*Padding-8);
    }
    float y=top-scroll;
    for(size_t i=0;i<snapshot.count;++i) {
        const auto& e=snapshot.entries[i];const float height=row_heights[i];
        if(y+height>top && y<bottom) {
            char name[144]{};std::snprintf(name,sizeof(name),"%s%s",
                s.chat_team_prefix.value?(e.team==3?"[Infected] ":"[Survivor] "):"",e.name);
            const auto tint=e.team==3?IM_COL32(242,139,145,255):IM_COL32(132,195,255,255);
            dl->PushClipRect({b.x+Padding,top},{b.right()-Padding-8,bottom},true);
            dl->AddText({b.x+Padding,y},tint,name);dl->PopClipRect();
            dl->AddText(nullptr,0,{b.x+Padding,y+ImGui::GetFontSize()+4},Color(style.text),e.text,nullptr,b.w-2*Padding-8);
        }
        y+=height;
    }
    dl->PopClipRect();
    if(content_height>body) {
        const float h=std::min(body,std::max(16.f,body*body/content_height));
        const float sy=top+scroll/(content_height-body)*(body-h);
        dl->AddRectFilled({b.right()-6,top},{b.right()-3,bottom},Color(style.slider_track),2);
        dl->AddRectFilled({b.right()-6,sy},{b.right()-3,sy+h},accent,2);
    }
    hud_widgets::Grip(dl,b,open);
    dl->PopClipRect();
}
}
