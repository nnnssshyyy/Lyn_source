#include "visual_removals.h"
#include <MinHook.h>
#include <array>
#include <cstdio>
#include <intrin.h>
struct Object {void** table{};unsigned fog_calls{},queries{},draws{};bool effect{true},allows_fog{true};};
bool running=true;
bool Active(){return running;}
__declspec(noinline) bool __fastcall NativeFog(void* self,void*) {auto& o=*static_cast<Object*>(self);++o.fog_calls;return o.allows_fog;}
__declspec(noinline) bool __fastcall NativeQuery(void* self,void*) {auto& o=*static_cast<Object*>(self);++o.queries;return o.effect;}
__declspec(noinline) void __fastcall NativeDraw(void* self,void*) {auto& o=*static_cast<Object*>(self);++o.draws;}
__declspec(noinline) bool __fastcall OtherFog(void*,void*) {return false;}
struct WeightState {unsigned handle{},calls{};float weight{};bool exclusive{};} weights;
void* observed_return{};unsigned color_calls{};
__declspec(noinline) void __fastcall NativeWeight(void* self,void*,unsigned handle,float weight,bool exclusive) {
 auto& state=*static_cast<WeightState*>(self);state.handle=handle;state.weight=weight;state.exclusive=exclusive;++state.calls;observed_return=_ReturnAddress();
}
__declspec(noinline) void InfectedColor() {NativeWeight(&weights,nullptr,12,.8f,false);color_calls+=1;}
__declspec(noinline) void GhostColor() {NativeWeight(&weights,nullptr,14,.7f,false);color_calls+=2;}
__declspec(noinline) void OtherColor() {NativeWeight(&weights,nullptr,12,.6f,true);color_calls+=3;}
std::array<void*,20> table{};
int main(){unsigned checks{},failed{};const auto expect=[&](bool ok,const char* text){++checks;if(!ok){++failed;std::printf("FAIL: %s\n",text);}};
 expect(MH_Initialize()==MH_OK,"native detour backend initializes");
 table.fill(reinterpret_cast<void*>(&NativeFog));Object object{table.data()};
 const auto image=memory::Module(GetModuleHandleW(nullptr));
 using Query=bool(__thiscall*)(void*);using Draw=void(__thiscall*)(void*);
 const auto fog_call=[&](unsigned slot=18){bool value{};memory::Invoke(memory::Virtual<Query>(&object,slot,image),value,&object);return value;};
 const auto query=[&]{bool value{};memory::Invoke(reinterpret_cast<Query>(&NativeQuery),value,&object);return value;};
 const auto draw=[&]{memory::InvokeVoid(reinterpret_cast<Draw>(&NativeDraw),&object);};
 auto routes=visual_removals::Install(&object,image,reinterpret_cast<void*>(&NativeQuery),reinterpret_cast<void*>(&NativeDraw),&Active);
 expect(routes.fog&&routes.vomit,"fog entry and both distinct bile render routes install");
 expect(fog_call()&&query(),"disabled options preserve original native bool returns");draw();
 expect(object.fog_calls==1&&object.queries==1&&object.draws==1,"disabled routes forward original this pointer and drawing");
 visual_removals::SetOptions(true,true);
 expect(!fog_call()&&!query(),"enabled options suppress fog and the bile shader enable query");
 const auto before=object.draws;draw();expect(object.draws==before,"bile particles are suppressed as well as the shader layer");
 expect(fog_call(17),"a shared return body still works for unrelated client-mode slots");
 expect(object.fog_calls==2&&object.queries==1,"suppressed queries do not run original code or mutate player state");
 running=false;expect(fog_call()&&query(),"inactive license/runtime restores original behavior immediately");draw();
 expect(object.draws==before+1,"inactive runtime forwards the original particle draw");running=true;
 visual_removals::SetOptions(false,true);expect(fog_call()&&!query(),"fog and bile switches are independent");
 visual_removals::SetOptions(true,false);expect(!fog_call()&&query(),"turning bile removal off restores its original query");draw();
 expect(object.draws==before+2,"turning bile removal off restores its particle drawing");
 object.allows_fog=false;visual_removals::SetOptions(false,false);expect(!fog_call(),"original false fog decisions are preserved too");object.allows_fog=true;
 visual_removals::Shutdown();expect(table[18]==reinterpret_cast<void*>(&NativeFog),"shutdown restores only the owned vtable entry");
 expect(fog_call()&&query(),"shutdown restores native dispatch and executable code");
 routes=visual_removals::Install(&object,image,nullptr,reinterpret_cast<void*>(&NativeDraw),&Active);
 expect(routes.fog&&!routes.vomit,"a missing bile route leaves fog independent and reports bile unavailable");
 visual_removals::SetOptions(false,true);expect(query(),"an incomplete bile route never installs a partial query detour");draw();
 table[18]=reinterpret_cast<void*>(&OtherFog);visual_removals::Shutdown();
 expect(table[18]==reinterpret_cast<void*>(&OtherFog),"another component's vtable replacement survives cleanup");
 routes=visual_removals::Install(nullptr,image,nullptr,nullptr,&Active);
 expect(!routes.fog&&!routes.vomit,"missing native routes fail closed without invented availability");
 visual_removals::Shutdown();
 InfectedColor();void* infected=observed_return;GhostColor();void* ghost=observed_return;
 expect(infected!=ghost,"native fixtures have independent observed color-weight submission sites");
 routes=visual_removals::Install(nullptr,image,nullptr,nullptr,&Active,reinterpret_cast<void*>(&NativeWeight),infected,ghost);
 expect(routes.infected_vision && !routes.fog && !routes.vomit,"color-weight route installs independently of fog and bile");
 InfectedColor();expect(weights.weight==.8f && weights.handle==12,"disabled vision option preserves native infected weight and handle");
 visual_removals::SetOptions(false,false,true);
 InfectedColor();expect(weights.weight==0.f && weights.handle==12 && !weights.exclusive,"infected submission suppresses weight while forwarding original this and argument ABI");
 GhostColor();expect(weights.weight==0.f && weights.handle==14,"ghost blue filter is suppressed on its separate submission route");
 OtherColor();expect(weights.weight==.6f && weights.handle==12 && weights.exclusive,"map and third-strike color weights are retained even when their handle equals an infected handle");
 running=false;GhostColor();expect(weights.weight==.7f,"inactive runtime immediately restores infected rendering without rewriting state");running=true;
 visual_removals::SetOptions(false,false,false);InfectedColor();expect(weights.weight==.8f,"turning the vision option off restores the original render weights");
 visual_removals::Shutdown();GhostColor();expect(weights.weight==.7f,"shutdown removes the color-weight detour and native calls still work");
 routes=visual_removals::Install(nullptr,image,nullptr,nullptr,&Active,reinterpret_cast<void*>(&NativeWeight),infected,nullptr);
 expect(!routes.infected_vision,"incomplete color correction sites cannot install a partially working filter");
 visual_removals::Shutdown();MH_Uninitialize();
 std::printf("%u visual removal checks, %u failed\n",checks,failed);return failed?1:0;
}
