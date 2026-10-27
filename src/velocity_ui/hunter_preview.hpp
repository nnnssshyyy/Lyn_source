#pragma once
#include "xdraw.hpp"
namespace xui {struct rect;}
namespace esp::layout {struct Rect;}

namespace hunter_preview {
Microsoft::WRL::ComPtr<IDirect3DTexture9> Load(int& width,int& height);
void Hitboxes(IDirect3DTexture9* texture,float height);
void Esp(IDirect3DTexture9* texture,float height);
void EmbeddedEsp(IDirect3DTexture9* texture,float height);
// Shared editor for both Hunter renderers; coordinates use menu units.
bool EditEspLayout(const xui::rect& canvas,esp::layout::Rect body);
void OpenHitboxes();
void BeginMenuInput();
void EndMenuInput();
void DrawWindows(IDirect3DTexture9* fallback);
void CancelInput();
bool OwnsPointer() noexcept;
void InvalidateDeviceObjects();
}
