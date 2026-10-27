#include "visuals_window.hpp"
#include "hud_widgets.hpp"
#include "hunter_preview.hpp"
#include "../visuals.h"
#include "../esp_style.h"
#include "../esp_font.h"
#include "../overlay_paint.h"
#include "../license.h"
#include <optional>
#include <cstdio>

namespace visuals_window {
namespace {
xui::rect bounds{};
visuals::Snapshot snapshot{};
int read_frame=-1;
bool dragging{},captured{};
float grab_x{},grab_y{};
std::optional<xui::input_state> saved;
void Read() {if(read_frame!=ImGui::GetFrameCount()){snapshot=visuals::ReadSnapshot();read_frame=ImGui::GetFrameCount();}}
bool Fresh() {return snapshot.valid&&GetTickCount64()>=snapshot.time&&GetTickCount64()-snapshot.time<250;}
bool Layout(bool open) {
    Read();auto& s=feature_ui::settings;const auto screen=ImGui::GetIO().DisplaySize;
    if(screen.x<320||screen.y<240||(!s.boss_hud.value&&!(open&&s.hud_editor.value)))return false;
    if(!(open&&s.hud_editor.value)&&(!Fresh()||!snapshot.boss_count))return false;
    auto sane=[](float v,float a,float b,float d){return std::isfinite(v)?std::clamp(v,a,b):d;};
    s.boss_scale=sane(s.boss_scale.value,.7f,1.5f,1);s.boss_width=sane(s.boss_width.value,230,560,310);
    s.boss_x=sane(s.boss_x.value,0,1,.5f);s.boss_y=sane(s.boss_y.value,0,1,.02f);
    bounds.w=std::min(s.boss_width.value,screen.x-16);
    bounds.h=34+s.boss_scale.value*64*std::max(1u,std::min(Fresh()?snapshot.boss_count:0u,4u));
    bounds.h=std::min(bounds.h,screen.y-16);
    bounds.x=8+s.boss_x.value*std::max(0.f,screen.x-bounds.w-16);bounds.y=8+s.boss_y.value*std::max(0.f,screen.y-bounds.h-16);return true;
}
float LabelSize(float scale=1.f) {return std::max(10.f,std::round(ImGui::GetFontSize()*scale));}
ImVec2 LabelMeasure(const char* text,float scale=1.f) {
    auto* font=esp::font::At(LabelSize(scale));
    return font->CalcTextSizeA(font->FontSize,FLT_MAX,0.f,text);
}
void Label(ImDrawList* dl,float x,float y,ImU32 color,const char* text,float scale=1.f) {
    auto* font=esp::font::At(LabelSize(scale));
    overlay_paint::Label(dl,{x,y},color,text,font,font->FontSize);
}
ImU32 ThreatColor(source::Kind kind) {
    const auto& s=feature_ui::settings;
    switch(kind) {
        case source::Kind::Tank:return s.esp_color_tank.value.val;
        case source::Kind::Witch:return s.esp_color_witch.value.val;
        case source::Kind::Smoker:return s.esp_color_smoker.value.val;
        case source::Kind::Boomer:return s.esp_color_boomer.value.val;
        case source::Kind::Hunter:return s.esp_color_hunter.value.val;
        case source::Kind::Spitter:return s.esp_color_spitter.value.val;
        case source::Kind::Jockey:return s.esp_color_jockey.value.val;
        case source::Kind::Charger:return s.esp_color_charger.value.val;
        default:return s.esp_color_common.value.val;
    }
}
void Effects() {
    if(!Fresh())return;
    const auto& s=feature_ui::settings;const auto screen=ImGui::GetIO().DisplaySize;auto* dl=ImGui::GetBackgroundDrawList();
    const auto now=GetTickCount64();
    dl->PushClipRect({0,0},screen,true);
    if(s.offscreen.value) {
        visuals::logic::Labels labels;
        // One nearest threat per direction sector prevents stacked arrows.
        std::array<int,24> selected{};selected.fill(-1);
        for(unsigned i=0;i<snapshot.threat_count;++i) {
            const auto& t=snapshot.threats[i];float x{},y{};auto point=t.position;point.z+=30;
            if(game::Project(snapshot.matrix,point,screen.x,screen.y,x,y)&&x>=20&&x<=screen.x-20&&y>=20&&y<=screen.y-20)continue;
            const auto a=visuals::logic::Edge(t.position.x-snapshot.origin.x,t.position.y-snapshot.origin.y,snapshot.yaw,screen.x,screen.y,s.offscreen_radius.value);
            if(!a.valid)continue;
            const int slot=std::clamp(int((std::atan2(a.dy,a.dx)+3.14159265f)/6.2831853f*24),0,23);
            if(selected[slot]<0||t.distance<snapshot.threats[selected[slot]].distance)selected[slot]=int(i);
        }
        for(int i:selected)if(i>=0) {
            const auto& t=snapshot.threats[i];const auto a=visuals::logic::Edge(t.position.x-snapshot.origin.x,t.position.y-snapshot.origin.y,snapshot.yaw,screen.x,screen.y,s.offscreen_radius.value);
            const auto tint=ThreatColor(t.kind);if(!(tint>>24))continue;
            const ImVec2 tip{a.x+a.dx*9,a.y+a.dy*9},b{a.x-a.dx*6-a.dy*5,a.y-a.dy*6+a.dx*5},c{a.x-a.dx*6+a.dy*5,a.y-a.dy*6-a.dx*5};
            dl->AddTriangle(tip,b,c,IM_COL32(1,4,9,225),4.f);dl->AddTriangleFilled(tip,b,c,tint);
            char label[64]{};std::snprintf(label,sizeof(label),"%s %.0fm",source::Label(t.kind),t.distance);
            const auto size=LabelMeasure(label,.85f);
            visuals::logic::Rect ink{a.x-a.dx*26-size.x*.5f-3,a.y-a.dy*26-size.y*.5f-1,size.x+6,size.y+2};
            if(labels.Place(ink,screen.x,screen.y))Label(dl,ink.x+3,ink.y+1,tint,label,.85f);
        }
    }
    if(s.trajectory.value&&snapshot.path.valid&&snapshot.path.count>1) {
        const auto& p=snapshot.path;
        for(unsigned i=1;i<p.count;++i) {
            float x{},y{},u{},v{};const auto& a=p.points[i-1];const auto& b=p.points[i];
            if(game::Project(snapshot.matrix,{a.x,a.y,a.z},screen.x,screen.y,x,y)&&game::Project(snapshot.matrix,{b.x,b.y,b.z},screen.x,screen.y,u,v)) {
                const float fade=.4f+.6f*static_cast<float>(i)/p.count;
                dl->AddLine({x,y},{u,v},IM_COL32(1,4,9,200),4);dl->AddLine({x,y},{u,v},overlay_paint::Fade(IM_COL32(164,218,250,255),fade),2);
            }
        }
        float x{},y{};const auto& end=p.points[p.count-1];
        if(game::Project(snapshot.matrix,{end.x,end.y,end.z},screen.x,screen.y,x,y)) {
            dl->AddCircleFilled({x,y},p.landed?10.f:7.f,IM_COL32(8,15,24,200),24);
            dl->AddCircle({x,y},p.landed?7.f:4.f,IM_COL32(164,218,250,245),24,1.5f);
            dl->AddCircleFilled({x,y},2.f,IM_COL32(234,246,255,245),12);
            char label[64]{};std::snprintf(label,sizeof(label),"~%.1fs | %s",p.seconds,p.landed?"impacto estimado":"trayectoria estimada");
            Label(dl,x+11,y-7,IM_COL32(220,239,226,240),label,.9f);
        }
    }
    if(s.death_particles.value)for(const auto& burst:snapshot.deaths.items)if(burst.time&&now>=burst.time) {
        const float age=float(now-burst.time)*.001f;
        const float lifetime=std::isfinite(s.death_particle_lifetime.value)?std::clamp(s.death_particle_lifetime.value,.4f,2.f):1.1f;
        if(age>=lifetime)continue;
        const auto& accent=xui::ctx().style.accent;
        const unsigned count=static_cast<unsigned>(std::clamp(s.death_particle_count.value,8,32));
        for(unsigned i=0;i<count;++i) {
            const auto spark=visuals::logic::DeathParticle(burst,i,age,lifetime);
            float x{},y{},u{},v{};
            if(spark.fade<=0||!game::Project(snapshot.matrix,{spark.point.x,spark.point.y,spark.point.z},screen.x,screen.y,x,y)||
               x<0||x>screen.x||y<0||y>screen.y)continue;
            const auto tint=IM_COL32(accent.r,accent.g,accent.b,int(230*spark.fade));
            if(game::Project(snapshot.matrix,{spark.tail.x,spark.tail.y,spark.tail.z},screen.x,screen.y,u,v)&&
               std::hypot(x-u,y-v)<80.f)dl->AddLine({u,v},{x,y},tint,1.f);
            dl->AddCircleFilled({x,y},spark.size,tint,6);
        }
    }
    if(s.hitmarker.value&&snapshot.damage.hit_time&&now>=snapshot.damage.hit_time&&now-snapshot.damage.hit_time<220) {
        const float progress=float(now-snapshot.damage.hit_time)/220.f;
        const float configured=std::isfinite(s.hitmarker_size.value)?std::clamp(s.hitmarker_size.value,4.f,16.f):8.f;
        const float alpha=(1-progress)*(1-progress),n=configured+4.f*(1-progress)*(1-progress);
        const float gap=std::min(n-1.f,3.f+progress);
        const auto col=snapshot.damage.head?IM_COL32(255,198,125,int(alpha*255)):IM_COL32(240,246,255,int(alpha*255));
        for(int i=0;i<4;++i){const float dx=i&1?1.f:-1.f,dy=i&2?1.f:-1.f;
            const ImVec2 a{screen.x*.5f+dx*gap,screen.y*.5f+dy*gap},b{screen.x*.5f+dx*n,screen.y*.5f+dy*n};
            dl->AddLine(a,b,IM_COL32(1,4,9,int(alpha*210)),3.5f);dl->AddLine(a,b,col,1.5f);}
        if(snapshot.damage.head)dl->AddCircleFilled({screen.x*.5f,screen.y*.5f},1.5f,col,12);
    }
    if(s.damage_numbers.value) {
        visuals::logic::Labels labels;
        std::array<unsigned,32> order{};for(unsigned i=0;i<order.size();++i)order[i]=i;
        std::sort(order.begin(),order.end(),[](unsigned a,unsigned b){return snapshot.damage.items[a].last>snapshot.damage.items[b].last;});
        for(unsigned i:order) {
            const auto& d=snapshot.damage.items[i];if(!d.last||!d.position||now<d.last)continue;
            const float age=float(now-d.last)*.001f;
            const float lifetime=std::isfinite(s.damage_lifetime.value)?std::clamp(s.damage_lifetime.value,.4f,2.f):1.f;
            if(age>=lifetime)continue;
            float x{},y{};if(!game::Project(snapshot.matrix,{d.point.x,d.point.y,d.point.z},screen.x,screen.y,x,y))continue;
            const float progress=age/lifetime,fade=std::min(1.f,(1.f-progress)*4.f);
            char value[24]{};std::snprintf(value,sizeof(value),d.head?"-%d HS":"-%d",d.amount);
            const auto size=LabelMeasure(value,1.05f);
            visuals::logic::Rect badge{x-size.x*.5f-7.f,y-18.f-std::sqrt(progress)*30.f,size.x+14.f,size.y+6.f};
            if(!labels.Place(badge,screen.x,screen.y))continue;
            const auto tint=d.head?IM_COL32(255,198,125,int(fade*255)):IM_COL32(238,246,255,int(fade*255));
            overlay_paint::Plate(dl,{badge.x,badge.y,badge.w,badge.h},overlay_paint::Fade(overlay_paint::Surface,fade),4.f);
            dl->AddRectFilled({badge.x,badge.y+3},{badge.x+2,badge.y+badge.h-3},tint,1.f);
            auto* font=esp::font::At(LabelSize(1.05f));
            overlay_paint::Label(dl,{badge.x+7,badge.y+3},tint,value,font,font->FontSize,false);
        }
    }
    dl->PopClipRect();
}
}
void BeginMenuInput(bool open) {
    hud_widgets::ResetInput(open);
    if(!open||!Layout(open)) {captured=dragging=false;return;}
    auto& s=feature_ui::settings;auto& c=xui::ctx();auto& i=c.input;
    if(!captured&&(hunter_preview::OwnsPointer()||c.overlay_blocking()||c.active_window||c.active_resize||c.active_slider||c.active_child_scroll))return;
    if(hud_widgets::Resize(&bounds,bounds,s.boss_width.value,nullptr,s.boss_x.value,s.boss_y.value,230,560)) {
        saved=i;hud_widgets::Mask(i);Layout(open);return;
    }
    const bool hovered=i.in_rect(bounds),owns=hovered||captured;
    if(hovered&&i.mouse_clicked){captured=true;dragging=i.mouse_y<bounds.y+34;grab_x=i.mouse_x-bounds.x;grab_y=i.mouse_y-bounds.y;}
    if(dragging&&i.mouse_down){const auto screen=ImGui::GetIO().DisplaySize;const float rx=screen.x-bounds.w-16,ry=screen.y-bounds.h-16;s.boss_x=rx>0?std::clamp((i.mouse_x-grab_x-8)/rx,0.f,1.f):0;s.boss_y=ry>0?std::clamp((i.mouse_y-grab_y-8)/ry,0.f,1.f):0;}
    if(!i.mouse_down)captured=dragging=false;
    if(owns){saved=i;hud_widgets::Mask(i);}Layout(open);
}
void EndMenuInput() {if(saved){xui::ctx().input=std::move(*saved);saved.reset();xui::ctx().input.clear_frame();}}
void Draw(bool open) {
    if(!licensing::Active())return;
    Read();Effects();if(!Layout(open))return;
    auto* dl=ImGui::GetForegroundDrawList();const auto& b=bounds;const auto& st=xui::ctx().style;const auto& s=feature_ui::settings;
    dl->AddRectFilled({b.x+2,b.y+4},{b.right()+2,b.bottom()+4},hud_widgets::Color({0,0,0,75}),10);
    dl->PushClipRect({b.x,b.y},{b.right(),b.bottom()},true);
    dl->AddRectFilled({b.x,b.y},{b.right(),b.bottom()},hud_widgets::Color(st.window_bg),8);
    dl->AddRect({b.x+.5f,b.y+.5f},{b.right()-.5f,b.bottom()-.5f},hud_widgets::Color(st.accent.alpha(75)),8);
    dl->AddCircleFilled({b.x+14,b.y+16},3.f,hud_widgets::Color(st.accent),12);
    auto* header_font=esp::font::At(LabelSize());
    dl->AddText(header_font,header_font->FontSize,{b.x+25,b.y+9},hud_widgets::Color(st.text),"Tank / Witch");
    const unsigned actual_count=Fresh()?std::min(snapshot.boss_count,4u):0;
    const bool preview=!actual_count&&open&&s.hud_editor.value;
    const unsigned count=preview?1:actual_count;
    char heading[12]{};if(preview)std::snprintf(heading,sizeof(heading),"DEMO");else std::snprintf(heading,sizeof(heading),"%02u",count);
    const auto heading_size=header_font->CalcTextSizeA(header_font->FontSize,FLT_MAX,0,heading);
    const float heading_x=b.right()-12-heading_size.x;
    dl->AddRectFilled({heading_x-5,b.y+7},{b.right()-7,b.y+26},hud_widgets::Color(st.accent.alpha(25)),4.f);
    dl->AddText(header_font,header_font->FontSize,{heading_x,b.y+9},hud_widgets::Color(st.text_dim),heading);
    dl->AddLine({b.x+12,b.y+33},{b.right()-12,b.y+33},hud_widgets::Color(st.text_dim.alpha(35)));
    static std::array<visuals::Boss,8> previous{};static std::array<float,8> animated{};
    const auto old_animated=animated;
    for(unsigned i=0;i<count;++i) {
        const visuals::Boss demo{0xffff,source::Kind::Tank,3200,4000,0,25,false};
        const auto& boss=preview?demo:snapshot.bosses[i];const float y=b.y+39+i*64*s.boss_scale.value;
        float old=float(boss.health);for(size_t j=0;j<previous.size();++j)if(previous[j].handle==boss.handle)old=old_animated[j];
        const float dt=std::clamp(ImGui::GetIO().DeltaTime,0.f,.1f);animated[i]=old+(boss.health-old)*(1-std::exp(-14.f*dt));
        const auto color=overlay_paint::Fade(ThreatColor(boss.kind),hud_widgets::Opacity());
        char label[128]{};
        auto* title_font=esp::font::At(ImGui::GetFontSize()*s.boss_scale.value);
        dl->AddText(title_font,title_font->FontSize,{std::round(b.x+12),std::round(y)},color,source::Label(boss.kind));
        if(boss.health>0&&boss.maximum>0)std::snprintf(label,sizeof(label),"%d / %d",boss.health,boss.maximum);
        else if(boss.health>0)std::snprintf(label,sizeof(label),"%d HP",boss.health);
        else std::snprintf(label,sizeof(label),"HP --");
        const auto hp_size=title_font->CalcTextSizeA(title_font->FontSize,FLT_MAX,0,label);
        const auto kind_size=title_font->CalcTextSizeA(title_font->FontSize,FLT_MAX,0,source::Label(boss.kind));
        const float hp_scale=std::min(1.f,(b.w-36-kind_size.x)/std::max(1.f,hp_size.x));
        dl->AddText(title_font,title_font->FontSize*hp_scale,{std::round(b.right()-12-hp_size.x*hp_scale),std::round(y)},hud_widgets::Color(st.text),label);
        const float top=y+23*s.boss_scale.value,w=b.w-24;
        dl->AddRectFilled({b.x+12,top},{b.right()-12,top+5},hud_widgets::Color(st.slider_track),2);
        const float f=boss.maximum>0?std::clamp(float(boss.health)/boss.maximum,0.f,1.f):0.f;
        const float trailing=boss.maximum>0?std::clamp(animated[i]/boss.maximum,0.f,1.f):0.f;
        if(trailing>f)dl->AddRectFilled({b.x+12,top},{b.x+12+w*trailing,top+5},overlay_paint::Fade(color,.3f),2);
        if(f>0)dl->AddRectFilled({b.x+12,top},{b.x+12+w*f,top+5},color,2);
        if(boss.kind==source::Kind::Witch)std::snprintf(label,sizeof(label),"%s | ira %d%%",boss.burning?"En llamas":boss.rage>=100?"Enfurecida":"Alerta",boss.rage);
        else std::snprintf(label,sizeof(label),"%s",boss.burning?"En llamas":"Activo");
        auto* detail_font=esp::font::At(ImGui::GetFontSize()*.85f*s.boss_scale.value);
        dl->AddText(detail_font,detail_font->FontSize,{std::round(b.x+12),std::round(top+10)},hud_widgets::Color(st.text_dim),label);
        char distance[24]{};std::snprintf(distance,sizeof(distance),"%.0f m",boss.distance);
        const auto distance_size=detail_font->CalcTextSizeA(detail_font->FontSize,FLT_MAX,0,distance);
        dl->AddText(detail_font,detail_font->FontSize,{std::round(b.right()-12-distance_size.x),std::round(top+10)},hud_widgets::Color(st.text_dim),distance);
    }
    previous=snapshot.bosses;hud_widgets::Grip(dl,b,open);dl->PopClipRect();
}
}
