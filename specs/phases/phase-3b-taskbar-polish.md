# Phase 3b — Taskbar Polish (groupmate)

Status: pending.

Depends on: Phase 3a pushed to `main` — `git pull` before starting.

## Scope (groupmate — looks only)

- Interiors of `src/ui/Taskbar.cpp` / `src/ui/MockApps.cpp`: active-button
  highlight, spacing/padding, hover tooltips, dock color, height tweak via
  `kTaskbarHeight`, dummy window content.
- Must not change: `RenderTaskbar()` / `RenderMockApps()` signatures,
  `AppState` flags, `CMakeLists.txt`, `src/main.cpp`.

## Exit criteria

- All 3a toggles still work (regression). Polish visible. No new warnings.

## Commit

- `style(taskbar): ...` on top of 3a, push to `main`.
