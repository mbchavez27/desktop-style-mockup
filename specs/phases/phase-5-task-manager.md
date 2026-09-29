# Phase 5 — Task Manager (floating window)

Status: pending.

Spec: `specs/feat/feat_taskmgr.md`.

Depends on: Phase 4 (theme pass — rebase on latest).

## Scope

- `include/ui/TaskManager.h` / `src/ui/TaskManager.cpp`:
  `void RenderTaskManager()`, gated on `AppState::show_task_mgr` with the
  flag passed into `ImGui::Begin()` so the native 'X' toggles state.
- Dummy `Process` list with `ImGui::BeginTable()` (Name / CPU / Memory),
  CPU values perturbed each frame, formatted `%.1f%%` / `%d MB`.
- `CMakeLists.txt`: add `src/ui/TaskManager.cpp`. `src/main.cpp`: call
  `RenderTaskManager()` in the mid layer (after `RenderMockApps()`,
  before `RenderTaskbar()`).

## Exit criteria

- Task Manager button toggles the window; native 'X' closes it (flag flips).
- Table renders with fluctuating CPU values. No new warnings.
- Desktop, dummies, taskbar, PWR behaviors unchanged (regression).

## Commit

- `feat(taskmgr): ...`, push to `main`.
