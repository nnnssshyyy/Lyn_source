#include "visual_removals.h"
#include <MinHook.h>
#include <atomic>
#include <intrin.h>

namespace visual_removals {
namespace {
using BoolMethod=bool(__thiscall*)(void*);
using DrawMethod=void(__thiscall*)(void*);
std::atomic_bool fog{},vomit{},infected_vision{};
bool(*is_active)(){};
BoolMethod original_fog{},original_vomit{};
DrawMethod original_draw{};
using ColorWeight=void(__thiscall*)(void*,unsigned,float,bool);
ColorWeight original_weight{};
void* weight_target{};void* infected_site{};void* ghost_site{};
void** fog_slot{};void* query_target{};void* draw_target{};
bool Enabled(const std::atomic_bool& setting) noexcept {return setting.load(std::memory_order_acquire)&&is_active&&is_active();}
bool __fastcall FogHook(void* self,void*) {return Enabled(fog)?false:original_fog(self);}
bool __fastcall VomitHook(void* self,void*) {return Enabled(vomit)?false:original_vomit(self);}
void __fastcall DrawHook(void* self,void*) {if(!Enabled(vomit))original_draw(self);}
void __fastcall WeightHook(void* self,void*,unsigned handle,float weight,bool exclusive) {
    const auto site=_ReturnAddress();
    if(Enabled(infected_vision) && (site==infected_site || site==ghost_site)) weight=0.f;
    original_weight(self,handle,weight,exclusive);
}
bool Replace(void** slot,void* expected,void* value) noexcept {
    void* current{};if(!memory::Read(slot,current)||current!=expected)return false;
    DWORD before{};if(!VirtualProtect(slot,sizeof(void*),PAGE_READWRITE,&before))return false;
    const bool wrote=memory::Copy(slot,&value,sizeof(value));DWORD ignored{};
    const bool protected_again=VirtualProtect(slot,sizeof(void*),before,&ignored)!=FALSE;
    if(wrote&&protected_again)return true;
    DWORD writable{};
    if(VirtualProtect(slot,sizeof(void*),PAGE_READWRITE,&writable)) {
        memory::Copy(slot,&expected,sizeof(expected));VirtualProtect(slot,sizeof(void*),before,&ignored);
    }
    return false;
}
}
void Shutdown() noexcept {
    fog=false;vomit=false;infected_vision=false;
    if(fog_slot)Replace(fog_slot,reinterpret_cast<void*>(&FogHook),reinterpret_cast<void*>(original_fog));
    fog_slot=nullptr;
    if(query_target){MH_DisableHook(query_target);MH_RemoveHook(query_target);}query_target=nullptr;
    if(draw_target){MH_DisableHook(draw_target);MH_RemoveHook(draw_target);}draw_target=nullptr;
    if(weight_target){MH_DisableHook(weight_target);MH_RemoveHook(weight_target);}weight_target=nullptr;
    infected_site=ghost_site=nullptr;
}
Availability Install(void* mode,memory::Range image,void* query,void* draw,bool(*active)(),void* weight,void* infected,void* ghost) noexcept {
    Shutdown();is_active=active;Availability available{};
    void** table{};void* candidate{};
    // Tiny return-true bodies can be shared with unrelated methods. Replace
    // only ShouldDrawFog's vtable entry instead of detouring shared code.
    if(memory::Read(mode,table)&&image.Contains(table,19*sizeof(void*))&&
       memory::Read(table+18,candidate)&&image.Contains(candidate)&&memory::Executable(candidate)) {
        original_fog=reinterpret_cast<BoolMethod>(candidate);
        if(Replace(table+18,candidate,reinterpret_cast<void*>(&FogHook))){fog_slot=table+18;available.fog=true;}
    }
    if(query&&draw&&query!=draw&&image.Contains(query)&&image.Contains(draw)&&memory::Executable(query)&&memory::Executable(draw)) {
        const bool first=MH_CreateHook(query,reinterpret_cast<void*>(&VomitHook),reinterpret_cast<void**>(&original_vomit))==MH_OK;
        const bool second=first&&MH_CreateHook(draw,reinterpret_cast<void*>(&DrawHook),reinterpret_cast<void**>(&original_draw))==MH_OK;
        if(second&&MH_EnableHook(query)==MH_OK&&MH_EnableHook(draw)==MH_OK){query_target=query;draw_target=draw;available.vomit=true;}
        else {if(first){MH_DisableHook(query);MH_RemoveHook(query);}if(second){MH_DisableHook(draw);MH_RemoveHook(draw);}}
    }
    if(weight && infected && ghost && infected!=ghost && image.Contains(weight) && image.Contains(infected) && image.Contains(ghost) && memory::Executable(weight)) {
        if(MH_CreateHook(weight,reinterpret_cast<void*>(&WeightHook),reinterpret_cast<void**>(&original_weight))==MH_OK) {
            infected_site=infected;ghost_site=ghost;
            if(MH_EnableHook(weight)==MH_OK) {weight_target=weight;available.infected_vision=true;}
            else {MH_RemoveHook(weight);infected_site=ghost_site=nullptr;}
        }
    }
    return available;
}
void SetOptions(bool no_fog,bool no_vomit,bool no_blue) noexcept {fog.store(no_fog,std::memory_order_release);vomit.store(no_vomit,std::memory_order_release);infected_vision.store(no_blue,std::memory_order_release);}
}
