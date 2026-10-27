#pragma once
#include <Windows.h>

namespace app {
void InitLog(HMODULE module) noexcept;
void Log(const char* format, ...) noexcept;
}

