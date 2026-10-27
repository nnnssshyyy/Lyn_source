#pragma once
#include <algorithm>
#include <cstdint>
namespace capture_overlay {
class FrameBudget {
    std::uint64_t next_{};
    unsigned rate_{60};
public:
    void Reset(unsigned refresh=60) noexcept {rate_=std::clamp(refresh,60u,90u);next_=0;}
    bool Due(std::uint64_t now) const noexcept {return !next_||now>=next_;}
    void Submitted(std::uint64_t now) noexcept {
        const auto period=1000000u/rate_;
        next_=!next_||now>next_+period?now+period:next_+period;
    }
    unsigned Rate() const noexcept {return rate_;}
};
}
