# Phase 8 — Task Manager (floating window)

Status: pending.

Spec: `specs/feat/feat_taskmgr.md`.

Depends on: Phase 7 (OOP refactor — rebase on latest).

## Scope

- `include/ui/TaskManager.h` / `src/ui/TaskManager.cpp`:
  `class TaskManager` with a public `Render()` and Doxygen docs per
  `specs/coding_conventions.md`, gated on `AppState::show_task_mgr` with
  the flag passed into `ImGui::Begin()` so the native 'X' toggles state.
  Singleton instance: `extern TaskManager g_task_manager;` in the header,
  defined in the cpp.
- Dummy `Process` list with `ImGui::BeginTable()` (Name / CPU / Memory),
  CPU values perturbed each frame, formatted `%.1f%%` / `%d MB`.
- `CMakeLists.txt`: add `src/ui/TaskManager.cpp`. `src/main.cpp`: call
  `g_task_manager.Render()` in the mid layer (after `g_mock_apps.Render()`,
  before `g_taskbar.Render()`) — z-order per `main_agenda` §3.

## Exit criteria

- Task Manager button toggles the window; native 'X' closes it (flag flips).
- Table renders with fluctuating CPU values. No new warnings.
- Desktop, dummies, taskbar, PWR behaviors unchanged (regression).

## Commit

- `feat(taskmgr): ...`, push to `main`.
