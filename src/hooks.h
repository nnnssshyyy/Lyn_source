#pragma once
#include "source.h"

namespace hooks {
bool Install(const source::Interfaces& interfaces);
bool MenuOpen() noexcept;
void SetMenuOpen(bool open) noexcept;
}

