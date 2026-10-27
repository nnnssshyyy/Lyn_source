#include "esp_font.h"
#include "velocity_ui/font_assets.hpp"
#include <Windows.h>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <vector>

namespace esp::font {
namespace {
std::vector<unsigned char> SystemFont(const wchar_t* filename) {
    wchar_t directory[MAX_PATH]{};
    const auto length=GetWindowsDirectoryW(directory,MAX_PATH);
    if(!length || length>=MAX_PATH) return {};
    std::ifstream file(std::filesystem::path(directory)/L"Fonts"/filename,std::ios::binary);
    return std::vector<unsigned char>(std::istreambuf_iterator<char>(file),{});
}
}
bool Initialize() {
    auto& io=ImGui::GetIO();family={};
    // Only bake ranges used by display names: extended Latin, Greek, Cyrillic,
    // combining marks, symbols, braille, styled math letters and emoji outlines.
    static constexpr ImWchar ranges[]{0x20,0x052f,0x1d00,0x1eff,0x2000,0x2bff,
        0x3040,0x30ff,0xa640,0xa69f,0xff00,0xffef,0xfffd,0xfffd,0x1d400,0x1d7ff,0x1f000,0x1faff,0};
    // Windows' symbol font is loaded locally; it is never redistributed.
    static const auto symbols=SystemFont(L"seguisym.ttf");
    static const auto kana=SystemFont(L"msgothic.ttc");
    for(int size=Minimum;size<=Maximum;++size) {
        ImFontConfig compact{};compact.FontDataOwnedByAtlas=false;
        compact.OversampleH=2;compact.OversampleV=1;compact.PixelSnapH=true;
        std::snprintf(compact.Name,sizeof(compact.Name),"VS3 ESP Unicode %d",size);
        auto* font=io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char*>(velocity_assets::inter_bold),
            sizeof(velocity_assets::inter_bold),static_cast<float>(size),&compact,ranges);
        if(!font) return false;
        ImFontConfig fallback=compact;fallback.MergeMode=true;
        const auto merge=[&](const unsigned char* data,size_t length) {
            return io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char*>(data),static_cast<int>(length),
                static_cast<float>(size),&fallback,ranges)!=nullptr;
        };
        if(!merge(velocity_assets::inter_medium,sizeof(velocity_assets::inter_medium)) ||
           !merge(velocity_assets::noto_math,sizeof(velocity_assets::noto_math))) return false;
        if(!symbols.empty() && !merge(symbols.data(),symbols.size())) return false;
        if(!kana.empty() && !merge(kana.data(),kana.size())) return false;
        family.sizes[size-Minimum]=font;
    }
    family.atlas=io.Fonts;
    io.FontDefault=family.sizes[Default-Minimum];
    return true;
}
}
