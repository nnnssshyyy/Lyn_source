#pragma once
#include <cstdint>
#include <string_view>
#include <string>

namespace licensing {
struct Identity { std::uint32_t pid; std::uint64_t created; };
inline constexpr std::string_view PublicX="5N647usF0bNqc5qiBTKAA5ciZgA3BvgegkqiiHusrMM";
inline constexpr std::string_view PublicY="bdjo7-fe4MCu_xtzV6mDB2c9dcwN8goXFxt1hIApxzA";
Identity Current() noexcept;
std::uint64_t Now() noexcept;
bool VerifySignature(std::string_view payload,std::string_view signature,std::string_view x=PublicX,std::string_view y=PublicY) noexcept;
bool Exchange(std::string_view request,std::string& response) noexcept;
// Signature verification is independent of the game and of the transport.
bool Verify(std::string_view wire, Identity identity, std::uint64_t now,
            std::string_view public_x, std::string_view public_y, std::uint64_t& expires) noexcept;
bool Active() noexcept;
bool RefreshFromLoader() noexcept;
}
