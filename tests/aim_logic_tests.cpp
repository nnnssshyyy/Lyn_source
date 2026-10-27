#include "aim_logic.h"
#include <cstdio>
#include <limits>

int main() {
    using namespace aimbot::logic;
    unsigned failed{},checks{};
    const auto expect=[&](bool condition,const char* scenario) {
        ++checks;if(!condition) {std::fprintf(stderr,"FAIL: %s\n",scenario);++failed;}
    };
    for(const auto name:{"Death_Standing_01","death_headshot","Burning_Death","CommonDeathForward","DEATH"})
        expect(DeathSequence(name),"common death labels reject upper, lower, and mixed case");
    for(const auto name:{"","Idle","Run","Shoved_Backward","Burning_Run","ClimbUp","Fall"})
        expect(!DeathSequence(name),"living idle, burning, shove, climb and fall labels are not corpses");
    // An intentionally empty custom mask must never enable automatic head aim.
    expect(!Allowed(Mask(4,0),1,1,false),"empty custom selection stays empty");
    expect(!Allowed(Mask(2,127),1,1,false),"torso preset excludes head despite weapon preference");
    expect(Allowed(Mask(2,0),3,1,false),"torso preset permits stomach");
    expect(Allowed(Mask(0,0),2,2,true),"automatic one-shot weapon selects chest");
    expect(Allowed(Mask(0,0),1,2,true),"automatic chest preference permits a visible head fallback");
    expect(!Allowed(127,8,1,false),"unsupported hitgroups are rejected");
    // The historical point can be visible after the live entity moved away.
    expect(Visible(1.f,false,false,false),"clear historical ray without live entity hit is visible");
    expect(!Visible(.6f,false,false,false),"wall before selected point blocks the shot");
    expect(!Visible(1.f,true,false,true),"starting inside solid rejects even an entity hit");
    expect(Visible(.8f,false,false,true),"ray hitting selected entity before center is visible");
    expect(!ShotVisible(1.f,false,false,false,false),"reaching empty live space is not a confirmed hit");
    expect(!ShotVisible(.9995f,false,false,false,true),"historical ray cannot accept a wall just before its endpoint");
    expect(ShotVisible(1.f,false,false,false,true),"clear historical point remains usable when its old hitbox moved away");
    expect(ShotVisible(.8f,false,false,true,false),"live ray confirms the selected entity");
    expect(!ShotVisible(1.f,true,false,true,false) && !ShotVisible(1.f,false,true,true,false),"solid traces never authorize a shot");
    expect(!ShotVisible(std::numeric_limits<float>::quiet_NaN(),false,false,true,false),"nonfinite trace fraction is rejected");
    constexpr int held=1|4|8|2048;
    expect(ShotButtons(held,0,true,false,.3f,.1f)==(held&~1),"normal manual attack waits for the hitbox alignment and preserves other buttons");
    expect(ShotButtons(held,0,true,false,.05f,.1f)==held,"aligned normal manual attack fires");
    expect(ShotButtons(held,0,false,false,99.f,.1f)==held,"manual fire without a selected target is preserved");
    expect(ShotButtons(held,1,true,false,99.f,.1f)==held,"client silent uses its direct aimed command");
    expect(ShotButtons(held,2,true,true,0.f,.1f)==(held&~1),"server flush cannot fire an extra bullet with the restored mouse view");
    expect(ShotButtons(held,2,true,false,0.f,.1f)==held,"server firing command preserves primary attack");
    expect(ShotButtons(held,0,true,false,std::numeric_limits<float>::quiet_NaN(),.1f)==(held&~1),"invalid alignment cannot authorize normal firing");
    expect(SilentPacket(2,true,false,false,false)==PacketAction::Hold,"server aimed shot remains held for this command");
    expect(SilentPacket(2,false,true,false,true)==PacketAction::Send,"next command flushes the retained aimed shot");
    expect(SilentPacket(2,true,true,false,false)==PacketAction::Keep,"a flush can never be re-choked by the silent policy");
    expect(SilentPacket(2,true,false,true,false)==PacketAction::Keep,"rapid burst transport retains its existing send policy");
    expect(SilentPacket(2,true,false,false,true)==PacketAction::Send,"forced recovery send takes precedence over silent hold");
    for(int mode:{0,1}) {
        expect(SilentPacket(mode,true,false,false,false)==PacketAction::Keep,"normal and client modes do not alter packet cadence");
        expect(SilentPacket(mode,true,false,false,true)==PacketAction::Send,"forced sends remain available outside server silent");
    }
    expect(ValidRecord(10.f,9.9f,.1f,.015f,.2f),"recent latency-matched history is accepted");
    expect(!ValidRecord(10.f,9.7f,.1f,.015f,.2f),"expired history is rejected");
    expect(!ValidRecord(10.f,10.1f,.1f,.015f,.2f),"future record is rejected");
    expect(!ValidRecord(10.f,9.9f,std::numeric_limits<float>::quiet_NaN(),.015f,.2f),"invalid network timing is rejected");
    expect(SmoothFraction(0.f)==1.f,"normal zero smoothing snaps to target");
    expect(SmoothFraction(100.f)>0.f,"maximum smoothing still makes progress");
    const float at30=SmoothFraction(75.f,1.f/30.f),at60=SmoothFraction(75.f,1.f/60.f);
    expect(std::abs(at30-(1.f-(1.f-at60)*(1.f-at60)))<.00001f,"smoothing has equal elapsed-time response at 30 and 60 ticks");
    expect(!ClearAdvantage(1.9f,2.f,0) && ClearAdvantage(.5f,2.f,0),"FOV retention requires a meaningful angular improvement");
    expect(!ClearAdvantage(990.f,1000.f,1) && ClearAdvantage(600.f,1000.f,1),"distance retention does not prevent a clearly closer target");
    expect(std::isfinite(SmoothFraction(std::numeric_limits<float>::quiet_NaN(),0.f)),"invalid smoothing inputs cannot propagate NaN into view angles");
    int tick=777;
    expect(!HistoricalTick(false,9.95f,.05f,1.f/30.f,300,10.f,.1f,.2f,tick) && tick==777,
           "live rendered pose preserves the original engine command tick");
    expect(HistoricalTick(true,9.9f,1.f/30.f,1.f/30.f,300,10.f,.1f,.2f,tick) && tick==298,
           "valid historical shot uses its pose time plus interpolation");
    expect(!HistoricalTick(true,9.7f,.05f,1.f/30.f,300,10.f,.1f,.2f,tick),"expired history cannot rewrite a shot tick");
    expect(!HistoricalTick(true,9.9f,.05f,1.f/30.f,100,10.f,.1f,.2f,tick),"unrelated command epoch cannot accept history");
    expect(!HistoricalTick(true,9.9f,.05f,1.f/30.f,300,10.f,std::numeric_limits<float>::quiet_NaN(),.2f,tick),
           "invalid network timing cannot rewrite a shot");
    expect(ShotTolerance(1000.f,3.f)<.13f && ShotTolerance(100.f,8.f)==.5f,"automatic firing tolerance shrinks with hitbox angular size");
    expect(ShotTolerance(1000.f,.6f)<ShotTolerance(1000.f,3.f),"exposed edge needs more precise alignment than center");
    expect(ShotTolerance(0.f,3.f)==0.f && ShotTolerance(100.f,-1.f)==0.f,"invalid geometry cannot loosen firing tolerance");
    if(failed) return 1;
    std::printf("%u aim policy scenarios passed\n",checks);
    return 0;
}
