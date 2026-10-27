#pragma once
#include <algorithm>
#include <array>
#include <cmath>

namespace movement_assists {
constexpr int FreezeTick=0xFFFFFF;
constexpr float CeilingPitch=12.f;
inline bool FastStrafe(bool bhop,bool fast,bool strafe) noexcept {return strafe || (bhop && fast);}
// Restore a command's own native tick before applying a hold again. Prediction
// can revisit an already edited command after the key or option was released.
class TickHistory {
    struct Entry {int number{},seed{},tick{};};
    std::array<Entry,150> entries_{};
public:
    void Reset() noexcept {entries_.fill({});}
    int Native(int number,int seed,int tick,int fallback) noexcept {
        if(number<=0) return tick;
        auto& e=entries_[static_cast<unsigned>(number)%entries_.size()];
        if(tick==FreezeTick) return e.number==number && e.seed==seed?e.tick:fallback;
        e={number,seed,tick};return tick;
    }
};
// Native input edges, independent of the OS mouse state. Replaying a command
// returns the same edge and never advances a shared timer twice.
class HunterClick {
    struct Entry {int number{},seed{};unsigned owner{};bool attack{};};
    std::array<Entry,150> entries_{};
    unsigned owner_{};int last_{},next_{};
public:
    void Reset() noexcept {*this={};}
    bool Run(unsigned owner,int number,int seed,float interval,bool held) noexcept {
        if(!held || !owner || number<=0 || !std::isfinite(interval) || interval<.001f || interval>.1f) {Reset();return false;}
        if(owner_!=owner || number<last_-149) Reset();
        auto& e=entries_[static_cast<unsigned>(number)%entries_.size()];
        if(e.owner==owner && e.number==number && e.seed==seed) return e.attack;
        owner_=owner;last_=std::max(last_,number);
        const bool attack=number>=next_;
        if(attack) next_=number+std::max(2,static_cast<int>(std::ceil(.1f/interval)));
        e={number,seed,owner,attack};return attack;
    }
};
struct MeleeInput {
    unsigned owner{},active{},melee{},primary{};
    int number{},melee_index{},primary_index{};
    float interval{};
    bool enabled{},held{},ready{},manual_select{};
    bool swing_known{},in_swing{},interrupted{};
    int seed{},selected_weapon{};
};
struct MeleeResult {int select{};bool suppress{},cycling{},swing{};};
class MeleeCycle {
    enum class Stage {Idle,Windup,Primary,Return};
    struct Entry {unsigned owner{},melee{},primary{};int number{},seed{};MeleeResult result{};};
    std::array<Entry,150> history_{};
    Stage stage_{};unsigned owner_{},melee_{},primary_{};
    int last_{},deadline_{},return_index_{};bool saw_swing_{};
public:
    void Reset() noexcept {*this={};}
    bool Active() const noexcept {return stage_!=Stage::Idle;}
    MeleeResult Run(const MeleeInput& i) noexcept {
        if(!i.enabled || !i.owner || i.number<=0 || i.number>0x3fffffff || i.interrupted ||
           !i.swing_known || !std::isfinite(i.interval) || i.interval<.001f || i.interval>.1f) {Reset();return {};}
        if(owner_ && owner_!=i.owner) Reset();
        if(stage_!=Stage::Idle && i.active!=i.melee && i.active!=i.primary) {Reset();return {};}
        auto& entry=history_[static_cast<unsigned>(i.number)%history_.size()];
        const bool cached=entry.owner==i.owner && entry.number==i.number && entry.seed==i.seed &&
            entry.melee==i.melee && entry.primary==i.primary;
        // Generated selections survive prediction replay; genuine new manual
        // selections and use/reload/shove still cancel immediately.
        const bool own_selection=cached && i.selected_weapon>0 && i.selected_weapon==entry.result.select;
        if(cached && (!i.manual_select || own_selection)) return entry.result;
        if(i.number<last_) return {};
        if(i.manual_select) {Reset();return {};}
        last_=i.number;owner_=i.owner;MeleeResult out{};
        if(stage_!=Stage::Idle && (i.melee!=melee_ || i.primary!=primary_ ||
           (i.active!=melee_ && i.active!=primary_))) {Reset();return {};}
        const auto ticks=[&](float seconds){return std::max(1,static_cast<int>(std::ceil(seconds/i.interval)));};
        if(stage_==Stage::Idle) {
            if(i.held && i.ready && !i.in_swing && i.active==i.melee && i.melee && i.primary && i.melee_index>0 && i.primary_index>0) {
                melee_=i.melee;primary_=i.primary;return_index_=i.melee_index;
                deadline_=i.number+ticks(.65f);saw_swing_=false;
                stage_=Stage::Windup;out.swing=true;
            }
        } else if(stage_==Stage::Windup) {
            if(!i.held) stage_=Stage::Idle;
            else {
                saw_swing_=saw_swing_ || i.in_swing;
                if(saw_swing_ && !i.in_swing) {
                    out.select=i.primary_index;stage_=Stage::Primary;deadline_=i.number+ticks(1.5f);
                } else if(!saw_swing_ && i.number>=deadline_) stage_=Stage::Idle;
                out.suppress=stage_!=Stage::Idle;
            }
        } else if(stage_==Stage::Primary) {
            out.suppress=true;
            if(i.active==primary_ || i.number>=deadline_) {out.select=return_index_;stage_=Stage::Return;}
            else out.select=i.primary_index;
        } else {
            out.suppress=true;
            if(i.active==melee_) stage_=Stage::Idle;
            else out.select=return_index_; // Keep the owned return until acknowledged or interrupted.
        }
        out.cycling=stage_!=Stage::Idle;entry={i.owner,i.melee,i.primary,i.number,i.seed,out};return out;
    }
};
}
