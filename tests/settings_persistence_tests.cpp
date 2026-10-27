#include "velocity_ui/features_ui.hpp"
#include "velocity_ui/client_guard_ui.hpp"
#include <cstdio>
#include <unordered_set>

// Only input registration is stubbed; the production settings and JSON
// registry run unchanged without creating a DirectX window.
namespace xui::binds {
std::vector<setting*>& Registry() {static std::vector<setting*> entries;return entries;}
void register_setting(setting* value) {Registry().push_back(value);}
std::span<setting* const> all() {return Registry();}
}
namespace {
void Defaults() {
#define F_BOOL(name,initial,category,label) feature_ui::settings.name.value=initial;
#define F_INT(name,initial,category,label) feature_ui::settings.name.value=initial;
#define F_FLOAT(name,initial,category,label) feature_ui::settings.name.value=initial;
#define F_COLOR(name,r,g,b,a,category,label) feature_ui::settings.name.value=xdraw::color{r,g,b,a};
#include "features_fields.inc"
#undef F_BOOL
#undef F_INT
#undef F_FLOAT
#undef F_COLOR
    for(auto& p:feature_ui::weapon_settings.items) p.Reset();
}
}
int main() {
    unsigned checks{},failures{};
    const auto expect=[&](bool ok,const char* name) {++checks;if(!ok) {++failures;std::fprintf(stderr,"FAIL: %s\n",name);}};
    config::initialize(&Defaults);
    const auto defaults=config::to_json();
    expect(!feature_ui::settings.box_precision.value,"box precision starts disabled by default");
    std::unordered_set<std::uint32_t> keys;
    for(const auto& field:config::detail::get_registry().fields) keys.insert(field.key);
    expect(keys.size()==config::detail::get_registry().fields.size(),"all global and per-weapon persistence keys are unique");
    for(size_t i=0;i<weapons::Count;++i) {
        auto& p=feature_ui::weapon_settings.items[i];
        p.combat.value=i%2==0;p.view.value=i%3==0;p.fov=1.f+static_cast<float>(i);p.vm_up=static_cast<float>(i)*.5f;
        p.shotgun_no_spread.value=weapons::IsShotgun(static_cast<int>(i)) && i%2==0;
    }
    auto& settings=feature_ui::settings;
    settings.box_precision.value=true;
    settings.name_position=1;settings.distance_position=0;
    settings.prioritize_classes.value=true;
    settings.priority_smoker.value=settings.priority_hunter.value=settings.priority_jockey.value=settings.priority_tank.value=true;
    settings.priority_boomer.value=settings.priority_spitter.value=settings.priority_charger.value=false;
    settings.charger_turn.value=true;
    settings.auto_ceiling.value=settings.hunter_click.value=settings.fast_melee.value=settings.no_fall_damage.value=settings.no_infected_vision.value=true;
    settings.lag_method=1;settings.fall_lag_method=1;settings.lag_amount=130;settings.lag_amount_v2=270;
    settings.ceiling_key=0x46;settings.hunter_click_key=0x47;settings.bhop_fast.value=false;
    settings.radar.value=true;settings.radar_x=.42f;settings.radar_range=125.f;
    settings.spectator_chase.value=false;settings.spectator_modes.value=false;settings.spectator_hide_empty.value=true;
    settings.chat_spy.value=true;settings.chat_panel.value=false;settings.chat_hud.value=true;
    settings.chat_deduplicate.value=false;settings.chat_clear_minutes=7;settings.chat_x=.6f;settings.chat_y=.25f;
    settings.rapidfire.value=true;settings.rapidfire_method.value=1;settings.rapidfire_boost.value=false;
    settings.rapidfire_storm_speed.value=9;settings.rapidfire_storm_ticks.value=12;
    settings.rapidfire_m1_speed.value=7;settings.rapidfire_m1_choke.value=42;
    settings.rapidfire_m2_speed.value=8;
    settings.hitmarker.value=settings.damage_numbers.value=settings.offscreen.value=settings.boss_hud.value=settings.trajectory.value=true;
    settings.death_particles.value=true;settings.death_particle_count=18;settings.death_particle_lifetime=1.4f;
    settings.hud_editor.value=true;settings.boss_x=.31f;settings.boss_y=.44f;settings.boss_scale=1.3f;settings.boss_width=410.f;
    settings.hud_opacity=.65f;settings.chat_width=550.f;settings.chat_height=420.f;settings.spectator_width=350.f;settings.spectator_rows=6;
    settings.no_fog.value=settings.no_vomit.value=settings.capture_privacy.value=true;
    settings.capture_method=1;
    const auto saved=config::to_json();
    expect(config::from_json(defaults),"default profile loads");
    expect(!settings.box_precision.value,"restoring defaults disables box precision");
    bool inherited=true;
    for(const auto& p:feature_ui::weapon_settings.items) inherited=inherited && !p.combat.value && !p.view.value && p.fov.value==180.f;
    expect(inherited,"old profiles start with global inheritance for every weapon");
    expect(config::from_json(saved),"custom profile loads");
    expect(settings.box_precision.value,"explicit box precision preference survives a profile reload");
    auto legacy_profile=saved;
    char precision_key[12]{};
    std::snprintf(precision_key,sizeof(precision_key),"%08x",config::detail::make_key("l4d2 players","box precision"));
    legacy_profile["fields"].erase(precision_key);
    expect(config::from_json(legacy_profile)&&!settings.box_precision.value,
           "profiles without the new option disable box precision instead of retaining a previous enabled value");
    expect(settings.death_particles.value&&settings.death_particle_count.value==18&&settings.death_particle_lifetime.value==1.4f,
           "death particle controls survive a profile reload");
    expect(settings.name_position.value==1&&settings.distance_position.value==0,
           "independent name and distance slots survive profile reload");
    expect(settings.prioritize_classes.value,"exclusive class selection survives profile reload");
    expect(settings.priority_smoker.value&&settings.priority_hunter.value&&settings.priority_jockey.value&&settings.priority_tank.value&&
        !settings.priority_boomer.value&&!settings.priority_spitter.value&&!settings.priority_charger.value,
        "selected and unselected special-infected priorities survive profile reload independently");
    expect(settings.no_fog.value&&settings.no_vomit.value&&settings.capture_privacy.value,
           "scene removals and private overlay preferences survive profile reload");
    expect(settings.capture_method.value==1,"the explicit lightweight capture method survives profile reload");
    expect(!keys.contains(config::detail::make_key("l4d2 players","avoid label overlap")),"the removed overlap switch is no longer registered in the profile schema");
    expect(settings.hitmarker.value&&settings.damage_numbers.value&&settings.offscreen.value&&settings.boss_hud.value&&settings.trajectory.value,
           "all new visual indicators survive profile reload");
    expect(settings.boss_x.value==.31f&&settings.boss_y.value==.44f&&settings.boss_width.value==410.f&&settings.boss_scale.value==1.3f&&
           settings.hud_opacity.value==.65f&&settings.chat_width.value==550.f&&settings.chat_height.value==420.f&&settings.spectator_width.value==350.f&&settings.spectator_rows.value==6,
           "HUD positions, sizes, scale and opacity round-trip together");
    bool independent=true;
    for(size_t i=0;i<weapons::Count;++i) {
        const auto& p=feature_ui::weapon_settings.items[i];
        independent=independent && p.combat.value==(i%2==0) && p.view.value==(i%3==0) &&
            p.fov.value==1.f+static_cast<float>(i) && p.vm_up.value==static_cast<float>(i)*.5f;
    }
    expect(independent,"all weapon profiles round-trip independently");
    bool shotgun_profiles=true;
    for(size_t i=0;i<weapons::Count;++i)
        shotgun_profiles=shotgun_profiles && feature_ui::weapon_settings.items[i].shotgun_no_spread.value==
            (weapons::IsShotgun(static_cast<int>(i)) && i%2==0);
    expect(shotgun_profiles,"shotgun client compensation round-trips independently for T1 and T2 profiles");
    expect(settings.radar.value && settings.radar_x.value==.42f && settings.radar_range.value==125.f,
           "radar position, enabled state and range survive a profile reload");
    expect(settings.charger_turn.value,"Charger turn survives a profile reload");
    expect(settings.lag_method.value==1 && settings.fall_lag_method.value==1 && settings.lag_amount.value==130 && settings.lag_amount_v2.value==270,
        "manual and automatic lag methods and both independent factors survive profile reload");
    expect(settings.auto_ceiling.value && settings.hunter_click.value && settings.fast_melee.value && settings.no_fall_damage.value && settings.no_infected_vision.value &&
        settings.ceiling_key.value==0x46 && settings.hunter_click_key.value==0x47 && !settings.bhop_fast.value,
        "ceiling and Hunter click binds remain separate and all movement options round-trip");
    expect(!settings.spectator_chase.value && !settings.spectator_modes.value && settings.spectator_hide_empty.value,
           "spectator filters and presentation survive a profile reload");
    expect(settings.chat_spy.value && !settings.chat_panel.value && settings.chat_hud.value &&
           !settings.chat_deduplicate.value && settings.chat_clear_minutes.value==7 &&
           settings.chat_x.value==.6f && settings.chat_y.value==.25f,
           "chat outputs, duplicate filter, timed clear and position survive a profile reload");
    expect(settings.rapidfire.value && settings.rapidfire_method.value==1 &&
           settings.rapidfire_storm_speed.value==9 && settings.rapidfire_storm_ticks.value==12 &&
           settings.rapidfire_m1_speed.value==7 && settings.rapidfire_m1_choke.value==42 &&
           settings.rapidfire_m2_speed.value==8,
           "rapidfire method and its three factor sets survive a profile reload");
    expect(!settings.rapidfire_boost.value,"an explicitly disabled rapidfire boost survives profile reload");
    expect(!keys.contains(config::detail::make_key("l4d2 exploit","rapidfire stable network")),
           "the removed grouped transport cannot be reactivated by a saved profile");
    settings.prioritize_classes.value=false;
    const auto disabled_classes=config::to_json();
    settings.prioritize_classes.value=true;
    expect(config::from_json(disabled_classes) && !settings.prioritize_classes.value &&
           settings.priority_smoker.value && settings.priority_hunter.value && !settings.priority_charger.value,
           "disabling class selection round-trips without erasing the saved class marks");
    config::from_json(defaults);
    const nlohmann::json legacy{{"version",2},{"fields",nlohmann::json::object()}};
    expect(config::from_json(legacy) && !feature_ui::weapon_settings.items[0].combat.value,
           "profiles without new fields keep weapon inheritance");
    expect(settings.name_position.value==2&&settings.distance_position.value==3,
           "legacy profiles keep the name above and distance below the box");
    expect(!settings.priority_smoker.value&&!settings.priority_boomer.value&&!settings.priority_hunter.value&&
        !settings.priority_spitter.value&&!settings.priority_jockey.value&&!settings.priority_charger.value&&!settings.priority_tank.value,
        "legacy profiles preserve the original aim ordering with no implicit preferred special classes");
    expect(!settings.prioritize_classes.value,"legacy profiles do not silently enable exclusive class selection");
    expect(settings.capture_method.value==0,"legacy profiles never silently switch from display privacy to game capture");
    expect(!settings.hitmarker.value&&!settings.damage_numbers.value&&!settings.offscreen.value&&!settings.boss_hud.value&&!settings.trajectory.value&&
           settings.hud_opacity.value==1.f&&settings.health_split.value&&!settings.no_fog.value&&!settings.no_vomit.value&&!settings.capture_privacy.value&&!settings.charger_turn.value,
           "legacy profiles keep extra effects off and legibility defaults enabled");
    expect(!settings.auto_ceiling.value && !settings.hunter_click.value && !settings.fast_melee.value && !settings.no_fall_damage.value && !settings.no_infected_vision.value && settings.bhop_fast.value,
        "old profiles leave new automation disabled and keep fast bunnyhop ready for the explicit bunnyhop switch");
    expect(settings.lag_method.value==0 && settings.fall_lag_method.value==0 && settings.lag_amount.value==100 && settings.lag_amount_v2.value==100,
        "legacy profiles choose Raze V1 and retain explicit default factors without enabling lag");
    bool shotgun_defaults=true;
    for(const auto& p:feature_ui::weapon_settings.items) shotgun_defaults=shotgun_defaults && !p.shotgun_no_spread.value;
    expect(shotgun_defaults,"legacy profiles leave independent shotgun compensation disabled");
    expect(!settings.chat_spy.value && settings.chat_panel.value && !settings.chat_hud.value &&
           settings.chat_deduplicate.value && settings.chat_clear_minutes.value==0,
           "legacy profiles keep chat disabled and use safe presentation defaults");
    expect(!settings.rapidfire.value && settings.rapidfire_method.value==0 &&
           settings.rapidfire_storm_speed.value==6 && settings.rapidfire_storm_ticks.value==1 &&
           settings.rapidfire_m1_speed.value==6 && settings.rapidfire_m1_choke.value==15 &&
           settings.rapidfire_m2_speed.value==6,
           "profiles without the new rapidfire fields start with the recovered defaults");
    expect(settings.rapidfire_boost.value,"existing profiles keep boost enabled without enabling rapidfire itself");
    expect(settings.infected_aim.value,"infected aim is available under the main aim switch by default");
    settings.infected_aim.value=false;const auto infected_disabled=config::to_json();
    settings.infected_aim.value=true;
    expect(config::from_json(infected_disabled) && !settings.infected_aim.value,"infected aim opt-out persists per profile");
    config::from_json(defaults);
    expect(settings.infected_aim_key.value==VK_XBUTTON2,"infected hold defaults to Mouse5");
    settings.infected_aim_key=0x46;settings.aim_key=0x47;settings.activation=2;
    const auto own_infected_key=config::to_json();settings.infected_aim_key=0x48;
    expect(config::from_json(own_infected_key) && settings.infected_aim_key.value==0x46 && settings.aim_key.value==0x47 && settings.activation.value==2,
        "infected hold key persists independently of survivor key and activation mode");
    expect(config::from_json(legacy) && settings.infected_aim_key.value==VK_XBUTTON2,
        "legacy profiles use Mouse5 rather than inheriting another profile's infected key");
    config::from_json(defaults);
    const char* shotgun_classes[]{"CPumpShotgun","CShotgun_Chrome","CAutoShotgun","CShotgun_SPAS"};
    for(int i=0;i<4;++i) {
        auto& profile=feature_ui::weapon_settings.items[weapons::Find(shotgun_classes[i])];
        expect(profile.shotgun_accuracy.value==100.f,"new shotgun intensity defaults to the previous full correction");
        profile.shotgun_accuracy=25.f*i;profile.shotgun_no_spread.value=true;
        profile.shotgun_no_spread.bind.key=0x46+i;profile.shotgun_no_spread.bind.mode=static_cast<xui::bind_mode>(i%3);
    }
    const auto accuracy_saved=config::to_json();
    for(auto& profile:feature_ui::weapon_settings.items) {profile.shotgun_accuracy=13.f;profile.shotgun_no_spread.bind={};}
    config::from_json(accuracy_saved);
    auto accuracy_legacy=accuracy_saved;
    for(int i=0;i<4;++i) {
        const int index=weapons::Find(shotgun_classes[i]);const auto& profile=feature_ui::weapon_settings.items[index];
        expect(profile.shotgun_accuracy.value==25.f*i && profile.shotgun_no_spread.value &&
            profile.shotgun_no_spread.bind.key==0x46+i && profile.shotgun_no_spread.bind.mode==static_cast<xui::bind_mode>(i%3),
            "each shotgun retains its independent intensity, binding and bind mode");
        char key[12]{};std::snprintf(key,sizeof(key),"%08x",config::detail::make_key(
            std::string("l4d2 weapons ")+weapons::Catalog[index].key,"shotgun accuracy percent"));
        accuracy_legacy["fields"].erase(key);
    }
    config::from_json(accuracy_legacy);
    for(int i=0;i<4;++i) {
        const auto& profile=feature_ui::weapon_settings.items[weapons::Find(shotgun_classes[i])];
        expect(profile.shotgun_accuracy.value==100.f && profile.shotgun_no_spread.value && profile.shotgun_no_spread.bind.key==0x46+i,
            "old profiles retain their enabled correction and bind while using 100 percent intensity");
    }
    config::from_json(defaults);expect(settings.fast_melee_key.value==VK_XBUTTON1,"fast melee hold defaults to Mouse4");
    settings.fast_melee_key=0x56;settings.fast_melee.value=true;const auto melee_bind=config::to_json();
    settings.fast_melee_key=0x47;settings.fast_melee.value=false;
    expect(config::from_json(melee_bind) && settings.fast_melee_key.value==0x56 && settings.fast_melee.value,
        "fast melee keeps its own hold key and enabled state per profile");
    expect(config::from_json(legacy) && settings.fast_melee_key.value==VK_XBUTTON1,
        "old profiles receive Mouse4 instead of another profile's melee key");
    config::from_json(defaults);
    expect(!settings.guard_queries.value&&!settings.guard_settings.value&&!settings.guard_commands.value&&!settings.guard_messages.value&&
        !settings.guard_crosshair.value&&!settings.guard_crc.value&&!settings.guard_null_sound.value&&!settings.guard_heartbeat.value&&!settings.guard_logging.value,
        "all imported protection options start disabled");
    auto& guard=feature_ui::guard_rules;
    const auto original_rules=guard.value;
    guard.value.queries={{"r_*","1"},{"private_*",""}};guard.value.commands={"exec","bind*"};guard.value.messages.clear();++guard.revision;
    settings.guard_commands.value=true;settings.guard_commands.bind.key=VK_F8;settings.guard_commands.bind.mode=xui::bind_mode::hold_on;
    const auto guard_saved=config::to_json();guard.Reset();settings.guard_commands.value=false;settings.guard_commands.bind={};
    config::from_json(guard_saved);
    expect(guard.value.queries==std::vector<client_guard::Rule>{{"r_*","1"},{"private_*",""}}&&guard.value.messages.empty()&&guard.value.commands.size()==2,
        "custom rules, wildcard patterns, blank block values and intentionally empty lists round-trip");
    expect(settings.guard_commands.value&&settings.guard_commands.bind.key==VK_F8&&settings.guard_commands.bind.mode==xui::bind_mode::hold_on,
        "protection options retain their independent hold bindings");
    const auto valid_rules=guard.value;const auto valid_revision=guard.revision;
    guard.deserialize({{"commands",nlohmann::json::array({"bad;exec"})}});
    expect(guard.value==valid_rules&&guard.revision==valid_revision,"malformed rules cannot partially replace a saved configuration");
    guard.deserialize({{"queries",nlohmann::json::array({{{"name","x"},{"value",123}}})}});
    expect(guard.value==valid_rules,"invalid JSON rule value types preserve the complete prior rule set");
    auto guard_legacy=guard_saved;char guard_key[12]{};std::snprintf(guard_key,sizeof(guard_key),"%08x",config::detail::make_key("l4d2 protection","rules"));
    guard_legacy["fields"].erase(guard_key);config::from_json(guard_legacy);
    expect(guard.value==original_rules,"legacy profiles restore the supplied rule defaults instead of leaking another profile's custom rules");
    guard.value.settings.clear();const auto empty_saved=config::to_json();guard.Reset();config::from_json(empty_saved);
    expect(guard.value.settings.empty(),"an intentionally empty server setting filter survives saving and loading");
    config::from_json(legacy);
    expect(!settings.guard_commands.value&&settings.guard_commands.bind.key==0&&!settings.guard_queries.value&&guard.value==original_rules,
        "old profiles reset protection switches, binds and rule defaults together");
    std::printf("%u settings persistence checks across %zu weapons, %u failed\n",checks,weapons::Count,failures);
    return failures?1:0;
}

