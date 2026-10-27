#include "hunter_preview.hpp"
#include "ui_preferences.hpp"
#include "hunter_model.hpp"
#include "features_ui.hpp"
#include "rendering.hpp"
#include "xui.hpp"
#include "../log.h"
#include "../esp_layout.h"
#include "../esp_style.h"
#include "../overlay_paint.h"
#include <cmath>

extern "C" IMAGE_DOS_HEADER __ImageBase;

namespace hunter_preview {
namespace {
using xdraw::color;
constexpr color ranks[]{ {105,112,123}, {232,65,57}, {246,159,54}, {75,202,84}, {0,177,218} };
constexpr const char* rank_names[]{"Ignore","First","Second","Third","Fourth"};
constexpr const char* zone_names[]{"Head","Chest","Stomach","Left arm","Right arm","Left leg","Right leg"};

struct Silhouette {
    std::array<Microsoft::WRL::ComPtr<IDirect3DTexture9>,7> masks;
    std::vector<std::uint8_t> hit_groups; // 0 = transparent; 1..7 = anatomical group.
    int width{},height{};
};
Silhouette silhouette;
struct PreviewWindow {
    bool open{true};
    float x{},y{80},w{350},h{690};
    hunter_model::View model;
};
// The ESP view lives in the menu. The hitbox window opens only on request.
std::array<PreviewWindow,2> windows{{{false,920},{false,1280}}};
int front_window=1,input_owner=-1,captured_window=-1,popup_owner=-1;
bool dragging_header{},dragging_model{};
std::optional<xui::input_state> main_input;
enum class LayoutPart { Health,Name,Distance,None };
struct PlacementDrag {
    bool active{};
    LayoutPart part{LayoutPart::None};
    int original{},target{-1};
    float start_x{},start_y{};
    esp::layout::Rect body;
};
PlacementDrag placement_drag;

void MaskInput() {
    auto& i=xui::ctx().input;
    i.mouse_x=i.mouse_y=i.prev_mouse_x=i.prev_mouse_y=-100000.f;
    i.mouse_clicked=i.mouse_double_clicked=i.mouse_down=i.mouse_released=false;
    i.rmb_clicked=i.rmb_down=i.rmb_released=false; i.scroll_delta=0;
}
void ClampWindows() {
    const auto [width,height]=xdraw::viewport_size();
    for(auto& w:windows) {
        w.w=std::min(350.f,std::max(240.f,static_cast<float>(width)-16.f));
        w.h=std::min(690.f,std::max(450.f,static_cast<float>(height)-16.f));
        w.x=std::clamp(w.x,8.f,std::max(8.f,width-w.w-8));
        w.y=std::clamp(w.y,8.f,std::max(8.f,height-w.h-8));
    }
}

// Only the internal seams are defined here. Every outer edge, finger, hood and
// boot comes from the illustration's alpha channel, never from a drawn polygon.
// These regions belong to the menu; live aiming still uses engine hitboxes.
int AnatomicalGroup(float x,float y) {
    const float neck=x-.535f;
    if(y<.247f-4.f*neck*neck && x>.41f && x<.64f) return 0;
    const float right_seam=.71f-.30f*std::clamp(y-.26f,0.f,.09f);
    if(y>.265f && y<.50f && x>right_seam) return 3;
    const float left_seam=.298f+.90f*std::clamp(y-.31f,0.f,.064f);
    if(y>.31f && y<.59f && x<left_seam) return 4;
    const float waist=x-.52f;
    if(y>.521f-.7f*waist*waist)
        return x<.552f-.06f*std::max(y-.6f,0.f)?6:5;
    const float chest=x-.515f;
    return y<.395f-.18f*chest*chest?1:2;
}

bool BuildSilhouette(IDirect3DTexture9* source,int width,int height) {
    silhouette={};
    if(!source || width<=0 || height<=0 || width>8192 || height>8192) return false;
    Silhouette next;
    // The model is displayed at about 200 x 315 px. Area-filter the original
    // alpha once to keep smooth edges without seven full-size GPU textures.
    next.width=(width+3)/4; next.height=(height+3)/4;
    const auto count=static_cast<size_t>(next.width)*next.height;
    std::vector<std::array<std::uint8_t,7>> coverage(count);
    next.hit_groups.resize(count);
    {
        D3DLOCKED_RECT lock{};
        if(FAILED(source->LockRect(0,&lock,nullptr,D3DLOCK_READONLY))) return false;
        struct Unlock { IDirect3DTexture9* texture; ~Unlock(){texture->UnlockRect(0);} } unlock{source};
        for(int y=0;y<next.height;++y) for(int x=0;x<next.width;++x) {
            const int x0=x*width/next.width,x1=(x+1)*width/next.width;
            const int y0=y*height/next.height,y1=(y+1)*height/next.height;
            std::array<unsigned,7> alpha{};
            for(int sy=y0;sy<y1;++sy) {
                const auto row=static_cast<const std::uint8_t*>(lock.pBits)+static_cast<size_t>(sy)*lock.Pitch;
                for(int sx=x0;sx<x1;++sx) {
                    const auto a=row[sx*4+3];
                    if(a) alpha[AnatomicalGroup((sx+.5f)/width,(sy+.5f)/height)]+=a;
                }
            }
            const unsigned samples=static_cast<unsigned>((x1-x0)*(y1-y0));
            const auto index=static_cast<size_t>(y)*next.width+x;
            unsigned strongest=0;
            for(int group=0;group<7;++group) {
                coverage[index][group]=static_cast<std::uint8_t>((alpha[group]+samples/2)/samples);
                if(alpha[group]>strongest) {
                    strongest=alpha[group]; next.hit_groups[index]=static_cast<std::uint8_t>(group+1);
                }
            }
            if(strongest<samples*32) next.hit_groups[index]=0;
        }
    }
    // White RGB lets the renderer apply the selected priority color directly.
    std::vector<std::uint8_t> pixels(count*4,255);
    for(int group=0;group<7;++group) {
        for(size_t i=0;i<count;++i) pixels[i*4+3]=coverage[i][group];
        next.masks[group]=xdraw::create_srv_from_rgba(pixels.data(),next.width,next.height);
        if(!next.masks[group]) return false;
    }
    silhouette=std::move(next);
    return true;
}

int HitGroup(float u,float v) {
    if(u<0.f || v<0.f || u>=1.f || v>=1.f || silhouette.hit_groups.empty()) return -1;
    const int x=static_cast<int>(u*silhouette.width),y=static_cast<int>(v*silhouette.height);
    return static_cast<int>(silhouette.hit_groups[static_cast<size_t>(y)*silhouette.width+x])-1;
}

struct SettingsRef { bool* enabled[7]; int* rank[7]; };
SettingsRef HitSettings() {
    auto& s=feature_ui::settings;
    return {{&s.hit_head.value,&s.hit_chest.value,&s.hit_stomach.value,&s.hit_left_arm.value,
             &s.hit_right_arm.value,&s.hit_left_leg.value,&s.hit_right_leg.value},
            {&s.rank_head.value,&s.rank_chest.value,&s.rank_stomach.value,&s.rank_left_arm.value,
             &s.rank_right_arm.value,&s.rank_left_leg.value,&s.rank_right_leg.value}};
}
int DisplayRank(int group) {
    const auto& s=feature_ui::settings;
    if(s.hitbox.value==1) return group==0?1:0;
    if(s.hitbox.value==2) return group==1 || group==2?1:0;
    if(s.hitbox.value==3) return group==0?1:2;
    // Automatic is weapon/target-dependent; avoid suggesting a fixed rank.
    if(s.hitbox.value==0) return 0;
    const auto refs=HitSettings();
    return *refs.enabled[group]?std::clamp(*refs.rank[group],1,4):0;
}
bool Interactive(const xui::rect& canvas) {
    const auto& c=xui::ctx(); const auto& input=c.input;
    if(c.overlay_blocking() || c.active_slider || c.active_resize || c.active_child_scroll ||
       c.active_keybind || c.active_text_input || !input.in_rect(canvas)) return false;
    // Invisible/scrolled-out regions must never consume clicks.
    for(const auto& win:c.windows) if(!input.in_rect(win.bounds)) return false;
    const auto& clips=xui::draw::current().clip_stack;
    if(!clips.empty()) {
        const auto& r=clips.back();
        if(input.mouse_x<r.left || input.mouse_x>=r.right || input.mouse_y<r.top || input.mouse_y>=r.bottom) return false;
    }
    return true;
}
void Centered(xdraw::draw_list& dl,float cx,float y,const char* text,color c,xdraw::font* font=nullptr) {
    dl.text(std::round(cx-xdraw::measure_text(text,font).first*.5f),std::round(y),text,c,xdraw::text_style::outlined,font);
}
void PreviewSkeleton(xdraw::draw_list& dl,float x,float y,float width,float height,float thickness) {
    // Illustration-space joints follow the Hunter's pose. In-game segments
    // instead come from the animated model's parent/child bone transforms.
    constexpr std::array<std::array<float,2>,23> joints{{
        {.54f,.17f}, {.53f,.248f}, {.518f,.315f}, {.52f,.407f}, {.529f,.539f},
        {.362f,.288f}, {.254f,.392f}, {.157f,.489f}, {.119f,.534f},
        {.665f,.282f}, {.773f,.362f}, {.863f,.413f}, {.892f,.444f},
        {.46f,.56f}, {.426f,.697f}, {.415f,.832f}, {.464f,.896f},
        {.61f,.56f}, {.588f,.685f}, {.597f,.811f}, {.668f,.855f},
        {.52f,.36f}, {.524f,.47f}
    }};
    constexpr std::array<std::array<int,2>,22> edges{{
        {0,1},{1,2},{2,21},{21,3},{3,22},{22,4},
        {1,5},{5,6},{6,7},{7,8}, {1,9},{9,10},{10,11},{11,12},
        {4,13},{13,14},{14,15},{15,16}, {4,17},{17,18},{18,19},{19,20}
    }};
    thickness=std::isfinite(thickness)?std::clamp(thickness,.5f,4.f):1.f;
    for(int pass=0;pass<2;++pass) {
        const auto tint=pass?color{esp::SkeletonColor}:color{0,0,0,200};
        const float stroke=pass?thickness:thickness+1.f;
        for(const auto& edge:edges) {
            const auto a=joints[edge[0]],b=joints[edge[1]];
            dl.line(x+a[0]*width,y+a[1]*height,x+b[0]*width,y+b[1]*height,tint,stroke);
        }
    }
}
// Silhouette glow, like the native outline: the model texture is redrawn a
// few pixels away in 8 directions with the glow tint, then the normal draw
// covers the interior so only a colored fringe hugs the model's outline.
void GlowOutline(xdraw::draw_list& dl,float x,float y,float w,float h,IDirect3DTexture9* texture,color tint) {
    if(!texture) return;
    constexpr std::array<std::array<float,2>,8> dirs{{
        {1.f,0.f},{-1.f,0.f},{0.f,1.f},{0.f,-1.f},
        {.707f,.707f},{-.707f,.707f},{.707f,-.707f},{-.707f,-.707f}
    }};
    constexpr std::array<float,3> radii{2.f,5.f,9.f};
    constexpr std::array<std::uint8_t,3> alphas{255,140,70};
    for(int ring=0;ring<3;++ring) {
        const auto step=tint.alpha(alphas[ring]);
        for(const auto& d:dirs)
            dl.image(x+d[0]*radii[ring],y+d[1]*radii[ring],w,h,texture,step);
    }
}
xui::rect Canvas(float height) {
    const auto r=xui::layout::item(xui::layout::avail().first,height);
    xui::draw::current().rect_filled(r.x,r.y,r.w,r.h,xui::ctx().style.child_bg,xdraw::corner_radius{8});
    return r;
}

void SlotGlow(xdraw::draw_list& dl,const esp::layout::Rect& bar,bool selected) {
    constexpr color gold{255,199,64},light{255,229,153};
    const float pulse=.8f+.2f*std::sin(static_cast<float>(ImGui::GetTime())*4.f);
    // Gold stays recognizable in every theme; the selected destination gains a
    // brighter core and a wider halo without covering the label itself.
    for(int i=4;i>=1;--i) {
        const float pad=static_cast<float>(i)*2;
        dl.rect_filled(bar.x-pad,bar.y-pad,bar.w+pad*2,bar.h+pad*2,
                       gold.alpha(static_cast<std::uint8_t>((selected?18.f:10.f)*pulse)),xdraw::corner_radius{5});
    }
    dl.rect_filled(bar.x-2,bar.y-2,bar.w+4,bar.h+4,gold.alpha(selected?65:25),xdraw::corner_radius{3});
    dl.rect(bar.x-2,bar.y-2,bar.w+4,bar.h+4,(selected?light:gold).alpha(selected?255:175),xdraw::corner_radius{3},selected?1.5f:1.f);
}
esp::layout::Rect HealthTextInk() {
    auto* font=rendering::g_fonts.esp;
    if(!font) return {};
    float pen=0,left=10000,top=10000,right=-10000,bottom=-10000;
    for(const auto c:std::string_view{"250"}) {
        const auto& glyph=font->get(c);
        left=std::min(left,pen+glyph.bearing_x);
        top=std::min(top,font->ascent-glyph.bearing_y);
        right=std::max(right,pen+glyph.bearing_x+glyph.width);
        bottom=std::max(bottom,font->ascent-glyph.bearing_y+glyph.height);
        pen+=glyph.advance;
    }
    return {left,top,right-left,bottom-top};
}
void DrawHealth(xdraw::draw_list& dl,esp::layout::HealthLayout layout,std::uint8_t alpha=255,float dx=0,float dy=0) {
    const auto rect=[&](const esp::layout::Rect& r,color c) { dl.rect_filled(r.x+dx,r.y+dy,r.w,r.h,c); };
    const auto fade=[&](ImU32 c){return color{overlay_paint::Fade(c,alpha/255.f)};};
    rect(layout.outline,fade(overlay_paint::Track));
    dl.rect(layout.outline.x+dx-.5f,layout.outline.y+dy-.5f,layout.outline.w+1,layout.outline.h+1,fade(overlay_paint::Border),1.f);
    const auto tint=fade(esp::style::HealthColor(1.f,feature_ui::settings.health_bands.value));
    rect(layout.fill,tint);
    if(layout.fill.h>2.f)rect({layout.fill.x,layout.fill.y,layout.fill.w,1.f},fade(IM_COL32(255,255,255,75)));
    const auto ink=HealthTextInk();
    dl.rect_filled(layout.text.x+dx-2,layout.text.y+dy-1,ink.w+4,ink.h+2,fade(overlay_paint::Surface),xdraw::corner_radius{3});
    dl.text(std::round(layout.text.x-ink.x)+dx,std::round(layout.text.y-ink.y)+dy,"250",{240,240,245,alpha},xdraw::text_style::outlined,
            rendering::g_fonts.esp);
}
bool DrawPlacements(const xui::rect& canvas,esp::layout::Rect body) {
    auto& settings=feature_ui::settings;
    const auto position=[&](LayoutPart part)->int& {
        if(part==LayoutPart::Health)return settings.health_position.value;
        return part==LayoutPart::Name?settings.name_position.value:settings.distance_position.value;
    };
    const auto enabled=[&](LayoutPart part) {
        if(part==LayoutPart::Health)return settings.health.value;
        return part==LayoutPart::Name?settings.names.value:part==LayoutPart::Distance&&settings.distance.value;
    };
    constexpr std::array parts{LayoutPart::Name,LayoutPart::Distance,LayoutPart::Health};
    for(const auto part:parts)position(part)=esp::layout::NormalizeSide(position(part));
    if(placement_drag.active&&!enabled(placement_drag.part))placement_drag={};
    if(placement_drag.active)body=placement_drag.body;
    const auto ink=HealthTextInk();
    const float tw=ink.w,th=ink.h;
    auto* font=rendering::g_fonts.esp;
    const auto name_size=xdraw::measure_text("Hunter",font),distance_size=xdraw::measure_text("15 m",font);
    struct Placement { esp::layout::HealthLayout health;esp::layout::LabelLayout labels; };
    const auto layout=[&](LayoutPart item=LayoutPart::None,int side=-1) {
        const auto value=[&](LayoutPart part) {return part==item&&side>=0?side:position(part);};
        Placement result;
        result.health=esp::layout::Health(body,value(LayoutPart::Health),1,tw,th,name_size.second);
        result.labels=esp::layout::Labels(body,value(LayoutPart::Name),value(LayoutPart::Distance),
            {0,0,name_size.first,name_size.second},{0,0,distance_size.first,distance_size.second},
            settings.health.value?&result.health:nullptr,value(LayoutPart::Health),settings.names.value,settings.distance.value);
        return result;
    };
    const auto bounds=[](const Placement& placement,LayoutPart part) {
        if(part==LayoutPart::Health)return placement.health.outline;
        return part==LayoutPart::Name?placement.labels.name:placement.labels.distance;
    };
    const auto slot=[&](const Placement& placement,LayoutPart part) {
        auto rect=bounds(placement,part);
        if(part!=LayoutPart::Health) {
            const float width=std::max(52.f,rect.w+8.f);
            rect.x-=(width-rect.w)*.5f;rect.w=width;rect.y-=3.f;rect.h+=6.f;
        }
        return rect;
    };
    auto& dl=xui::draw::current();const auto& input=xui::ctx().input;
    const auto draw_part=[&](const Placement& placement,LayoutPart part,std::uint8_t alpha=255,float dx=0.f,float dy=0.f) {
        if(!enabled(part))return;
        if(part==LayoutPart::Health) {DrawHealth(dl,placement.health,alpha,dx,dy);return;}
        const auto rect=bounds(placement,part);
        const auto tint=part==LayoutPart::Name?settings.esp_color_hunter.value:color{240,240,245};
        const float opacity=alpha/255.f*(tint.a/255.f);
        dl.rect_filled(std::round(rect.x)+dx-3,std::round(rect.y)+dy-1,rect.w+6,rect.h+2,
                       color{overlay_paint::Fade(overlay_paint::Surface,opacity)},xdraw::corner_radius{3});
        dl.text(std::round(rect.x)+dx,std::round(rect.y)+dy,part==LayoutPart::Name?"Hunter":"15 m",
                tint.alpha(static_cast<std::uint8_t>(255*opacity)),xdraw::text_style::outlined,font);
    };
    auto current=layout();
    const bool interactive=Interactive(canvas);
    LayoutPart hovered=LayoutPart::None;
    if(interactive&&!dragging_model&&!placement_drag.active)for(const auto part:parts) {
        if(enabled(part)&&(bounds(current,part).contains(input.mouse_x,input.mouse_y,part==LayoutPart::Health?6.f:3.f)||
           (part==LayoutPart::Health&&current.health.text.contains(input.mouse_x,input.mouse_y,4.f)))) {
            hovered=part;break;
        }
    }
    if(hovered!=LayoutPart::None&&input.mouse_clicked) {
        placement_drag={true,hovered,position(hovered),-1,input.mouse_x,input.mouse_y,body};
        dragging_model=false;
        xui::ctx().active_window=xui::null_id;
    }
    const bool was_dragging=placement_drag.active;
    if(placement_drag.active&&(input.key_down(VK_ESCAPE)||input.rmb_clicked||xui::ctx().overlay_blocking()))placement_drag={};
    if(placement_drag.active) {
        // Freeze the anchor geometry until release so the running Hunter cannot
        // move the drop targets out from under the pointer.
        std::array<Placement,esp::layout::SideCount> slots;
        int target=-1;float nearest=22.f*22.f;
        for(int side=0;side<esp::layout::SideCount;++side) {
            slots[side]=layout(placement_drag.part,side);
            if(!interactive) continue;
            const auto area=slot(slots[side],placement_drag.part);
            const float dx=input.mouse_x-std::clamp(input.mouse_x,area.x,area.right());
            const float dy=input.mouse_y-std::clamp(input.mouse_y,area.y,area.bottom());
            const bool hp_text=placement_drag.part==LayoutPart::Health&&slots[side].health.text.contains(input.mouse_x,input.mouse_y,4);
            const float distance=hp_text?0.f:dx*dx+dy*dy;
            if(distance<nearest) {nearest=distance;target=side;}
        }
        placement_drag.target=target;
        for(int side=0;side<esp::layout::SideCount;++side)SlotGlow(dl,slot(slots[side],placement_drag.part),side==target);
        if(input.mouse_released) {
            if(target>=0)position(placement_drag.part)=target;
            placement_drag={};current=layout();
        } else if(!input.mouse_down) {
            placement_drag={}; // Lost button/focus without a normal drop.
        } else {
            const auto part=placement_drag.part;
            draw_part(current,part,55);
            if(target>=0)current=slots[target];
            for(const auto item:parts)if(item!=part||target>=0)draw_part(current,item);
            if(target<0)draw_part(current,part,240,input.mouse_x-placement_drag.start_x,input.mouse_y-placement_drag.start_y);
            const auto item=part==LayoutPart::Health?"HP":part==LayoutPart::Name?"Name":"Distance";
            const auto label=target>=0?std::string(ui::translate("Release"))+": "+std::string(ui::translate(item))+" / "+
                std::string(ui::translate(esp::layout::HealthSideNames[target])):std::string(ui::translate("Drop on a golden slot"));
            dl.text(canvas.x+10,canvas.y+9,label,tokens::col_text,xdraw::text_style::outlined);
            return true;
        }
    }
    if(hovered!=LayoutPart::None&&!was_dragging)SlotGlow(dl,slot(current,hovered),true);
    for(const auto part:parts)draw_part(current,part);
    return hovered!=LayoutPart::None||was_dragging;
}
}

bool EditEspLayout(const xui::rect& canvas,esp::layout::Rect body) {return DrawPlacements(canvas,body);}

Microsoft::WRL::ComPtr<IDirect3DTexture9> Load(int& width,int& height) {
    const auto module=reinterpret_cast<HMODULE>(&__ImageBase);
    const auto resource=FindResourceW(module,MAKEINTRESOURCEW(201),RT_RCDATA);
    if(!resource) {app::Log("Hunter preview: missing embedded resource");return {};}
    const auto size=SizeofResource(module,resource);
    const auto handle=LoadResource(module,resource);
    const auto data=static_cast<const std::byte*>(handle?LockResource(handle):nullptr);
    if(!data || !size) return {};
    auto texture=xdraw::load_texture({data,size},&width,&height);
    if(!texture) app::Log("Hunter preview: texture decode failed");
    else {
        try {
            if(!BuildSilhouette(texture.Get(),width,height)) app::Log("Hunter preview: silhouette masks unavailable");
        } catch(...) {
            silhouette={};
            app::Log("Hunter preview: silhouette allocation failed");
        }
    }
    return texture;
}

void StaticHitboxes(IDirect3DTexture9* texture,float height) {
    auto& s=feature_ui::settings;
    const auto r=Canvas(height); auto& dl=xui::draw::current();
    dl.push_clip(r.x,r.y,r.w,r.h);
    const float ih=r.h-15.f,iw=ih*(992.f/1586.f);
    const float ix=r.x+(r.w-iw)*.5f+20.f,iy=r.y-5.f;
    if(texture) dl.image(ix,iy,iw,ih,texture);
    const auto& input=xui::ctx().input;
    const bool interactive=Interactive(r);
    int hovered=-1;
    if(interactive) {
        const float mx=(input.mouse_x-ix)/iw,my=(input.mouse_y-iy)/ih;
        hovered=HitGroup(mx,my);
    }
    if(hovered>=0 && (input.mouse_clicked || input.rmb_clicked)) {
        const int previous=DisplayRank(hovered);
        if(s.hitbox.value!=4) {
            // Start custom editing from the visible preset, preserving ignored zones.
            const auto refs=HitSettings();
            for(int i=0;i<7;++i) {
                const int rank=DisplayRank(i);
                *refs.enabled[i]=rank!=0;
                if(rank) *refs.rank[i]=rank;
            }
            s.hitbox.value=4;
        }
        const int next=(previous+(input.rmb_clicked?4:1))%5;
        const auto refs=HitSettings();
        *refs.enabled[hovered]=next!=0;
        if(next) *refs.rank[hovered]=next;
        xui::ctx().active_window=xui::null_id;
    }
    // Hover shows a pulsing fill preview of the zone before it gets a rank.
    const float pulse=.5f+.5f*std::sin(static_cast<float>(GetTickCount64()%4389)*0.006f);
    for(int group=0;group<7;++group) {
        if(!silhouette.masks[group]) continue;
        const int rank=DisplayRank(group);
        const bool over=hovered==group;
        if(!rank && !over) continue;
        auto tint=rank?ranks[rank]:color{210,218,225,60};
        if(over) {
            tint=xui::lerp(tint,color{255,255,255},.2f+.35f*pulse);
            tint.a=static_cast<std::uint8_t>(std::min<int>(255,static_cast<int>(tint.a)*2));
        }
        // Flat, opaque fill clipped to this body part's actual silhouette.
        dl.image(ix,iy,iw,ih,silhouette.masks[group].Get(),tint);
    }
    dl.text(r.x+10,r.y+10,ui::translate(s.hitbox.value==0?"AUTO / WEAPON":"PRIORITY"),tokens::col_text);
    for(int i=0;i<5;++i) {
        const int rank=i==0?0:5-i;
        const float y=r.y+42.f+static_cast<float>(i)*22.f;
        dl.rect_filled(r.x+10,y+4,7,7,ranks[rank]);
        dl.text(r.x+23,y,ui::translate(rank_names[rank]),tokens::col_text);
    }
    if(hovered>=0) {
        char label[64]{};
        std::snprintf(label,sizeof(label),"%s  /  %s",zone_names[hovered],
                      s.hitbox.value==0?"Auto":rank_names[DisplayRank(hovered)]);
        Centered(dl,r.x+r.w*.5f,r.y+r.h-24,label,tokens::col_text);
    } else if(s.hitbox.value==4) {
        bool any=false;
        for(int i=0;i<7;++i) any=any || DisplayRank(i)>0;
        if(!any) Centered(dl,r.x+r.w*.5f,r.y+r.h-24,"Choose at least one zone",ranks[2]);
    }
    if(!texture) Centered(dl,r.x+r.w*.5f,r.y+r.h-44,"Hunter image unavailable",tokens::col_text);
    dl.pop_clip();
}

void StaticEsp(IDirect3DTexture9* texture,float height) {
    const auto& s=feature_ui::settings;
    const auto r=Canvas(height); auto& dl=xui::draw::current();
    dl.push_clip(r.x,r.y,r.w,r.h);
    const float ih=r.h-36.f,iw=ih*(992.f/1586.f);
    const float ix=r.x+(r.w-iw)*.5f+10.f,iy=r.y+8.f;
    const color outline=overlay_paint::Fade(IM_COL32(1,4,9,235),s.esp_color_hunter.value.a/255.f);
    // Silhouette glow sits behind the model so only the fringe stays visible.
    const color glow_tint=s.glow_color_infected.value;
    if(s.glow.value) GlowOutline(dl,ix,iy,iw,ih,texture,glow_tint);
    if(texture) dl.image(ix,iy,iw,ih,texture);
    dl.text(r.x+10,r.y+10,ui::translate("PREVIEW"),tokens::col_text);
    if(s.skeleton.value) PreviewSkeleton(dl,ix,iy,iw,ih,s.skeleton_width.value);
    const float left=ix+.075f*iw,right=ix+.925f*iw,top=iy+.09f*ih,bottom=iy+.924f*ih;
    const float thickness=std::clamp(s.box_width.value,.5f,4.f);
    // ESP box follows the configured Hunter palette color.
    const auto esp=s.esp_color_hunter.value;
    // Cham fill preview tracks the live toggle.
    if(s.chams.value && texture) dl.image(ix,iy,iw,ih,texture,esp.alpha(150));
    if(s.boxes.value) {
        if(s.corners.value) {
            const float w=std::min(18.f,(right-left)*.25f),h=std::min(24.f,(bottom-top)*.2f);
            for(int i=0;i<4;++i) {
                const float x=i&1?right:left,y=i&2?bottom:top;
                const float x2=x+(i&1?-w:w),y2=y+(i&2?-h:h);
                if(s.box_outlines.value) {dl.line(x,y,x2,y,outline,thickness+2.f);dl.line(x,y,x,y2,outline,thickness+2.f);}
                dl.line(x,y,x2,y,esp,thickness);dl.line(x,y,x,y2,esp,thickness);
            }
        } else {
            if(s.box_outlines.value) dl.rect(left,top,right-left,bottom-top,outline,thickness+2.f);
            dl.rect(left,top,right-left,bottom-top,esp,thickness);
        }
    }
    EditEspLayout(r,{left,top,right-left,bottom-top});
    if(!texture) Centered(dl,(left+right)*.5f,(top+bottom)*.5f,"Hunter image unavailable",tokens::col_text);
    dl.pop_clip();
}

namespace {
void ChangeRank(int group,bool backwards) {
    auto& s=feature_ui::settings;
    const int previous=DisplayRank(group);
    const auto refs=HitSettings();
    if(s.hitbox.value!=4) {
        for(int i=0;i<7;++i) {
            const int rank=DisplayRank(i);
            *refs.enabled[i]=rank!=0;
            if(rank) *refs.rank[i]=rank;
        }
        s.hitbox.value=4;
    }
    const int next=(previous+(backwards?4:1))%5;
    *refs.enabled[group]=next!=0;
    if(next) *refs.rank[group]=next;
}
void ModelInput(hunter_model::View& model,const xui::rect& r,bool selector) {
    if(placement_drag.active) return;
    const auto& input=xui::ctx().input;
    if(Interactive(r)) {
        if(input.scroll_delta) model.zoom=std::clamp(model.zoom+input.scroll_delta*.06f,.6f,1.4f);
        if(input.mouse_clicked&&(!selector||input.shift_held())) {
            dragging_model=true;
            xui::ctx().active_window=xui::null_id;
        }
    }
    if(dragging_model&&input.mouse_down) {
        model.yaw=std::remainder(model.yaw+input.mouse_delta_x()*.7f,360.f);
    }
}
bool EspMarks(const hunter_model::View& view,const xui::rect& r) {
    const auto& s=feature_ui::settings; auto& dl=xui::draw::current();
    if(s.skeleton.value) hunter_model::Skeleton(view,dl,r.x,r.y,r.w,r.h,s.skeleton_color_infected.value,
                                               std::clamp(s.skeleton_width.value,.5f,4.f));
    const auto& box=s.box_precision.value?view.bounds:view.collision_bounds;
    const float left=r.x+box[0]*r.w/view.width,right=r.x+box[2]*r.w/view.width;
    const float top=r.y+box[1]*r.h/view.height,bottom=r.y+box[3]*r.h/view.height;
    const color outline=overlay_paint::Fade(IM_COL32(1,4,9,235),s.esp_color_hunter.value.a/255.f);
    // ESP box follows the configured Hunter palette color.
    const color esp=s.esp_color_hunter.value;
    const float thickness=std::clamp(s.box_width.value,.5f,4.f);
    if(s.chams.value) dl.image(r.x,r.y,r.w,r.h,view.texture.Get(),esp.alpha(150));
    if(s.boxes.value) {
        if(s.corners.value) {
            const float w=std::min(18.f,(right-left)*.25f),h=std::min(24.f,(bottom-top)*.2f);
            for(int i=0;i<4;++i) {
                const float x=i&1?right:left,y=i&2?bottom:top;
                const float x2=x+(i&1?-w:w),y2=y+(i&2?-h:h);
                if(s.box_outlines.value) {dl.line(x,y,x2,y,outline,thickness+2.f);dl.line(x,y,x,y2,outline,thickness+2.f);}
                dl.line(x,y,x2,y,esp,thickness);dl.line(x,y,x,y2,esp,thickness);
            }
        } else {
            if(s.box_outlines.value) dl.rect(left,top,right-left,bottom-top,outline,thickness+2.f);
            dl.rect(left,top,right-left,bottom-top,esp,thickness);
        }
    }
    return EditEspLayout(r,{left,top,right-left,bottom-top});
}
bool ModelCanvas(bool selector,float height) {
    auto& view=windows[selector?1:0].model;
    const auto [width,unused]=xui::layout::avail();
    const float label_padding=selector?0.f:28.f;
    const float model_height=std::max(64.f,height-label_padding*2.f);
    // Match the panel aspect ratio. Separate RTs prevent one window replacing the other.
    const float sampling=std::min(1.5f,768.f/std::max(width,model_height));
    const int rw=static_cast<int>(width*sampling),rh=static_cast<int>(model_height*sampling);
    static int pulse_zone=-1;
    std::array<color,7> fills{};
    if(selector) for(int i=0;i<7;++i) {
        const int rank=DisplayRank(i); fills[i]=rank?ranks[rank]:color{0,0,0,0};
    }
    if(selector && pulse_zone>=0) {
        // One-frame-old hover keeps the pulsing fill aligned with the pointer.
        const float pulse=.5f+.5f*std::sin(static_cast<float>(GetTickCount64()%4389)*0.006f);
        auto tint=fills[pulse_zone];
        if(!tint.a) tint={210,218,225,60};
        tint=xui::lerp(tint,color{255,255,255},.2f+.35f*pulse);
        tint.a=static_cast<std::uint8_t>(std::min<int>(255,static_cast<int>(tint.a)*2));
        fills[pulse_zone]=tint;
    }
    try {
        // Pause only the preview pose while placing a label/bar; preserve playback.
        const bool playing=view.playing;
        struct RestorePlayback { hunter_model::View& view; bool playing; ~RestorePlayback(){view.playing=playing;} } restore{view,playing};
        if(!selector&&placement_drag.active) view.playing=false;
        if(!hunter_model::Render(view,rw,rh,selector?&fills:nullptr)) return false;
    } catch(...) { return false; }
    const auto canvas=Canvas(height);auto& dl=xui::draw::current();
    const xui::rect r{canvas.x,canvas.y+label_padding,canvas.w,model_height};
    dl.push_clip(canvas.x,canvas.y,canvas.w,canvas.h);
    // Glow outline must be behind the model; EspMarks draws the rest on top.
    if(!selector && feature_ui::settings.glow.value)
        GlowOutline(dl,r.x,r.y,r.w,r.h,view.texture.Get(),feature_ui::settings.glow_color_infected.value);
    dl.image(r.x,r.y,r.w,r.h,view.texture.Get());
    if(selector) {
        ModelInput(view,r,true);
        const auto& input=xui::ctx().input;
        int hovered=-1;
        if(Interactive(r)&&!dragging_model) hovered=hunter_model::Pick(view,
            (input.mouse_x-r.x)*view.width/r.w,(input.mouse_y-r.y)*view.height/r.h);
        if(hovered>=0&&!input.shift_held()&&(input.mouse_clicked||input.rmb_clicked)) ChangeRank(hovered,input.rmb_clicked);
        pulse_zone=hovered;
        dl.text(r.x+10,r.y+10,ui::translate(feature_ui::settings.hitbox.value==0?"AUTO / WEAPON":"PRIORITY"),tokens::col_text);
        for(int i=0;i<5;++i) {
            const int rank=i==0?0:5-i;const float y=r.y+36+static_cast<float>(i)*21;
            dl.rect_filled(r.x+10,y+4,7,7,ranks[rank]);dl.text(r.x+23,y,ui::translate(rank_names[rank]),tokens::col_text);
        }
        if(hovered>=0) {
            char label[80]{};std::snprintf(label,sizeof(label),"%s / %s",zone_names[hovered],
                feature_ui::settings.hitbox.value==0?"Auto":rank_names[DisplayRank(hovered)]);
            Centered(dl,r.center_x(),r.bottom()-23,label,tokens::col_text);
        }
    } else if(!EspMarks(view,r)) ModelInput(view,canvas,false);
    dl.pop_clip();
    return true;
}
void Controls(hunter_model::View& view) {
    const char* animations[]{"idle","walk","run"};
    if(xui::combo("animation",view.clip,animations,3)) view.time=0;
    xui::slider_float("speed",view.speed,.25f,2.f,"%.2fx");
    xui::slider_float("zoom",view.zoom,.6f,1.4f,"%.2fx");
    if(xui::button(view.playing?"pause":"play",90,24)) view.playing=!view.playing;
    xui::layout::same_line();
    if(xui::button("reset view",110,24)) { view.yaw=15;view.zoom=1;view.time=0; }
}
}
void Hitboxes(IDirect3DTexture9* texture,float height) {
    if(!ModelCanvas(true,height)) {
        StaticHitboxes(texture,height);
        xui::text("3D unavailable / static fallback",tokens::col_text_dim);
    }
}
void Esp(IDirect3DTexture9* texture,float height) {
    if(!ModelCanvas(false,height)) {
        StaticEsp(texture,height);
        xui::text("3D unavailable / static fallback",tokens::col_text_dim);
    }
}
void EmbeddedEsp(IDirect3DTexture9* texture,float height) {
    auto& view=windows[0].model;
    const char* animations[]{"Reposo","Caminar","Correr"};
    if(xui::combo("Animación",view.clip,animations,3)) view.time=0;
    // Reserve the controls before drawing, so the preview never scrolls away.
    const float canvas_height=std::max(100.f,xui::layout::avail().second-40.f);
    Esp(texture,std::min(height,canvas_height));
    if(xui::button(view.playing?"Pausar":"Reproducir",112,24)) view.playing=!view.playing;
    xui::layout::same_line();
    if(xui::button("Restablecer",112,24)) { view.yaw=15;view.zoom=1;view.time=0; }
}
void OpenHitboxes() { windows[1].open=true;front_window=1; }
void CancelInput() { captured_window=-1;dragging_header=dragging_model=false;placement_drag={}; }
bool OwnsPointer() noexcept { return captured_window>=0 || dragging_model || placement_drag.active; }
void BeginMenuInput() {
    ClampWindows();
    const auto& input=xui::ctx().input;
    // Keep ownership on the release frame so labels/HP can commit the drop before the
    // underlying window receives pointer input again.
    if(!input.mouse_down&&!input.rmb_down&&!(placement_drag.active&&input.mouse_released)) CancelInput();
    if(!xui::ctx().overlay_blocking()) popup_owner=-1;
    input_owner=-1;
    if(xui::ctx().overlay_blocking()) input_owner=popup_owner;
    else if(captured_window>=0) input_owner=captured_window;
    else if(captured_window==-2) input_owner=-1;
    else {
        for(int index:{front_window,1-front_window}) {
            const auto& w=windows[index];
            if(w.open&&input.in_rect({w.x,w.y,w.w,w.h})) {input_owner=index;break;}
        }
    }
    if(input_owner>=0&&(input.mouse_clicked||input.rmb_clicked)&&!xui::ctx().overlay_blocking()) {
        captured_window=front_window=input_owner;
        const auto& w=windows[input_owner];
        dragging_header=input.mouse_clicked&&input.in_rect({w.x,w.y,w.w-32,29});
    }
    else if(input_owner<0&&(input.mouse_clicked||input.rmb_clicked)) captured_window=-2;
    if(dragging_header&&captured_window>=0&&input.mouse_down) {
        auto& w=windows[captured_window];w.x+=input.mouse_delta_x();w.y+=input.mouse_delta_y();ClampWindows();
    }
    if(input_owner>=0) { main_input=input;MaskInput(); }
}
void EndMenuInput() {
    if(main_input) {xui::ctx().input=std::move(*main_input);main_input.reset();}
}
void DrawWindows(IDirect3DTexture9* fallback) {
    // Render back to front and route pointer input to exactly one window.
    for(int index:{1-front_window,front_window}) {
        auto& w=windows[index];if(!w.open) continue;
        auto saved=xui::ctx().input;
        if(input_owner!=index) MaskInput();
        const bool had_popup=xui::ctx().overlay_blocking();
        const bool clicked=xui::ctx().input.mouse_clicked;
        xui::ctx().input.mouse_clicked=false; // This window drags by its title only.
        xui::begin_window(index?"##hunter_hitboxes":"##hunter_esp",w.x,w.y,w.w,w.h);
        xui::ctx().input.mouse_clicked=clicked;
        auto& dl=xui::draw::current();
        dl.text(w.x+12,w.y+9,ui::translate(index?"Hitbox Selection / Hunter":"ESP Preview / Hunter"),tokens::col_text);
        xui::layout::set_cursor(w.w-30,5);
        if(xui::button("x",24,23)) {w.open=false;CancelInput();}
        xui::layout::set_cursor(10,36);
        if(index) {
            const char* presets[]{"automatic by weapon","head only","torso","all hitboxes","custom zones"};
            xui::combo("preset",feature_ui::settings.hitbox.value,presets,5);
            Hitboxes(fallback,std::max(120.f,w.h-285));
        } else {
            Esp(fallback,std::max(140.f,w.h-235));
        }
        Controls(w.model);
        xui::end_window();
        if(!had_popup&&xui::ctx().overlay_blocking()) popup_owner=index;
        xui::ctx().input=std::move(saved);
    }
}
void InvalidateDeviceObjects() { for(auto& w:windows) hunter_model::Invalidate(w.model); }
}
