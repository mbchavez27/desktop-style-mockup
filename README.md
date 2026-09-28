# desktop-style-mockup
Desktop-Style OS Mock-up | For CSOPESY

## Dependencies

- CMake >= 3.16
- C++17 compiler:
  - Linux: GCC 9+ or Clang 10+ (`g++ --version`)
  - Windows: Visual Studio 2022 with Desktop C++ workload, or MinGW-w64 GCC
- No external libraries. Standard library only.

Check versions:

```bash
cmake --version
g++ --version   # Linux / MinGW
```

## Project layout

```text
CMakeLists.txt
include/        # public headers (currently empty)
src/main.cpp    # entry point
build/          # out-of-source build output (gitignored)
```

Target: `mockup_app` (C++17, `/W4` on MSVC, `-Wall -Wextra -Wpedantic` otherwise).

## How to run

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

## Clean rebuild

```bash
cmake -S . -B build --fresh
cmake --build build
```
