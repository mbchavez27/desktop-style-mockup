# Phase 4 — OS Theme Pass (desktop + taskbar)

Status: done (`main` @ `5cbcc35`).

Depends on: Phase 3a pushed to `main` (+ 3b if landed — rebase on latest).

## Scope (assignee — looks only, no behavior changes)

- Restyle `src/ui/Desktop.cpp` (wallpaper, clock, PWR) and
  `src/ui/Taskbar.cpp` / `src/ui/MockApps.cpp` (bar, buttons, dummies)
  to resemble one real OS of your choice.
- Must not change: `RenderDesktop()` / `RenderTaskbar()` / `RenderMockApps()`
  signatures, `AppState` flags, `CMakeLists.txt`, `src/main.cpp` wiring,
  window flags, or the Desktop → Apps → Taskbar draw order.

## Required design record (fill before coding — review gate)

- **Chosen OS + version** (e.g. Ubuntu 24.04, Windows 11, macOS Sequoia)
  and 1–2 reference screenshots (link or `assets/images/`).
- **Palette**: gradient stops, bar color, text/button colors (IM_COL32 values).
- **Layout**: clock format/position, PWR placement, button order/spacing,
  bar height (via `kTaskbarHeight`) — and why each matches the reference.
- **Deliberately not copied**: list what you skipped and why (scope control).

## Exit criteria

- All 3a behaviors still work (toggles, PWR exits 0, 'X' ignored).
- Resemblance recognizable against the stated reference. No new warnings.

## Commit

- `style(theme): ...`, push to `main`.
