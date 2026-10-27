#include "aimbot.h"
#include "aim_camera.h"
#include "aim_tracking.h"
#include "features.h"
#include <cstdio>
#include <cmath>

namespace {
struct Body {float simulation{9.95f};int sequence{};};
struct Fixture {Body body{};game::Entity entity{};std::vector<game::Point> points;bool alive{true},visible{true},block_live{},empty_live{};float blocked_y{99999.f},blocked_fraction{.5f};const char* sequence_name{"Run"};};
std::array<Fixture,2> targets;
game::Entity local{},weapon{};
game::Fields player_fields{},weapon_fields{},target_fields{};
game::Globals current_clock{};
float bone_time{};
unsigned bone_calls{},sequence_calls{};
bool steam_friend{};
int model_token{};
void* model_table[9]{};
struct Renderable {void** table{model_table};} renderable;
const void* __fastcall GetModel(void*) {return &model_token;}
void Place(int i,float x,float y) {
    auto& t=targets[i];t.entity.origin={x,y,0};
    t.points={{{x,y,60},1,1,0,0,3},{{x,y-3,60},1,1,0,1,.6f},{{x,y+3,60},1,1,0,2,.6f},{{x,y,45},2,2,1,0,8}};
}
}

// A deterministic engine boundary. The selector/point policy below is the
// actual production aimbot.cpp; only entity, bones, visibility and clocks are mocked.
namespace game {
Vec3 Add(Vec3 a,Vec3 b) noexcept {return {a.x+b.x,a.y+b.y,a.z+b.z};}
Vec3 Sub(Vec3 a,Vec3 b) noexcept {return {a.x-b.x,a.y-b.y,a.z-b.z};}
Vec3 Mul(Vec3 a,float s) noexcept {return {a.x*s,a.y*s,a.z*s};}
float Length(Vec3 a) noexcept {return std::sqrt(a.x*a.x+a.y*a.y+a.z*a.z);}
float Normalize(float a) noexcept {return std::remainder(a,360.f);}
Vec3 Angle(Vec3 a,Vec3 b) noexcept {const auto d=Sub(b,a);return {-std::atan2(d.z,std::hypot(d.x,d.y))*57.2957795f,std::atan2(d.y,d.x)*57.2957795f,0};}
float Fov(Vec3 a,Vec3 b) noexcept {return std::hypot(Normalize(a.x-b.x),Normalize(a.y-b.y));}
bool Api::Read(int index,Entity& e,bool,bool) const noexcept {
    if(index<2 || index>3 || !targets[index-2].alive) return false;
    e=targets[index-2].entity;return true;
}
bool Api::Local(Entity& e,bool,bool) const noexcept {e=local;return true;}
bool Api::Clock(Globals& c) const noexcept {c=current_clock;return true;}
bool Api::Sequence(const Entity& entity,char* name,size_t capacity) const noexcept {
    ++sequence_calls;
    const auto* sequence=targets[entity.index-2].sequence_name;
    if(!sequence) return false;
    return std::snprintf(name,capacity,"%s",sequence)>=0;
}
bool Api::Friend(const Entity&,bool& value) const noexcept {value=steam_friend;return true;}
bool Api::Hitboxes(const Entity& e,std::vector<Point>& p,float time) const {
    ++bone_calls;bone_time=time;p=targets[e.index-2].points;return !p.empty();
}
float Api::Interpolation(float) const noexcept {return .05f;}
float Api::NetworkLatency() const noexcept {return .05f;}
bool Api::Cast(Vec3,Vec3 end,Filter&,Trace& tr,unsigned,float) const noexcept {
    tr={};tr.end=end;tr.fraction=1.f;
    for(const auto& t:targets) if(std::abs(end.x-t.entity.origin.x)<.001f) {
        const bool live=std::any_of(t.points.begin(),t.points.end(),[&](const auto& p){return Length(Sub(p.position,end))<.001f;});
        if(!t.visible || (t.block_live && live) || std::abs(end.y-t.blocked_y)<.001f) {tr.fraction=t.blocked_fraction;return true;}
        if(t.empty_live && live) return true;
        tr.entity=t.entity.ptr;tr.hitgroup=end.z<50.f?2:1;return true;
    }
    return true;
}
}

int main() {
    unsigned checks{},failed{};
    const auto expect=[&](bool ok,const char* name){++checks;if(!ok){++failed;std::fprintf(stderr,"FAIL: %s\n",name);}};
    game::Api api;api.base.client_image=memory::Module(GetModuleHandleW(nullptr));
    local.handle=0x1001;local.ptr=&local;local.team=2;
    weapon.handle=0x1005;weapon.ptr=&weapon;weapon.fields=&weapon_fields;
    weapon_fields.weapon_group=2;std::snprintf(weapon_fields.name,sizeof(weapon_fields.name),"CRifle_AK47");
    target_fields.kind=source::Kind::Common;target_fields.simulation=offsetof(Body,simulation);
    model_table[8]=reinterpret_cast<void*>(&GetModel);
    current_clock.time=10.f;current_clock.interval=1.f/30.f;
    for(int i=0;i<2;++i) {
        auto& t=targets[i];t.entity.ptr=&t.body;t.entity.renderable=&renderable;t.entity.fields=&target_fields;
        t.entity.index=i+2;t.entity.handle=0x1002+i;t.entity.team=3;t.entity.kind=source::Kind::Common;
        t.entity.mins={-4,-4,0};t.entity.maxs={4,4,120};
    }
    features::Config cfg;cfg.aim=true;cfg.aim_mode=0;cfg.priority=0;cfg.hitbox=1;cfg.fov=20.f;cfg.history=false;
    const int indices[]{2,3};game::Command command{};command.number=1;command.tick=300;
    aimbot::Result result{};
    const auto choose=[&] {++command.number;return aimbot::Select(api,cfg,local,weapon,command,current_clock,{0,0,60},indices,result);};
    aimbot::Reset();Place(0,1000,35);Place(1,1100,43);
    expect(choose() && result.entity.index==2 && result.sample==0,"normal acquires a head center instead of the closest edge");
    Place(1,1100,36);
    expect(choose() && result.entity.index==2,"small FOV ranking changes do not alternate targets");
    Place(1,1100,3);
    expect(choose() && result.entity.index==3,"a clearly better FOV target can replace the retained target");
    targets[1].visible=false;
    expect(choose() && result.entity.index==2,"an occluded retained target releases immediately");
    targets[1].visible=true;targets[1].alive=false;
    Place(0,1000,35);aimbot::ReleaseTarget();choose();
    targets[0].blocked_y=35;
    expect(choose() && result.sample==1,"visible edge remains available when the center is covered");
    targets[0].blocked_y=99999.f;
    expect(choose() && result.sample==0,"exposed center replaces a narrow edge to recover interior hit margin");
    targets[0].blocked_y=32;
    expect(choose() && result.sample==0,"a covered retained edge switches to a visible center");
    targets[0].blocked_y=99999.f;cfg.hitbox=2;
    expect(choose() && result.hitgroup==2,"changing the hitbox preset releases the old point");
    cfg.hitbox=1;cfg.priority=1;targets[1].alive=true;
    Place(0,1000,0);Place(1,1020,0);aimbot::ReleaseTarget();choose();
    Place(1,990,0);
    expect(choose() && result.entity.index==2,"nearest mode tolerates small distance swaps");
    Place(1,600,0);
    expect(choose() && result.entity.index==3,"nearest mode still switches for a significant improvement");
    cfg.priority=2;Place(0,1000,0);Place(1,1020,0);aimbot::Reset();choose();
    aimbot::OnShot(result,current_clock,.1f);
    expect(choose() && result.entity.index==2,"normal does not abandon a living target just because a shot was queued");
    cfg.aim_mode=1;aimbot::Reset();choose();aimbot::OnShot(result,current_clock,.1f);
    expect(choose() && result.entity.index==2,"client silent retains a living target during acknowledgement latency");
    cfg.aim_mode=2;aimbot::Reset();choose();aimbot::OnShot(result,current_clock,.1f);
    expect(choose() && result.entity.index==2,"server silent keeps a living target until its death is actually observed");
    targets[0].alive=false;
    expect(choose() && result.entity.index==3,"dead target is removed without a sticky grace period");
    targets[0].alive=true;targets[1].alive=false;
    cfg.priority=0;cfg.history=true;Place(0,1000,10);aimbot::Reset();aimbot::Capture(api,cfg,indices);
    Place(0,1000,20);
    expect(choose() && !result.historical && result.point.y==20 && bone_time==10.f,"client silent chooses live bones using the animation clock");
    cfg.aim_mode=0;
    expect(choose() && !result.historical && result.point.y==20,"normal also prefers the live pose over valid old history");
    targets[0].block_live=true;
    expect(choose() && result.historical && result.point.y==10,"visible valid history remains a fallback when the live pose is covered");
    targets[0].block_live=false;
    expect(choose() && !result.historical && result.point.y==20,"tracking resumes the current pose as soon as it is visible");
    targets[0].visible=false;
    expect(!choose(),"normal refuses both live and historical rays behind a wall");
    targets[0].visible=true;cfg.aim_mode=2;
    expect(choose() && !result.historical && result.point.y==20 && result.sample==0,
           "server silent selects exposed current bones instead of a different historical position");
    for(int mode:{0,1,2}) {
        cfg.aim_mode=mode;targets[0].empty_live=true;
        expect(choose() && result.historical && result.point.y==10,"empty current ray falls back only to a validated historical record in every mode");
        cfg.history=false;
        expect(!choose(),"empty live trace cannot authorize an aimed shot in any mode");
        targets[0].empty_live=false;targets[0].visible=false;targets[0].blocked_fraction=.9995f;
        expect(!choose(),"a wall near the endpoint is still an obstruction in every mode");
        targets[0].visible=true;targets[0].blocked_fraction=.5f;cfg.history=true;
        expect(choose() && !result.historical && result.point.y==20,"current confirmed hit is preferred again when it becomes available");
    }
    cfg.aim_mode=0;cfg.history=false;aimbot::ReleaseTarget();choose();
    targets[0].entity.handle+=0x1000;Place(0,1000,30);
    expect(choose() && result.entity.handle==targets[0].entity.handle && result.sample==0,"reused entity slots do not inherit old point identity");
    expect(result.traces<=192,"retention obeys the existing trace budget");
    cfg.priority=2;targets[1].alive=true;Place(1,1020,0);aimbot::Reset();choose();
    targets[1].entity.kind=source::Kind::Smoker;
    expect(choose() && result.entity.index==3,"a higher smart-shot threat overrides retention immediately");
    targets[1].visible=false;
    expect(choose() && result.entity.index==2,"an occluded higher threat does not starve a visible retained candidate");

    // Dedicated Versus opponents use player slots, unlike common-infected
    // fixtures. Exercise the real selector and its rejection diagnostics.
    targets[1].alive=false;targets[0].entity.kind=source::Kind::Smoker;
    cfg.aim_mode=0;cfg.history=false;cfg.target_special=true;
    Place(0,1000,10);aimbot::Reset();
    expect(choose() && result.entity.index==2,"normal acquires a human-slot special infected in Versus");
    steam_friend=true;aimbot::Reset();
    expect(!choose() && result.filtered_friends==1 && result.candidates==0,
           "Steam friend rejection is visible instead of looking like a missing dedicated-server target");
    cfg.target_friends=true;
    expect(choose() && !result.filtered_friends,"explicit target-friends setting restores normal tracking");
    cfg.target_special=false;
    expect(!choose() && result.filtered_types==1,"disabled special-infected filter is reported separately");
    cfg.target_special=true;targets[0].entity.ghost=true;
    expect(!choose() && result.filtered_ghosts==1 && !result.traces,"replicated Versus ghosts are counted but never aimed at");
    targets[0].entity.ghost=false;targets[0].entity.kind=source::Kind::Survivor;
    targets[0].entity.team=2;local.team=3;
    expect(!choose() && result.filtered_types==1,"infected player needs the enemy-survivor filter to acquire survivors");
    cfg.target_survivor=true;
    expect(choose(),"enabling enemy survivors restores acquisition for the infected team");
    local.team=2;targets[0].entity.team=3;targets[0].entity.kind=source::Kind::Smoker;
    cfg.aim_distance=1.f;
    expect(!choose() && result.outside_range==1,"range rejection is distinct from target filters");
    cfg.aim_distance=150.f;cfg.fov=1.f;Place(0,1000,200);
    expect(!choose() && result.outside_fov==1,"coarse FOV rejection is distinct from missing hitboxes");
    cfg.activation=2;cfg.auto_fire=false;
    expect(features::ShouldAutoShoot(cfg,true,0.f),"always initiates fire without manual attack or the optional auto-shoot switch");
    for(int mode:{0,1,2}) {
        cfg.aim_mode=mode;
        expect(features::ShouldAutoShoot(cfg,true,0.f),"always initiates fire in every aim mode");
        expect(!features::ShouldAutoShoot(cfg,false,0.f),"always still respects weapon readiness in every mode");
    }
    cfg.aim_mode=0;
    expect(!features::ShouldAutoShoot(cfg,true,2.f),"always waits for normal aim smoothing to align");
    for(int activation:{0,1}) {
        cfg.activation=activation;
        expect(!features::ShouldAutoShoot(cfg,true,0.f),"manual activation modes keep their saved auto-shoot preference");
    }
    cfg.activation=2;
    auto inherited=cfg;inherited.weapons[0].combat=true;inherited.weapons[0].aim=true;inherited.weapons[0].auto_fire=false;
    features::ApplyWeaponProfile(inherited,0);
    expect(features::ShouldAutoShoot(inherited,true,0.f),"a per-weapon auto-shoot override cannot silently disable Always");
    cfg.auto_fire=true;
    expect(features::ShouldAutoShoot(cfg,true,0.f) && !features::ShouldAutoShoot(cfg,false,0.f),
           "automatic shots still require weapon readiness");
    expect(!features::ShouldAutoShoot(cfg,true,2.f),"normal smoothing must align before automatic fire");
    cfg.aim_mode=1;
    expect(features::ShouldAutoShoot(cfg,true,2.f),"client silent preserves its direct shot policy");

    aimbot::VisibleCamera camera;
    camera.Update(local.handle,1000);
    expect(camera.Active(local.handle,1100) && !camera.Active(local.handle+0x1000,1100),"camera lease is tied to the complete local handle");
    expect(!camera.Active(local.handle,1250),"silent camera lease expires across long cooldowns");
    const auto first=aimbot::VisibleViewAngles({4,10,0},{1,2,0},false);
    const auto moved=aimbot::VisibleViewAngles({5,20,0},{1,2,0},false);
    expect(first.x==5 && first.y==12 && moved.x==6 && moved.y==22,"isolated camera follows fresh mouse input and native recoil");
    const auto hidden=aimbot::VisibleViewAngles({5,20,0},{1,2,0},true);
    expect(hidden.x==5 && hidden.y==20,"hidden visual recoil is not subtracted twice");
    camera.Reset();expect(!camera.Active(local.handle,1100),"map/menu reset releases isolated view");

    cfg.aim_mode=0;cfg.auto_fire=true;
    const float head_tolerance=aimbot::logic::ShotTolerance(1000.f,3.f);
    expect(!features::ShouldAutoShoot(cfg,true,.4f,head_tolerance),
           "normal does not auto-fire outside a small distant head despite being within the former half-degree threshold");
    expect(features::ShouldAutoShoot(cfg,true,.05f,head_tolerance),"aligned normal automatic shots remain available");
    for(int mode:{0,1,2}) {
        cfg.aim_mode=mode;cfg.history=false;cfg.hitbox=1;cfg.fov=20.f;cfg.priority=0;
        aimbot::Reset();Place(0,1000,10);
        expect(choose() && result.sample==0 && result.radius==3.f,"each aim mode receives center geometry for the real selected hitbox");
        Place(0,1000,30);
        expect(choose() && result.point.y==30.f && !result.historical,"each aim mode follows the latest moving pose without stale history");
    }
    aimbot::NormalTracking tracking;
    game::Vec3 view{},goal{};
    tracking.Update(1,2,3,1,0,0,view,goal,75.f,1.f/30.f);
    bool follows=true;
    for(int n=2;n<40;++n) {
        goal.y+=.4f;view=tracking.Update(1,2,3,n,0,0,view,goal,75.f,1.f/30.f);
        follows=follows && std::abs(view.y-goal.y)<.0001f;
    }
    expect(follows,"normal follows measured angular motion without accumulating smoothing lag");
    const auto replay=tracking.Update(1,2,3,39,0,0,view,goal,75.f,1.f/30.f);
    expect(replay.y==view.y,"reprocessing a normal command does not advance smoothing or motion twice");
    const auto acquire=tracking.Update(1,2,4,40,0,0,{0,0,0},{0,10,0},75.f,1.f/30.f);
    expect(std::abs(acquire.y-2.5f)<.0001f,"a new target retains configured acquisition smoothing");
    const auto jump=tracking.Update(1,2,4,41,0,0,acquire,{0,80,0},75.f,1.f/30.f);
    expect(jump.y<30.f,"a discontinuous pose cannot produce a feed-forward camera snap");
    tracking.Reset();
    const auto wrap1=tracking.Update(1,2,3,1,0,0,{0,179,0},{0,179,0},75.f,1.f/30.f);
    const auto wrap2=tracking.Update(1,2,3,2,0,0,wrap1,{0,-179,0},75.f,1.f/30.f);
    expect(std::abs(wrap2.y+179.f)<.0001f,"measured motion crosses yaw wrap along the short arc");
    const auto manual=tracking.Update(1,2,3,3,0,0,{0,-178,0},{0,-178,0},75.f,1.f/30.f);
    expect(manual.y==-178.f,"mouse movement that already follows the target is not compensated twice");
    for(float smoothing:{0.f,35.f,75.f,95.f}) {
        tracking.Reset();game::Vec3 input{};const game::Vec3 destination{3,6,0};
        bool fired{},premature{};
        for(int number=1;number<=400 && !fired;++number) {
            input=tracking.Update(1,2,3,number,0,0,input,destination,smoothing,1.f/30.f);
            const float error=game::Fov(input,destination);
            const int buttons=aimbot::logic::ShotButtons(1|4,0,true,false,error,head_tolerance);
            if(buttons&1) {fired=true;premature=error>head_tolerance;}
            expect((buttons&4)!=0,"aim alignment gating preserves simultaneous movement input");
        }
        expect(fired && !premature,"manual automatic-weapon fire first opens only inside the hitbox tolerance at every smoothing level");
    }
    cfg.aim_mode=2;cfg.history=true;aimbot::Reset();targets[0].body.simulation=9.99f;
    Place(0,1000,10);aimbot::Capture(api,cfg,indices);Place(0,1000,30);
    targets[0].block_live=true;
    expect(choose() && result.historical && std::abs(result.simulation_time-9.95f)<.00001f,
           "rendered history is timestamped at interpolation time rather than the newer network simulation time");
    targets[0].block_live=false;
    command.tick=10;
    expect(choose() && !result.historical && result.point.y==30.f,
           "history that cannot produce a valid shot tick falls back to current bones");
    const auto previous_bones=bone_calls;current_clock.time=10.005f;
    aimbot::Capture(api,cfg,indices);
    expect(bone_calls==previous_bones,"high render FPS does not rebuild historical points multiple times within one command interval");
    current_clock.time=10.04f;targets[0].body.simulation=10.f;
    aimbot::Capture(api,cfg,indices);
    expect(bone_calls==previous_bones+1,"history resumes capture after one command interval");
    cfg.aim_mode=0;cfg.hitbox=0;cfg.history=false;cfg.ignore_idle=false;cfg.ignore_burning=false;
    cfg.target_special=true;cfg.target_tank=true;targets[1].alive=false;
    for(const auto kind:{source::Kind::Smoker,source::Kind::Boomer,source::Kind::Hunter,
                        source::Kind::Spitter,source::Kind::Jockey,source::Kind::Charger,source::Kind::Tank}) {
        aimbot::Reset();Place(0,1000,0);targets[0].entity.kind=kind;
        targets[0].points.clear();targets[0].points.push_back({{1000,0,40},2,0,1,0,8.f});
        expect(choose() && result.hitgroup==2,"automatic aim falls back to a torso zone for every special infected");
    }
    steam_friend=false;
    for(int mode:{0,1,2}) for(int policy:{0,1,2}) {
        cfg=features::Config{};cfg.aim=true;cfg.aim_mode=mode;cfg.priority=policy;
        cfg.hitbox=1;cfg.history=false;cfg.fov=20.f;cfg.ignore_idle=false;
        for(auto& target:targets) {
            target.alive=target.visible=true;target.block_live=false;target.blocked_y=99999.f;
            target.entity.ghost=false;target.entity.team=3;target.sequence_name="Run";
        }
        targets[0].entity.kind=source::Kind::Smoker;targets[1].entity.kind=source::Kind::Charger;
        Place(0,1000,0);Place(1,1200,40);aimbot::Reset();
        expect(choose() && result.entity.index==2,"unmarked classes retain the existing FOV, distance and smart selection in every mode");
        cfg.priority_charger=true;
        expect(choose() && result.entity.index==2,"class marks remain inactive while the master checkbox is disabled");
        cfg.prioritize_classes=true;
        expect(choose() && result.entity.index==3,"a selected class overrides both an easier target and retained aim in every mode and policy");
        targets[1].visible=false;
        expect(!choose(),"an occluded marked class never falls back to an unmarked target");
        targets[1].visible=true;
        expect(choose() && result.entity.index==3,"a prioritized target regains precedence when it becomes visible");
        targets[1].entity.ghost=true;
        expect(!choose(),"class selection never permits a ghost or an unmarked fallback");
        targets[1].entity.ghost=false;Place(1,1200,3000);
        expect(!choose(),"class selection cannot bypass the configured FOV");
        Place(1,10000,0);
        expect(!choose(),"class selection cannot bypass the configured range");
        Place(1,1200,40);targets[0].entity.kind=source::Kind::Common;cfg.target_special=false;
        expect(!choose(),"a marked special remains excluded when its target category is disabled without a common fallback");
        cfg.target_special=true;
        expect(choose() && result.entity.index==3,"a marked special takes precedence over commons once enabled");
        cfg.priority_charger=false;cfg.priority_smoker=true;targets[0].entity.kind=source::Kind::Smoker;
        expect(choose() && result.entity.index==2,"changing the marked classes immediately releases the former priority");
        expect(result.traces<=192,"special class priority keeps the existing bounded trace budget");
        cfg.priority_smoker=false;
        expect(!choose() && result.candidates==0 && result.traces==0,
               "an enabled empty class selection never targets or traces an unmarked infected");
        cfg.prioritize_classes=false;
        expect(choose() && result.entity.index==2,"disabling class selection immediately restores ordinary target filters");
    }
    const source::Kind special_kinds[]{source::Kind::Smoker,source::Kind::Boomer,source::Kind::Hunter,
        source::Kind::Spitter,source::Kind::Jockey,source::Kind::Charger,source::Kind::Tank};
    bool features::Config::* const priority_flags[]{&features::Config::priority_smoker,&features::Config::priority_boomer,
        &features::Config::priority_hunter,&features::Config::priority_spitter,&features::Config::priority_jockey,
        &features::Config::priority_charger,&features::Config::priority_tank};
    for(size_t i=0;i<std::size(special_kinds);++i) {
        cfg=features::Config{};cfg.aim=true;cfg.aim_mode=1;cfg.priority=0;cfg.hitbox=1;cfg.history=false;cfg.fov=20.f;
        cfg.prioritize_classes=true;
        cfg.*priority_flags[i]=true;
        targets[0].entity.kind=source::Kind::Common;targets[1].entity.kind=special_kinds[i];
        Place(0,1000,0);Place(1,1200,40);aimbot::Reset();
        expect(choose() && result.entity.index==3,"each of the seven independently selected special classes outranks a closer common");
        if(special_kinds[i]==source::Kind::Tank) {
            cfg.target_tank=false;
            expect(!choose(),"Tank selection respects its separate target switch without a common fallback");
        }
        for(const auto candidate:special_kinds) {
            targets[0].entity.kind=candidate;
            cfg.target_tank=true;
            expect(aimbot::Eligible(api,cfg,targets[0].entity,local)==(candidate==special_kinds[i]),
                   "each class mask admits its marked class and rejects all six unmarked special classes");
        }
    }
    cfg=features::Config{};cfg.aim=true;cfg.aim_mode=0;cfg.priority=0;cfg.hitbox=1;cfg.history=false;cfg.fov=20.f;
    cfg.prioritize_classes=true;
    cfg.priority_smoker=cfg.priority_charger=true;
    targets[0].entity.kind=source::Kind::Smoker;targets[1].entity.kind=source::Kind::Charger;
    Place(0,1000,0);Place(1,1200,40);aimbot::Reset();
    expect(choose() && result.entity.index==2,"multiple marked classes use the configured policy within their priority band");
    expect(!features::SpecialPriorityBit(source::Kind::Witch) && !features::SpecialPriorityBit(source::Kind::Survivor) &&
           !features::SpecialPriorityBit(source::Kind::Common),"special priority never leaks to the adjacent Witch enum or other target types");

    for(int mode:{0,1,2}) {
        cfg=features::Config{};cfg.aim=true;cfg.aim_mode=mode;cfg.priority=0;cfg.hitbox=1;cfg.history=true;cfg.fov=20.f;
        current_clock.time=10.04f;
        for(auto& target:targets) {target.body.simulation=10.f;target.visible=target.alive=true;target.entity.ghost=false;}
        targets[0].entity.kind=source::Kind::Common;targets[1].entity.kind=source::Kind::Charger;
        Place(0,1000,0);Place(1,1200,40);aimbot::Reset();aimbot::Capture(api,cfg,indices);
        expect(choose() && result.entity.index==2,"ordinary selection can retain a common before class filtering is enabled");
        cfg.prioritize_classes=cfg.priority_charger=true;
        expect(choose() && result.entity.index==3,"class filtering releases an unmarked retained target even with saved pose history");
        targets[1].visible=false;
        expect(!choose(),"saved history cannot reintroduce an unmarked common when the selected special is occluded");
        cfg.prioritize_classes=false;
        expect(choose() && result.entity.index==2,"disabling class filtering restores the ordinary target with history enabled");
    }

    // Commons often retain solidity and have no replicated health while their
    // death sequence plays. They remain readable here, unlike the earlier
    // fixture that removed an entity as soon as its alive flag became false.
    target_fields.sequence=offsetof(Body,sequence);
    current_clock.time=10.f;command.tick=300;steam_friend=false;
    for(int mode:{0,1,2}) for(bool with_history:{false,true}) {
        cfg=features::Config{};cfg.aim=true;cfg.aim_mode=mode;cfg.hitbox=1;
        cfg.fov=20.f;cfg.history=with_history;cfg.ignore_idle=false;cfg.ignore_burning=false;
        for(auto& target:targets) {
            target.alive=target.visible=true;target.block_live=false;target.blocked_y=99999.f;
            target.entity.kind=source::Kind::Common;target.entity.health=-1;
            target.body.sequence=0;target.body.simulation=9.99f;target.sequence_name="Run";
        }
        Place(0,1000,0);Place(1,1100,40);aimbot::Reset();
        expect(choose() && result.entity.index==2,"every mode initially acquires the living common");
        if(with_history) aimbot::Capture(api,cfg,indices);
        targets[0].body.sequence=1;targets[0].sequence_name="Death_Standing_01";
        expect(choose() && result.entity.index==3 && result.candidates==1,
               "every mode releases a still-solid dying common and selects a living enemy");
        targets[1].body.sequence=1;targets[1].sequence_name="Death_Headshot";
        expect(!choose() && !result.traces,"readable corpses cannot produce current or historical aim rays");
        targets[0].body.sequence=2;targets[0].sequence_name="Run";targets[0].points.clear();
        expect(!choose(),"a dead common's cached pose cannot be reused after its death state clears");
        Place(0,1000,0);
        expect(choose() && result.entity.index==2,"fresh living geometry remains selectable after death invalidation");
        targets[0].body.sequence=3;targets[0].sequence_name="death_standing_01";
        expect(!choose(),"death sequence rejection is independent of letter case and optional ignore filters");
        targets[0].entity.handle+=0x1000;targets[0].sequence_name="Run";
        expect(choose(),"a new serial in the same slot cannot inherit the preceding corpse's death state");
        const auto cached_lookups=sequence_calls;choose();
        expect(sequence_calls==cached_lookups,"unchanged sequences use the cached death state without extra label lookups");
        targets[0].body.sequence=4;targets[0].sequence_name=nullptr;choose();
        targets[0].sequence_name="Death_Standing_01";
        expect(!choose(),"a transient sequence-label failure is retried even when its numeric ID stays unchanged");
    }
    cfg=features::Config{};cfg.aim=true;cfg.prioritize_classes=true;cfg.priority_smoker=true;
    cfg.target_survivor=false;cfg.history=true;cfg.aim_mode=2;
    local.team=3;local.health=100;local.kind=source::Kind::Hunter;local.fields=&player_fields;
    for(int i=0;i<2;++i) {
        targets[i].alive=true;targets[i].visible=true;targets[i].block_live=false;
        targets[i].sequence_name="Run";targets[i].body.sequence=20;
        targets[i].entity.kind=source::Kind::Survivor;targets[i].entity.team=2;
        targets[i].entity.ghost=false;Place(i,300.f+i*200,0);
    }
    aimbot::Reset();
    const auto infected=[&] {++command.number;return aimbot::SelectInfected(api,cfg,local,command,current_clock,{0,0,60},indices,result);};
    for(const auto kind:{source::Kind::Smoker,source::Kind::Boomer,source::Kind::Hunter,
        source::Kind::Spitter,source::Kind::Jockey,source::Kind::Charger,source::Kind::Tank}) {
        local.kind=kind;
        expect(infected() && result.entity.kind==source::Kind::Survivor && result.hitgroup==2 && !result.historical,
            "each infected class tracks a live survivor torso without a firearm or survivor-side filters");
    }
    expect(cfg.aim_mode==2 && cfg.history && cfg.prioritize_classes && !cfg.target_survivor,
        "infected selection never rewrites the saved survivor configuration");
    targets[0].entity.team=3;targets[1].entity.team=3;
    expect(!infected(),"infected aim never selects teammates");
    targets[0].entity.team=2;targets[0].entity.kind=source::Kind::Common;
    expect(!infected(),"infected aim only accepts survivors even with an anomalous enemy team");
    targets[0].entity.kind=source::Kind::Survivor;targets[0].visible=false;
    expect(!infected(),"infected aim does not use historical poses through cover");
    targets[0].visible=true;targets[0].entity.ghost=true;
    expect(!infected(),"replicated ghosts remain unavailable");
    targets[0].entity.ghost=false;local.ghost=true;
    expect(!infected(),"local ghost cannot aim or trigger a spawn");
    local.ghost=false;local.health=0;expect(!infected(),"dead infected cannot acquire targets");
    local.health=100;cfg.infected_aim=false;expect(!infected(),"infected toggle disables only this selector");
    cfg.infected_aim=true;cfg.aim=false;expect(!infected(),"main aim switch still controls infected aim");
    cfg.aim=true;cfg.fov=.1f;Place(0,300,100);expect(!infected(),"infected selection respects the configured FOV");
    cfg.fov=180;cfg.aim_distance=1;expect(!infected(),"infected selection respects configured distance");
    cfg.aim_distance=150;steam_friend=true;aimbot::Reset();expect(!infected(),"infected selection respects Steam friend exclusion");
    cfg.target_friends=true;expect(infected(),"explicit Steam friend opt-in still works");steam_friend=false;
    local.kind=source::Kind::Witch;expect(!infected(),"only the seven playable infected classes are supported");
    cfg=features::Config{};cfg.aim=true;cfg.aim_key=VK_XBUTTON1;cfg.infected_aim_key=0x46;
    for(int activation:{0,1,2}) {
        cfg.activation=activation;
        expect(features::InfectedAimRequested(cfg,true,true,false),"infected hold uses its own key in every survivor activation mode");
        expect(!features::InfectedAimRequested(cfg,false,true,false),"releasing infected hold always stops, including always and on-attack modes");
    }
    cfg.infected_aim_key=0;expect(!features::InfectedAimRequested(cfg,true,true,false),"unassigned infected key cannot activate aim");
    cfg.infected_aim_key=256;expect(!features::InfectedAimRequested(cfg,true,true,false),"invalid infected key cannot activate aim");
    cfg.infected_aim_key=0x46;cfg.aim=false;expect(!features::InfectedAimRequested(cfg,true,true,false),"main aim switch gates the independent hold");
    cfg.aim=true;cfg.infected_aim=false;expect(!features::InfectedAimRequested(cfg,true,true,false),"infected switch gates its hold");
    cfg.infected_aim=true;expect(!features::InfectedAimRequested(cfg,true,false,false),"focus loss releases infected hold");
    expect(!features::InfectedAimRequested(cfg,true,true,true),"flush commands cannot activate infected hold");
    std::printf("%u aim selection / camera checks, %u failed\n",checks,failed);return failed?1:0;
}
