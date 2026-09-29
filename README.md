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
  libxkbcommon-devel wayland-protocols-devel
```

Ubuntu / Debian:

```bash
sudo apt install -y cmake g++ git \
  libx11-dev libxext-dev libxrandr-dev libxcursor-dev \
  libxi-dev libxinerama-dev libgl1-mesa-dev \
  libxkbcommon-dev wayland-protocols
```

Check versions:

```bash
cmake --version
g++ --version
```

### Windows

- Visual Studio 2022 with Desktop C++ workload, or MinGW-w64 GCC.
- CMake >= 3.16 and git (via `winget install Kitware.CMake Git.Git`).
- OpenGL ships with Windows; GLFW / ImGui are auto-fetched. No extra install.

### macOS

```bash
xcode-select --install
brew install cmake git
```

OpenGL framework ships with macOS (deprecated but functional);
GLFW / ImGui are auto-fetched. No extra install.

## Project layout

```text
CMakeLists.txt      # FetchContent: GLFW 3.4 + Dear ImGui (docking)
include/            # public headers (AppState, ui)
src/main.cpp        # entry point (GLFW + ImGui loop)
src/core/           # AppState definitions
src/ui/             # Desktop / Taskbar / TaskManager
build/              # out-of-source build output + _deps/ (gitignored)
```

Target: `mockup_app` (C++17, `/W4` on MSVC, `-Wall -Wextra -Wpedantic` otherwise).

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

```powershell
cmake -S . -B build
cmake --build build --config Release
.\build\Release\mockup_app.exe
```

One-step build + run:

```powershell
cmake --build build --config Release --target run
```

### Windows (MinGW / Ninja)

```powershell
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
.\build\mockup_app.exe
```

### macOS

```bash
cmake -S . -B build
cmake --build build
./build/mockup_app
```

## Clean rebuild

```bash
cmake -S . -B build --fresh
cmake --build build
```
