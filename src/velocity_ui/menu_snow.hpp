#pragma once
#include <array>
#include <cstddef>
namespace xdraw {struct draw_list;}

namespace menu_effects {
// Fixed storage and a private seed keep decoration independent of game RNG,
// input, frame rate and the menu's requested scale.
class TriangleSnow {
public:
    struct Particle {float x{},y{},angle{},speed{},size{},spin{};};
    static constexpr std::size_t Count=72;
    TriangleSnow();
    const auto& particles() const {return particles_;}
    void advance(float width,float height,float dt);
    void draw(xdraw::draw_list& dl,float x,float y,float width,float height,float dt,float opacity,
              const std::array<float,4>* exclusion=nullptr);
private:
    std::array<Particle,Count> particles_{};
};

class CometField {
public:
    struct Particle {float x{},y{},speed{},length{},size{},phase{};};
    static constexpr std::size_t Count=18;
    CometField();
    const auto& particles() const {return particles_;}
    void advance(float width,float height,float dt);
    // Optional exclusion uses left/top/right/bottom in the same pixel space.
    void draw(xdraw::draw_list& dl,float x,float y,float width,float height,float dt,float opacity,
              const std::array<float,4>* exclusion=nullptr);
private:
    struct Star {float x{},y{},brightness{};};
    std::array<Particle,Count> particles_{};
    std::array<Star,48> stars_{};
};
}
