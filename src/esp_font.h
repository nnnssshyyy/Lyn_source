#pragma once
#include <imgui.h>
#include <algorithm>
#include <array>
#include <cmath>

namespace esp::font {
inline constexpr int Minimum=10,Maximum=18,Default=12;
struct Family {ImFontAtlas* atlas{};std::array<ImFont*,Maximum-Minimum+1> sizes{};};
inline Family family;
bool Initialize();
inline ImFont* At(float requested) noexcept {
    auto& io=ImGui::GetIO();
    // Contexts without the menu/font initialization retain their own valid font.
    if(!family.atlas || family.atlas!=io.Fonts || io.FontDefault!=family.sizes[Default-Minimum])
        return ImGui::GetFont();
    const int size=std::isfinite(requested)?std::clamp(static_cast<int>(std::round(requested)),Minimum,Maximum):Default;
    return family.sizes[size-Minimum];
}
}
