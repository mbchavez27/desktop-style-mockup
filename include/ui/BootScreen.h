#pragma once

// Draws the boot splash (assets/images/boot.png) fullscreen on top of
// everything. Fades in, holds ~3s, then fades out. Returns true while the
// splash is still active; when true the caller should skip the desktop UI
// so nothing underneath steals input or shows through.
bool RenderBootScreen();
