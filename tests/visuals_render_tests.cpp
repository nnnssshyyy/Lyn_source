#include "velocity_ui/visuals_window.hpp"
#include "velocity_ui/hud_widgets.hpp"
#include "visuals.h"
#include "overlay_paint.h"
#include <cstdio>

// Exercise production ImGui drawing and pointer routing without an engine or
// DirectX device. Only the engine snapshot and the host menu context are mocked.
namespace {visuals::Snapshot frame{};bool licensed=true;unsigned checks{},failures{};}
namespace licensing {bool Active() noexcept{return licensed;}}
namespace visuals {Snapshot ReadSnapshot() noexcept{return frame;}}
namespace hunter_preview {bool OwnsPointer(){return false;}}
namespace xui {
context& ctx(){static context c{};return c;}
bool context::overlay_blocking()const{return false;}
void input_state::clear_frame(){mouse_clicked=mouse_released=mouse_double_clicked=rmb_clicked=rmb_released=false;scroll_delta=0;char_count=key_press_count=key_release_count=0;prev_mouse_x=mouse_x;prev_mouse_y=mouse_y;}
namespace binds {void register_setting(setting*){} }
}
void expect(bool pass,const char* label){++checks;if(!pass){++failures;std::printf("FAIL: %s\n",label);}}
int Draw(bool open,float width=1280,float height=720){
    auto& io=ImGui::GetIO();io.DisplaySize={width,height};io.DeltaTime=1.f/60;
    ImGui::NewFrame();visuals_window::BeginMenuInput(open);visuals_window::EndMenuInput();visuals_window::Draw(open);ImGui::Render();
    auto* data=ImGui::GetDrawData();bool finite=true;
    for(const auto* list:data->CmdLists)for(const auto& vertex:list->VtxBuffer)finite&=std::isfinite(vertex.pos.x)&&std::isfinite(vertex.pos.y);
    expect(finite,"all generated vertices are finite");return data->TotalVtxCount;
}
int main(){
    struct LocalData {int health;unsigned char life;} local_data{100,0};
    game::Fields local_fields{};local_fields.health=offsetof(LocalData,health);local_fields.life=offsetof(LocalData,life);
    game::Entity local{};local.ptr=&local_data;local.fields=&local_fields;local.health=-1;
    expect(visuals::AliveForTrajectory(local),"throw preview accepts an alive visual entity whose health placeholder is unset");
    local_data.life=1;expect(!visuals::AliveForTrajectory(local),"dead players cannot predict a throw");
    local_data.life=0;local_fields.health=-1;expect(!visuals::AliveForTrajectory(local),"unreadable health fails closed for throw prediction");
    ImGui::CreateContext();auto& io=ImGui::GetIO();io.IniFilename=nullptr;io.Fonts->AddFontDefault();
    unsigned char* pixels{};int w{},h{};io.Fonts->GetTexDataAsRGBA32(&pixels,&w,&h);
    auto& s=feature_ui::settings;frame.time=GetTickCount64();frame.valid=true;frame.matrix.m[0][0]=.01f;frame.matrix.m[1][2]=.01f;frame.matrix.m[3][3]=1;
    s.boss_hud.value=true;frame.boss_count=2;frame.bosses[0]={1,source::Kind::Tank,3000,4000,0,40,true};frame.bosses[1]={2,source::Kind::Witch,500,1000,70,18,false};
    expect(Draw(false)>150,"boss names, health bars and states render");
    s.boss_hud.value=false;s.hitmarker.value=true;frame.damage.Add(5,5,4,22,{},false,false,frame.time);
    expect(Draw(false)>0,"confirmed center marker renders without world position");
    s.hitmarker.value=false;s.damage_numbers.value=true;frame.damage.Add(5,5,5,25,{0,0,20},true,true,frame.time);
    expect(Draw(false)>0,"floating confirmed damage renders at victim position");
    s.damage_numbers.value=false;s.death_particles.value=true;
    frame.deaths.Add(42,{0,0,20},frame.time-150);
    expect(Draw(false)>0,"death particles render independently of ESP and damage labels");
    frame.deaths={};frame.deaths.Add(43,{0,0,20},frame.time-2100);
    expect(Draw(false)==0,"expired death bursts produce no geometry");
    s.death_particles.value=false;
    s.damage_numbers.value=false;s.offscreen.value=true;frame.threat_count=2;frame.threats[0]={{-400,0,0},source::Kind::Smoker,15};frame.threats[1]={{-500,0,0},source::Kind::Tank,20};
    const int one=Draw(false);expect(one>0,"offscreen class and arrow render");
    frame.threat_count=1;expect(Draw(false)==one,"same-sector threats do not stack arrows");
    s.offscreen.value=false;s.trajectory.value=true;frame.path.valid=true;frame.path.count=3;frame.path.points[0]={0,0,0};frame.path.points[1]={20,0,10};frame.path.points[2]={40,0,0};
    expect(Draw(false)>0,"estimated trajectory and endpoint render");
    frame.path.valid=false;expect(Draw(false)==0,"invalid traces produce no trajectory");
    s.trajectory.value=false;s.hud_editor.value=true;expect(Draw(true)>0,"editor previews empty boss panel");
    auto& input=xui::ctx().input;s.boss_x=.2f;s.boss_y=.1f;
    Draw(true);const float old=s.boss_x.value;
    input.mouse_x=8+.2f*(1280-s.boss_width.value-16)+15;input.mouse_y=8+.1f*(720-98-16)+15;input.mouse_clicked=input.mouse_down=true;
    Draw(true);input.mouse_clicked=false;input.mouse_x+=100;Draw(true);
    expect(s.boss_x.value>old,"dragging the HUD title updates persisted normalized position");
    input.mouse_down=false;Draw(true);
    xui::rect resize_box{100,100,310,98};float rw=310,rx=.2f,ry=.1f;
    input.mouse_x=405;input.mouse_y=193;input.mouse_clicked=input.mouse_down=true;
    expect(hud_widgets::Resize(&resize_box,resize_box,rw,nullptr,rx,ry,230,560),"editor resize handle captures the pointer");
    input.mouse_clicked=false;input.mouse_x+=70;
    hud_widgets::Resize(&resize_box,resize_box,rw,nullptr,rx,ry,230,560);
    expect(rw==380&&rx>=0&&rx<=1&&ry>=0&&ry<=1,"resizing updates width while preserving the screen anchor");
    input.mouse_down=false;hud_widgets::ResetInput(false);
    expect(!hud_widgets::OwnsPointer(),"closing the menu releases a resize operation");
    s.hud_opacity=.65f;expect((hud_widgets::Color({255,255,255,200})>>24)==130,"HUD opacity applies to the existing theme alpha");
    s.boss_scale=1.5f;s.boss_width=560;expect(Draw(true,640,480)>0,"HUD scale stays finite on a smaller screen");
    s.boss_hud.value=true;s.boss_width=230;frame.boss_count=4;
    for(unsigned i=0;i<4;++i)frame.bosses[i]={i+10,source::Kind::Tank,999999,1000000,0,100,true};
    frame.time=GetTickCount64();
    expect(Draw(false,640,480)>0,"narrow boss panel supports four large HP values at maximum scale");
    s.hitmarker.value=true;s.hitmarker_size=NAN;s.damage_numbers.value=true;s.damage_lifetime=NAN;
    frame.damage.Add(5,5,7,200,{0,0,30},true,true,GetTickCount64());
    expect(Draw(false)>0,"invalid cosmetic sizes use finite drawing defaults");
    s.hitmarker.value=s.damage_numbers.value=false;s.boss_hud.value=false;s.hud_editor.value=false;
    io.DisplaySize={640,480};ImGui::NewFrame();auto* transparent=ImGui::GetBackgroundDrawList();
    overlay_paint::Box(transparent,{10,10},{100,200},IM_COL32(255,100,100,0),true,1,true);
    overlay_paint::Label(transparent,{10,10},IM_COL32(255,100,100,0),"Hidden");
    expect(transparent->VtxBuffer.empty(),"zero alpha suppresses text plates, shadows and box outlines together");ImGui::Render();
    frame.time=GetTickCount64()-1000;s.hud_editor.value=false;s.boss_hud.value=true;expect(Draw(false)==0,"stale world snapshots disappear");
    licensed=false;s.hud_editor.value=true;expect(Draw(true)==0,"expired license suppresses new HUD rendering");
    ImGui::DestroyContext();std::printf("%u visual rendering checks, %u failed\n",checks,failures);return failures?1:0;
}
