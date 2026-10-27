#pragma once
#include "license.h"
#include <MinHook.h>
#include <span>
namespace remote_hooks {
std::string Request(std::span<const MH_REMOTE_PATCH> rows,std::string_view nonce);
bool VerifyPlan(std::string_view wire,std::span<MH_REMOTE_PATCH> rows,licensing::Identity identity,
                std::string_view nonce,std::uint64_t now,std::string_view x=licensing::PublicX,std::string_view y=licensing::PublicY) noexcept;
// Worker thread only. No HTTP, pipe, or signature work on the game/render path.
int Pump() noexcept;
}
