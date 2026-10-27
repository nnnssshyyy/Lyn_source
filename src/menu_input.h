#pragma once
#include <cstdint>

namespace menu_input {
// A requested menu may intercept input only while its frames are actually
// being presented. A failed/hidden renderer must never trap game controls.
constexpr bool Captures(bool requested,bool active,bool focused,std::uint64_t now,std::uint64_t presented) noexcept {
    return requested && active && focused && presented && now >= presented && now-presented < 300;
}
}
