# Phase 3a — Taskbar Baseline (bottom bar + toggles)

Status: pending.

Spec: `specs/feat/feat_taskbar.md` (bottom placement, 40–50px height range).

Depends on: Phase 2 (desktop base layer).

## Scope (you)

- `include/ui/Taskbar.h` / `src/ui/Taskbar.cpp`: `void RenderTaskbar();`
  plus `constexpr float kTaskbarHeight = 44.0f;` as the single height knob.
- Bottom bar: pos `(WorkPos.x, WorkPos.y + WorkSize.y − kTaskbarHeight)`,
  size `(WorkSize.x, kTaskbarHeight)`; spec flags verbatim
  (`NoTitleBar|NoResize|NoMove|NoScrollbar|NoSavedSettings|NoDocking`);
  dark `PushStyleColor(WindowBg)` / `PopStyleColor`.
- Three plain buttons on one `SameLine()` row: `App 1` → `show_app_1`,
  `App 2` → `show_app_2`, `Task Manager` → `show_task_mgr`. No active styling.
- `include/ui/MockApps.h` / `src/ui/MockApps.cpp` (new): `void RenderMockApps();`
  two bare dummy windows gated on the flags, native 'X' wired back.
- `CMakeLists.txt`: add both new sources. `src/main.cpp`: +2 includes,
  loop `RenderDesktop()` → `RenderMockApps()` → `RenderTaskbar()` last.

## Exit criteria

- Clean build, no warnings. Bar pinned bottom on resize.
- Buttons toggle the dummies; PWR exits 0; OS 'X' still ignored.

## Handoff contract (frozen for 3b)

- `RenderTaskbar()` / `RenderMockApps()` signatures, the three flags,
  `CMakeLists.txt`, `main.cpp` wiring. Polish only via `kTaskbarHeight`
  and the interiors of `Taskbar.cpp` / `MockApps.cpp`.
