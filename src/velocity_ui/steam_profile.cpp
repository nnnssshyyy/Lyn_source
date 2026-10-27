#include "steam_profile.hpp"
#include "xdraw.hpp"
#include "../memory.h"

namespace steam_profile {
namespace {
constexpr std::uint64_t PollInterval=2000;
constexpr std::uint32_t MaxAvatarSize=512;
Profile profile;
Microsoft::WRL::ComPtr<IDirect3DTexture9> texture;
std::uint64_t uploaded_revision{},next_update{},next_upload{};

template<class Fn> Fn exported(HMODULE module,const char* name) {
    return reinterpret_cast<Fn>(GetProcAddress(module,name));
}

Api resolve_api() {
    Api api;
    // The game owns initialization, callback pumping and shutdown. Never load
    // another Steam binary or open a second session from the menu.
    const auto module=GetModuleHandleW(L"steam_api.dll");
    if(!module)return api;
    using GetUser=int(__cdecl*)();
    const auto get_user=exported<GetUser>(module,"SteamAPI_GetHSteamUser");
    int user_handle{};
    if(!memory::Invoke(get_user,user_handle)||!user_handle)return api;
    using Get=void*(__cdecl*)();
    const auto get_friends=exported<Get>(module,"SteamAPI_SteamFriends_v017");
    const auto get_account=exported<Get>(module,"SteamAPI_SteamUser_v021");
    const auto get_utils=exported<Get>(module,"SteamAPI_SteamUtils_v010");
    if(!memory::Invoke(get_friends,api.friends)||!memory::Invoke(get_account,api.user))return {};
    memory::Invoke(get_utils,api.utils);
    api.get_id=exported<decltype(api.get_id)>(module,"SteamAPI_ISteamUser_GetSteamID");
    api.get_name=exported<decltype(api.get_name)>(module,"SteamAPI_ISteamFriends_GetPersonaName");
    api.get_avatar=exported<decltype(api.get_avatar)>(module,"SteamAPI_ISteamFriends_GetLargeFriendAvatar");
    api.image_size=exported<decltype(api.image_size)>(module,"SteamAPI_ISteamUtils_GetImageSize");
    api.image_rgba=exported<decltype(api.image_rgba)>(module,"SteamAPI_ISteamUtils_GetImageRGBA");
    return api;
}

std::string display_name(const char* source) {
    std::string name;
    if(!source)return name;
    // Steam names are UTF-8, at most 128 bytes. Keep them as literal text and
    // turn control characters into spaces so they cannot break the footer.
    for(std::size_t i=0;i<128;++i) {
        char c{};
        if(!memory::Read(source+i,c))return {};
        if(!c)return name;
        name+=static_cast<unsigned char>(c)<32||c==127?' ':c;
    }
    return {}; // Reject a malformed, unterminated API result.
}
}

void Profile::clear_avatar() {
    if(!rgba_.empty())++image_revision_;
    rgba_.clear();width_=height_=0;image_handle_=0;
}

void Profile::poll(const Api& api,std::uint64_t now) {
    if(now<next_poll_)return;
    next_poll_=now+PollInterval;
    std::uint64_t id{};
    const char* text{};
    if(!api.user||!api.friends||!memory::Invoke(api.get_id,id,api.user)||!id||
       !memory::Invoke(api.get_name,text,api.friends)) {
        name_.clear();user_id_=0;clear_avatar();return;
    }
    if(id!=user_id_) {clear_avatar();user_id_=id;}
    name_=display_name(text);
    if(!api.utils||!api.get_avatar||!api.image_size||!api.image_rgba)return;
    int handle{};
    if(!memory::Invoke(api.get_avatar,handle,api.friends,id))return;
    if(handle==0) {clear_avatar();return;}
    // -1 means Steam is fetching the image. Retry at the next poll, while the
    // game's own callbacks continue normally. Decode a ready handle only once.
    if(handle<0||handle==image_handle_)return;
    std::uint32_t width{},height{};bool ok{};
    if(!memory::Invoke(api.image_size,ok,api.utils,handle,&width,&height)||!ok||
       !width||!height||width>MaxAvatarSize||height>MaxAvatarSize)return;
    std::vector<std::uint8_t> pixels(static_cast<std::size_t>(width)*height*4);
    if(!memory::Invoke(api.image_rgba,ok,api.utils,handle,pixels.data(),static_cast<int>(pixels.size()))||!ok)return;
    rgba_=std::move(pixels);width_=width;height_=height;image_handle_=handle;++image_revision_;
}

const Profile& current() {
    const auto now=GetTickCount64();
    if(now>=next_update) {next_update=now+PollInterval;profile.poll(resolve_api(),now);}
    return profile;
}

IDirect3DTexture9* avatar() {
    const auto& state=current();
    if(uploaded_revision!=state.image_revision()) {
        texture.Reset();uploaded_revision=state.image_revision();next_upload=0;
    }
    const auto now=GetTickCount64();
    if(!texture&&!state.rgba().empty()&&now>=next_upload) {
        next_upload=now+PollInterval;
        texture=xdraw::create_srv_from_rgba(state.rgba().data(),static_cast<int>(state.width()),static_cast<int>(state.height()));
    }
    return texture.Get();
}

void invalidate_device_objects() {texture.Reset();next_upload=0;}
}
