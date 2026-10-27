#include "overlay_snapshot.h"
#include <cstdio>
#include <memory>
#include <chrono>
#include <atomic>

using features::snapshot::Overlay;
__declspec(noinline) void LegacyCopy(Overlay& out,const Overlay& in) {out=in;}
__declspec(noinline) void ActiveCopy(Overlay& out,const Overlay& in) {features::snapshot::Copy(out,in);}

int main() {
    unsigned checks{},failed{};
    const auto expect=[&](bool ok,const char* name){++checks;if(!ok){++failed;std::fprintf(stderr,"FAIL: %s\n",name);}};
    const auto a=std::make_unique<Overlay>(),b=std::make_unique<Overlay>();
    a->count=a->targets.size();a->bone_count=a->bones.size();a->radar.count=a->radar.points.size();
    for(size_t i=0;i<a->count;++i) {
        a->targets[i].health=static_cast<int>(i);a->world[i]=(i&1)!=0;
        a->info[i].rage=static_cast<int>(i);a->skeletons[i]={static_cast<unsigned short>(i),1};
    }
    for(size_t i=0;i<a->bone_count;++i) a->bones[i].end.x=static_cast<float>(i);
    for(size_t i=0;i<a->radar.count;++i) a->radar.points[i].position.y=static_cast<float>(i);
    a->time=55;a->valid=true;a->has_target=true;a->aim_point={1,2,3};
    a->targets.back().bounds_source=esp::Target::BoundsSource::Render;
    a->targets.back().render_corners[7]={17,29,41};
    features::snapshot::Copy(*b,*a);
    expect(b->count==384 && b->bone_count==8192 && b->radar.count==512,"full pools retain every target, bone and radar point");
    expect(b->targets.back().health==383 && b->bones.back().end.x==8191 && b->radar.points.back().position.y==511,"copy includes pool boundary elements");
    expect(b->valid && b->time==55 && b->has_target && b->aim_point.z==3,"metadata and command marker are preserved");
    expect(b->targets.back().bounds_source==esp::Target::BoundsSource::Render&&b->targets.back().render_corners[7].z==41,
           "snapshot includes transformed corners and their geometry source at the pool boundary");
    a->count=2;a->bone_count=3;a->radar.count=1;a->targets[0].health=12;
    a->skeletons[1]={1,2};a->bones[2].end.x=42;a->radar.points[0].position.y=9;
    const auto bytes=features::snapshot::Copy(*b,*a);
    expect(b->count==2 && b->bone_count==3 && b->radar.count==1,"shrinking publishes lengths so old tails cannot appear");
    expect(b->targets[0].health==12 && b->skeletons[1].count==2 && b->bones[2].end.x==42 && b->radar.points[0].position.y==9,"small snapshots include all active animated payload");
    expect(bytes<sizeof(Overlay)/100,"small captures do not copy unused bone/target pools");
    a->count=a->bone_count=a->radar.count=0;a->valid=false;
    features::snapshot::Copy(*b,*a);
    expect(!b->valid && !b->count && !b->bone_count && !b->radar.count,"map/disabled captures invalidate all previous geometry immediately");
    a->count=a->bone_count=a->radar.count=999999;
    features::snapshot::Copy(*b,*a);
    expect(b->count==384 && b->bone_count==8192 && b->radar.count==512,"invalid lengths are bounded before copying");
    features::Config first{},second{};
    expect(first==second,"identical defaults compare by values");
    second.weapons.back().vm_roll=20;
    expect(!(first==second),"a change in the last weapon profile is not skipped by config deduplication");
    first=second;second.chat_panel=!first.chat_panel;
    expect(!(first==second),"a presentation setting still triggers publication");
    first=second;second.skeleton_color_infected^=0x100;
    expect(!(first==second),"color changes are detected without relying on struct padding");
    // A repeatable memory-copy benchmark, not an in-game FPS measurement.
    a->count=20;a->bone_count=500;a->radar.count=20;a->valid=true;
    const auto active_bytes=features::snapshot::Copy(*b,*a);
    const auto measure=[&](auto copy) {
        const auto begin=std::chrono::steady_clock::now();
        for(int i=0;i<4000;++i) {a->time=static_cast<ULONGLONG>(i);copy(*b,*a);std::atomic_signal_fence(std::memory_order_seq_cst);}
        return std::chrono::duration<double,std::micro>(std::chrono::steady_clock::now()-begin).count()/4000.;
    };
    const auto full_us=measure(LegacyCopy),active_us=measure(ActiveCopy);
    std::printf("Snapshot copy: %zu -> %zu bytes (20 targets, 500 bones), %.3f -> %.3f us/copy\n",sizeof(Overlay),active_bytes,full_us,active_us);
    std::printf("%u snapshot/config performance checks, %u failed\n",checks,failed);return failed?1:0;
}
