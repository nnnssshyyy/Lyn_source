#pragma once
#include <algorithm>
#include <cmath>

// Raze rapidfire policy (docs/RAZE_RAPIDFIRE_ANALYSIS.md):
// - Holding the key arms N extra CL_Move calls for the current frame.
// - N = max(configured_speed, int(0.06f / interval_per_tick + 0.5f)), N >= 0.
// - The correction N is stored per command in a 150-slot ring indexed by
//   command_number % 150; RunCommand subtracts it from m_nTickBase.
// - Arm before Prediction::Update, and replay the original CL_Move arguments.
// - Every method declares its own network channel state and forces the send.
// The old experimental policy (idle credit, one packet per burst) is gone.
namespace rapid_fire {
inline constexpr int RingSize=150;
inline constexpr int SpeedMinimum=1,SpeedMaximum=200;
inline constexpr int StormTicksMinimum=1,StormTicksMaximum=100;
inline constexpr int ChokeMinimum=1,ChokeMaximum=255;
// The original factor remains 1..200. Boost quadruples the generated count,
// independently of the method, while keeping an explicit execution ceiling.
inline constexpr int BoostMultiplier=4;
inline constexpr int MaxExtraMoves=SpeedMaximum*BoostMultiplier;
// Recovery is separate from arming. Holding again cancels an earlier release;
// only an inactive, fully acknowledged burst with no real send queue may reset
// native prediction history. A failed/unavailable reset is attempted once.
struct ReleaseRecovery {
    int last{};
    bool pending{};
    void Begin(int command) noexcept {last=command;pending=command>0;}
    void Cancel() noexcept {last=0;pending=false;}
    bool Take(int acknowledged,int queued) noexcept {
        if(!pending||queued!=0||acknowledged<last)return false;
        pending=false;return true;
    }
};
inline int UnusedCorrection(int planned,int generated,bool corrected) noexcept {
    planned=std::max(planned,0);
    return corrected?planned-std::clamp(generated,0,planned):0;
}

// Menu labels these Methot 1..3; the internal indexes are 0..2.
enum class Method : int { Storm=0,Choked=1,Flush=2 };
inline Method FromIndex(int index) noexcept {
    return index<=0?Method::Storm:index==1?Method::Choked:Method::Flush;
}

// Raze arm path: `N = max(configured, int(0.06f/interval + 0.5f)); N = max(N,0)`.
// The reference load follows the server tick rate; the configured factor can
// only raise it. An unreadable interval falls back to the factor alone.
inline int ExtraMoves(int factor_speed,float interval_per_tick) noexcept {
    const int configured=std::clamp(factor_speed,0,SpeedMaximum);
    if(!std::isfinite(interval_per_tick) || interval_per_tick<=0.f) return configured;
    const double tick_moves=std::clamp(0.06/static_cast<double>(interval_per_tick)+0.5,0.,static_cast<double>(SpeedMaximum));
    return std::max(configured,static_cast<int>(tick_moves));
}
inline int BurstMoves(int factor_speed,float interval_per_tick,bool boost) noexcept {
    return ExtraMoves(factor_speed,interval_per_tick)*(boost?BoostMultiplier:1);
}

// Command numbers are positive; negative inputs still map to a valid slot.
inline int Slot(int command_number) noexcept {
    const int slot=command_number%RingSize;
    return slot<0?slot+RingSize:slot;
}

// Per-command corrections (Raze 0x103F690C). Written by the command callback,
// read by the RunCommand hook, inspected by the Storm channel variant.
struct Ring {
    int shift[RingSize]{};
    int command[RingSize]{};
    unsigned corrections{};
    int acknowledged{};
    void Clear(int command_number) noexcept {
        if(command_number<=0)return;
        const int index=Slot(command_number);
        // An extra has no correction of its own. Clearing its modulo slot
        // must not erase an earlier base that prediction can still replay.
        if(command[index]!=command_number)return;
        auto& value=shift[index];
        if(value>0) --corrections;
        value=0;command[Slot(command_number)]=0;
    }
    void Set(int command_number,int correction) noexcept {
        if(command_number<=acknowledged||command[Slot(command_number)]>command_number)return;
        if(correction<=0){Clear(command_number);return;}
        const int index=Slot(command_number);
        if(shift[index]>0)--corrections;
        command[index]=command_number;
        shift[index]=correction;
        ++corrections;
    }
    int Get(int command_number) const noexcept {
        const int index=Slot(command_number);
        return command[index]==command_number&&command_number>acknowledged?shift[index]:0;
    }
    void Retire(int ack) noexcept {
        if(ack<=acknowledged)return;
        acknowledged=ack;
        for(int i=0;i<RingSize;++i)if(shift[i]>0&&command[i]<=ack) {
            shift[i]=command[i]=0;--corrections;
        }
    }
    bool Empty() const noexcept {return corrections==0;}
};
inline int LocalCorrection(const Ring& ring,int command,const void* player,const void* local) noexcept {
    return local&&player==local&&command>0?ring.Get(command):0;
}
inline bool ManualPrediction(bool inside_move,bool armed,const void* local) noexcept {
    return inside_move&&armed&&local;
}

// Raze 0x1016FF8E -> 0x1016FFC8: clear the current slot first; only a base
// move (remaining == -1) may arm. Call Prediction::Update AFTER this helper.
inline bool PrepareCommand(Ring& ring,int& remaining,int command_number,int extras,bool ready) noexcept {
    if(command_number<=ring.acknowledged)return false;
    ring.Clear(command_number);
    if(!ready || remaining!=-1 || extras<=0) return false;
    remaining=std::clamp(extras,0,MaxExtraMoves);
    ring.Set(command_number,remaining);
    return true;
}

struct MoveArguments {float samples;bool final;};
// Raze 0x1016CE90-0x1016CE9F reuses BOTH native arguments on every rapidfire
// move. Speedhack keeps its separate one-input-sample / final-flush policy.
inline MoveArguments ReplayArguments(float samples,bool final,bool rapid,int index,int extras) noexcept {
    return rapid?MoveArguments{samples,final}:MoveArguments{0.f,final && index==extras};
}

// The burst replays one input frame inside a single engine tick, so the
// replicated world cannot change between the base command and its extras.
// The target selection is computed once per frame and reused by every
// generated command. A reuse window capped at 32 commands forced a fresh
// SetupBones + trace sweep for each extra past it, which is what collapsed
// the frame rate when the aimbot and the rapidfire ran at the same time.
inline bool ReuseSelection(bool inside_tick_move,int extra_index,bool valid,
                            unsigned selection_generation,unsigned frame_generation) noexcept {
    return inside_tick_move && extra_index>0 && valid && selection_generation==frame_generation;
}

// Storm writes only while the previous command carries no correction: the two
// sequences share one datagram state. Unreachable equality branch omitted
// (current == previous is impossible for positive command numbers).
inline bool StormWrite(bool key_held,int storm_ticks,int previous_correction) noexcept {
    return key_held && storm_ticks>StormTicksMinimum-1 && previous_correction==0;
}
// Methot 2 writes its choke declaration whenever the channel resolves.
inline bool ChokedWrite(bool key_held,bool channel_available) noexcept {
    return key_held && channel_available;
}
// Methot 3 and Storm declare the same outgoing ACK / retained-packet pair.
inline bool FlushWrite(bool key_held) noexcept {return key_held;}

// Pistol rapid: semi-auto weapons (pistols, dual pistols, magnum) only fire on
// a fresh press edge. While the player holds attack, expose IN_ATTACK on
// alternating commands so the weapon re-triggers without repeated clicks; the
// rapidfire extras then advance the weapon clock for every generated command.
struct PistolEdge {
    bool shown{};
    // Returns the attack state to expose for one command. A released command
    // resets the alternation so the next hold always starts with a press.
    bool Next(bool held) noexcept {
        if(!held) {shown=false;return false;}
        const bool press=!shown;
        shown=press;
        return press;
    }
};
}
