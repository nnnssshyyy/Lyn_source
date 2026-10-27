#include "spectators_window.hpp"
#include "features_ui.hpp"
#include "hunter_preview.hpp"
#include "hud_widgets.hpp"
#include "../spectators.h"
#include <imgui.h>
#include <cmath>
#include <cstring>
#include <optional>
#include <cstdio>

namespace spectators_window {
namespace {
// Normalized position survives changes of resolution; these fields belong to
// the same profile as the enabled checkbox, without becoming game settings.
config::val<float> saved_x{1.f,"l4d2 spectators window","position x"};
config::val<float> saved_y{.08f,"l4d2 spectators window","position y"};
constexpr float Header=34,Padding=6,Row=24;
spectators::Snapshot snapshot{};
struct AnimatedRow { float y{},opacity{}; };
std::array<AnimatedRow,spectators::MaxPlayers> rows{};
int animation_frame{-1};
float animated_body{},target_body{};
xui::rect bounds{};
float scroll{},grab_x{},grab_y{};
bool captured{},dragging{},close_hovered{};
std::optional<xui::input_state> saved_input;
ImU32 ThemeColor(xdraw::color color) noexcept { return hud_widgets::Color(color); }

float Ease(float value,float target,float step) noexcept {
    const float next=value+(target-value)*step;
    return std::abs(next-target)<.01f?target:next;
}
void ResetAnimation() {
    snapshot={};rows={};animated_body=target_body=scroll=0;animation_frame=-1;
}
void UpdateAnimation(float max_body) {
    // Layout also runs for input. Advance only once so menu visibility and FPS
    // do not change the animation speed or mix different rosters in one frame.
    const int frame=ImGui::GetFrameCount();
    if(animation_frame==frame) return;
    animation_frame=frame;
    auto next=spectators::ReadSnapshot();
    next.count=std::min(next.count,next.entries.size());
    float dt=ImGui::GetIO().DeltaTime;
    if(!std::isfinite(dt) || dt<0) dt=0;
    const float step=1.f-std::exp(-16.f*std::min(dt,.1f));
    const auto previous_rows=rows;
    std::array<bool,spectators::MaxPlayers> used{};
    for(size_t i=0;i<next.count;++i) {
        const float y=static_cast<float>(i)*Row;
        AnimatedRow row{y-8.f,0};
        // Player identity survives duplicate names, renames and roster sorting.
        for(size_t j=0;j<snapshot.count;++j) {
            if(!used[j] && next.entries[i].userid==snapshot.entries[j].userid &&
               next.entries[i].index==snapshot.entries[j].index) {
                row=previous_rows[j];used[j]=true;break;
            }
        }
        rows[i]={Ease(row.y,y,step),Ease(row.opacity,1.f,step)};
    }
    snapshot=next;
    target_body=snapshot.count?Padding*2+Row*std::min(static_cast<float>(snapshot.count),float(std::clamp(feature_ui::settings.spectator_rows.value,1,16))):0.f;
    target_body=std::min(target_body,max_body);
    animated_body=Ease(animated_body,target_body,step);
}
bool UpdateLayout() {
    const auto screen=ImGui::GetIO().DisplaySize;
    if(!std::isfinite(screen.x) || !std::isfinite(screen.y) || screen.x<180 || screen.y<160) return false;
    UpdateAnimation(std::max(0.f,screen.y-16-Header));
    const float width=feature_ui::settings.spectator_width.value;
    bounds.w=std::min(std::isfinite(width)?std::clamp(width,200.f,520.f):300.f,screen.x-16);
    bounds.h=std::min(Header+animated_body,screen.y-16);
    if(!std::isfinite(saved_x.value)) saved_x=1.f;
    if(!std::isfinite(saved_y.value)) saved_y=.08f;
    saved_x=std::clamp(saved_x.value,0.f,1.f);saved_y=std::clamp(saved_y.value,0.f,1.f);
    bounds.x=8+saved_x.value*std::max(0.f,screen.x-bounds.w-16);
    // Anchor the title independently of roster height. Only the screen edge
    // can push it upward when the window needs more room.
    bounds.y=std::min(8+saved_y.value*std::max(0.f,screen.y-Header-16),screen.y-bounds.h-8);
    const float content=static_cast<float>(snapshot.count)*Row;
    const float body_h=std::max(0.f,bounds.h-Header-Padding*2);
    scroll=std::clamp(scroll,0.f,std::max(0.f,content-body_h));
    return true;
}
xui::rect CloseRect() { return {bounds.right()-30,bounds.y+7,22,22}; }
void MaskPointer() {
    auto& i=xui::ctx().input;
    i.mouse_x=i.mouse_y=i.prev_mouse_x=i.prev_mouse_y=-100000.f;
    i.mouse_clicked=i.mouse_released=i.mouse_double_clicked=i.mouse_down=false;
    i.rmb_clicked=i.rmb_released=i.rmb_down=false;i.scroll_delta=0;
}
}

void ResetPosition() { saved_x=1.f;saved_y=.08f;scroll=0;captured=dragging=false; }
bool OwnsPointer() {return captured;}
void CancelInput() { captured=dragging=close_hovered=false;saved_input.reset(); }
void BeginMenuInput(bool menu_open) {
    close_hovered=false;
    if(!menu_open || !feature_ui::settings.spectator_list.value || !UpdateLayout()) { CancelInput();return; }
    auto& c=xui::ctx();auto& input=c.input;
    if(hud_widgets::Resize(&bounds,bounds,feature_ui::settings.spectator_width.value,nullptr,saved_x.value,saved_y.value,200,520)) {
        // Spectators anchor vertically to the title, not the roster height.
        const auto screen=ImGui::GetIO().DisplaySize;
        saved_y=screen.y>Header+16?std::clamp((bounds.y-8)/(screen.y-Header-16),0.f,1.f):0.f;
        saved_input=input;MaskPointer();UpdateLayout();return;
    }
    // Let a drag that started in the menu/Hunter complete before taking input.
    if(!captured && (c.overlay_blocking() || c.active_window || c.active_resize ||
       c.active_slider || c.active_child_scroll || hunter_preview::OwnsPointer())) return;
    const bool hovered=input.in_rect(bounds);
    close_hovered=hovered && input.in_rect(CloseRect());
    bool owns=captured || hovered;
    if(hovered && (input.mouse_clicked || input.rmb_clicked)) {
        captured=true;
        dragging=input.mouse_clicked && !close_hovered && input.mouse_y<bounds.y+Header;
        grab_x=input.mouse_x-bounds.x;grab_y=input.mouse_y-bounds.y;
        if(input.mouse_clicked && close_hovered) {
            feature_ui::settings.spectator_list.value=false;
            dragging=false;
        }
    }
    if(captured && dragging && input.mouse_down) {
        const auto screen=ImGui::GetIO().DisplaySize;
        const float range_x=screen.x-bounds.w-16,range_y=screen.y-Header-16;
        saved_x=range_x>0?std::clamp((input.mouse_x-grab_x-8)/range_x,0.f,1.f):0.f;
        saved_y=range_y>0?std::clamp((input.mouse_y-grab_y-8)/range_y,0.f,1.f):0.f;
    }
    if(hovered && input.scroll_delta!=0) scroll-=input.scroll_delta*Row*3;
    if(captured && !input.mouse_down && !input.rmb_down) captured=dragging=false;
    if(owns) { saved_input=input;MaskPointer(); }
    UpdateLayout();
}
void EndMenuInput() {
    if(!saved_input) return;
    xui::ctx().input=std::move(*saved_input);saved_input.reset();
    // The menu has consumed this frame. Do not replay a click after unmasking,
    // and restore the real previous position to avoid a jump on the next drag.
    xui::ctx().input.clear_frame();
}
void Draw(bool menu_open) {
    if(!feature_ui::settings.spectator_list.value) { ResetAnimation();return; }
    if(!UpdateLayout()) return;
    if(!menu_open && !snapshot.count && feature_ui::settings.spectator_hide_empty.value) return;
    auto* dl=ImGui::GetForegroundDrawList();
    const auto& b=bounds;
    const float font=ImGui::GetFontSize();
    const auto& style=xui::ctx().style;
    const ImU32 accent=ThemeColor(style.accent),muted=ThemeColor(style.text_dim);
    dl->PushClipRect({0,0},ImGui::GetIO().DisplaySize,true);
    // Use the menu's current tint and alpha; no opaque layer under the roster.
    dl->AddRectFilled({b.x,b.y},{b.right(),b.bottom()},ThemeColor(style.window_bg),8);
    dl->AddRect({b.x,b.y},{b.right(),b.bottom()},ThemeColor(style.accent.alpha(90)),8);
    dl->AddRectFilled({b.x+1,b.y+1},{b.right()-1,b.y+Header},ThemeColor(style.window_bg.alpha(28)),8,
        animated_body>.5f?ImDrawFlags_RoundCornersTop:ImDrawFlags_RoundCornersAll);
    dl->AddText({b.x+12,b.y+9},ThemeColor(style.text),"Spectators");
    char count[16]{};std::snprintf(count,sizeof(count),"%zu",snapshot.count);
    const auto count_size=ImGui::CalcTextSize(count);
    dl->AddText({b.right()-(menu_open?44.f:14.f)-count_size.x,b.y+9},accent,count);
    if(menu_open) {
        const auto r=CloseRect();const auto col=close_hovered?accent:muted;
        if(close_hovered) dl->AddRectFilled({r.x,r.y},{r.right(),r.bottom()},ThemeColor(style.accent.alpha(40)),4);
        dl->AddLine({r.x+7,r.y+7},{r.right()-7,r.bottom()-7},col,1.5f);
        dl->AddLine({r.right()-7,r.y+7},{r.x+7,r.bottom()-7},col,1.5f);
    }
    const float body_top=b.y+Header+Padding,body_bottom=b.bottom()-Padding;
    hud_widgets::Grip(dl,b,menu_open);
    if(!snapshot.count || body_bottom<=body_top) { dl->PopClipRect();return; }
    dl->PushClipRect({b.x+8,body_top},{b.right()-12,body_bottom},true);
    for(size_t i=0;i<snapshot.count;++i) {
        const float y=body_top+rows[i].y-scroll;
        if(y+Row<body_top || y>=body_bottom) continue;
        const auto alpha=static_cast<std::uint8_t>(static_cast<float>(style.text.a)*rows[i].opacity);
        const bool modes=feature_ui::settings.spectator_modes.value;
        const float label_x=b.right()-45.f;
        if(modes) dl->AddText({label_x,y+(Row-font)*.5f},ThemeColor(style.text_dim.alpha(alpha)),
                             snapshot.entries[i].mode==4?"1P":"3P");
        dl->PushClipRect({b.x+12,body_top},{modes?label_x-8.f:b.right()-12.f,body_bottom},true);
        dl->AddText({b.x+12,y+(Row-font)*.5f},ThemeColor(style.text.alpha(alpha)),snapshot.entries[i].name);
        dl->PopClipRect();
    }
    dl->PopClipRect();
    const float content=static_cast<float>(snapshot.count)*Row,body_h=body_bottom-body_top;
    // Do not flash a scrollbar just because the opening animation clips rows.
    if(content>std::max(0.f,target_body-Padding*2)+.5f && content>body_h) {
        const float thumb_h=std::min(body_h,std::max(16.f,body_h*body_h/content));
        const float thumb_y=body_top+scroll/(content-body_h)*(body_h-thumb_h);
        dl->AddRectFilled({b.right()-6,body_top},{b.right()-3,body_bottom},ThemeColor(style.slider_track),2);
        dl->AddRectFilled({b.right()-6,thumb_y},{b.right()-3,thumb_y+thumb_h},accent,2);
    }
    dl->PopClipRect();
}
}
