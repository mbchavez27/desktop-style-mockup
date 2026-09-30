# desktop-style-mockup
Desktop-Style OS Mock-up | For CSOPESY

## Dependencies

- CMake >= 3.16, git, and a C++17 compiler.
- System OpenGL + windowing headers (GLFW itself is auto-fetched, see below).
- GLFW 3.4 and Dear ImGui (`docking` branch) are fetched automatically
  by CMake FetchContent at first configure into `build/_deps/`.
  No manual install needed for those two.

### Linux

Fedora / RHEL:

```bash
sudo dnf install -y cmake g++ git \
  libX11-devel libXext-devel libXrandr-devel libXcursor-devel \
  libXi-devel libXinerama-devel mesa-libGL-devel \
  libxkbcommon-devel wayland-devel wayland-protocols-devel
```

Ubuntu / Debian:

```bash
sudo apt install -y cmake g++ git \
  libx11-dev libxext-dev libxrandr-dev libxcursor-dev \
  libxi-dev libxinerama-dev libgl1-mesa-dev \
  libxkbcommon-dev libwayland-dev wayland-protocols
```

Check versions:

```bash
cmake --version
g++ --version
```

### Windows

- Pick ONE toolchain:
  - Visual Studio 2022 with Desktop C++ workload, OR
  - MinGW-w64 GCC (e.g. via `winget install BrechtSanders.WinLibs.POSIX.UCRT`).
- CMake >= 3.16 and git (via `winget install Kitware.CMake Git.Git`).
- OpenGL ships with Windows; GLFW / ImGui are auto-fetched. No extra install.
- Check what you have (PowerShell):
  ```powershell
  cmake --version
  g++ --version   # MinGW path
  ninja --version # optional, faster backend bundled with WinLibs
  ```

### macOS

```bash
xcode-select --install
brew install cmake git
```

OpenGL framework ships with macOS (deprecated but functional);
GLFW / ImGui are auto-fetched. No extra install.

## Project layout

```text
CMakeLists.txt          # FetchContent: GLFW 3.4 + Dear ImGui (docking)
include/
  core/AppState.h       # global visibility flags (is_running, show_*)
  ui/*.h                # one class per UI layer (Doxygen-documented)
src/main.cpp            # entry point: GLFW + ImGui loop
src/core/AppState.cpp
src/ui/*.cpp            # BootScreen, Desktop, DesktopIcons, Taskbar,
                        #   MockApps (owns PaintApp + CalculatorApp), TaskManager (WIP)
specs/                  # phase + feature specs, coding conventions
build/                  # out-of-source build output + _deps/ (gitignored)
```

Target: `mockup_app` (C++17, `/W4` on MSVC, `-Wall -Wextra -Wpedantic` otherwise).

## How it works

This is an immediate-mode compositor: it does not implement real windows,
processes, or an OS kernel. Every frame it redraws a fake desktop inside a
single 1280x720 GLFW window using Dear ImGui.

### Boot flow

`BootScreen::Render()` draws a fullscreen splash (`assets/images/boot.png`)
that fades in, holds ~3s, then fades out; any mouse click skips the wait.
While it returns `true`, the desktop layers are not drawn, so nothing shows
through or steals input.

### Render loop (z-order)

`src/main.cpp` draws layers back-to-front every frame — the order is the
z-index (main_agenda §3):

```text
g_boot_screen.Render()   // splash, topmost (skips the rest while active)
  g_desktop.Render()     // wallpaper + clock + PWR button (base layer)
  g_desktop_icons.Render()  // clickable icon column
  g_mock_apps.Render()   // app windows (Calculator, Paint)
  g_taskbar.Render()     // XP-style bar, tray clock, app buttons (top layer)
```

### Layers

| Class | Draws |
|---|---|
| `BootScreen` | Fullscreen splash with fade/click-to-skip (only `bool Render()`) |
| `Desktop` | Wallpaper stretched over the viewport (gradient fallback) |
| `DesktopIcons` | Top-left icon column; toggles app visibility flags |
| `MockApps` | Hosts `CalculatorApp` + `PaintApp`, gated on their `AppState` flags |
| `Taskbar` | XP Luna bar: app buttons, tray clock, `PWR` (the only way to exit) |

### State

Global `AppState` statics (`is_running`, `show_app_1` (Calculator),
`show_app_2` (Paint), `show_task_mgr`). Desktop icons and taskbar buttons
flip the same flags, so both launchers stay in sync. App windows receive
`&flag` in `ImGui::Begin()`, so the native 'X' closes just that window.
Clicking the OS-level window 'X' is intercepted and ignored — only the
in-app `PWR` button sets `is_running = false` and exits.

### The mock apps

- **Calculator** — basic four-function: display + keypad (`+ - * /`,
  `=`, `C`, `+/-`, decimal), left-to-right evaluation, divide-by-zero
  shows `Error`.
- **Paint** — canvas drawing: brush/eraser toggle, color swatches +
  `ColorEdit3`, separate brush/eraser stroke sizes, undo, clear.

### Architecture

UI code is class-based: one class per layer with a documented `Render()`
method and a singleton instance (`extern Desktop g_desktop;` etc.).
Conventions are spelled out in
[`specs/coding_conventions.md`](specs/coding_conventions.md); phase-by-phase
progress lives in [`specs/`](specs/).

## How to run

First configure downloads GLFW 3.4 + ImGui (`docking`) into `build/_deps/`,
so it needs network and takes longer than usual.

### Linux

```bash
cmake -S . -B build
cmake --build build
./build/mockup_app
```

One-step build + run:

```bash
cmake --build build --target run
```

### Windows (PowerShell, Visual Studio generator)

Only if Visual Studio 2022 + C++ workload is installed
(a plain `cmake -S . -B build` picks this by default):

```powershell
cmake -S . -B build
cmake --build build --config Release
.\build\Release\mockup_app.exe
```

One-step build + run:

```powershell
cmake --build build --config Release --target run
```

### Windows (PowerShell, MinGW-w64 / Ninja — no Visual Studio)

If you do NOT have Visual Studio, you must pass `-G` explicitly.
Otherwise configure defaults to `NMake Makefiles` and fails with:

```text
Running 'nmake' '-?' failed with: no such file or directory
```

MinGW Makefiles backend:

```powershell
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
.\build\mockup_app.exe
```

One-step build + run:

```powershell
cmake --build build --target run
```

Ninja backend (faster, bundled with WinLibs):

```powershell
cmake -S . -B build -G Ninja
cmake --build build
.\build\mockup_app.exe
```

Notes:

- MinGW / Ninja are single-config: do NOT pass `--config Release`.
- The exe lands at `.\build\mockup_app.exe`
  (VS instead puts it at `.\build\Release\mockup_app.exe`).
- Switching generators requires a clean `build/` (see below),
  otherwise the stale `CMakeCache.txt` keeps pointing at the old generator.

### macOS

```bash
cmake -S . -B build
cmake --build build
./build/mockup_app
```

## Clean rebuild

Required when switching generators (e.g. NMake -> MinGW -> Ninja)
or after a failed configure. `--fresh` needs CMake >= 3.24:

```bash
cmake -S . -B build --fresh
cmake --build build
```

PowerShell equivalent (works on any CMake version):

```powershell
Remove-Item -Recurse -Force build
cmake -S . -B build -G "MinGW Makefiles"  # or -G Ninja, or omit for VS
cmake --build build
```
