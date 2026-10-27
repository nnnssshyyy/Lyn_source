#include "charger_teleport.h"
#include "features.h"
#include <cmath>
#include <cstdio>
#include <cstring>

int main() {
    unsigned checks{},failed{};
    const auto expect=[&](bool ok,const char* name) {++checks;if(!ok){++failed;std::printf("FAIL: %s\n",name);}};
    using charger_teleport::Input;using charger_teleport::Pulse;
    Input ready{true,true,true,true,false,true,true,true,false,3,100,0x1001,0x2002};
    expect(!features::Config{}.charger_teleport,"feature starts disabled");
    expect(std::isfinite(charger_teleport::Roll) && std::bit_cast<unsigned>(charger_teleport::Roll)==0x7cf0bdc2u,
        "payload matches the finite Raze immediate exactly");
    game::Command command{};
    std::memset(&command,0x35,sizeof(command));command.angles={25.f,-61.f,0.f};
    auto expected=command;expected.angles.z=std::bit_cast<float>(0x7cf0bdc2u);
    charger_teleport::Apply(command.angles);
    expect(!std::memcmp(&command,&expected,sizeof(command)),"only command roll changes; movement tick seed and other fields survive");
    for(int gate=0;gate<14;++gate) {
        auto in=ready;
        switch(gate) {
        case 0:in.enabled=false;break;case 1:in.focused=false;break;case 2:in.in_game=false;break;
        case 3:in.alive=false;break;case 4:in.ghost=true;break;case 5:in.charger=false;break;
        case 6:in.channel=false;break;case 7:in.carry_known=false;break;case 8:in.frozen=true;break;
        case 9:in.team=2;break;case 10:in.command=0;break;case 11:in.owner=0;break;
        case 12:in.victim=0;break;case 13:in.victim=UINT32_MAX;break;
        }
        Pulse pulse;
        expect(!pulse.Prepare(in) && !pulse.Commit(),"unavailable gameplay or activation gate cannot commit");
        expect(pulse.Prepare(ready) && pulse.Commit(),"a closed gate does not consume the next valid carry");
    }
    Pulse pulse;
    expect(pulse.Prepare(ready),"first grab prepares one command");
    expect(pulse.Prepare(ready),"failed command copy remains retryable");
    expect(pulse.Commit(),"successful command copy consumes the carry");
    expect(!pulse.Commit(),"double commit is inert");
    expect(pulse.Prepare(ready) && !pulse.Commit(),"replaying the same command reproduces payload without recounting");
    for(int number=101;number<=180;++number) {
        auto in=ready;in.command=number;
        expect(!pulse.Prepare(in) && !pulse.Commit(),"subsequent commands cannot repeatedly trigger a carried victim");
    }
    auto next=ready;next.command=181;
    next.enabled=false;expect(!pulse.Prepare(next),"disabling during a carry pauses output");
    next.enabled=true;expect(!pulse.Prepare(next),"toggling a bind cannot retrigger a committed carry");
    next.focused=false;expect(!pulse.Prepare(next),"focus loss pauses output");
    next.focused=true;expect(!pulse.Prepare(next),"focus restoration does not retrigger");
    next.carry_known=false;next.victim=0;expect(!pulse.Prepare(next),"failed read is not a release");
    next.carry_known=true;next.victim=ready.victim;expect(!pulse.Prepare(next),"recovered field does not retrigger");
    next.victim=0;expect(!pulse.Prepare(next),"real release rearms the state");
    next.victim=ready.victim;expect(pulse.Prepare(next) && pulse.Commit(),"same survivor can be grabbed again after release");
    next.command++;next.victim+=0x1000;
    expect(pulse.Prepare(next) && pulse.Commit(),"changed victim serial starts a new carry");
    next.command++;next.owner+=0x1000;
    expect(pulse.Prepare(next) && pulse.Commit(),"respawned owner has independent carry state");
    next.in_game=false;expect(!pulse.Prepare(next) && !pulse.Tracking(),"disconnect clears pending and committed state");
    next.in_game=true;next.command=1;expect(pulse.Prepare(next) && pulse.Commit(),"new map command sequence can start at one");
    pulse.Reset();expect(!pulse.Tracking() && !pulse.Commit(),"explicit level cleanup clears pending writes");
    auto disabled=ready;disabled.enabled=false;
    expect(!pulse.Prepare(disabled),"disabled grab does not fire");
    expect(pulse.Prepare(ready) && pulse.Commit(),"enabling on an unconsumed carry matches Raze");
    std::printf("%u charger teleport checks, %u failed\n",checks,failed);
    return failed?1:0;
}
