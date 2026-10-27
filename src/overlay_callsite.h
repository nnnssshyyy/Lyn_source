#pragma once
#include <cstddef>
#include <cstdint>

namespace capture_overlay {
enum class Route { Game, PrivateWindow, SteamOverlay };
constexpr Route SelectRoute(bool privacy,int method) noexcept {
    return !privacy?Route::Game:method==1?Route::SteamOverlay:Route::PrivateWindow;
}
// One validated Steam call site, at most once between game presentations.
// Missing/reloaded modules never fall back to an arbitrary game EndScene.
class OverlayCallsite {
    std::uintptr_t base_{},selected_{};
    std::size_t size_{};
    std::uint64_t frame_{};
    bool drawn_{};
public:
    void Reset() noexcept {*this={};}
    void Module(std::uintptr_t base,std::size_t size) noexcept {
        if(base_!=base||size_!=size){Reset();base_=base;size_=size;}
    }
    bool Contains(std::uintptr_t caller) const noexcept {
        return base_&&caller>=base_&&caller-base_<size_;
    }
    bool Candidate(std::uintptr_t caller,std::uint64_t frame) const noexcept {
        return Contains(caller)&&(!selected_||selected_==caller)&&(!drawn_||frame_!=frame);
    }
    bool Accept(std::uintptr_t caller,std::uint64_t frame) noexcept {
        if(!Candidate(caller,frame))return false;
        selected_=caller;frame_=frame;drawn_=true;return true;
    }
};
}
