#pragma once
#include "esp_layout.h"
#include <imgui.h>
#include <cmath>
#include <cfloat>

// Small shared primitives for the live ESP, fallback overlay and HUD previews.
namespace overlay_paint {
inline constexpr ImU32 Surface=IM_COL32(9,14,23,185);
inline constexpr ImU32 Track=IM_COL32(10,16,25,220);
inline constexpr ImU32 Border=IM_COL32(135,160,188,65);
inline constexpr ImU32 Text=IM_COL32(240,244,250,255);
inline constexpr ImU32 Muted=IM_COL32(172,188,207,255);
inline ImU32 Fade(ImU32 color,float amount) noexcept {
    amount=std::isfinite(amount)?std::clamp(amount,0.f,1.f):0.f;
    return (color&0x00ffffffu)|(static_cast<ImU32>((color>>24)*amount)<<24);
}
inline bool Valid(esp::layout::Rect r) noexcept {
    return std::isfinite(r.x)&&std::isfinite(r.y)&&std::isfinite(r.w)&&std::isfinite(r.h)&&r.w>0&&r.h>0;
}
inline void Plate(ImDrawList* dl,esp::layout::Rect r,ImU32 color=Surface,float rounding=3.f) {
    if(!dl||!Valid(r)||!(color>>24))return;
    dl->AddRectFilled({r.x,r.y},{r.right(),r.bottom()},color,rounding);
}
inline void Label(ImDrawList* dl,ImVec2 pos,ImU32 color,const char* value,ImFont* font=nullptr,float size=0.f,bool plate=true) {
    if(!dl||!value||!*value||!(color>>24)||!std::isfinite(pos.x)||!std::isfinite(pos.y))return;
    if(!font)font=ImGui::GetFont();if(!std::isfinite(size)||size<=0.f)size=font->FontSize;
    pos={std::round(pos.x),std::round(pos.y)};
    const float opacity=(color>>24)/255.f;
    if(plate){const auto ink=font->CalcTextSizeA(size,FLT_MAX,0.f,value);Plate(dl,{pos.x-3,pos.y-1,ink.x+6,ink.y+2},Fade(Surface,opacity));}
    dl->AddText(font,size,{pos.x+1,pos.y+1},Fade(IM_COL32(0,0,0,235),opacity),value);
    dl->AddText(font,size,pos,color,value);
}
inline void Box(ImDrawList* dl,ImVec2 lo,ImVec2 hi,ImU32 color,bool corners,float width,bool outlines=true) {
    if(!dl||!(color>>24)||!std::isfinite(width)||!Valid({lo.x,lo.y,hi.x-lo.x,hi.y-lo.y}))return;
    width=std::clamp(width,.5f,4.f);
    lo={std::round(lo.x)+.5f,std::round(lo.y)+.5f};hi={std::round(hi.x)+.5f,std::round(hi.y)+.5f};
    const auto stroke=[&](ImU32 tint,float thickness) {
        if(!corners){dl->AddRect(lo,hi,tint,0,0,thickness);return;}
        const float x=std::min(18.f,(hi.x-lo.x)*.25f),y=std::min(24.f,(hi.y-lo.y)*.2f);
        for(unsigned i=0;i<4;++i){const ImVec2 p{(i&1)?hi.x:lo.x,(i&2)?hi.y:lo.y};
            dl->AddLine(p,{p.x+((i&1)?-x:x),p.y},tint,thickness);
            dl->AddLine(p,{p.x,p.y+((i&2)?-y:y)},tint,thickness);}
    };
    if(outlines)stroke(Fade(IM_COL32(1,4,9,235),(color>>24)/255.f),width+2.f);
    stroke(color,width);
}
inline void HealthTrack(ImDrawList* dl,esp::layout::Rect r) {
    if(!dl||!Valid(r))return;
    Plate(dl,r,Track,1.f);dl->AddRect({r.x-.5f,r.y-.5f},{r.right()+.5f,r.bottom()+.5f},Border,1.f);
}
inline void HealthFill(ImDrawList* dl,esp::layout::Rect r,ImU32 color) {
    if(!dl||!Valid(r))return;
    dl->AddRectFilled({r.x,r.y},{r.right(),r.bottom()},color);
    if(r.h>2.f)dl->AddRectFilled({r.x,r.y},{r.right(),r.y+1.f},Fade(IM_COL32(255,255,255,75),(color>>24)/255.f));
}
}
