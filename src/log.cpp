#include "log.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <iterator>

namespace app {
namespace {
wchar_t log_path[32768]{};
SRWLOCK log_lock = SRWLOCK_INIT;
}
void InitLog(HMODULE module) noexcept {
    const DWORD count = GetModuleFileNameW(module, log_path, static_cast<DWORD>(std::size(log_path)));
    if (!count || count >= std::size(log_path)) { log_path[0] = 0; return; }
    wchar_t* slash = std::wcsrchr(log_path, L'\\');
    if (!slash || slash - log_path > static_cast<ptrdiff_t>(std::size(log_path) - 20)) {
        log_path[0] = 0; return;
    }
    wcscpy_s(slash + 1, std::size(log_path) - static_cast<size_t>(slash + 1 - log_path), L"lyn.log");
}
void Log(const char* format, ...) noexcept {
    char message[1024]{};
    va_list args;
    va_start(args, format);
    vsnprintf_s(message, sizeof(message), _TRUNCATE, format, args);
    va_end(args);
    char line[1152]{};
    SYSTEMTIME now{};
    GetLocalTime(&now);
    snprintf(line, sizeof(line), "[%02u:%02u:%02u] %s\r\n", now.wHour, now.wMinute, now.wSecond, message);
    OutputDebugStringA(line);
    AcquireSRWLockExclusive(&log_lock);
    if (log_path[0]) {
        HANDLE file = CreateFileW(log_path, FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE,
                                  nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (file != INVALID_HANDLE_VALUE) {
            DWORD written{};
            WriteFile(file, line, static_cast<DWORD>(std::strlen(line)), &written, nullptr);
            CloseHandle(file);
        }
    }
    ReleaseSRWLockExclusive(&log_lock);
}
}

