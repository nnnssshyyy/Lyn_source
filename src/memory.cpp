#include "memory.h"
#include <cstring>

namespace memory {
int FaultFilter(unsigned long code) noexcept {
    return code == EXCEPTION_ACCESS_VIOLATION || code == EXCEPTION_IN_PAGE_ERROR ||
           code == EXCEPTION_GUARD_PAGE ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH;
}
bool Copy(void* destination, const void* source, size_t size) noexcept {
    if (!destination || !source || !size ||
        reinterpret_cast<uintptr_t>(source) > UINTPTR_MAX - (size - 1) ||
        reinterpret_cast<uintptr_t>(destination) > UINTPTR_MAX - (size - 1)) return false;
    __try { std::memcpy(destination, source, size); return true; }
    __except (FaultFilter(GetExceptionCode())) { return false; }
}
bool Executable(const void* address) noexcept {
    MEMORY_BASIC_INFORMATION page{};
    if (!address || !VirtualQuery(address, &page, sizeof(page)) || page.State != MEM_COMMIT ||
        (page.Protect & (PAGE_GUARD | PAGE_NOACCESS))) return false;
    const DWORD protection = page.Protect & 0xff;
    return protection == PAGE_EXECUTE || protection == PAGE_EXECUTE_READ ||
           protection == PAGE_EXECUTE_READWRITE || protection == PAGE_EXECUTE_WRITECOPY;
}
Range Module(HMODULE module) noexcept {
    IMAGE_DOS_HEADER dos{};
    if (!Read(module, dos) || dos.e_magic != IMAGE_DOS_SIGNATURE || dos.e_lfanew <= 0 || dos.e_lfanew > 0x100000)
        return {};
    IMAGE_NT_HEADERS32 nt{};
    if (!Read(reinterpret_cast<const char*>(module) + dos.e_lfanew, nt) ||
        nt.Signature != IMAGE_NT_SIGNATURE || nt.OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR32_MAGIC ||
        nt.OptionalHeader.SizeOfImage < sizeof(nt)) return {};
    return {reinterpret_cast<uintptr_t>(module), nt.OptionalHeader.SizeOfImage};
}
bool String(Range range, const char* source, char* destination, size_t capacity) noexcept {
    if (!destination || !capacity) return false;
    destination[0] = 0;
    if (!range.Contains(source)) return false;
    const size_t remaining = range.size - (reinterpret_cast<uintptr_t>(source) - range.base);
    const size_t count = remaining < capacity ? remaining : capacity;
    if (!Copy(destination, source, count)) { destination[0] = 0; return false; }
    const bool terminated = std::memchr(destination, 0, count) != nullptr;
    destination[capacity - 1] = 0;
    if (count < capacity) destination[count] = 0;
    return terminated;
}
}

