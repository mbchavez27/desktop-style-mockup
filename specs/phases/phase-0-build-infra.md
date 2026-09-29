# Phase 0 — Build Infrastructure

Status: done (`main` @ `996b033`).

Source specs: `specs/main_agenda.md` (prerequisite for Phase 1).

## Commits

- `3207254` — FetchContent for GLFW 3.4 + ImGui docking, README OS deps.
- `996b033` — `DOWNLOAD_EXTRACT_TIMESTAMP`, `wayland-devel` requirement.

## What was set up

- `CMakeLists.txt`: `find_package(OpenGL REQUIRED)`, GLFW 3.4 tarball +
  Dear ImGui `docking` branch via FetchContent into `build/_deps/`,
  `imgui` static lib (core + `impl_glfw` + `impl_opengl3` backends),
  `mockup_app` links `imgui glfw OpenGL::GL`, scoped to
  `src/main.cpp` + `src/core/AppState.cpp` + `src/ui/Desktop.cpp`.
- `README.md`: system deps per OS — Fedora (`wayland-devel` et al via `dnf`),
  Ubuntu (`libwayland-dev` via `apt`), Windows (`winget` for CMake/git),
  macOS (`brew` for CMake/git). GLFW/ImGui auto-fetched, no manual install.

## Verified

- `cmake -S . -B build --fresh && cmake --build build` passes.
- `build/mockup_app` links but is still the hello-world stub.

## Known state (Phase 1–2 not started)

- `include/core/AppState.h`, `src/core/AppState.cpp`: 0 bytes.
- `include/ui/Desktop.h`, `src/ui/Desktop.cpp`: 0 bytes.
- `src/main.cpp`: 95-byte hello-world stub — no GLFW window, ImGui context,
  render loop, or close hook yet.
