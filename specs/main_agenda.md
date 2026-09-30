# AI Orchestrator: CSOPESY OS Mockup Master Plan

## 1. System Prompt & Context

You are an expert C++ systems and graphics programmer using Dear ImGui, GLFW, and OpenGL 3. Your objective is to build a real-time, desktop-style OS mockup called "CSOPESY Emulator".

This is an immediate-mode compositor shell, not a real OS kernel. It must render at 60+ FPS in a standard GLFW application window but visually simulate an operating system desktop environment.

## 2. Global Architecture & State

All UI components must be modular. State is managed globally.

- **Target Files to Create/Modify:**
  - `include/core/AppState.h`: Define a global struct or namespace to hold visibility flags.
  - `src/main.cpp`: Setup GLFW, OpenGL context, ImGui backends, and the main render loop.

**Required Global State Variables (within an AppState struct or namespace):**

- `static bool is_running`: Set to true. If false, main loop breaks.
- `static bool show_task_mgr`: Toggles Task Manager visibility.
- `static bool show_app_1`: Toggles Mock App 1 visibility.
- `static bool show_app_2`: Toggles Mock App 2 visibility.

**Coding conventions:** UI code follows `specs/coding_conventions.md`
(class-based layers, Doxygen docs, naming, ImGui rules, quality gates).

## 3. Execution Constraints

- **Window Close Hook:** Intercept the GLFW window close callback. If the user clicks the OS-level 'X', ignore it. The application must _only_ exit when `AppState::is_running` is set to `false` via the in-app PWR button.
- **Render Loop Sequence:** Inside the `while(!glfwWindowShouldClose())` loop, render components strictly in this order to simulate z-indexing:
  1. Desktop Background (Base layer)
  2. Active Apps & Task Manager (Mid layers)
  3. Taskbar (Top layer)

## 4. Phase Execution

Execute the development in these phases. Read the specific `.md` files for exact implementation details.

- **Phase 1:** Setup `src/main.cpp`, CMakeLists, and basic ImGui loop.
- **Phase 2:** Implement `specs/feat/feat_desktop.md`.
- **Phase 3:** Implement `specs/feat/feat_taskbar.md` (split: 3a baseline, 3b polish).
- **Phase 4:** OS theme pass over desktop + taskbar (`specs/phases/phase-4-os-theme.md`).
- **Phase 5:** MS Paint canvas app (`specs/phases/phase-5-paint.md`).
- **Phase 6:** MS Calculator app (`specs/phases/phase-6-calculator.md`).
- **Phase 7:** OOP refactor — documented UI classes (`specs/phases/phase-7-oop-refactor.md`).
- **Phase 8:** Implement `specs/feat/feat_taskmgr.md` (`specs/phases/phase-8-task-manager.md`).
