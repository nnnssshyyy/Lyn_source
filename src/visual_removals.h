#pragma once
#include "memory.h"
namespace visual_removals {
struct Availability {bool fog{},vomit{},infected_vision{};};
// Install on actual rendering routes. Timers/network fields are never edited.
Availability Install(void* client_mode,memory::Range client_image,void* vomit_query,void* vomit_draw,bool(*active)(),
                     void* color_weight=nullptr,void* infected_return=nullptr,void* ghost_return=nullptr) noexcept;
void SetOptions(bool no_fog,bool no_vomit,bool no_infected_vision=false) noexcept;
void Shutdown() noexcept;
}
