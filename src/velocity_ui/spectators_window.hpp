#pragma once
namespace spectators_window {
void BeginMenuInput(bool menu_open);
void EndMenuInput();
void Draw(bool menu_open);
void CancelInput();
void ResetPosition();
bool OwnsPointer();
}
