#include "common.hpp"
#include "ui_preferences.hpp"
#include "xui.hpp"
#include "rendering.hpp"

namespace ui {
namespace { Preferences current{}; bool persist=true; float previous_scale=1.f; }
Preferences& preferences() { return current; }
void set_persistence(bool enabled) { persist=enabled; }
void load_preferences() {
    if(!persist)return;
    DWORD value{},size=sizeof(value);
    if(RegGetValueW(HKEY_CURRENT_USER,L"Software\\lyn\\interface",L"Language",RRF_RT_REG_DWORD,nullptr,&value,&size)==ERROR_SUCCESS)
        current.language=std::clamp(static_cast<int>(value),0,1);
    size=sizeof(value);
    if(RegGetValueW(HKEY_CURRENT_USER,L"Software\\lyn\\interface",L"Scale",RRF_RT_REG_DWORD,nullptr,&value,&size)==ERROR_SUCCESS)
        current.scale=std::clamp(static_cast<int>(value),0,2);
    size=sizeof(value);
    if(RegGetValueW(HKEY_CURRENT_USER,L"Software\\lyn\\interface",L"Snow",RRF_RT_REG_DWORD,nullptr,&value,&size)==ERROR_SUCCESS)
        current.snow=std::clamp(static_cast<int>(value),0,1);
}
bool save_preferences() {
    if(!persist)return true;
    HKEY key{};
    if(RegCreateKeyExW(HKEY_CURRENT_USER,L"Software\\lyn\\interface",0,nullptr,0,KEY_SET_VALUE,nullptr,&key,nullptr)!=ERROR_SUCCESS)return false;
    const DWORD language=std::clamp(current.language,0,1),scale=std::clamp(current.scale,0,2),snow=std::clamp(current.snow,0,1);
    const auto a=RegSetValueExW(key,L"Language",0,REG_DWORD,reinterpret_cast<const BYTE*>(&language),sizeof(language));
    const auto b=RegSetValueExW(key,L"Scale",0,REG_DWORD,reinterpret_cast<const BYTE*>(&scale),sizeof(scale));
    const auto c=RegSetValueExW(key,L"Snow",0,REG_DWORD,reinterpret_cast<const BYTE*>(&snow),sizeof(snow));
    RegCloseKey(key);return a==ERROR_SUCCESS&&b==ERROR_SUCCESS&&c==ERROR_SUCCESS;
}
float requested_scale() { return 1.f+.25f*std::clamp(current.scale,0,2); }
#include "ui_text.inl"
std::string_view translate(std::string_view text) {
    static const auto dictionary=[] {
        std::unordered_map<std::string,const Text*> result;
        for(const auto& item:texts) {
            result.try_emplace(search_text(item.en),&item);
            result.try_emplace(search_text(item.es),&item);
        }
        return result;
    }();
    const auto found=dictionary.find(search_text(text));
    if(found==dictionary.end())return text;
    return current.language==1?found->second->en:found->second->es;
}
std::string search_text(std::string_view text) {
    std::string result(text);
    std::transform(result.begin(),result.end(),result.begin(),[](unsigned char c){return static_cast<char>(std::tolower(c));});
    static constexpr std::pair<std::string_view,std::string_view> accents[]{
        {"á","a"},{"Á","a"},{"é","e"},{"É","e"},{"í","i"},{"Í","i"},
        {"ó","o"},{"Ó","o"},{"ú","u"},{"Ú","u"},{"ü","u"},{"Ü","u"},{"ñ","n"},{"Ñ","n"}
    };
    for(const auto& [accent,plain]:accents) {
        size_t at{};while((at=result.find(accent,at))!=std::string::npos) {result.replace(at,accent.size(),plain);at+=plain.size();}
    }
    return result;
}
FrameScale::FrameScale(float& x,float& y,float width,float height) {
    const auto [vw,vh]=xdraw::viewport_size();
    scale_=std::max(.5f,std::min({requested_scale(),(vw-16.f)/width,(vh-16.f)/height}));
    x*=previous_scale/scale_;y*=previous_scale/scale_;previous_scale=scale_;
    x=std::clamp(x,8.f/scale_,std::max(8.f/scale_,vw/scale_-width-8.f/scale_));
    y=std::clamp(y,8.f/scale_,std::max(8.f/scale_,vh/scale_-height-8.f/scale_));
    auto& input=xui::ctx().input;
    input.mouse_x/=scale_;input.mouse_y/=scale_;input.prev_mouse_x/=scale_;input.prev_mouse_y/=scale_;
    xdraw::interface_scale=scale_;
    xdraw::push_font(rendering::g_fonts.menu_body);
}
FrameScale::~FrameScale() {
    auto& input=xui::ctx().input;
    input.mouse_x*=scale_;input.mouse_y*=scale_;input.prev_mouse_x*=scale_;input.prev_mouse_y*=scale_;
    xdraw::pop_font();
    xdraw::interface_scale=1.f;
}
}
