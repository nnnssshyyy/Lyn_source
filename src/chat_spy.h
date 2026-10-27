#pragma once
#include "game.h"
#include "chat_logic.h"

namespace chat_spy {
struct Options {bool enabled{},panel{true},hud{},deduplicate{true},team_prefix{true};int clear_minutes{};};
struct Entry {char name[96]{},text[512]{};int team{},userid{};std::uint64_t serial{};};
struct Snapshot {
    std::array<Entry,logic::HistoryLimit> entries{};size_t count{};
    bool listener{},hud_hook{},hud_ready{};
    unsigned received{},suppressed{},dropped{},generation{};
    char status[128]{"Chat spy off"};
};
void Initialize(const game::Api& api) noexcept;
void SetOptions(Options options) noexcept;
void Frame() noexcept;
void ClearLevel() noexcept;
void RequestClear() noexcept;
void Suspend() noexcept;
Snapshot ReadSnapshot() noexcept;
}
