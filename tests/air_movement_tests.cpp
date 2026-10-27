#include "air_movement.h"
#include "movement_assists.h"
#include "lag_exploit.h"
#include "storm_fall.h"
#include <cstdio>
#include <limits>

namespace {
struct Velocity {float x{},y{};float Speed() const {return std::hypot(x,y);}};
// Independent forward model of the installed AirAccelerate SSE routine. The
// solver is judged by the resulting velocity, not its own reported gain.
Velocity Accelerate(Velocity v,float wx,float wy,const air_movement::Settings& s) {
    const float length=std::hypot(wx,wy);
    if(length<.001f) return v;
    const float wishspeed=std::min(length,s.max_speed);
    wx/=length;wy/=length;
    const float add=std::min(wishspeed,30.f)-(v.x*wx+v.y*wy);
    if(add<=0.f) return v;
    const float amount=std::min(add,s.air_acceleration*wishspeed*s.interval*s.friction);
    return {v.x+amount*wx,v.y+amount*wy};
}
Velocity Apply(Velocity v,const air_movement::Result& r,const air_movement::Settings& s) {
    const float yaw=r.basis_yaw*air_movement::Pi/180.f;
    return Accelerate(v,std::cos(yaw)*r.forward+std::sin(yaw)*r.side,
                      std::sin(yaw)*r.forward-std::cos(yaw)*r.side,s);
}
bool Near(float a,float b,float epsilon=.002f) {return std::abs(a-b)<epsilon;}
}
int main() {
    using namespace air_movement;
    unsigned checks{},failures{};
    const auto expect=[&](bool ok,const char* name){++checks;if(!ok){++failures;std::fprintf(stderr,"FAIL: %s\n",name);}};
    expect(MobileForAssistance(false,true,3),"a Versus ghost is eligible despite preceding-life incap or pin flags");
    expect(!MobileForAssistance(false,false,3)&&!MobileForAssistance(false,true,2),"immobilized living players and incapped survivors stay paused");
    expect(WalkPhysics(2,0)&&WalkPhysics(2,1),"dry and shallow-water ghost walking retains ground physics");
    for(int type:{0,4,5,8,9,10,11}) expect(!WalkPhysics(type,0),"ladder noclip spectator and non-walk modes keep native jump handling");
    expect(!WalkPhysics(-1,0)&&!WalkPhysics(2,-1)&&!WalkPhysics(2,2),"unknown movement data and swimming fail closed");
    Controller controller;
    Player player{0x1001,true,false,0,220.f,0.f};
    Settings settings{true,true,true,1.f/30.f,10.f,220.f,1.f};
    Input input{1,123,Jump,0,0.f,450.f,0.f};
    auto result=controller.Run(input,player,settings);
    const auto improved=Apply({220,0},result,settings);
    const float old_ideal=std::atan2(30.f,220.f);
    const auto old=Accelerate({220,0},450.f*std::cos(old_ideal),450.f*std::sin(old_ideal),settings);
    expect(result.strafed && improved.Speed()>220.1f && Near(old.Speed(),220.f),
           "new strafe gains native-model speed where the old almost-forward input gains none");
    expect(Near(std::hypot(result.forward,result.side),450.f) && input.yaw==0.f,
           "full air input respects the command limit and does not turn the camera");
    expect(!result.jump && result.hop_applied && result.held,"held jump is released while airborne without losing its intent");
    for(float interval:{1.f/30.f,1.f/60.f,1.f/100.f,1.f/128.f}) {
        for(float speed:{0.f,15.f,30.f,220.f,600.f,1800.f}) {
            for(float acceleration:{1.f,10.f,100.f}) {
                controller.Reset();settings.interval=interval;settings.air_acceleration=acceleration;
                player.vx=speed;player.vy=0.f;input.forward=0.f;
                const auto solved=controller.Run(input,player,settings);
                const auto next=Apply({speed,0},solved,settings);
                float best=speed;
                for(int sample=0;sample<720;++sample) {
                    const float angle=static_cast<float>(sample)*Pi/360.f;
                    best=std::max(best,Accelerate({speed,0},450.f*std::cos(angle),450.f*std::sin(angle),settings).Speed());
                }
                expect(solved.strafed && next.Speed()+.015f>=best && next.Speed()+.001f>=speed,
                       "analytic result meets a 720-direction brute-force search at multiple tickrates speeds and accelerations");
            }
        }
    }
    settings={true,true,true,1.f/30.f,10.f,220.f,1.f};player={0x1001,true,false,0,220.f,0.f};
    for(float side:{-450.f,450.f}) {
        controller.Reset();input={2,123,Jump,0,0,0,side};
        result=controller.Run(input,player,settings);
        const auto next=Apply({220,0},result,settings);
        expect(result.strafed && next.y*side<0.f && next.Speed()>220.f,
               "directional A and D turn the trajectory toward the selected side while gaining speed");
    }
    controller.Reset();input={3,123,Jump,0,0,-450.f,0};
    auto v=Velocity{220,0};
    for(int tick=0;tick<100;++tick) {
        input.number=tick+10;player.vx=v.x;player.vy=v.y;
        v=Apply(v,controller.Run(input,player,settings),settings);
    }
    expect(v.x<0.f && v.Speed()>220.f,"holding S steers around toward reverse travel without disabling assistance");
    settings.directional=false;controller.Reset();player={0x1001,true,false,0,220,0};
    input={1,123,Jump,0,0,0,-450};
    const auto no_keys=controller.Run(input,player,settings);controller.Reset();input.side=450;
    const auto other_key=controller.Run(input,player,settings);
    expect(Near(no_keys.forward,other_key.forward) && Near(no_keys.side,other_key.side),
           "non-directional mode follows camera heading even while a movement key is held");
    settings.directional=true;controller.Reset();input={5,123,Jump|Left,0,0,0,0};
    result=controller.Run(input,player,settings);
    expect(Apply({220,0},result,settings).y>0.f,"button-only movement binds still supply directional intent");
    controller.Reset();player.vx=player.vy=0;player.grounded=true;input={6,123,Jump,0,15,0,0};
    result=controller.Run(input,player,settings);
    expect(result.jump && result.forward==450.f && result.side==0.f,"holding jump alone launches from rest with forward input");
    controller.Reset();player.grounded=false;input.number++;
    result=controller.Run(input,player,settings);
    expect(result.strafed && Apply({0,0},result,settings).Speed()>0.f,"air assistance also accelerates from zero speed");
    controller.Reset();player.vx=220;input={8,123,Jump,0,0,450,0};
    result=controller.Run(input,player,settings);
    auto replay=input;replay.buttons&=~Jump;replay.forward=result.forward;replay.side=result.side;replay.yaw=80;
    const auto repeated=controller.Run(replay,player,settings);
    expect(repeated.held && Near(repeated.forward,result.forward) && Near(repeated.side,result.side) && repeated.basis_yaw==0.f,
           "repredicting an already modified command does not reinterpret strafe output as user input");
    player.grounded=true;
    const auto landing=controller.Run(replay,player,settings);
    expect(landing.jump,"replayed original jump re-arms immediately after grounding");
    expect(landing.restore_move && landing.forward==450.f && landing.side==0.f && landing.basis_yaw==0.f,
           "landing replay restores the original ground movement instead of retaining generated air strafe");
    replay.number++;
    expect(!controller.Run(replay,player,settings).jump,"jump release on the next command never inherits a held key");
    player.grounded=false;player.owner=0x2001;replay.number=8;
    expect(!controller.Run(replay,player,settings).held,"a respawned player cannot inherit old jump input");
    player.owner=0x1001;replay.number=158;
    expect(!controller.Run(replay,player,settings).held,"command ring reuse cannot inherit another command's jump input");
    controller.Reset();replay=input;replay.buttons=0;
    expect(!controller.Run(replay,player,settings).held,"map reset clears cached jump intent");
    input.number=20;
    for(int cycle=0;cycle<3;++cycle) {
        controller.Reset();player.grounded=true;
        const auto ground=controller.Run(input,player,settings);++input.number;player.grounded=false;
        const auto air=controller.Run(input,player,settings);++input.number;player.grounded=true;
        const auto landed=controller.Run(input,player,settings);++input.number;
        expect(ground.jump && !air.jump && landed.jump,"continuous hold repeats jump edges at successive landings");
    }
    player.grounded=false;controller.Reset();player.walk=false;
    result=controller.Run(input,player,settings);
    expect(!result.strafed && !result.hop_applied && result.jump,"ladder noclip and other move types preserve manual input");
    player.walk=true;player.water=2;
    expect(!controller.Run(input,player,settings).hop_applied,"swimming never consumes the jump button");
    player.water=0;settings.interval=0;
    expect(!controller.Run(input,player,settings).strafed,"paused or invalid tick interval cannot create air input");
    settings.interval=1.f/30.f;settings.air_acceleration=0;
    expect(!controller.Run(input,player,settings).strafed,"zero server acceleration does not pretend to accelerate");
    settings.air_acceleration=10;settings.friction=.25f;controller.Reset();
    result=controller.Run(input,player,settings);
    expect(result.strafed && Apply({player.vx,player.vy},result,settings).Speed()>player.vx,
           "surface friction participates in the actual acceleration budget");
    settings.friction=0;expect(!controller.Run(input,player,settings).strafed,"zero surface friction preserves unassisted movement");
    settings.friction=1;player.vx=std::numeric_limits<float>::quiet_NaN();
    expect(!controller.Run(input,player,settings).strafed,"invalid velocity never enters the solver");
    player.vx=220;settings.bhop=settings.strafe=false;
    result=controller.Run(input,player,settings);
    expect(!result.strafed && !result.hop_applied && result.jump && result.forward==input.forward,
           "both disabled features preserve the complete manual movement request");
    controller.Reset();settings.bhop=true;settings.strafe=false;player={0x1001,true,true,0,220,0};
    input={700,123,Jump,0,0,0,0};
    expect(controller.Run(input,player,settings).jump,"grounded first attempt requests jump");
    ++input.number;
    expect(!controller.Run(input,player,settings).jump,"rejected ground jump releases old input latch");
    ++input.number;
    expect(controller.Run(input,player,settings).jump,"grounded cooldown retries with a new jump edge");
    auto held_replay=input;held_replay.buttons=0;
    expect(controller.Run(held_replay,player,settings).jump,"ground pulse replay preserves original held intent");
    ++input.number;player.grounded=false;
    expect(!controller.Run(input,player,settings).jump,"takeoff clears jump even after a ground retry");
    ++input.number;player.grounded=true;
    expect(controller.Run(input,player,settings).jump,"next landing re-arms immediately after airborne command");
    // The same controller handles a mobile, walking infected ghost. No combat
    // entity or active weapon is necessary for its movement assistance.
    controller.Reset();player={0x3001,WalkPhysics(2,0),true,0,220,0};input={900,555,Jump,0,0,450,0};
    expect(MobileForAssistance(false,true,3)&&controller.Run(input,player,settings).jump,"ghost bunnyhop starts while waiting to spawn");
    player.grounded=false;++input.number;
    expect(!controller.Run(input,player,settings).jump,"ghost airborne input releases jump");
    player.grounded=true;++input.number;
    expect(controller.Run(input,player,settings).jump,"ghost landing re-arms jump without spawning or using a weapon");
    ++input.number;input.buttons=0;
    expect(!controller.Run(input,player,settings).jump,"releasing jump also stops the ghost bunnyhop");
    {
        using namespace movement_assists;
        expect(!FastStrafe(false,true,false) && FastStrafe(true,true,false) && !FastStrafe(true,false,false) && FastStrafe(false,false,true),
            "fast bunnyhop enables the validated air solver only while bunnyhop is enabled");
        settings={true,FastStrafe(true,true,false),true,1.f/30.f,10.f,220.f,1.f};
        controller.Reset();player={0x123,true,false,0,220,0};input={1001,43,Jump,0,0,450,0};
        result=controller.Run(input,player,settings);
        expect(Apply({220,0},result,settings).Speed()>220.f,"fast bunnyhop gains real velocity under native air projection and acceleration limits");
        TickHistory ticks;
        expect(ticks.Native(20,12,500,0)==500 && ticks.Native(20,12,FreezeTick,9)==500,"revisited frozen command restores its original tick on release");
        expect(ticks.Native(20,13,FreezeTick,9)==9,"reused command slot with another seed never restores a stale tick");
        ticks.Reset();expect(ticks.Native(20,12,FreezeTick,10)==10,"level reset removes stale frozen command history");
        expect(CeilingPitch==12.f,"ceiling locks exactly the requested twelve degree pitch");
        HunterClick click;
        for(float dt:{1.f/30.f,1.f/60.f,1.f/100.f}) {
            click.Reset();int previous=-100,last_attack=-100,pulses=0;
            for(int n=1;n<=120;++n) {
                bool shot=click.Run(0x123,n,n*7,dt,true);
                expect(shot==click.Run(0x123,n,n*7,dt,true),"Hunter input replays do not advance the click timer");
                if(shot) {expect(n-last_attack>=2,"Hunter pounce always has a native release edge");last_attack=n;++pulses;}
                previous=n;
            }
            expect(pulses>=10 && previous==120,"Hunter click cadence runs independently of render frame rate");
        }
        expect(!click.Run(0x123,121,3,1.f/30.f,false),"releasing the Hunter bind stops all synthetic attack input");
        expect(click.Run(0x124,122,3,1.f/30.f,true),"new Hunter entity gets a fresh initial press");
        expect(!click.Run(0x124,123,3,0,true),"invalid clock cannot generate Hunter clicks");
        using storm_fall::Requested;
        expect(!Requested(0,1.f/30.f,false) && !Requested(100,1.f/30.f,false),"Storm waits for a dangerous fall rather than any negative vertical speed");
        expect(Requested(540,1.f/30.f,false) && !Requested(540,1.f/100.f,false),"Storm anticipates one tick of gravity at the actual server tickrate");
        expect(Requested(560,1.f/100.f,false) && !Requested(700,1.f/30.f,true),"ground contact releases the automatic request even with residual fall velocity");
        expect(!Requested(-700,1.f/30.f,false) && !Requested(700,0,false) && !Requested(700,std::numeric_limits<float>::infinity(),false),"upward or invalid fall and clock samples cannot trigger Storm");
        expect(!Requested(std::numeric_limits<float>::quiet_NaN(),1.f/30.f,false),"nonfinite falling speed cannot enter a sequence calculation");
        expect(storm_fall::SequenceShift(1)==450 && storm_fall::SequenceShift(150)==450 && storm_fall::SequenceShift(151)==600,"Storm rounds tickbase up to 150 and reserves another 300 ticks");
        expect(storm_fall::SequenceShift(30000)==30300 && storm_fall::SequenceShift(30001)==30450,"large native tickbase determines the shift instead of the old 100-point slider");
        expect(storm_fall::SequenceShift(151,1)==450 && !storm_fall::SequenceShift(0) && !storm_fall::SequenceShift(-400) && !storm_fall::SequenceShift(100000000),"invalid or already negative simulation clocks never create a second shift");
        storm_fall::History history;history.Reset(0x1001);
        expect(history.Store(0x1001,100,123,450) && history.Get(0x1001,100,123)==450,"the exact command seed and owner retain the applied Storm correction");
        expect(!history.Store(0x1001,100,123,450) && !history.Get(0x1001,100,124) && !history.Get(0x2001,100,123),"prediction replay and different command identities cannot duplicate or inherit a shift");
        expect(!history.CanStore(0x1001,250) && !history.Store(0x1001,250,123,450),"unacknowledged modulo collisions pause new pushes instead of losing prediction history");
        expect(history.Get(0x1001,100,123)==450 && history.Count()==1,"landing or disabling new requests does not erase an outstanding command");
        history.Retire(99);expect(!history.Empty(),"an earlier ACK cannot retire a later correction");
        history.Retire(100);expect(history.Empty() && !history.Get(0x1001,100,123) && history.Store(0x1001,250,9,600),"ACK releases the old slot and permits a later native command");
        history.Reset(0x2001);expect(history.Empty() && !history.Get(0x1001,250,9),"respawn and map reset cannot carry an old player's correction");
        for(int n=1;n<=150;++n) expect(history.Store(0x2001,n,n*3,600),"each of 150 distinct native commands gets one bounded history slot");
        expect(history.Count()==150 && !history.Store(0x2001,151,0,600),"the prediction ring never overwrites an outstanding slot after a long unacknowledged fall");
        history.Retire(150);expect(history.Empty() && history.Store(0x2001,151,0,600),"network ACKs recover the bounded ring without a timed blind reset");
        expect(storm_fall::PredictionEnd(100,450,true)==150 && storm_fall::PredictionEnd(100,450,false)==450,"only a pending Storm correction folds prediction to its native 150-command window");
        expect(storm_fall::PredictionEnd(100,110,true)==110 && storm_fall::PredictionEnd(100,90,true)==90,"ordinary prediction and invalid reversed ranges remain unchanged");
        expect(storm_fall::MovementButtons(0x7fffffff,false)==(0x7fffffff&~1564) && (storm_fall::MovementButtons(0,true)&4),"negative-clock movement clears only Storm's input mask and restores actual duck state");
        lag_exploit::CommandGate lag_gate;
        expect(lag_gate.Take(1,10) && !lag_gate.Take(1,10) && !lag_gate.Take(1,9),
            "manual and automatic requests cannot push twice on replay, key toggle or older command");
        expect(lag_gate.Same(1,10) && !lag_gate.Same(2,10) && !lag_gate.Same(1,9),
            "only the same owner and native command may reuse the previous lag execution result");
        expect(lag_gate.Take(1,11) && lag_gate.Take(2,1),"a fresh command and changed player each receive their own network update");
        expect(!lag_gate.Take(0,20) && !lag_gate.Take(2,0),"invalid command context cannot arm a network write");
        lag_gate.Reset();expect(lag_gate.Take(2,1),"level reset clears the shared lag command history");
        expect(lag_exploit::RequiresSendContext(lag_exploit::Method::V1) && !lag_exploit::RequiresSendContext(lag_exploit::Method::V2),
            "only V1 requires the validated current packet context to force sending");
        MeleeCycle cycle;
        MeleeInput m{1,40,40,50,1,4,5,1.f/30.f,true,true,true,false};
        m.swing_known=true;
        auto action=cycle.Run(m);expect(action.swing && action.cycling && !action.select,"melee starts with a real attack using the current native cooldown");
        expect(cycle.Run(m).swing,"melee command replay keeps the same swing");
        m.number=2;m.in_swing=true;expect(!cycle.Run(m).select,"native swing must be observed before recovery");m.in_swing=false;
        m.number=5;action=cycle.Run(m);expect(action.select==5 && action.suppress,"melee recovery selects the actual owned primary index and suppresses attacks");
        m.number=6;action=cycle.Run(m);expect(action.select==5,"return swap waits for primary weapon acknowledgement");
        m.number=7;m.active=50;action=cycle.Run(m);expect(action.select==4,"acknowledged primary returns to the original melee entity");
        m.number=8;m.held=false;action=cycle.Run(m);expect(action.select==4 && action.suppress,"releasing attack during a swap still completes the owned return");
        m.number=9;m.active=40;expect(!cycle.Run(m).cycling,"melee return acknowledgement completes the cycle");
        m.number=10;m.held=true;expect(cycle.Run(m).swing,"another held swing starts when the actual weapon is ready");
        m.number=11;m.manual_select=true;expect(!cycle.Run(m).cycling,"manual weapon selection cancels the automatic melee cycle");
        m.number=12;m.manual_select=false;m.primary=0;expect(!cycle.Run(m).swing,"missing primary never causes an invented slot switch");
        m.number=13;m.primary=50;m.ready=false;expect(!cycle.Run(m).swing,"melee cannot bypass a native attack cooldown with a client field write");
        cycle.Reset();m.number=20;m.ready=true;cycle.Run(m);m.number=21;m.active=80;expect(!cycle.Run(m).cycling,"switching to another item cancels automation");
        cycle.Reset();m.active=40;m.number=30;cycle.Run(m);m.number=31;m.enabled=false;expect(!cycle.Run(m).cycling,"disabled fast melee never selects a weapon");
        cycle.Reset();m={1,40,40,50,1,4,5,1.f/30.f,true,true,true,false};m.swing_known=true;
        expect(cycle.Run(m).swing,"fresh melee hold starts with the player's native attack");
        m.number=5;m.in_swing=true;expect(!cycle.Run(m).select,"120ms cannot cancel a swing that is still active");
        m.number=14;expect(!cycle.Run(m).select,"longer native swings remain intact");
        m.number=15;m.in_swing=false;action=cycle.Run(m);expect(action.select==5,"recovery starts only after the native swing ends");
        m.manual_select=true;m.selected_weapon=5;
        expect(cycle.Run(m).select==5 && cycle.Active(),"prediction replay recognizes its own generated weapon selection");
        m.number=16;m.manual_select=false;m.selected_weapon=0;m.active=50;
        expect(cycle.Run(m).select==4,"primary acknowledgement requests the original melee");
        m.number=15;m.manual_select=true;m.selected_weapon=5;
        expect(cycle.Run(m).select==5,"an older cached command keeps its original selection");
        m.number=12;m.manual_select=false;m.selected_weapon=0;
        expect(!cycle.Run(m).select && cycle.Active(),"an older uncached command cannot erase the pending melee return");
        m.number=17;expect(cycle.Run(m).select==4,"older prediction replay never rewinds the latest recovery stage");
        m.number=65;m.held=false;expect(cycle.Run(m).select==4 && cycle.Active(),"a slow native weapon switch still returns after releasing the bind");
        m.number=66;m.active=40;expect(!cycle.Run(m).cycling,"actual melee acknowledgement ends the owned return");
        cycle.Reset();m.number=70;m.held=true;m.ready=true;m.swing_known=false;
        expect(!cycle.Run(m).swing,"missing swing state cannot fall back to a guessed damage delay");
        m.swing_known=true;cycle.Run(m);m.number=100;
        expect(!cycle.Run(m).select && !cycle.Active(),"an unconfirmed attack expires without cancelling or swapping a native swing");
        cycle.Reset();m.number=110;cycle.Run(m);m.number=111;m.interrupted=true;
        expect(!cycle.Run(m).cycling,"use, reload or shove interrupts the cycle immediately");
        m.interrupted=false;cycle.Reset();m.number=120;cycle.Run(m);m.number=121;m.in_swing=true;cycle.Run(m);
        m.number=122;m.in_swing=false;cycle.Run(m);m.active=80;
        expect(!cycle.Run(m).select && !cycle.Active(),"cached recovery cannot force a weapon switch away from another active item");
    }
    std::printf("%u air movement checks, %u failed\n",checks,failures);
    return failures?1:0;
}
