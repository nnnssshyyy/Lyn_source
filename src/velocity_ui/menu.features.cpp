#include "common.hpp"
#include "features_ui.hpp"
#include "client_guard_ui.hpp"
#include "../client_guard.h"
#include "rendering.hpp"
#include "hunter_preview.hpp"
#include "spectators_window.hpp"
#include "../esp_layout.h"
#include "../spectators.h"
#include "../movement_batch.h"
#include "../rapid_fire.h"
#include "../chat_spy.h"
#include "../visuals.h"
#include <atomic>

namespace feature_ui {
namespace { std::atomic_uint third_person_toggles{}; }
void ToggleThirdPerson() noexcept { third_person_toggles.fetch_xor(1,std::memory_order_relaxed); }
void PollHotkeys() {
    static bool initialized{};
    if(!initialized||third_person_toggles.load(std::memory_order_relaxed)) {
        Sync();initialized=true;
    }
}
void Defaults() {
    guard_rules.Reset();
    for(auto& weapon:weapon_settings.items) weapon.Reset();
#define F_BOOL(name,initial,category,label) settings.name.value=initial;
#define F_INT(name,initial,category,label) settings.name=initial;
#define F_FLOAT(name,initial,category,label) settings.name=initial;
#define F_COLOR(name,r,g,b,a,category,label) settings.name=xdraw::color{r,g,b,a};
#include "../features_fields.inc"
#undef F_BOOL
#undef F_INT
#undef F_FLOAT
#undef F_COLOR
}
void Sync() {
    static unsigned rules_revision{};
    if(rules_revision!=guard_rules.revision&&client_guard::SetRules(guard_rules.value))rules_revision=guard_rules.revision;
    settings.name_position.value=esp::layout::NormalizeSide(settings.name_position.value);
    settings.distance_position.value=esp::layout::NormalizeSide(settings.distance_position.value);
    settings.capture_method.value=std::clamp(settings.capture_method.value,0,1);
    settings.rapidfire_method.value=std::clamp(settings.rapidfire_method.value,0,2);
    settings.rapidfire_storm_speed.value=std::clamp(settings.rapidfire_storm_speed.value,rapid_fire::SpeedMinimum,rapid_fire::SpeedMaximum);
    settings.rapidfire_storm_ticks.value=std::clamp(settings.rapidfire_storm_ticks.value,rapid_fire::StormTicksMinimum,rapid_fire::StormTicksMaximum);
    settings.rapidfire_m1_speed.value=std::clamp(settings.rapidfire_m1_speed.value,rapid_fire::SpeedMinimum,rapid_fire::SpeedMaximum);
    settings.rapidfire_m1_choke.value=std::clamp(settings.rapidfire_m1_choke.value,rapid_fire::ChokeMinimum,rapid_fire::ChokeMaximum);
    settings.rapidfire_m2_speed.value=std::clamp(settings.rapidfire_m2_speed.value,rapid_fire::SpeedMinimum,rapid_fire::SpeedMaximum);
    if(third_person_toggles.exchange(0,std::memory_order_relaxed)&1) settings.thirdperson.value=!settings.thirdperson.value;
    features::Config out{};
#define F_BOOL(name,initial,category,label) out.name=settings.name.value;
#define F_INT(name,initial,category,label) out.name=settings.name.value;
#define F_FLOAT(name,initial,category,label) out.name=settings.name.value;
#define F_COLOR(name,r,g,b,a,category,label) out.name=settings.name.value.val;
#include "../features_fields.inc"
#undef F_BOOL
#undef F_INT
#undef F_FLOAT
#undef F_COLOR
    for(size_t i=0;i<out.weapons.size();++i) {
        const auto& item=weapon_settings.items[i];auto& profile=out.weapons[i];
#define W_BOOL(name,initial,label) profile.name=item.name.value;
#define W_INT(name,initial,label) profile.name=item.name.value;
#define W_FLOAT(name,initial,label) profile.name=item.name.value;
#include "../weapon_fields.inc"
#undef W_BOOL
#undef W_INT
#undef W_FLOAT
    }
    static features::Config previous{};static bool published=false;
    if(!published || !(out==previous)) {
        features::SetConfig(out);
        spectators::SetEnabled(out.spectator_list);
        spectators::SetChase(out.spectator_chase);
        previous=out;published=true;
    }
}
void DrawGuardRules() {
    static int group{},selection{},old_group{-1},old_selection{-1};
    static unsigned revision{};static std::string name,value;static bool invalid{};
    const char* groups[]{"ConVar queries","Server ConVars","Server commands","User messages"};
    xui::text("Filter rules",tokens::col_text);
    xui::combo("rule type",group,groups,4);
    auto& pairs=group==0?guard_rules.value.queries:guard_rules.value.settings;
    auto& singles=group==2?guard_rules.value.commands:guard_rules.value.messages;
    const bool pair=group<2;const auto count=pair?pairs.size():singles.size();
    if(old_group!=group)selection=0;
    selection=std::clamp(selection,0,static_cast<int>(count));
    std::vector<const char*> labels{"New rule"};
    for(size_t i=0;i<count;++i)labels.push_back(pair?pairs[i].name.c_str():singles[i].c_str());
    xui::combo("rule",selection,labels.data(),static_cast<int>(labels.size()));
    if(old_group!=group||old_selection!=selection||revision!=guard_rules.revision) {
        name=selection?(pair?pairs[selection-1].name:singles[selection-1]):"";
        value=selection&&pair?pairs[selection-1].value:"";
        old_group=group;old_selection=selection;revision=guard_rules.revision;invalid=false;
    }
    xui::text("pattern",tokens::col_text_dim);
    xui::text_input("guard pattern",name,client_guard::TextLimit);
    if(pair) {
        xui::text("replacement (empty = block)",tokens::col_text_dim);
        xui::text_input("guard replacement",value,client_guard::TextLimit);
    }
    if(xui::button(selection?"Save rule":"Add rule",140,26)) {
        invalid=!client_guard::ValidName(name)||!client_guard::ValidValue(value)||(!selection&&count>=client_guard::RuleLimit);
        if(!invalid) {
            if(pair) {if(selection)pairs[selection-1]={name,value};else pairs.push_back({name,value});}
            else {if(selection)singles[selection-1]=name;else singles.push_back(name);}
            if(!selection)selection=static_cast<int>(count+1);
            ++guard_rules.revision;
        }
    }
    if(selection&&xui::button("Delete rule",140,26)) {
        if(pair)pairs.erase(pairs.begin()+selection-1);else singles.erase(singles.begin()+selection-1);
        selection=0;++guard_rules.revision;
    }
    if(xui::button("Restore filter rules",180,26)){guard_rules.Reset();selection=0;}
    if(invalid)xui::text("Invalid rule or list full",{255,130,130,255});
}
}
namespace rendering {
void menu::draw_ragebot(float) const {
    auto& s=feature_ui::settings;
    const float x=tokens::gap+tokens::sidebar_w+tokens::gap;
    const float y=tokens::gap+tokens::subtab_bar_h+tokens::gap;
    const float w=(m_w-x-tokens::gap*2)*.5f;
    xui::layout::set_cursor(x,y);
    const float h=m_h-y-tokens::gap*2;
    const auto heading=[](std::string_view title) {
        xdraw::push_font(g_fonts.menu_heading);xui::text(title,tokens::col_text);xdraw::pop_font();
    };
    if(m_subtab==2) {feature_ui::DrawWeapons(x,y,w,h);return;}
    if(m_subtab==1) {
        if(xui::begin_child("##infected_activation",w,h,true)) {
            heading("Infected aim");
            xui::checkbox("enabled",s.aim);
            xui::checkbox("aim as infected",s.infected_aim);
            xui::keybind("infected aim key (hold)",s.infected_aim_key.value);
            xui::layout::separator();heading("Tracking");
            xui::slider_float("max FOV",s.fov,0.1f,180.f,"%.1f deg");
            xui::slider_float("range",s.aim_distance,1.f,500.f,"%.0f m");
            xui::slider_float("smoothing (normal)",s.smoothing,0.f,100.f,"%.0f%%");
            xui::end_child();
        }
        xui::layout::set_cursor(x+w+tokens::gap,y);
        if(xui::begin_child("##infected_targets",w,h,true)) {
            heading("Survivor targets");
            xui::checkbox("target Steam friends",s.target_friends);
            xui::layout::separator();heading("On-screen guides");
            xui::checkbox("FOV circle",s.draw_fov);xui::checkbox("target marker",s.draw_target);
            xui::end_child();
        }
        return;
    }
    if(xui::begin_child("aimbot L4D2",w,h,true)) {
        heading("Activation");
        xui::checkbox("enabled",s.aim);
        const char* modes[]{"normal","silent client","silent server"};
        xui::combo("aim mode",s.aim_mode.value,modes,3);
        const char* activation[]{"hold key","on attack","always / auto shoot"};
        xui::combo("activation",s.activation.value,activation,3);
        if(s.activation.value!=2) xui::checkbox("auto shoot",s.auto_fire);
        if(s.activation.value==0) xui::keybind("aim key",s.aim_key.value);
        xui::layout::separator();heading("Tracking");
        xui::slider_float("max FOV",s.fov,0.1f,180.f,"%.1f deg");
        xui::slider_float("range",s.aim_distance,1.f,500.f,"%.0f m");
        xui::slider_float("smoothing (normal)",s.smoothing,0.f,100.f,"%.0f%%");
        const char* priority[]{"closest FOV","nearest distance","smart shot"};
        xui::combo("target priority",s.priority.value,priority,3);
        xui::end_child();
    }
    xui::layout::set_cursor(x+w+tokens::gap,y);
    if(xui::begin_child("hitbox selection",w,h,true)) {
        heading("Hitbox Selection");
        const char* hitbox[]{"automatic by weapon","head only","torso","all hitboxes","custom zones"};
        xui::combo("preset",s.hitbox.value,hitbox,5);
        if(xui::button("open hitbox selector",240.f,30.f)) hunter_preview::OpenHitboxes();
        const int equipped=features::GetStatus().weapon_index;
        if(equipped>=0&&static_cast<size_t>(equipped)<weapons::Count&&xui::button("apply main aim to this weapon",260.f,26.f)) {
            auto& profile=feature_ui::weapon_settings.items[equipped];
            profile.aim.value=s.aim.value;profile.aim_mode.value=s.aim_mode.value;profile.auto_fire.value=s.auto_fire.value;
        }
        xui::layout::separator();heading("Shot compensation");
        xui::checkbox("recoil compensation",s.recoil);xui::checkbox("spread compensation",s.no_spread);
        xui::layout::separator();heading("Position history");
        xui::checkbox("position history",s.history);
        xui::slider_int("history window",s.history_ms,0,200,"%d ms");
        xui::checkbox("search older valid records",s.accurate_unlag);
        xui::layout::separator();heading("On-screen guides");
        xui::checkbox("FOV circle",s.draw_fov);xui::checkbox("target marker",s.draw_target);
        xui::end_child();
    }
}
void menu::draw_legitbot(float) const {
    auto& s=feature_ui::settings;
    const float x=tokens::gap+tokens::sidebar_w+tokens::gap,y=tokens::gap+tokens::subtab_bar_h+tokens::gap;
    const float w=(m_w-x-tokens::gap*2)*.5f,h=m_h-y-tokens::gap*2;
    const auto heading=[](std::string_view title) {
        xdraw::push_font(g_fonts.menu_heading);xui::text(title,tokens::col_text);xdraw::pop_font();
    };
    xui::layout::set_cursor(x,y);
    if(m_subtab==1) {
        if(xui::begin_child("##combat_automation",w,h,true)) {
            heading("Trigger");
            xui::checkbox("trigger",s.trigger);xui::keybind("trigger key",s.trigger_key.value);
            xui::slider_int("trigger delay",s.trigger_delay,0,1000,"%d ms");
            xui::checkbox("head only",s.trigger_head);
            xui::layout::separator();heading("Weapon assistance");
            xui::checkbox("auto pistol",s.auto_pistol);xui::checkbox("fast melee",s.fast_melee);
            xui::keybind("fast melee key (hold)",s.fast_melee_key.value);
            xui::end_child();
        }
        xui::layout::set_cursor(x+w+tokens::gap,y);
        if(xui::begin_child("##shove_automation",w,h,true)) {
            heading("Auto shove");
            xui::checkbox("auto shove",s.auto_bash);xui::checkbox("include common infected",s.bash_common);
            xui::slider_float("shove FOV",s.bash_fov,1.f,180.f,"%.0f deg");
            xui::slider_float("shove range",s.bash_range,10.f,75.f,"%.0f units");
            xui::end_child();
        }
        return;
    }
    if(xui::begin_child("##target_filters",w,h,true)) {
        heading("Target types");
        xui::checkbox("common infected",s.target_common);xui::checkbox("special infected",s.target_special);
        xui::checkbox("Tank",s.target_tank);xui::checkbox("Witch",s.target_witch);
        xui::checkbox("enemy survivors",s.target_survivor);
        xui::layout::separator();heading("Target conditions");
        xui::checkbox("ignore burning",s.ignore_burning);xui::checkbox("ignore idle commons",s.ignore_idle);
        xui::checkbox("ignore calm Witch",s.ignore_calm_witch);
        xui::layout::separator();heading("Friends and teammates");
        xui::checkbox("target Steam friends",s.target_friends);xui::checkbox("avoid friendly fire",s.avoid_teammates);
        xui::end_child();
    }
    xui::layout::set_cursor(x+w+tokens::gap,y);
    if(xui::begin_child("##special_infected_priority",w,h,true)) {
        heading("Special infected priority");
        xui::checkbox("prioritize classes",s.prioritize_classes);
        xui::checkbox("Smoker",s.priority_smoker);
        xui::checkbox("Boomer",s.priority_boomer);
        xui::checkbox("Hunter",s.priority_hunter);
        xui::checkbox("Spitter",s.priority_spitter);
        xui::checkbox("Jockey",s.priority_jockey);
        xui::checkbox("Charger",s.priority_charger);
        xui::checkbox("Tank",s.priority_tank);
        if(s.prioritize_classes.value && !s.priority_smoker.value && !s.priority_boomer.value &&
           !s.priority_hunter.value && !s.priority_spitter.value && !s.priority_jockey.value &&
           !s.priority_charger.value && !s.priority_tank.value)
            xui::text("select at least one class",tokens::col_text);
        xui::end_child();
    }
}
void menu::draw_player(float) const {
    auto& s=feature_ui::settings;
    const float x=tokens::gap+tokens::sidebar_w+tokens::gap,y=tokens::gap+tokens::subtab_bar_h+tokens::gap;
    const float w=(m_w-x-tokens::gap*2)*.5f,h=m_h-y-tokens::gap*2;
    const auto heading=[](std::string_view title) {
        xdraw::push_font(g_fonts.menu_heading);
        xui::text(title,tokens::col_text);xdraw::pop_font();
    };
    xui::layout::set_cursor(x,y);
    const char* cards[]{"##players_general","##players_appearance","##players_colors"};
    if(xui::begin_child(cards[std::clamp(m_subtab,0,2)],w,h,true)) {
        if(m_subtab==0) {
            heading("ESP settings");
            xui::checkbox("ESP",s.esp);
            xui::checkbox("names",s.names);xui::checkbox("distance",s.distance);xui::checkbox("health",s.health);
            xui::slider_float("ESP range",s.esp_distance,1.f,500.f,"%.0f m");
            xui::layout::separator();heading("Labels");
            xui::checkbox("status flags",s.esp_flags);xui::checkbox("infected class label",s.esp_class);
            xui::checkbox("status icons",s.esp_status_icons);xui::checkbox("Steam friend tag and color",s.esp_friends);
            xui::layout::separator();heading("Show entities");
            xui::checkbox("survivors",s.esp_survivor);xui::checkbox("special infected",s.esp_special);
            xui::checkbox("common infected",s.esp_common);xui::checkbox("Witch",s.esp_witch);xui::checkbox("ghost infected",s.esp_ghosts);
        } else if(m_subtab==1) {
            heading("Boxes and skeleton");
            xui::checkbox("boxes",s.boxes);xui::checkbox("box precision",s.box_precision);
            xui::checkbox("corner boxes",s.corners);xui::checkbox("box outlines",s.box_outlines);
            xui::slider_float("line width",s.box_width,.5f,4.f,"%.2f px");
            xui::checkbox("skeleton",s.skeleton);xui::slider_float("skeleton width",s.skeleton_width,.5f,4.f,"%.2f px");
            xui::layout::separator();heading("Label positions");
            xui::combo("name position",s.name_position.value,esp::layout::HealthSideNames,esp::layout::SideCount);
            xui::combo("distance position",s.distance_position.value,esp::layout::HealthSideNames,esp::layout::SideCount);
            xui::layout::separator();heading("Health bar");
            xui::combo("health position",s.health_position.value,esp::layout::HealthSideNames,esp::layout::SideCount);
            xui::checkbox("stepped colors",s.health_bands);xui::checkbox("segmented bar",s.health_segments);
            xui::checkbox("separate temporary health",s.health_split);
            if(!s.capture_privacy.value) {
                xui::layout::separator();heading("In-game model effects");
                xui::checkbox("native glow",s.glow);xui::checkbox("model color",s.chams);xui::checkbox("through walls",s.chams_hidden);
            }
        } else {
            heading("ESP palette");
            xui::color_picker("survivors##esp",s.esp_color_survivor.value);xui::color_picker("commons##esp",s.esp_color_common.value);
            xui::color_picker("smoker##esp",s.esp_color_smoker.value);xui::color_picker("boomer##esp",s.esp_color_boomer.value);
            xui::color_picker("hunter##esp",s.esp_color_hunter.value);xui::color_picker("spitter##esp",s.esp_color_spitter.value);
            xui::color_picker("jockey##esp",s.esp_color_jockey.value);xui::color_picker("charger##esp",s.esp_color_charger.value);
            xui::color_picker("tank##esp",s.esp_color_tank.value);xui::color_picker("witch##esp",s.esp_color_witch.value);
            xui::color_picker("ghosts##esp",s.esp_color_ghost.value);xui::color_picker("friends##esp",s.esp_color_friend.value);
            xui::layout::separator();heading("Skeleton colors");
            xui::color_picker("survivors##skel",s.skeleton_color_survivor.value);xui::color_picker("infected##skel",s.skeleton_color_infected.value);
            if(!s.capture_privacy.value) {
                xui::layout::separator();heading("Glow colors");
                xui::color_picker("survivors##glow",s.glow_color_survivor.value);xui::color_picker("infected##glow",s.glow_color_infected.value);
            }
        }
        xui::end_child();
    }
    xui::layout::set_cursor(x+w+tokens::gap,y);
    if(xui::begin_child("##players_preview",w,h,false)) {
        heading("Hunter preview");hunter_preview::EmbeddedEsp(m_textures.hunter.resource.Get(),h);xui::end_child();
    }
}
void menu::draw_world(float) const {
    auto& s=feature_ui::settings;
    const float x=tokens::gap+tokens::sidebar_w+tokens::gap,y=tokens::gap+tokens::subtab_bar_h+tokens::gap;
    const float w=(m_w-x-tokens::gap*2)*.5f;
    const float h=m_h-y-tokens::gap*2;
    const auto heading=[](std::string_view title) {
        xdraw::push_font(g_fonts.menu_heading);xui::text(title,tokens::col_text);xdraw::pop_font();
    };
    if(m_subtab==3) {
        xui::layout::set_cursor(x,y);
        if(xui::begin_child("##impact_effects",w,h,true)) {
            heading("Hit feedback");
            xui::checkbox("confirmed hitmarker",s.hitmarker);
            xui::slider_float("marker size",s.hitmarker_size,4.f,16.f,"%.0f px");
            xui::checkbox("floating damage",s.damage_numbers);
            xui::slider_float("damage duration",s.damage_lifetime,.4f,2.f,"%.1f s");
            xui::layout::separator();heading("Death effects");
            xui::checkbox("death particles",s.death_particles);
            if(s.death_particles.value) {
                xui::slider_int("particle count",s.death_particle_count,8,32,"%d");
                xui::slider_float("particle duration",s.death_particle_lifetime,.4f,2.f,"%.1f s");
            }
            xui::layout::separator();heading("Visibility");
            xui::checkbox("no fog",s.no_fog);xui::checkbox("no vomit",s.no_vomit);
            xui::checkbox("no infected blue vision",s.no_infected_vision);
            xui::end_child();
        }
        xui::layout::set_cursor(x+w+tokens::gap,y);
        if(xui::begin_child("##navigation_effects",w,h,true)) {
            heading("Offscreen indicators");
            xui::checkbox("offscreen arrows",s.offscreen);xui::checkbox("include common infected",s.offscreen_common);
            xui::slider_float("arrow range",s.offscreen_range,5.f,500.f,"%.0f m");
            xui::slider_float("arrow radius",s.offscreen_radius,.35f,.9f,"%.2f");
            xui::layout::separator();heading("World indicators");
            xui::checkbox("Tank / Witch HUD",s.boss_hud);xui::checkbox("estimated grenade path",s.trajectory);
            xui::slider_float("prediction horizon",s.trajectory_seconds,.5f,4.f,"%.1f s");
            xui::layout::separator();heading("Capture privacy");
            xui::checkbox("anti OBS",s.capture_privacy);
            if(s.capture_privacy.value) {
                const char* methods[]{"display / window","game capture"};
                xui::combo("capture method",s.capture_method.value,methods,2);
            }
            xui::end_child();
        }
        return;
    }
    if(m_subtab==4) {
        xui::layout::set_cursor(x,y);
        if(xui::begin_child("##hud_appearance",w,h,true)) {
            heading("Watermark");
            xui::checkbox("Mostrar marca de agua",s.watermark);
            if(s.watermark.value) xui::checkbox("Mostrar FPS",s.watermark_fps);
            xui::layout::separator();heading("HUD layout");
            xui::checkbox("edit HUD",s.hud_editor);
            xui::slider_float("panel opacity",s.hud_opacity,.2f,1.f,"%.2f");
            xui::layout::separator();heading("Boss panel");
            xui::slider_float("boss width",s.boss_width,230.f,560.f,"%.0f px");
            xui::slider_float("boss scale",s.boss_scale,.7f,1.5f,"%.2fx");
            if(xui::button("reset HUD layout",240.f,26.f)) {
                s.boss_x=.5f;s.boss_y=.02f;s.boss_width=310.f;s.boss_scale=1.f;
                s.radar_x=0.f;s.radar_y=.15f;s.radar_size=230.f;
                s.chat_x=0.f;s.chat_y=.7f;s.chat_width=460.f;s.chat_height=300.f;
                s.spectator_width=300.f;s.spectator_rows=10;s.hud_opacity=1.f;spectators_window::ResetPosition();
            }
            xui::end_child();
        }
        xui::layout::set_cursor(x+w+tokens::gap,y);
        if(xui::begin_child("##hud_panels",w,h,true)) {
            heading("Radar panel");xui::slider_float("radar size",s.radar_size,160.f,400.f,"%.0f px");
            xui::layout::separator();heading("Spectator panel");
            xui::slider_float("spectators width",s.spectator_width,200.f,520.f,"%.0f px");
            xui::slider_int("spectators rows",s.spectator_rows,1,16,"%d");
            xui::layout::separator();heading("Chat panel");
            xui::slider_float("chat width",s.chat_width,280.f,800.f,"%.0f px");
            xui::slider_float("chat height",s.chat_height,180.f,650.f,"%.0f px");
            xui::end_child();
        }
        return;
    }
    if(m_subtab==2) {
        const float h=m_h-y-tokens::gap*2;
        xui::layout::set_cursor(x,y);
        if(xui::begin_child("chat spy settings",w,h,true)) {
            xui::checkbox("chat spy",s.chat_spy);
            xui::checkbox("show panel",s.chat_panel);
            if(!s.capture_privacy.value)xui::checkbox("show in game HUD",s.chat_hud);
            xui::checkbox("hide normal chat duplicates",s.chat_deduplicate);
            xui::checkbox("team prefix",s.chat_team_prefix);
            xui::checkbox("auto scroll",s.chat_auto_scroll);xui::checkbox("hide empty panel",s.chat_hide_empty);
            xui::slider_int("clear history every",s.chat_clear_minutes,0,240,"%d min");
            if(xui::button("clear chat history",240.f,26.f)) chat_spy::RequestClear();
            if(xui::button("show / reset chat panel",240.f,26.f)) {
                s.chat_spy.value=s.chat_panel.value=true;s.chat_x=0.f;s.chat_y=.7f;
            }
            xui::end_child();
        }
        xui::layout::set_cursor(x+w+tokens::gap,y);
        if(xui::begin_child("chat layout",w,h,true)) {
            xui::slider_float("width",s.chat_width,280.f,800.f,"%.0f px");
            xui::slider_float("height",s.chat_height,180.f,650.f,"%.0f px");
            xui::end_child();
        }
        return;
    }
    if(m_subtab==1) {
        const float h=m_h-y-tokens::gap*2;
        xui::layout::set_cursor(x,y);
        if(xui::begin_child("radar settings",w,h,true)) {
            xui::checkbox("radar",s.radar);xui::checkbox("rotate with view",s.radar_rotate);
            xui::slider_float("range",s.radar_range,5.f,200.f,"%.0f m");
            xui::slider_float("window size",s.radar_size,160.f,400.f,"%.0f px");
            xui::checkbox("survivors",s.radar_survivor);xui::checkbox("special infected",s.radar_special);
            xui::checkbox("common infected",s.radar_common);xui::checkbox("Witch",s.radar_witch);
            xui::checkbox("ghost infected",s.radar_ghosts);
            if(xui::button("show / reset radar",240.f,26.f)) {s.radar.value=true;s.radar_x=0.f;s.radar_y=.15f;}
            xui::end_child();
        }
        xui::layout::set_cursor(x+w+tokens::gap,y);
        if(xui::begin_child("spectator settings",w,h,true)) {
            xui::checkbox("spectator list",s.spectator_list);
            xui::checkbox("include chase camera",s.spectator_chase);
            xui::checkbox("camera labels",s.spectator_modes);
            xui::checkbox("hide empty list",s.spectator_hide_empty);
            if(xui::button("show / reset spectator window",240.f,26.f)) {
                s.spectator_list.value=true;spectators_window::ResetPosition();
            }
            xui::end_child();
        }
        return;
    }
    xui::layout::set_cursor(x,y);
    if(xui::begin_child("world ESP",w,m_h-y-tokens::gap*2,true)) {
        heading("ESP settings");
        xui::checkbox("enabled",s.world); xui::checkbox("boxes",s.world_boxes);
        xui::checkbox("names",s.world_names); xui::checkbox("distance",s.world_distance);
        xui::slider_float("range",s.world_range,1.f,200.f,"%.0f m");
        if(!s.capture_privacy.value) {
            xui::layout::separator();heading("In-game model effects");
            xui::checkbox("object glow",s.world_glow);xui::checkbox("object model color",s.world_chams);
        }
        xui::end_child();
    }
    xui::layout::set_cursor(x+w+tokens::gap,y);
    if(xui::begin_child("objects",w,m_h-y-tokens::gap*2,true)) {
        heading("Show objects");
        xui::checkbox("weapons",s.world_weapons); xui::checkbox("health items",s.world_health);
        xui::checkbox("ammo / upgrades",s.world_ammo); xui::checkbox("throwables",s.world_throwables);
        xui::checkbox("projectiles / fire",s.world_projectiles); xui::checkbox("carryable objectives",s.world_objectives);
        xui::layout::separator();heading("Object colors");
        xui::color_picker("weapons##wc",s.world_color_weapon.value);
        xui::color_picker("health items##wc",s.world_color_health.value);
        xui::color_picker("ammo##wc",s.world_color_ammo.value);
        xui::color_picker("throwables##wc",s.world_color_throwable.value);
        xui::color_picker("projectiles##wc",s.world_color_projectile.value);
        xui::color_picker("objectives##wc",s.world_color_objective.value);
        xui::end_child();
    }
}
void menu::draw_misc(float) const {
    auto& s=feature_ui::settings;
    const float x=tokens::gap+tokens::sidebar_w+tokens::gap,y=tokens::gap+tokens::subtab_bar_h+tokens::gap;
    const float w=(m_w-x-tokens::gap*2)*.5f;
    xui::layout::set_cursor(x,y);
    if(m_subtab==0) {
        if(xui::begin_child("movement",w,m_h-y-tokens::gap*2,true)) {
            xui::text("Movement",tokens::col_text);
            xui::checkbox("auto bunny hop",s.bhop);
            xui::checkbox("fast bunnyhop",s.bhop_fast);
            xui::checkbox("auto strafe",s.strafe);
            xui::checkbox("directional strafe",s.strafe_directional);
            xui::checkbox("charger turn",s.charger_turn);
            xui::checkbox("charger teleport",s.charger_teleport);
            xui::end_child();
        }
        xui::layout::set_cursor(x+w+tokens::gap,y);
        if(xui::begin_child("air control",w,m_h-y-tokens::gap*2,true)) {
            xui::text("Hunter / air control",tokens::col_text);
            xui::checkbox("auto ceiling",s.auto_ceiling);
            xui::keybind("ceiling key",s.ceiling_key.value);
            xui::checkbox("Hunter auto click",s.hunter_click);
            xui::keybind("Hunter click key",s.hunter_click_key.value);
            xui::end_child();
        }
    } else if(m_subtab==3) {
        if(xui::begin_child("client guard",w,m_h-y-tokens::gap*2,true)) {
            xui::text("Anti Anti Cheat",tokens::col_text);
            xui::checkbox("filter ConVar queries",s.guard_queries);
            xui::checkbox("filter server ConVars",s.guard_settings);
            xui::checkbox("filter server commands",s.guard_commands);
            xui::checkbox("filter user messages",s.guard_messages);
            xui::checkbox("block crosshair angle",s.guard_crosshair);
            xui::checkbox("block file CRC messages",s.guard_crc);
            xui::layout::separator();
            xui::checkbox("block null sounds",s.guard_null_sound);
            xui::checkbox("mute heartbeat loop",s.guard_heartbeat);
            xui::checkbox("log filtered events",s.guard_logging);
            xui::end_child();
        }
        xui::layout::set_cursor(x+w+tokens::gap,y);
        if(xui::begin_child("guard rules",w,m_h-y-tokens::gap*2,true)) {feature_ui::DrawGuardRules();xui::end_child();}
    } else if(m_subtab==2) {
        if(xui::begin_child("exploits",w,m_h-y-tokens::gap*2,true)) {
            xui::text("Exploits",tokens::col_text);
            xui::checkbox("speedhack",s.speedhack);
            xui::keybind("speed key",s.speedhack_key.value);
            xui::slider_float("multiplier",s.speedhack_scale,1.f,movement_batch::MaxMultiplier,"%.1fx");
            xui::layout::separator();
            xui::checkbox("airstuck",s.airstuck);
            xui::keybind("stuck key",s.airstuck_key.value);
            xui::layout::separator();
            const char* lag_methods[]{"V1","V2"};
            xui::checkbox("lag exploit",s.lag_exploit);
            if(s.lag_exploit.value) {
                xui::keybind("lag key",s.lag_key.value);
                xui::combo("lag method",s.lag_method.value,lag_methods,2);
                if(s.lag_method.value==1)xui::slider_int("sequence push",s.lag_amount_v2,10,1000,"%d");
                else xui::slider_int("sequence push",s.lag_amount,10,1000,"%d");
            }
            xui::layout::separator();
            xui::checkbox("automatic no fall damage",s.no_fall_damage);
            xui::end_child();
        }
        xui::layout::set_cursor(x+w+tokens::gap,y);
        if(xui::begin_child("rapidfire",w,m_h-y-tokens::gap*2,true)) {
            xui::text("Rapidfire",tokens::col_text);
            xui::checkbox("rapidfire",s.rapidfire);
            xui::checkbox("boost 4x",s.rapidfire_boost);
            xui::keybind("rapid key",s.rapidfire_key.value);
            const char* rapid_methods[]{"V1","V2","V3"};
            xui::combo("method",s.rapidfire_method.value,rapid_methods,3);
            if(s.rapidfire_method.value==0) {
                xui::slider_int("speed",s.rapidfire_storm_speed,rapid_fire::SpeedMinimum,rapid_fire::SpeedMaximum,"%d");
                xui::slider_int("factor",s.rapidfire_storm_ticks,rapid_fire::StormTicksMinimum,rapid_fire::StormTicksMaximum,"%d");
            } else if(s.rapidfire_method.value==1) {
                xui::slider_int("speed",s.rapidfire_m1_speed,rapid_fire::SpeedMinimum,rapid_fire::SpeedMaximum,"%d");
                xui::slider_int("choke packets",s.rapidfire_m1_choke,rapid_fire::ChokeMinimum,rapid_fire::ChokeMaximum,"%d");
            } else {
                xui::slider_int("speed",s.rapidfire_m2_speed,rapid_fire::SpeedMinimum,rapid_fire::SpeedMaximum,"%d");
            }
            xui::layout::separator();
            xui::checkbox("pistol rapid",s.pistol_rapid);
            xui::end_child();
        }
    } else if(m_subtab==1) {
        if(xui::begin_child("camera",w,m_h-y-tokens::gap*2,true)) {
            xui::checkbox("third person",s.thirdperson); xui::keybind("third person key",s.third_key.value); xui::checkbox("collision",s.camera_collision);
            xui::checkbox("centered behind player",s.camera_centered);
            xui::slider_float("distance",s.camera_distance,30.f,300.f,"%.0f units");
            if(!s.camera_centered.value) {
                xui::slider_float("shoulder",s.camera_right,-64.f,64.f,"%.0f units");
                xui::slider_float("height",s.camera_up,-64.f,64.f,"%.0f units");
            }
            xui::checkbox("override FOV",s.override_fov); xui::slider_float("FOV",s.camera_fov,70.f,140.f,"%.0f deg");
            xui::checkbox("no visual recoil",s.no_visual_recoil);
            xui::end_child();
        }
        xui::layout::set_cursor(x+w+tokens::gap,y);
        if(xui::begin_child("viewmodel",w,m_h-y-tokens::gap*2,true)) {
            xui::checkbox("offset",s.viewmodel);
            xui::slider_float("forward",s.vm_forward,-60.f,60.f,"%.1f"); xui::slider_float("right",s.vm_right,-60.f,60.f,"%.1f");
            xui::slider_float("up",s.vm_up,-60.f,60.f,"%.1f"); xui::slider_float("pitch",s.vm_pitch,-89.f,89.f,"%.0f deg");
            xui::slider_float("yaw",s.vm_yaw,-180.f,180.f,"%.0f deg"); xui::slider_float("roll",s.vm_roll,-180.f,180.f,"%.0f deg");
            xui::checkbox("override weapon FOV",s.vm_fov); xui::slider_float("weapon FOV",s.viewmodel_fov,40.f,120.f,"%.0f deg");
            xui::end_child();
        }
    }
}
}
