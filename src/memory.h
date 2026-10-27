#pragma once
#include <Windows.h>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace memory {
static_assert(sizeof(void*) == 4, "L4D2 x86 ABI required");
bool Copy(void* destination, const void* source, size_t size) noexcept;
bool Executable(const void* address) noexcept;
int FaultFilter(unsigned long code) noexcept;

struct Range {
    uintptr_t base{};
    size_t size{};
    bool Contains(const void* pointer, size_t count = 1) const noexcept {
        const auto value = reinterpret_cast<uintptr_t>(pointer);
        return base && value >= base && count <= size && value - base <= size - count;
    }
};
Range Module(HMODULE module) noexcept;

template<class T> bool Read(const void* address, T& value) noexcept {
    static_assert(std::is_trivially_copyable_v<T>);
    return Copy(&value, address, sizeof(T));
}
template<class T> bool Field(const void* object, int offset, T& value) noexcept {
    const uintptr_t base = reinterpret_cast<uintptr_t>(object);
    if (!base || offset < 0 || offset > 0x40000 || base > UINTPTR_MAX - static_cast<unsigned>(offset))
        return false;
    return Read(reinterpret_cast<const void*>(base + static_cast<unsigned>(offset)), value);
}
template<class Fn> Fn Virtual(void* object, unsigned slot, Range code = {}) noexcept {
    uintptr_t table{};
    uintptr_t address{};
    if (!object || slot > 128 || !Read(object, table) || !table || table > UINTPTR_MAX - slot * sizeof(void*) ||
        !Read(reinterpret_cast<const void*>(table + slot * sizeof(void*)), address) || !address)
        return nullptr;
    // Engine entity methods remain inside client.dll. Checking that range avoids
    // a VirtualQuery for every entity field, every frame.
    if (code.base ? !code.Contains(reinterpret_cast<void*>(address)) : !Executable(reinterpret_cast<void*>(address)))
        return nullptr;
    return reinterpret_cast<Fn>(address);
}
template<class Fn, class Result, class... Args>
bool Invoke(Fn function, Result& result, Args... args) noexcept {
    static_assert(std::is_trivially_copyable_v<Result>);
    if (!function) return false;
    __try { result = function(args...); return true; }
    __except (FaultFilter(GetExceptionCode())) { return false; }
}
template<class Fn, class... Args> bool InvokeVoid(Fn function, Args... args) noexcept {
    if (!function) return false;
    __try { function(args...); return true; }
    __except (FaultFilter(GetExceptionCode())) { return false; }
}
bool String(Range range, const char* source, char* destination, size_t capacity) noexcept;
}

