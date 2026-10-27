#pragma once
#include "source.h"

namespace spectators {
constexpr size_t MaxPlayers=64;
enum class State { Waiting, NotInGame, Unavailable, Ready };
struct Entry { char name[64]{}; int userid{},index{},mode{}; };
struct Snapshot {
    std::array<Entry,MaxPlayers> entries{};
    size_t count{};
    ULONGLONG captured{};
    State state{State::Waiting};
};
void Initialize(const source::Interfaces& interfaces) noexcept;
void SetEnabled(bool enabled) noexcept;
void SetChase(bool enabled) noexcept;
void Frame(int stage) noexcept;
void ClearLevel() noexcept;
Snapshot ReadSnapshot() noexcept;
}
