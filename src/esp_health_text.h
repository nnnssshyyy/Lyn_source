#pragma once
#include "esp_layout.h"
#include <imgui.h>
#include <cfloat>

namespace esp::layout {
// HP labels contain ASCII digits, spaces and arithmetic separators. Measure
// their visible glyphs, excluding the font's invisible line-height padding.
inline Rect HealthTextInk(ImFont* font,const char* text) noexcept {
    if(!font || !text || font->FontSize<=0.f) return {};
    const float scale=HealthTextSize/font->FontSize;
    float pen=0.f,left=FLT_MAX,top=FLT_MAX,right=-FLT_MAX,bottom=-FLT_MAX;
    for(auto p=reinterpret_cast<const unsigned char*>(text);*p;++p) {
        const auto* glyph=font->FindGlyph(*p);
        if(!glyph) continue;
        if(glyph->Visible) {
            left=std::min(left,pen+glyph->X0*scale);
            top=std::min(top,glyph->Y0*scale);
            right=std::max(right,pen+glyph->X1*scale);
            bottom=std::max(bottom,glyph->Y1*scale);
        }
        pen+=glyph->AdvanceX*scale;
    }
    return left<=right?Rect{left,top,right-left,bottom-top}:Rect{};
}
inline ImVec2 HealthTextOrigin(const Rect& text,const Rect& ink) noexcept {
    return {std::round(text.x-ink.x),std::round(text.y-ink.y)};
}
}
