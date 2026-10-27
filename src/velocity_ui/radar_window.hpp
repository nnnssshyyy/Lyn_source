#pragma once
namespace radar_window {
void BeginMenuInput(bool open);
void EndMenuInput();
void Draw(bool open);
void CancelInput();
bool OwnsPointer();
}
