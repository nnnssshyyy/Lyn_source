#pragma once
#include "client_guard_logic.h"
#include "memory.h"
#include <memory>

namespace client_guard {
struct Options {bool queries{},settings{},commands{},messages{},crosshair{},crc{},null_sound{},heartbeat{},logging{};};
struct Targets {void* send{};void* settings{};void* command{};void* crosshair{};void* dispatch{};void* sound{};};
struct Availability {bool send{},settings{},command{},crosshair{},messages{},sound{};};
// Layouts verified in the installed engine's encoders and handler dispatchers.
namespace native {
struct Vector {void* data{};int allocated{},grow{},size{};void* elements{};};
struct Cvar {char name[260]{},value[260]{};};
inline constexpr int Payload=84,QueryName=88,QueryValue=92;
static_assert(sizeof(Vector)==20&&sizeof(Cvar)==520);
}
Availability Install(Targets,memory::Range engine,memory::Range client,bool(*active)()) noexcept;
void SetOptions(Options) noexcept;
bool SetRules(const Rules&) noexcept;
std::shared_ptr<const Rules> GetRules() noexcept;
// Resolve user-message names from the native registration sequence. Never use
// network IDs as user IDs; unknown/incomplete registries disable this filter.
bool ResolveMessages(void* registration,memory::Range client) noexcept;
void Suspend() noexcept;
void Shutdown() noexcept;
}
