# Phase 8a — Task Manager (floating window)

Status: pending.

Spec: `specs/feat/feat_taskmgr.md`.

Depends on: Phase 7 (OOP refactor — rebase on latest).

## Scope

- `include/ui/TaskManager.h` / `src/ui/TaskManager.cpp` (already tracked,
  currently empty): `class TaskManager` with a public `Render()` and
  Doxygen docs per `specs/coding_conventions.md`, gated on
  `AppState::show_task_mgr` with the flag passed into
  `ImGui::Begin()` so the native 'X' toggles state. Singleton instance:
  `extern TaskManager g_task_manager;` in the header, defined in the cpp.
- Private `struct Process { std::string name; float cpu; int mem_mb; }`
  with a `std::vector<Process>` seeded with 4–5 dummy entries
  ("csopesy.exe", "dwm.exe", "System Idle", ...). RNG state lives as
  instance members — no file-local statics (conventions §4).
- `ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver)`;
  dummy `Process` list with `ImGui::BeginTable()` (Name / CPU / Memory,
  `Borders | RowBg | Resizable`), CPU values perturbed each frame and
  clamped to [0, 100], formatted `%.1f%%` / `%d MB`.
- `CMakeLists.txt`: add `src/ui/TaskManager.cpp`. `src/main.cpp`: call
  `g_task_manager.Render()` in the mid layer (after `g_mock_apps.Render()`,
  before `g_taskbar.Render()`) — z-order per `main_agenda` §3.
- Not in scope: the summary footer and all README / `Status` doc flips —
  those are Phase 8b.

## Exit criteria

- Task Manager opens from Start Menu, taskbar, and desktop icon; native
  'X' closes it (flag flips) and relaunch reopens it.
- Table renders with fluctuating CPU values. No new warnings
  (`-Wall -Wextra -Wpedantic`).
- Desktop, dummies, taskbar, PWR behaviors unchanged (regression).
- This file's `Status:` flipped to `done.` on merge.

## Handoff contract (frozen for 8b)

- `TaskManager::Render()` signature, the three table columns and their
  formats, `AppState::show_task_mgr`, `CMakeLists.txt`, `src/main.cpp`
  wiring. 8b adds a footer inside `TaskManager.cpp` and touches `.md`
  files only.

## Commit

- `feat(taskmgr): ...`, push to `main`.
