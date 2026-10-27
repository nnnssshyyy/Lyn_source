#pragma once
#include <algorithm>
#include <cmath>

// Shared by the ImGui game overlay and the XUI preview. Coordinates are pixels.
namespace esp::layout {
inline constexpr float HealthTextSize=12.f;
inline constexpr float HealthTextOffsetY=2.f;
enum HealthSide { Left, Right, Top, Bottom, SideCount };
inline constexpr const char* HealthSideNames[]{"left","right","top","bottom"};
struct Rect {
    float x{},y{},w{},h{};
    float right() const noexcept { return x+w; }
    float bottom() const noexcept { return y+h; }
    bool contains(float px,float py,float padding=0) const noexcept {
        return px>=x-padding && px<=right()+padding && py>=y-padding && py<=bottom()+padding;
    }
};
struct HealthLayout {
    Rect outline,fill,text;
    float name_y{},distance_y{};
};
inline int NormalizeSide(int side) noexcept { return std::clamp(side,0,int(SideCount)-1); }
inline HealthLayout Health(Rect body,int side,float fraction,float text_width,float text_height,float label_height=0.f) noexcept {
    side=NormalizeSide(side);
    fraction=std::isfinite(fraction)?std::clamp(fraction,0.f,1.f):0.f;
    text_width=std::max(0.f,text_width); text_height=std::max(1.f,text_height);
    const float row_height=label_height>0.f?label_height:text_height;
    HealthLayout out{};
    out.name_y=body.y-row_height-3;
    out.distance_y=body.bottom()+3;
    if(side==Left || side==Right) {
        const float x=side==Left?body.x-8:body.right()+3;
        const float bar_top=std::round(body.y),bar_bottom=std::round(body.bottom());
        const float fill_top=bar_bottom-std::round((bar_bottom-bar_top)*fraction);
        out.outline={x,body.y-1,5,body.h+2};
        out.fill={x+1,fill_top,3,bar_bottom-fill_top};
        // Follow the live fill edge as HP drops, staying centered on the bar.
        const float center_x=out.outline.x+out.outline.w*.5f;
        out.text={center_x-text_width*.5f,fill_top-text_height-1.f+HealthTextOffsetY,text_width,text_height};
        // Reserve the name row at the full bar so names do not move with HP.
        out.name_y=out.outline.y-text_height-row_height-5.f;
    } else {
        const float y=side==Top?body.y-8:body.bottom()+3;
        out.outline={body.x-1,y,body.w+2,5};
        out.fill={body.x,y+1,body.w*fraction,3};
        const float tx=body.x+(body.w-text_width)*.5f;
        out.text={tx,(side==Top?y-3-text_height:y+8)+HealthTextOffsetY,text_width,text_height};
        // The HP label owns the first row outside a horizontal health bar.
        if(side==Top) out.name_y=out.text.y-row_height-3;
        else out.distance_y=out.text.bottom()+3;
    }
    return out;
}

struct LabelLayout { Rect name,distance; };
// Each side can stack both labels. Health reserves its own lane, including the
// full-health label position so a falling HP value never makes names jump.
inline LabelLayout Labels(Rect body,int name_side,int distance_side,Rect name_size,Rect distance_size,
                          const HealthLayout* health=nullptr,int health_side=Left,
                          bool show_name=true,bool show_distance=true,float right_reserved=0.f) noexcept {
    name_side=NormalizeSide(name_side);distance_side=NormalizeSide(distance_side);
    const float row=std::max(1.f,distance_size.h);
    const float top_edge=health?health->name_y+row:body.y-3.f;
    const float bottom_edge=health?health->distance_y:body.bottom()+3.f;
    const auto place=[&](Rect size,int side) {
        Rect out{body.x+(body.w-size.w)*.5f,body.y+(body.h-size.h)*.5f,size.w,size.h};
        if(side==Top)out.y=top_edge-size.h;
        else if(side==Bottom)out.y=bottom_edge;
        else if(side==Left)out.x=(health&&health_side==Left?std::min(health->outline.x,health->text.x):body.x)-7.f-size.w;
        else {
            out.x=(health&&health_side==Right?std::max(health->outline.right(),health->text.right()):body.right())+7.f;
            out.y=std::max(out.y,body.y+right_reserved+(right_reserved>0.f?3.f:0.f));
        }
        return out;
    };
    LabelLayout result{place(name_size,name_side),place(distance_size,distance_side)};
    if(show_name&&show_distance&&name_side==distance_side) {
        if(name_side==Top)result.name.y=result.distance.y-name_size.h-3.f;
        else if(name_side==Bottom)result.distance.y=result.name.bottom()+3.f;
        else {
            float y=body.y+(body.h-name_size.h-distance_size.h-3.f)*.5f;
            if(name_side==Right)y=std::max(y,body.y+right_reserved+(right_reserved>0.f?3.f:0.f));
            result.name.y=y;result.distance.y=result.name.bottom()+3.f;
        }
    }
    return result;
}
}
