#include "features_ui.hpp"
#include "rendering.hpp"
#include "ui_preferences.hpp"
#include <cstdio>

extern "C" IMAGE_DOS_HEADER __ImageBase;
namespace feature_ui {
namespace {
void Text(std::string_view text) {xui::text(text,tokens::col_text_dim);}
void Preview(int index) {
    const auto& definition=weapons::Catalog[index];
    if(!definition.image) return;
    struct Image {Microsoft::WRL::ComPtr<IDirect3DTexture9> texture;int w{},h{};bool attempted{};};
    static std::array<Image,weapons::Count> images{};
    auto& image=images[index];
    if(!image.attempted) {
        image.attempted=true;
        const auto module=reinterpret_cast<HMODULE>(&__ImageBase);
        const auto resource=FindResourceW(module,MAKEINTRESOURCEW(definition.image),RT_RCDATA);
        const auto handle=resource?LoadResource(module,resource):nullptr;
        const auto data=static_cast<const std::byte*>(handle?LockResource(handle):nullptr);
        if(data) image.texture=xdraw::load_texture({data,SizeofResource(module,resource)},&image.w,&image.h);
    }
    if(!image.texture || image.w<1 || image.h<1) return;
    const auto [width,unused]=xui::layout::avail();(void)unused;
    const auto canvas=xui::layout::item(width,135.f);
    auto& dl=xui::draw::current();
    dl.rect_filled(canvas.x,canvas.y,canvas.w,canvas.h,{255,255,255,255},xdraw::corner_radius{6});
    const float scale=std::min((canvas.w-16)/image.w,(canvas.h-16)/image.h);
    const float w=image.w*scale,h=image.h*scale;
    dl.image(canvas.x+(canvas.w-w)*.5f,canvas.y+(canvas.h-h)*.5f,w,h,image.texture.Get());
}
void CopyGlobal(WeaponSettings& p) {
#define COPY(name) p.name.value=settings.name.value
    COPY(aim);COPY(auto_fire);COPY(recoil);COPY(no_spread);COPY(trigger);COPY(trigger_head);COPY(auto_pistol);
    COPY(aim_mode);COPY(hitbox);COPY(trigger_delay);COPY(fov);COPY(smoothing);COPY(aim_distance);
    COPY(vm_forward);COPY(vm_right);COPY(vm_up);COPY(vm_pitch);COPY(vm_yaw);COPY(vm_roll);COPY(viewmodel_fov);
#undef COPY
}
}
void DrawWeapons(float x,float y,float w,float h) {
    selected_weapon=std::clamp(selected_weapon,0,static_cast<int>(weapons::Count)-1);
    xui::layout::set_cursor(x,y);
    if(xui::begin_child("weapon profiles",w,h,true)) {
        static const auto names=[] {
            std::array<const char*,weapons::Count> result{};
            for(size_t i=0;i<result.size();++i) result[i]=weapons::Catalog[i].name;
            return result;
        }();
        xui::combo("weapon",selected_weapon,names.data(),static_cast<int>(names.size()));
        const auto weapon_status=features::GetStatus();
        const int equipped=weapon_status.weapon_index;
        if(equipped>=0 && static_cast<size_t>(equipped)<weapons::Count) {
            Text(std::string(ui::translate("Equipped: "))+weapons::Catalog[equipped].name);
            if(xui::button("select equipped weapon",240.f,26.f)) selected_weapon=equipped;
        }
        Preview(selected_weapon);
        auto& p=weapon_settings.items[selected_weapon];
        xui::layout::separator();
        if(xui::button("copy global settings",240.f,26.f)) CopyGlobal(p);
        if(xui::button("reset this weapon",240.f,26.f)) p.Reset();
        if(weapons::IsShotgun(selected_weapon)) {
            Text(selected_weapon<=7?"T1 shotgun":"T2 shotgun");
            xui::checkbox("Accuracy (shotgun)",p.shotgun_no_spread);
            xui::slider_float("Accuracy",p.shotgun_accuracy,0.f,100.f,"%.0f%%");
            xui::layout::separator();
        }
        if(weapons::Catalog[selected_weapon].group>=0) {
            xui::checkbox("custom combat settings",p.combat);
            if(p.combat.value) {
                xui::checkbox("aim assistance",p.aim);
                const char* modes[]{"normal","silent client","silent server"};
                xui::combo("aim mode",p.aim_mode.value,modes,3);
                if(settings.activation.value!=2) xui::checkbox("auto shoot",p.auto_fire);
                xui::slider_float("max FOV",p.fov,.1f,180.f,"%.1f deg");
                xui::slider_float("range",p.aim_distance,1.f,500.f,"%.0f m");
                xui::slider_float("smoothing (normal)",p.smoothing,0.f,100.f,"%.0f%%");
                const char* hitboxes[]{"automatic by weapon","head only","torso","all hitboxes","custom zones"};
                xui::combo("hitbox selection",p.hitbox.value,hitboxes,5);
                xui::checkbox("recoil compensation",p.recoil);
                if(!weapons::IsShotgun(selected_weapon)) xui::checkbox("spread compensation",p.no_spread);
                xui::checkbox("auto pistol",p.auto_pistol);
                xui::checkbox("trigger",p.trigger);xui::checkbox("trigger head only",p.trigger_head);
                xui::slider_int("trigger delay",p.trigger_delay,0,1000,"%d ms");
            }
        }
        xui::end_child();
    }
    xui::layout::set_cursor(x+w+tokens::gap,y);
    if(xui::begin_child("weapon viewmodel",w,h,true)) {
        auto& p=weapon_settings.items[selected_weapon];
        xui::text(weapons::Catalog[selected_weapon].name,tokens::col_text);
        xui::checkbox("custom viewmodel settings",p.view);
        if(p.view.value) {
            xui::slider_float("forward",p.vm_forward,-60.f,60.f,"%.1f");
            xui::slider_float("right",p.vm_right,-60.f,60.f,"%.1f");
            xui::slider_float("up",p.vm_up,-60.f,60.f,"%.1f");
            xui::slider_float("pitch",p.vm_pitch,-89.f,89.f,"%.0f deg");
            xui::slider_float("yaw",p.vm_yaw,-180.f,180.f,"%.0f deg");
            xui::slider_float("roll",p.vm_roll,-180.f,180.f,"%.0f deg");
            xui::slider_float("weapon FOV",p.viewmodel_fov,40.f,120.f,"%.0f deg");
        }
        xui::end_child();
    }
}
}
