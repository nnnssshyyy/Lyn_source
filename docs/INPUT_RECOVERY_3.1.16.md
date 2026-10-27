# Input recovery, module 3.1.16

The private visuals window is now an owned, disabled, nonactivating tool window.
It does not become a global topmost window, and it is shown only when visibility
changes. Position updates preserve activation and owner order. The game window
remains enabled; the overlay cannot receive keyboard focus or user mouse input.

Menu input interception and cursor unlocking now require a focused, active menu
with a successfully presented frame in the last 300 ms. A menu that could not be
drawn, a failed presentation, device reset or loss of focus closes the menu and
releases its mouse capture on the game window thread. Escape also closes it.
The normal game WndProc receives input when the menu is closed.

Regression coverage exercises the actual overlay window factory, ownership,
disabled input state, hit testing, activation/focus preservation, capture
affinity and DX9 backbuffer isolation. Deterministic cases exercise closed,
unpresented, stale, unfocused and inactive menu input. All 20 native suites pass.
This does not replace a live L4D2 session test; none was performed for this build.

Windows input behavior reference:
https://learn.microsoft.com/en-us/windows/win32/winmsg/window-features

Accounts uses one mint accent palette for account forms, selected tabs,
navigation, license state and downloads. Existing geometry, authentication,
signed module delivery and loader 3.1.6 remain in place.
