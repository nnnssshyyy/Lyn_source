#include "combat_input.h"
#include <cstdio>

int main() {
    unsigned checks{},failed{};
    const auto expect=[&](bool ok,const char* name) {
        ++checks;
        if(!ok) {++failed;std::fprintf(stderr,"FAIL: %s\n",name);}
    };
    rapid_fire::PistolEdge edge;
    constexpr int secondary=2048,reload=8192,moving=8,held=1|secondary|moving;

    // Regression: the old incap/pin branch fed Next(false) for every non-semi
    // weapon and then cleared M1 from the command, including Smoker captures.
    bool automatic_preserved=true;
    for(int command=0;command<20;++command)
        automatic_preserved=automatic_preserved && combat_input::PausedPrimary(held,true,false,edge)==held;
    expect(automatic_preserved,"pinned automatic weapons retain every held primary command");
    expect(combat_input::PausedPrimary(held,false,true,edge)==held,
           "disabled pulse retains manual attack on a semi-auto weapon");
    expect(combat_input::PausedPrimary(secondary|reload,true,false,edge)==(secondary|reload),
           "unknown or non-semi weapons preserve other input without synthesizing attack");

    expect(combat_input::PausedPrimary(held,true,true,edge)==held,
           "confirmed semi-auto starts a held input with a press");
    expect(combat_input::PausedPrimary(held,true,true,edge)==(held&~1),
           "confirmed semi-auto releases only primary on the next held command");
    expect(combat_input::PausedPrimary(held,true,true,edge)==held,
           "confirmed semi-auto re-arms the next press");
    expect(combat_input::PausedPrimary(secondary|moving,true,true,edge)==(secondary|moving),
           "manual release never creates an attack and retains secondary and movement");
    expect(combat_input::PausedPrimary(held,true,true,edge)==held,
           "a new hold after manual release starts with a press");

    expect(combat_input::PausedPrimary(held,true,false,edge)==held,
           "changing to a non-semi or unreadable weapon preserves primary immediately");
    expect(combat_input::PausedPrimary(held,true,true,edge)==held,
           "returning from a non-semi or unreadable weapon resets the pulse phase");
    expect(combat_input::PausedPrimary(held,false,true,edge)==held &&
           combat_input::PausedPrimary(held,true,true,edge)==held,
           "a flush or disabled interval resets the pulse without dropping the next press");

    std::printf("%u paused combat input checks, %u failed\n",checks,failed);
    return failed?1:0;
}
