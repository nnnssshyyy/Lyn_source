#pragma once
#include "memory.h"

namespace charger_turn {
constexpr bool Eligible(bool enabled,bool running,bool in_game,bool known,int team,bool charger,bool alive,bool ghost) noexcept {
    return enabled&&running&&in_game&&known&&team==3&&charger&&alive&&!ghost;
}
// Resolves a validated view-freeze block, never an absolute module RVA.
class Patch {
    unsigned char* site_{};
    unsigned char displacement_{};
    bool applied_{},conflict_{};
    bool Change(unsigned char expected,unsigned char replacement) noexcept;
public:
    bool Bind(memory::Range image,void* block) noexcept;
    bool Set(bool enabled) noexcept;
    bool Available() const noexcept {return site_&&!conflict_;}
    bool Active() const noexcept {return applied_;}
};
}
