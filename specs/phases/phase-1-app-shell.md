# Phase 1 — App Shell (main loop + global state)

Status: done.

Verified: clean `cmake --build build` (no warnings), smoke run shows
black 1280x720 window, OS 'X' ignored as specified.

Spec: `specs/main_agenda.md` §2 (Global Architecture & State),
§3 (Execution Constraints), Phase 1.

Depends on: Phase 0 (build infra).

## Scope

- `include/core/AppState.h` / `src/core/AppState.cpp`: `AppState` struct with
  `is_running` (true), `show_task_mgr`, `show_app_1`, `show_app_2` (false).
- `src/main.cpp`: GLFW window + OpenGL 3 context, ImGui context + backends,
  window close hook (OS 'X' ignored, only in-app PWR exits), main render loop
  with z-order: Desktop → Apps/Task Manager → Taskbar.

## Exit criteria

- App opens a 1280x720 window titled "CSOPESY Emulator".
- Clicking the OS 'X' does nothing; `AppState::is_running = false` exits 0.
- Clean build, no warnings (`-Wall -Wextra -Wpedantic`).
