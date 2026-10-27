#pragma once
#include <cstdint>
#include <string>
#include <vector>
struct IDirect3DTexture9;

namespace steam_profile {
// Signatures from Valve's steam_api_flat.h. Only the local user's public
// display name and avatar are read from the game's existing Steam session.
struct Api {
    void* user{};
    void* friends{};
    void* utils{};
    std::uint64_t (__cdecl* get_id)(void*){};
    const char* (__cdecl* get_name)(void*){};
    int (__cdecl* get_avatar)(void*,std::uint64_t){};
    bool (__cdecl* image_size)(void*,int,std::uint32_t*,std::uint32_t*){};
    bool (__cdecl* image_rgba)(void*,int,std::uint8_t*,int){};
};

class Profile {
public:
    void poll(const Api& api,std::uint64_t now);
    const std::string& name() const { return name_; }
    const std::vector<std::uint8_t>& rgba() const { return rgba_; }
    std::uint32_t width() const { return width_; }
    std::uint32_t height() const { return height_; }
    std::uint64_t image_revision() const { return image_revision_; }
private:
    void clear_avatar();
    std::string name_{};
    std::vector<std::uint8_t> rgba_{};
    std::uint64_t user_id_{},next_poll_{},image_revision_{};
    std::uint32_t width_{},height_{};
    int image_handle_{};
};

const Profile& current();
IDirect3DTexture9* avatar();
void invalidate_device_objects();
}
