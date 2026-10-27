#pragma once
#include "esp.h"
#include <d3d9.h>

namespace menu {
void Style();
bool Initialize(IDirect3DDevice9* device,HWND hwnd);
void Input(UINT message,WPARAM wparam,LPARAM lparam);
void ClearInput();
void PrepareFrame();
void InvalidateDeviceObjects();
void Draw(esp::Options& options, const esp::Frame& frame, unsigned boxes_drawn);
}

