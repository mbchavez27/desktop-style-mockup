# Phase 2 — Desktop Compositor (base layer)

Status: done.

Verified: gradient fills viewport on resize, clock ticks HH:MM:SS
top-right, PWR exits 0, OS 'X' ignored.

Spec: `specs/feat/feat_desktop.md`.

Depends on: Phase 1 (app shell + `AppState::is_running`).

## Scope

- `include/ui/Desktop.h` / `src/ui/Desktop.cpp`: `void RenderDesktop()`.
- Full-viewport window with no-background flags, gradient wallpaper via
  `AddRectFilledMultiColor`, real-time `HH:MM:SS` clock (top-right,
  `<chrono>` + `<ctime>`), `PWR` button (bottom-right) setting
  `AppState::is_running = false`.

## Exit criteria

- Gradient fills viewport on resize, behind all other windows.
- Clock ticks every second, right-aligned.
- PWR exits the app; OS 'X' still ignored.
