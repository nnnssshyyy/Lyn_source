#include "spectators.h"
#include "player_name.h"
#include "log.h"
#include <array>
#include <atomic>
#include <cstdio>
#include <algorithm>

namespace spectators {
namespace {
constexpr int InEye=4,Chase=5;
struct Offsets {
    source::ClientClass* cc{};
    int mode{-1},target{-1};
};
source::Interfaces api{};
std::array<Offsets,64> classes{};
size_t class_count{};
ULONGLONG next_scan{};
Snapshot published{};
std::atomic_bool enabled{true};
std::atomic_bool include_chase{true};
SRWLOCK collector_lock=SRWLOCK_INIT,snapshot_lock=SRWLOCK_INIT;
struct CollectLock {
    CollectLock() { AcquireSRWLockExclusive(&collector_lock); }
    ~CollectLock() { ReleaseSRWLockExclusive(&collector_lock); }
};

void Publish(const Snapshot& frame) noexcept {
    AcquireSRWLockExclusive(&snapshot_lock);
    published=frame;
    ReleaseSRWLockExclusive(&snapshot_lock);
}
Offsets Fields(source::ClientClass* address) noexcept {
    for(size_t i=0;i<class_count;++i) if(classes[i].cc==address) return classes[i];
    if(class_count>=classes.size()) return {};
    source::ClientClass cc{};
    if(!api.client_image.Contains(address,sizeof(cc)) || !memory::Read(address,cc)) return {};
    Offsets out{address,source::FindField(api.client_image,cc.table,"m_iObserverMode"),
                        source::FindField(api.client_image,cc.table,"m_hObserverTarget")};
    if(out.mode<=0 || out.target<=0) out.mode=out.target=-1;
    classes[class_count++]=out;
    if(out.mode>0) app::Log("Spectators: observer mode=%X target=%X",out.mode,out.target);
    return out;
}
void Collect() noexcept {
    Snapshot frame{};
    frame.captured=GetTickCount64();
    bool in_game{};
    if(!enabled.load(std::memory_order_acquire)) {
        next_scan=0;Publish(frame);return;
    }
    if(frame.captured<next_scan) return;
    next_scan=frame.captured+100; // Bounded 10 Hz roster scan, separate from the model/ESP collector.
    frame.state=State::Unavailable;
    if(!memory::Invoke(api.in_game,in_game,api.engine)) { Publish(frame);return; }
    if(!in_game) { frame.state=State::NotInGame;Publish(frame);return; }
    int local_index{},max_clients{};
    void* local{};
    if(!memory::Invoke(api.local_player,local_index,api.engine) ||
       !memory::Invoke(api.max_clients,max_clients,api.engine) || max_clients<1 || max_clients>int(MaxPlayers) ||
       local_index<1 || local_index>max_clients ||
       !memory::Invoke(api.entity,local,api.entities,local_index) || !local) { Publish(frame);return; }
    using GetHandleEntity=void*(__thiscall*)(void*,unsigned);
    const auto from_handle=memory::Virtual<GetHandleEntity>(api.entities,4,api.client_image);
    if(!from_handle) { Publish(frame);return; }
    bool has_players=false,has_camera_fields=false;
    for(int index=1;index<=max_clients;++index) {
        if(index==local_index) continue;
        void* player{},*network{};source::ClientClass* cc{};
        if(!memory::Invoke(api.entity,player,api.entities,index) || !player || player==local ||
           !memory::Invoke(api.networkable,network,api.entities,index) || !network ||
           !memory::Invoke(memory::Virtual<source::GetClass>(network,1,api.client_image),cc,network) || !cc) continue;
        has_players=true;
        const auto fields=Fields(cc);
        has_camera_fields=has_camera_fields || (fields.mode>0 && fields.target>0);
        int mode{};unsigned target{};void* watching{};
        // Spectators may have dormant/idle bodies. Camera netvars alone decide
        // the association; no life, team, PVS, distance or ESP filters are used.
        if(!memory::Field(player,fields.mode,mode) || (mode!=InEye && !(mode==Chase && include_chase.load(std::memory_order_acquire))) ||
           !memory::Field(player,fields.target,target) || !target || target==UINT_MAX ||
           !memory::Invoke(from_handle,watching,api.entities,target) || watching!=local) continue;
        // Resolve the full handle (including its serial); a recycled entity slot
        // must not inherit an earlier observer target or a cached player name.
        source::PlayerInfo info{};bool connected{};
        if(!memory::Invoke(api.player_info,connected,api.engine,index,&info) || !connected) continue;
        info.name[sizeof(info.name)-1]=0;
        auto& row=frame.entries[frame.count++];
        row.userid=info.userid;row.index=index;row.mode=mode;
        player_name::Copy(row.name,info.name);
        for(char& ch:row.name) {
            if(!ch) break;
            if(static_cast<unsigned char>(ch)<32 || ch==127) ch=' ';
        }
        if(!row.name[0]) std::snprintf(row.name,sizeof(row.name),"Player %d",info.userid);
    }
    std::sort(frame.entries.begin(),frame.entries.begin()+frame.count,[](const Entry& a,const Entry& b) {
        const int order=_stricmp(a.name,b.name);
        return order?order<0:a.userid<b.userid;
    });
    // Rebuild every sample, so departed observers disappear immediately.
    frame.state=(!has_players || has_camera_fields)?State::Ready:State::Unavailable;
    Publish(frame);
}
}

void Initialize(const source::Interfaces& interfaces) noexcept {
    CollectLock guard;
    api=interfaces;classes={};class_count=0;next_scan=0;Publish({});
    app::Log("Spectators: persistent window / first-person and optional chase cameras");
}
void ClearLevel() noexcept {
    CollectLock guard;
    classes={};class_count=0;next_scan=0;Publish({});
}
void SetEnabled(bool value) noexcept {
    if(enabled.exchange(value,std::memory_order_acq_rel)!=value) ClearLevel();
}
void SetChase(bool value) noexcept {
    if(include_chase.exchange(value,std::memory_order_acq_rel)!=value) ClearLevel();
}
void Frame(int stage) noexcept {
    if(stage!=4) return; // FRAME_NET_UPDATE_END, after receiving observer netvars.
    CollectLock guard;Collect();
}
Snapshot ReadSnapshot() noexcept {
    Snapshot frame{};
    AcquireSRWLockShared(&snapshot_lock);frame=published;ReleaseSRWLockShared(&snapshot_lock);
    // A missing update clears names, never the window itself.
    if(!enabled.load(std::memory_order_acquire) || !frame.captured || GetTickCount64()-frame.captured>500)
        return {};
    return frame;
}
}
