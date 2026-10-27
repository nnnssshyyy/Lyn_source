#include "features.h"
#include "spread_logic.h"
#include "shot_clock.h"
#include <cstdio>
#include <cstring>
#include <limits>

namespace {
struct Weapon {int cookie{123};float spread{4.f},next{8.f};} object;
int seed=77,observed_seed{};
unsigned updates{},random_calls{};
bool invalid_random{},invalid_vertical{},outside_random{},boundary_random{};
game::Globals* observed_clock{};
bool clock_decay{};
float update_time{},update_frame_time{};
int update_tick{},update_seed{};
void __fastcall UpdateSpread(void* pointer,void*) {
    auto& weapon=*static_cast<Weapon*>(pointer);
    if(observed_clock) {
        update_time=observed_clock->time;update_frame_time=observed_clock->frame_time;
        update_tick=observed_clock->tick;update_seed=seed;
    }
    weapon.spread=clock_decay && observed_clock
        ? weapon.spread-5.f*observed_clock->frame_time*(weapon.spread-weapon.next) : weapon.next;
    ++updates;
}
float __cdecl SharedRandom(const char* name,float minimum,float maximum,int extra) {
    ++random_calls;observed_seed=seed;
    if(invalid_random || extra!=0 || minimum!=-maximum) return std::numeric_limits<float>::quiet_NaN();
    if(!std::strcmp(name,"CTerrorGun::FireBullet HorizSpread")) return maximum*.25f;
    if(!std::strcmp(name,"CTerrorGun::FireBullet VertSpread")) {
        if(invalid_vertical) return std::numeric_limits<float>::quiet_NaN();
        if(outside_random) return maximum+1.f;
        if(boundary_random) return minimum;
        return -maximum*.5f;
    }
    return std::numeric_limits<float>::quiet_NaN();
}
bool Near(float a,float b) {return std::isfinite(a) && std::abs(a-b)<.0001f;}
}
int main() {
    unsigned checks{},failures{};
    const auto expect=[&](bool pass,const char* name) {
        ++checks;if(!pass) {++failures;std::fprintf(stderr,"FAIL: %s\n",name);}
    };
    const char* classes[]{"CPumpShotgun","CShotgun_Chrome","CAutoShotgun","CShotgun_SPAS"};
    for(const auto* name:classes) {
        const int index=weapons::Find(name);
        expect(weapons::IsShotgun(index),"T1 and T2 shotguns resolve to the supported catalog family");
        features::Config config{};config.weapons[index].shotgun_no_spread=true;
        features::ApplyWeaponProfile(config,index);
        expect(config.no_spread && config.shotgun_no_spread && !config.aim && !config.weapons[index].combat,
               "per-shotgun correction activates without aimbot or combat overrides");
        config={};config.weapons[index].combat=true;config.weapons[index].no_spread=false;
        config.weapons[index].shotgun_no_spread=true;
        features::ApplyWeaponProfile(config,index);
        expect(config.no_spread && config.shotgun_no_spread,"combat override cannot suppress the independent shotgun control");
        config={};features::ApplyWeaponProfile(config,index);
        expect(!config.no_spread && !config.shotgun_no_spread,"new and legacy weapon profiles leave correction disabled");
    }
    for(int index:{-1,static_cast<int>(weapons::Count),weapons::Find("CPistol"),weapons::Find("CTerrorMeleeWeapon")}) {
        features::Config config{};
        for(auto& weapon:config.weapons) weapon.shotgun_no_spread=true;
        config.shotgun_no_spread=true;
        features::ApplyWeaponProfile(config,index);
        expect(!config.shotgun_no_spread && !config.no_spread,"unknown weapons pistols and melee never activate shotgun-only correction");
    }
    game::Api api{};
    api.shared_random=reinterpret_cast<void*>(&SharedRandom);api.prediction_seed=&seed;
    api.update_spread=reinterpret_cast<void*>(&UpdateSpread);api.spread_offset=offsetof(Weapon,spread);
    game::Fields fields{};fields.weapon_group=3;
    game::Entity weapon{};weapon.ptr=&object;weapon.fields=&fields;
    game::Command command{};command.number=17;command.tick=88;command.seed=123456;
    source::Vec3 offset{};
    expect(api.Spread(weapon,command,offset,true) && offset.x==2.f && offset.y==-4.f,
           "real API samples native updated spread rather than stale weapon spread");
    expect(updates==1 && object.spread==4.f && object.cookie==123,"temporary native spread update restores the original field");
    expect(observed_seed==command.seed && seed==77,"native random samples use the shot seed and restore global prediction seed");
    expect(command.number==17 && command.tick==88 && command.seed==123456,"command sequence tick and seed remain unchanged");
    object.next=0.f;const auto before=random_calls;
    expect(api.Spread(weapon,command,offset,true) && offset.x==0.f && offset.y==0.f && random_calls==before,
           "zero spread succeeds without touching the RNG");
    object.next=std::numeric_limits<float>::quiet_NaN();
    expect(!api.Spread(weapon,command,offset,true) && object.spread==4.f && seed==77,
           "invalid native spread fails without leaking temporary state");
    object.next=8.f;invalid_random=true;
    expect(!api.Spread(weapon,command,offset,true) && object.spread==4.f && seed==77,
           "invalid RNG output fails with spread and seed restored");
    invalid_random=false;invalid_vertical=true;offset={11.f,12.f,13.f};
    expect(!api.Spread(weapon,command,offset,true) && offset.x==0.f && offset.y==0.f && offset.z==0.f &&
           object.spread==4.f && seed==77,
           "second-axis failure publishes no partial correction and restores weapon and seed");
    invalid_vertical=false;outside_random=true;
    expect(!api.Spread(weapon,command,offset,true) && offset.x==0.f && offset.y==0.f &&
           object.spread==4.f && seed==77,
           "finite RNG output outside the requested spread range is rejected atomically");
    outside_random=false;boundary_random=true;
    expect(api.Spread(weapon,command,offset,true) && offset.y==-8.f && seed==77,
           "an RNG sample exactly on the requested range boundary remains valid");
    boundary_random=false;
    const auto invalid_command_updates=updates;
    command.seed=-1;
    expect(!api.Spread(weapon,command,offset,true) && updates==invalid_command_updates && seed==77,
           "direct sampling rejects unassigned seeds before invoking the updater");
    command.seed=123456;command.number=0;
    expect(!api.Spread(weapon,command,offset,true) && updates==invalid_command_updates && seed==77,
           "direct sampling rejects unassigned command numbers before invoking the updater");
    command.number=17;api.update_spread=nullptr;
    expect(!api.Spread(weapon,command,offset,true),"missing native updater fails instead of claiming correction");
    api.update_spread=reinterpret_cast<void*>(&UpdateSpread);fields.weapon_group=0;
    const auto previous_updates=updates;
    expect(api.Spread(weapon,command,offset,true) && updates==previous_updates+1,"native gun spread updater supports pistols as well as shotguns");
    expect(api.Spread(weapon,command,offset) && offset.x==1.f && offset.y==-2.f,
           "existing non-shotgun spread correction keeps its original sampling path");

    // The production updater interpolates spread with frame_time. Test a
    // changing spread value, not a constant mock that hides render FPS drift.
    fields.weapon_group=3;clock_decay=true;object.next=2.f;
    struct Player {int cookie{321};int tickbase{300};} player;
    game::Fields local_fields{};local_fields.tickbase=offsetof(Player,tickbase);
    game::Entity local{};local.ptr=&player;local.fields=&local_fields;
    game::Globals clock{};clock.time=123.f;clock.frame_time=1.f/144.f;
    clock.tick=999;clock.interval=1.f/30.f;clock.max_clients=32;
    api.globals=&clock;observed_clock=&clock;
    for(int group:{0,1,2,3,4,5}) {
        fields.weapon_group=group;
        expect(api.SpreadAtTick(local,weapon,command,offset) && Near(offset.x,11.f/12.f) &&
               Near(offset.y,-11.f/6.f) && seed==77 && object.spread==4.f,
               "every hitscan weapon group samples current shot spread and restores native state");
    }
    for(const auto& definition:weapons::Catalog) {
        fields.weapon_group=definition.group;
        const auto prior_updates=updates;
        const bool gun=definition.group>=0;
        const bool sampled=api.SpreadAtTick(local,weapon,command,offset);
        expect(sampled==gun && updates==prior_updates+(gun?1u:0u) && object.spread==4.f && seed==77 &&
               (gun?(Near(offset.x,11.f/12.f) && Near(offset.y,-11.f/6.f)):(offset.x==0.f && offset.y==0.f)),
               definition.name);
    }
    for(int group:{6,99}) {
        fields.weapon_group=group;const auto prior_updates=updates;
        expect(!api.SpreadAtTick(local,weapon,command,offset) && updates==prior_updates && seed==77 &&
               !api.Spread(weapon,command,offset,true) && updates==prior_updates,
               "unrecognized weapon families cannot enter the native gun updater");
    }
    fields.weapon_group=-1;const auto unsupported_updates=updates;
    expect(!api.SpreadAtTick(local,weapon,command,offset) && updates==unsupported_updates,
           "non-hitscan weapon cannot invoke gun spread sampling");
    fields.weapon_group=3;
    expect(api.SpreadAtTick(local,weapon,command,offset) && Near(offset.x,11.f/12.f) && Near(offset.y,-11.f/6.f),
           "shot updater uses simulation interval for spread decay instead of render frame duration");
    expect(Near(update_time,10.f) && update_tick==300 && update_frame_time==clock.interval && update_seed==command.seed,
           "clock tickbase and command seed are prepared before the native updater runs");
    expect(clock.time==123.f && clock.tick==999 && clock.frame_time==1.f/144.f && seed==77 && object.spread==4.f,
           "successful sample restores clock prediction seed and weapon spread");
    expect(command.number==17 && command.tick==88 && command.seed==123456 && player.tickbase==300 && player.cookie==321,
           "historical target tick is not used as the shot clock and no command or player field is rewritten");
    for(float fps:{30.f,60.f,144.f,360.f}) {
        clock.frame_time=1.f/fps;
        expect(api.SpreadAtTick(local,weapon,command,offset) && Near(offset.x,11.f/12.f) &&
               clock.frame_time==1.f/fps && seed==77 && object.spread==4.f,
               "identical shot state produces the same correction at different render FPS and restores each frame time");
    }
    object.next=std::numeric_limits<float>::quiet_NaN();
    expect(!api.SpreadAtTick(local,weapon,command,offset) && clock.time==123.f && clock.tick==999 &&
           clock.frame_time==1.f/360.f && seed==77 && object.spread==4.f && offset.x==0.f && offset.y==0.f,
           "invalid updater output rolls back the complete temporary shot context");
    object.next=2.f;invalid_random=true;
    expect(!api.SpreadAtTick(local,weapon,command,offset) && clock.time==123.f && seed==77 && object.spread==4.f,
           "RNG failure also restores the prepared clock and outer seed");
    invalid_random=false;const auto prepared_updates=updates;
    player.tickbase=0;
    expect(!api.SpreadAtTick(local,weapon,command,offset) && updates==prepared_updates && seed==77,
           "invalid tickbase is rejected before touching the updater or RNG");
    player.tickbase=300;local_fields.tickbase=-1;
    expect(!api.SpreadAtTick(local,weapon,command,offset),"missing player tickbase storage disables prepared sampling");
    local_fields.tickbase=offsetof(Player,tickbase);command.seed=-1;
    expect(!api.SpreadAtTick(local,weapon,command,offset) && seed==77,"unassigned command seed cannot enter the context");
    command.seed=123456;clock.frame_time=0.f;
    expect(!api.SpreadAtTick(local,weapon,command,offset) && updates==prepared_updates,
           "paused clock does not generate or cache a compensated shot");
    clock.frame_time=1.f/144.f;clock.interval=std::numeric_limits<float>::quiet_NaN();
    expect(!api.SpreadAtTick(local,weapon,command,offset) && seed==77,"invalid tick interval leaves prediction state untouched");
    clock.interval=1.f/30.f;api.globals=nullptr;
    expect(!api.SpreadAtTick(local,weapon,command,offset),"missing game clock fails without fallback to a render-dependent sample");
    api.globals=&clock;
    {
        game::ScopedShotClock scope;
        expect(scope.Begin(&clock,&seed,300,command.seed),"scoped clock can be prepared independently");
        expect(!scope.Begin(&clock,&seed,301,command.seed),"a live scope cannot overwrite its own restoration snapshot");
        clock.frame=42; // Unrelated state must not be overwritten by rollback.
    }
    expect(clock.time==123.f && clock.tick==999 && clock.frame_time==1.f/144.f && seed==77 && clock.frame==42,
           "scope destructor restores owned fields on early exit and preserves unrelated globals");
    auto* readonly_clock=static_cast<game::Globals*>(VirtualAlloc(nullptr,4096,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE));
    bool readonly_restored=false;
    if(readonly_clock) {
        *readonly_clock=clock;DWORD previous{};
        if(VirtualProtect(readonly_clock,4096,PAGE_READONLY,&previous)) {
            api.globals=readonly_clock;
            readonly_restored=!api.SpreadAtTick(local,weapon,command,offset) && seed==77 && readonly_clock->time==123.f;
        }
        VirtualFree(readonly_clock,0,MEM_RELEASE);
    }
    expect(readonly_restored,"failed clock write rolls back the seed already written during partial context setup");
    api.globals=&clock;observed_clock=nullptr;clock_decay=false;

    const spread_logic::Identity identity{17,88,123456,0x1001,0x2002,300,1.f/30.f};
    const source::Vec3 input{12.f,179.f,0.f},spread{2.f,-4.f,0.f};
    source::Vec3 corrected{};
    expect(spread_logic::Correct(nullptr,identity,input,spread,corrected) && Near(corrected.x+spread.x,input.x) &&
           Near(std::remainder(corrected.y+spread.y-input.y,360.f),0.f),"full-strength correction reconstructs the requested shot axis including yaw wrap");
    const spread_logic::Sample sample{identity,spread,corrected,true};
    source::Vec3 repeated{};
    expect(spread_logic::Correct(&sample,identity,corrected,spread,repeated) && corrected.x==repeated.x && corrected.y==repeated.y,
           "evaluating an already corrected shot does not subtract spread twice");
    auto other=identity;++other.weapon;
    expect(!sample.Matches(other),"reused weapon index with another serial cannot reuse a sample");
    other=identity;++other.player;expect(!sample.Matches(other),"another player cannot reuse a sample");
    other=identity;++other.seed;expect(!sample.Matches(other),"another random seed cannot reuse a sample");
    other=identity;++other.tick;expect(!sample.Matches(other),"another historical tick cannot reuse a sample");
    other=identity;other.number+=150;expect(!sample.Matches(other),"ring slot reuse cannot inherit another command's correction");
    other=identity;++other.tickbase;expect(!sample.Matches(other),"tickbase correction invalidates an earlier cached sample");
    other=identity;other.interval=1.f/60.f;expect(!sample.Matches(other),"tick rate change invalidates an earlier cached sample");
    corrected={33.f,44.f,0.f};
    expect(!spread_logic::Correct(nullptr,identity,{-89.f,0.f,0.f},{1.f,0.f,0.f},corrected) && corrected.x==33.f && corrected.y==44.f,
           "unrepresentable pitch leaves the original shot untouched instead of clipping the correction");
    expect(!spread_logic::Correct(nullptr,identity,input,{std::numeric_limits<float>::quiet_NaN(),0.f,0.f},corrected),
           "nonfinite offsets cannot modify command angles");
    for(const auto* name:classes) {
        const int index=weapons::Find(name);
        for(float percent:{0.f,25.f,50.f,100.f}) {
            features::Config cfg{};cfg.weapons[index].shotgun_no_spread=true;cfg.weapons[index].shotgun_accuracy=percent;
            features::ApplyWeaponProfile(cfg,index);
            expect(Near(features::SpreadCorrectionStrength(cfg,true,true,false),percent*.01f),
                "each shotgun uses its own 0-100 intensity without enabling aimbot");
        }
        features::Config cfg{};cfg.no_spread=cfg.aim=true;
        features::ApplyWeaponProfile(cfg,index);
        expect(features::SpreadCorrectionStrength(cfg,true,true,true)==0.f,
            "disabled shotgun Accuracy cannot be bypassed by main aim or generic spread compensation");
        cfg.weapons[index].shotgun_no_spread=true;cfg.weapons[index].shotgun_accuracy=50.f;
        features::ApplyWeaponProfile(cfg,index);
        expect(Near(features::SpreadCorrectionStrength(cfg,true,false,true),.5f),
            "aimed shotgun commands obey the same selected intensity");
        expect(features::SpreadCorrectionStrength(cfg,true,false,false)==0.f,"idle commands do not receive accuracy correction");
    }
    features::Config intensity{};intensity.shotgun_no_spread=true;
    intensity.shotgun_accuracy=-20.f;expect(features::SpreadCorrectionStrength(intensity,true,true,false)==0.f,"negative intensity clamps to zero");
    intensity.shotgun_accuracy=200.f;expect(features::SpreadCorrectionStrength(intensity,true,true,false)==1.f,"intensity never exceeds 100 percent");
    intensity.shotgun_accuracy=std::numeric_limits<float>::quiet_NaN();
    expect(features::SpreadCorrectionStrength(intensity,true,true,false)==0.f,"invalid intensity fails closed");
    intensity.no_spread=true;
    expect(features::SpreadCorrectionStrength(intensity,false,true,false)==1.f,"non-shotgun correction retains full strength");
    source::Vec3 half{},zero{};
    expect(spread_logic::Correct(nullptr,identity,input,spread,zero,0.f) && Near(zero.x,input.x) && Near(zero.y,input.y),
        "zero percent preserves the shot axis");
    expect(spread_logic::Correct(nullptr,identity,input,spread,half,.5f) && Near(half.x,11.f) && Near(half.y,-179.f),
        "half strength applies exactly half of both offsets including yaw wrapping");
    const spread_logic::Sample half_sample{identity,spread,half,true,.5f};
    expect(spread_logic::Correct(&half_sample,identity,half,spread,repeated,.5f) && Near(repeated.x,half.x) && Near(repeated.y,half.y),
        "partial correction is idempotent during repeated prediction");
    expect(spread_logic::Correct(&half_sample,identity,half,spread,repeated,1.f) && Near(repeated.x,10.f) && Near(repeated.y,-177.f),
        "changing cached intensity reconstructs the original axis before applying the new amount");
    expect(spread_logic::Correct(&sample,identity,sample.result,spread,repeated,0.f) && Near(repeated.x,input.x) && Near(repeated.y,input.y),
        "zero intensity can restore an already corrected cached command");
    expect(!spread_logic::Correct(nullptr,identity,input,spread,repeated,1.1f),"out-of-range correction factor is rejected");
    std::printf("%u shotgun client spread checks, %u failed\n",checks,failures);
    return failures?1:0;
}
