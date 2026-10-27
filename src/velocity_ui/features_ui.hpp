#pragma once
#include "config.hpp"
#include "../features.h"
namespace feature_ui {
struct Settings {
#define F_BOOL(name,value,category,label) xui::setting name{value,{},label,category};
#define F_INT(name,value,category,label) config::val<int> name{value,category,label};
#define F_FLOAT(name,value,category,label) config::val<float> name{value,category,label};
#define F_COLOR(name,r,g,b,a,category,label) config::col name{{r,g,b,a},category,label};
#include "../features_fields.inc"
#undef F_BOOL
#undef F_INT
#undef F_FLOAT
#undef F_COLOR
};
inline Settings settings;
struct WeaponSettings {
#define W_BOOL(name,value,label) xui::setting name{value};
#define W_INT(name,value,label) config::val<int> name{value};
#define W_FLOAT(name,value,label) config::val<float> name{value};
#include "../weapon_fields.inc"
#undef W_BOOL
#undef W_INT
#undef W_FLOAT
    void Reset() {
#define W_BOOL(name,initial,label) name.value=initial;
#define W_INT(name,initial,label) name.value=initial;
#define W_FLOAT(name,initial,label) name.value=initial;
#include "../weapon_fields.inc"
#undef W_BOOL
#undef W_INT
#undef W_FLOAT
    }
};
struct WeaponSettingsList {
    std::array<WeaponSettings,weapons::Count> items{};
    WeaponSettingsList() {
        for(size_t i=0;i<items.size();++i) {
            auto& item=items[i];const std::string category=std::string("l4d2 weapons ")+weapons::Catalog[i].key;
#define W_BOOL(member,value,label) item.member.name=label;item.member.category=category;
#define W_INT(member,value,label) item.member.reg(category,label);
#define W_FLOAT(member,value,label) item.member.reg(category,label);
#include "../weapon_fields.inc"
#undef W_BOOL
#undef W_INT
#undef W_FLOAT
            config::detail::get_registry().missing_defaults[
                config::detail::make_key(category,"shotgun accuracy percent")]=100.f;
        }
    }
};
inline WeaponSettingsList weapon_settings;
inline int selected_weapon{};
void DrawWeapons(float x,float y,float width,float height);
void Defaults();
void Sync();
void PollHotkeys();
void ToggleThirdPerson() noexcept;
}
