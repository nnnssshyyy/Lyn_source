#pragma once
#include "source.h"
#include <imgui.h>
#include <array>
#include <span>

namespace esp {
constexpr size_t MaxTargets = 256;
constexpr ImU32 SkeletonColor=IM_COL32(238,242,248,255);
struct Options {
    bool enabled{false};
    bool boxes{true}, health{true}, names{true}, distance{true};
    bool survivors{true}, infected{true}, witch{true};
    bool corner_boxes{false};
    float max_distance{200.f};
    float thickness{1.f};
    ImVec4 survivor_color{0.35f,0.95f,0.67f,1.f};
    ImVec4 infected_color{1.f,0.35f,0.42f,1.f};
    ImVec4 tank_color{1.f,0.7f,0.2f,1.f};
    ImVec4 witch_color{0.82f,0.5f,1.f,1.f};
};
struct Target {
    enum class BoundsSource : unsigned char { Collision, Render, Missing };
    source::Vec3 origin{}, mins{}, maxs{};
    source::Kind kind{source::Kind::None};
    int health{-1}, max_health{-1};
    int permanent_health{-1}, temporary_health{};
    bool incapacitated{};
    float meters{};
    bool has_distance{};
    char name[64]{};
    BoundsSource bounds_source{BoundsSource::Collision};
    std::array<source::Vec3,8> render_corners{};
};
struct Frame {
    source::Matrix matrix{};
    std::array<Target,MaxTargets> targets{};
    size_t count{};
    ULONGLONG captured_at{};
    bool ready{}, in_game{}, has_matrix{};
    unsigned resolved_classes{}, skipped_classes{}, candidates{}, read_failures{};
    float collection_ms{};
    char status[192]{"Esperando al motor"};
};
void Initialize(const source::Interfaces& interfaces);
void OnGameFrame(int stage) noexcept;
void ClearLevel() noexcept;
void SetEnabled(bool enabled) noexcept;
bool Enabled() noexcept;
void Retry() noexcept;
void CopyFrame(Frame& result) noexcept;
unsigned Draw(const Frame& frame, const Options& options, ImDrawList* list, ImVec2 screen);
bool ProjectBounds(const Target& target,const source::Matrix& matrix,ImVec2 screen,ImVec2& min,ImVec2& max) noexcept;
bool IntersectsView(const Target& target,const source::Matrix& matrix) noexcept;
bool ProjectSegment(source::Vec3 start,source::Vec3 end,const source::Matrix& matrix,ImVec2 screen,
                    ImVec2& from,ImVec2& to) noexcept;
void DrawSkeleton(std::span<const source::BoneSegment> bones,const source::Matrix& matrix,
                  ImDrawList* list,ImVec2 screen,float thickness,ImU32 color=SkeletonColor) noexcept;
}

