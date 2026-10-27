#include "rapid_fire.h"
#include "movement_batch.h"
#include <cstdio>
#include <limits>
#include <vector>

int main() {
    unsigned checks{},failed{};
    const auto expect=[&](bool ok,const char* name) {
        ++checks;
        if(!ok) {++failed;std::fprintf(stderr,"FAIL: %s\n",name);}
    };

    // Method indexes come from Raze's menu order (Methot 1..3).
    expect(rapid_fire::FromIndex(0)==rapid_fire::Method::Storm &&
           rapid_fire::FromIndex(1)==rapid_fire::Method::Choked &&
           rapid_fire::FromIndex(2)==rapid_fire::Method::Flush,
           "method indexes map to Storm, choke and flush");
    expect(rapid_fire::FromIndex(-1)==rapid_fire::Method::Storm &&
           rapid_fire::FromIndex(99)==rapid_fire::Method::Flush,
           "out-of-range method indexes clamp to the first/last method");
    expect(rapid_fire::SpeedMaximum==200&&rapid_fire::MaxExtraMoves==800 && rapid_fire::RingSize==150,
           "4x boost preserves original factors and the native ring size with an 800-extra ceiling");

    // Raze: N = max(configured, int(0.06/interval + 0.5)), N >= 0.
    expect(rapid_fire::ExtraMoves(6,1.f/30.f)==6,
           "configured speed dominates a 30 Hz interval");
    expect(rapid_fire::ExtraMoves(1,1.f/100.f)==6,
           "1/100 s interval raises one configured move to six");
    expect(rapid_fire::ExtraMoves(2,1.f/100.f)==6,
           "interval-derived extras can exceed a small configured factor");
    expect(rapid_fire::ExtraMoves(8,1.f/100.f)==8,
           "a configured factor above the interval-derived count wins");
    expect(rapid_fire::ExtraMoves(-5,1.f/100.f)==6,
           "negative configured speeds clamp to zero before the interval term");
    expect(rapid_fire::ExtraMoves(-5,0.f)==0,
           "negative factors with no interval fall back to zero");
    expect(rapid_fire::ExtraMoves(200,0.1f)==200,
           "the maximum configured factor is not reduced by a slow interval");
    expect(rapid_fire::ExtraMoves(0,0.1f)==1,
           "a 60 ms interval still arms one extra at factor zero");
    expect(rapid_fire::ExtraMoves(0,1.f/100.f)==6,
           "factor zero can still be raised by the interval term");
    expect(rapid_fire::ExtraMoves(3,std::numeric_limits<float>::quiet_NaN())==3 &&
           rapid_fire::ExtraMoves(3,0.f)==3 &&
           rapid_fire::ExtraMoves(3,-0.01f)==3 &&
           rapid_fire::ExtraMoves(3,std::numeric_limits<float>::infinity())==3,
           "an unreadable interval falls back to the configured factor alone");
    expect(rapid_fire::BurstMoves(6,1.f/60.f,true)==24&&rapid_fire::BurstMoves(6,1.f/60.f,false)==6,
           "4x boost applies to existing factors and respects an explicitly disabled boost");
    expect(rapid_fire::BurstMoves(200,1.f/60.f,true)==800&&rapid_fire::BurstMoves(1000,1.f/60.f,true)==800&&
           rapid_fire::BurstMoves(1,std::numeric_limits<float>::denorm_min(),true)==800,
           "high factors and tiny intervals remain finite and within the boosted execution ceiling");
    expect(rapid_fire::BurstMoves(1,1.f/100.f,true)==24&&
           rapid_fire::BurstMoves(6,std::numeric_limits<float>::quiet_NaN(),true)==24,
           "boost applies after the tick-rate minimum and retains the invalid-clock fallback");

    // Ring slots: C modulo is truncated, so negative command numbers wrap.
    expect(rapid_fire::Slot(0)==0 && rapid_fire::Slot(42)==42 &&
           rapid_fire::Slot(149)==149 && rapid_fire::Slot(150)==0 &&
           rapid_fire::Slot(151)==1,
           "positive command numbers wrap at the 150-slot ring");
    expect(rapid_fire::Slot(-1)==149 && rapid_fire::Slot(-150)==0 &&
           rapid_fire::Slot(-151)==149,
           "negative command numbers wrap without indexing outside the ring");

    rapid_fire::Ring ring;
    expect(ring.Get(42)==0,"a fresh ring has no corrections");
    ring.Set(42,5);
    expect(ring.Get(42)==5 && ring.Get(42+rapid_fire::RingSize)==0,
           "a correction is visible only to its full command identity");
    ring.Set(42,9);
    expect(ring.Get(42)==9,"setting a slot replaces the previous correction");
    ring.Set(42,-3);
    expect(ring.Get(42)==0,"negative corrections are clamped to zero");
    ring.Set(42,7);
    ring.Clear(42);
    expect(ring.Get(42)==0,"clearing a slot removes its correction");
    ring.Clear(42);
    expect(ring.Get(42)==0,"clearing an empty slot is idempotent");
    ring.Set(149,4);
    ring.Set(-1,6);
    expect(ring.Get(149)==4&&ring.Get(-1)==0,"negative command identities cannot replace or publish a correction");
    ring.Clear(-1);
    expect(ring.Get(149)==4,"clearing command -1 leaves the valid command in slot 149 intact");
    ring.Set(-151,3);
    expect(ring.Get(149)==4&&ring.Get(-151)==0,"wrapped negative commands leave valid correction history intact");
    ring.Set(7,-1);
    expect(ring.Get(7)==0,"a negative correction never survives a set");

    // Raze ApplyNetChannelMode conditions.
    expect(rapid_fire::StormWrite(true,1,0),
           "Storm writes with the key held, a positive factor and an empty previous slot");
    expect(!rapid_fire::StormWrite(false,1,0),
           "Storm does not write without the key");
    expect(!rapid_fire::StormWrite(true,0,0),
           "Storm does not write with factor zero");
    expect(!rapid_fire::StormWrite(true,1,1),
           "Storm skips commands whose predecessor carries a correction");
    expect(!rapid_fire::StormWrite(true,1,-1),
           "a negative predecessor correction also suppresses Storm");
    expect(rapid_fire::ChokedWrite(true,true),
           "Methot 2 writes when the key is held and the channel resolves");
    expect(!rapid_fire::ChokedWrite(false,true) && !rapid_fire::ChokedWrite(true,false),
           "Methot 2 requires both the key and the channel");
    expect(rapid_fire::FlushWrite(true) && !rapid_fire::FlushWrite(false),
           "Methot 3 flushes exactly while the key is held");

    // Pistol rapid: semi-autos only fire on a fresh press edge, so holding the
    // attack button must alternate press/release across generated commands.
    rapid_fire::PistolEdge edge;
    expect(edge.Next(true),"the first held command exposes a fresh press");
    expect(!edge.Next(true),"the next held command exposes a release");
    expect(edge.Next(true) && !edge.Next(true) && edge.Next(true),
           "holding attack alternates press/release without extra clicks");
    expect(!edge.Next(false),"a released command never exposes a press");
    expect(edge.Next(true),"releasing attack resets the alternation");
    expect(!edge.Next(false) && !edge.Next(false),"idle commands keep the state cleared");
    expect(edge.Next(true),"the next hold starts with a press again");

    // One frame with N=3: the base command arms, extras only clear their own
    // slots. RunCommand subtracts the base correction and nothing else.
    ring=rapid_fire::Ring{};
    const int base=100,extras=3;
    ring.Clear(base);
    expect(rapid_fire::StormWrite(true,1,ring.Get(base-1)),
           "the base command's predecessor slot is empty before arming");
    ring.Set(base,extras);
    int tickbase=1000,generated=0;
    for(int number=base;number<=base+extras;++number) {
        if(number>base) {
            ring.Clear(number);
            expect(ring.Get(number)==0,"an extra command clears its own slot instead of arming");
        }
        tickbase-=ring.Get(number);
        ++generated;
    }
    expect(generated==extras+1,"arming N produces one base command plus N extras");
    expect(tickbase==1000-extras,"only the base command's correction reaches RunCommand");
    expect(ring.Get(base)==extras && ring.Get(base+1)==0 && ring.Get(base+2)==0 && ring.Get(base+3)==0,
           "the correction stays on the armed command and extras carry none");
    expect(!rapid_fire::StormWrite(true,1,ring.Get(base)),
           "the first extra sees the base correction and skips the Storm write");
    expect(rapid_fire::StormWrite(true,1,ring.Get(base+1)),
           "the second extra's predecessor has been cleared and writes again");

    // Raze clears only the current slot on release. The earlier armed command
    // must still be replayable when the server has not consumed it yet.
    ring=rapid_fire::Ring{};int remaining=-1;
    expect(ring.Empty(),"a reset ring has no pending correction");
    expect(rapid_fire::PrepareCommand(ring,remaining,1000,8,true) && remaining==8 && ring.Get(1000)==8,
           "the base command arms its correction before prediction runs");
    expect(!ring.Empty() && ring.corrections==1,"one armed command tracks one occupied slot");
    remaining=7;
    expect(!rapid_fire::PrepareCommand(ring,remaining,1001,200,true) && remaining==7 && ring.Get(1000)==8,
           "an extra never replaces the armed count with the configured speed");
    remaining=-1;
    expect(!rapid_fire::PrepareCommand(ring,remaining,1002,8,false) && ring.Get(1000)==8,
           "releasing the key does not erase an earlier correction");
    expect(rapid_fire::PrepareCommand(ring,remaining,1003,200,true) && remaining==200 && ring.Get(1003)==200,
           "holding again may arm the full configured speed without an ACK delay");
    expect(ring.corrections==2,"independent armed commands keep independent occupied slots");
    ring.Clear(1000);ring.Clear(1000);
    expect(ring.corrections==1 && !ring.Empty(),"repeated clears cannot underflow the occupied-slot count");
    ring.Set(1003,4);
    expect(ring.corrections==1,"replacing a correction does not double-count its slot");
    remaining=0;
    rapid_fire::PrepareCommand(ring,remaining,1003+rapid_fire::RingSize,200,false);
    expect(ring.Get(1003)==4&&ring.corrections==1,"an ordinary wrapped command cannot discard an unacknowledged base correction");
    ring.Retire(1003);
    expect(ring.Empty(),"the retained correction is retired by its ACK");

    // The previous runtime used samples=0/final=false for intermediate rapid
    // extras. The binary repeats the original arguments, including final=true.
    for(int index=1;index<=8;++index) {
        const auto args=rapid_fire::ReplayArguments(.125f,true,true,index,8);
        expect(args.samples==.125f && args.final,"every rapidfire extra preserves samples and final=true");
    }
    const auto not_final=rapid_fire::ReplayArguments(.25f,false,true,3,8);
    expect(not_final.samples==.25f && !not_final.final,"rapidfire preserves final=false from the engine too");
    const auto speed_middle=rapid_fire::ReplayArguments(.125f,true,false,1,8);
    const auto speed_last=rapid_fire::ReplayArguments(.125f,true,false,8,8);
    expect(speed_middle.samples==0.f && !speed_middle.final && speed_last.samples==0.f && speed_last.final,
           "speedhack retains its existing input and last-move flush behavior");

    // Frame-scoped aim selection: every generated command of a burst reuses
    // the base command's selection, so the SetupBones + trace sweep runs once
    // per frame instead of once per extra command.
    expect(!rapid_fire::ReuseSelection(true,0,true,7,7),
           "the base command always performs the real target selection");
    expect(rapid_fire::ReuseSelection(true,1,true,7,7) &&
           rapid_fire::ReuseSelection(true,32,true,7,7) &&
           rapid_fire::ReuseSelection(true,200,true,7,7),
           "every extra of the burst reuses the frame selection");
    expect(!rapid_fire::ReuseSelection(true,33,true,6,7),
           "a selection from a previous frame is never reused");
    expect(!rapid_fire::ReuseSelection(true,1,false,7,7),
           "an unresolved selection is not reused");
    expect(!rapid_fire::ReuseSelection(false,1,true,7,7),
           "commands outside the burst never reuse the frame selection");

    ring={};ring.Set(1000,8);ring.Set(1005,3);
    ring.Retire(1000);
    expect(ring.Get(1000)==0&&ring.Get(1005)==3&&ring.corrections==1,"an ACK retires old corrections while retaining later unacknowledged commands");
    ring.Retire(999);ring.Set(1000,8);
    expect(ring.Get(1000)==0&&ring.Get(1005)==3,"stale ACKs and replayed acknowledged commands cannot revive retired shifts");
    ring.Retire(1005);
    expect(ring.Empty(),"release recovery completes on ACK rather than 150 normal commands later");
    ring={};ring.Set(1000,8);ring.Set(1150,3);ring.Clear(1000);ring.Retire(1000);ring.Set(1000,8);
    expect(ring.Get(1000)==0&&ring.Get(1150)==3,"old prediction callbacks cannot overwrite a newer command that reused their slot");
    expect(rapid_fire::LocalCorrection(ring,1005,&ring,&edge)==0,"remote player prediction never changes the local clock");
    ring.Set(1010,7);
    expect(rapid_fire::LocalCorrection(ring,1010,&ring,&ring)==7&&
           rapid_fire::LocalCorrection(ring,1010,&edge,&ring)==0&&rapid_fire::LocalCorrection(ring,1160,&ring,&ring)==0,
           "only the exact local player and command receive a stored correction");
    expect(rapid_fire::UnusedCorrection(14,3,true)==11&&rapid_fire::UnusedCorrection(14,14,true)==0&&
           rapid_fire::UnusedCorrection(14,3,false)==0,"early release returns only already-borrowed unused prediction ticks");
    expect(!rapid_fire::ManualPrediction(true,false,&ring)&&rapid_fire::ManualPrediction(true,true,&ring)&&
           !rapid_fire::ManualPrediction(false,true,&ring)&&!rapid_fire::ManualPrediction(true,true,nullptr),
           "manual prediction belongs only to an armed native burst with a local owner");
    int no_moves=-1;ring={};
    expect(!rapid_fire::PrepareCommand(ring,no_moves,1000,0,true)&&ring.Empty(),"zero extras cannot arm a correction");
    // Two-second holds, packet loss and three-frame ACK latency, followed by
    // release. A per-message or pending-ACK cap must not shrink the native
    // replay loop, including at factors above fourteen and above ring size.
    for(int method=0;method<3;++method)for(int factor:{6,40,200})for(bool boost:{false,true}) {
        struct Delivery {int frame,command;};std::vector<Delivery> deliveries;
        rapid_fire::Ring history;int number=1000,ack=999,ordinary{},after_release{},armed_frames{},empty_at=-1;
        bool full_speed=true,no_release_updates=true,history_intact=true;
        const int expected=rapid_fire::BurstMoves(factor,1.f/60.f,boost);
        int generated{};
        for(int frame=0;frame<130;++frame) {
            for(const auto& delivery:deliveries)if(delivery.frame==frame)ack=std::max(ack,delivery.command);
            history.Retire(ack);
            const bool held=frame<120;const int base_number=number++;
            const int available=rapid_fire::BurstMoves(factor,1.f/60.f,boost);int left=-1;
            const bool armed=rapid_fire::PrepareCommand(history,left,base_number,available,held);
            ++ordinary;++generated;
            if(armed) {
                ++armed_frames;
                const int replayed=movement_batch::Combined(0,left);
                full_speed&=replayed==expected;
                for(int extra=1;extra<=replayed;++extra) {
                    history.Clear(number);++number;++generated;
                    history_intact&=history.Get(base_number)==expected;
                }
            }
            if(!held){++after_release;no_release_updates&=!armed&&!rapid_fire::ManualPrediction(true,armed,&history);}
            if(frame%7!=0)deliveries.push_back({frame+3,number-1});
            if(!held&&history.Empty()&&empty_at<0)empty_at=frame;
        }
        expect(ordinary==130&&after_release==10&&armed_frames==120&&full_speed&&generated==130+120*expected,
               "every held frame executes the full factor through the production batch policy despite delayed ACKs and losses");
        expect(history_intact,"long boosted bursts keep their base correction through every wrapped extra");
        expect(no_release_updates&&empty_at>=120&&empty_at<=124,
               "full-speed two-second holds still retire correction history without manual prediction after release");
        int left=-1;
        expect(rapid_fire::PrepareCommand(history,left,number,expected,true)&&movement_batch::Combined(0,left)==expected,
               "pressing again immediately restores the full replay count");
    }
    for(int factor:{6,40,200,400,800})for(int executed:{0,3,factor/2,factor}) {
        rapid_fire::Ring history;int left=-1;
        rapid_fire::PrepareCommand(history,left,1000,factor,true);
        for(int number=1001;number<=1000+executed;++number)history.Clear(number);
        int tickbase=5000-factor+executed;
        tickbase+=rapid_fire::UnusedCorrection(factor,executed,true);
        history.Set(1000,executed);
        expect(tickbase==5000&&history.Get(1000)==executed,
               "release during a full-speed burst returns unused borrowed ticks and keeps only the executed correction");
        history.Retire(1000);
        expect(history.Empty(),"a canceled full-speed burst retires on its ACK");
    }
    ring={};ring.Set(1000,400);ring.Clear(1150);ring.Set(1300,0);
    expect(ring.Get(1000)==400&&ring.corrections==1,
           "neither a wrapped extra nor a zero correction evicts pending prediction history");
    ring.Set(1150,12);ring.Clear(1000);
    expect(ring.Get(1000)==0&&ring.Get(1150)==12&&ring.corrections==1,
           "a newer armed base replaces its occupied slot without aliasing or count leaks");
    rapid_fire::ReleaseRecovery recovery;
    expect(!recovery.Take(1000,0),"normal inactive input never resets prediction without a prior release");
    recovery.Begin(1400);
    expect(!recovery.Take(1399,0)&&!recovery.Take(1400,1)&&recovery.pending,
           "release recovery waits for confirmation and a drained native queue without changing live burst counts");
    expect(recovery.Take(1400,0)&&!recovery.Take(1401,0),"confirmed release resets native history exactly once");
    recovery.Begin(1500);recovery.Cancel();
    expect(!recovery.Take(1500,0),"holding again or changing owner cancels an older recovery");
    for(int extras:{12,167,200,334,400,800}) {
        const int end=1000+extras;recovery.Begin(end);
        int resets{};
        for(int frame=0;frame<120;++frame) {
            const int ack=frame<3?end-1:end+frame-3;
            resets+=recovery.Take(ack,frame<2?1:0)?1:0;
        }
        expect(resets==1,"observed long-burst factors recover once after ACK rather than resetting on every released frame");
    }
    std::printf("%u rapid fire checks, %u failed\n",checks,failed);
    return failed?1:0;
}
