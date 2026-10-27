#pragma once
#include <string_view>
#include <string>

namespace ui {
// Interface preferences belong to this Windows user, independently of profiles.
struct Preferences { int language{}; int scale{}; int snow{1}; };
Preferences& preferences();
void load_preferences();
bool save_preferences();
void set_persistence(bool enabled);
std::string_view translate(std::string_view text);
std::string search_text(std::string_view text);
void poll_profile_state(bool force=false);
std::string_view active_profile();
bool profile_dirty();
void profile_saved(std::wstring_view name);
void profile_removed(std::wstring_view name);
float requested_scale();
class FrameScale {
    float scale_{1.f};
public:
    FrameScale(float& x,float& y,float width,float height);
    ~FrameScale();
    FrameScale(const FrameScale&)=delete;
    FrameScale& operator=(const FrameScale&)=delete;
};
}
