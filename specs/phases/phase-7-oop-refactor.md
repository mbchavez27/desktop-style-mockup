# Phase 7 — OOP refactor (documented UI classes)

Status: done (`main` @ `ad6139b`).

Depends on: Phase 5 (Paint — both apps landed) + boot splash commit
(`7065ecf`).

## Scope

- Each UI module becomes a class with a public `Render()` and Doxygen
  docs: `BootScreen`, `Desktop`, `DesktopIcons`, `Taskbar`, `MockApps`,
  `PaintApp` (6 classes). File/function statics move to
  instance members. `BootScreen` is the only class with a non-void
  `Render()` — its bool signals "still animating" to main.cpp.
- Singleton instances: `extern T g_t;` in headers, defined in the cpp;
  `src/main.cpp` calls `g_*.Render()` (layer order unchanged).
- `MockApps` owns `PaintApp` as a member; visibility gating
  (`show_app_2`) moves to `MockApps::Render()` (`show_app_1` is reserved
  for the calculator).
- Unchanged: `AppState` statics (main_agenda §2), `IconCache` free
  functions, `CMakeLists.txt` sources, render z-order.
- Deliberate deviation: phase-4's "don't change Render* signatures" gate
  is superseded; main_agenda §3 mandates layer order, not symbol names.
- Phase 8a's TaskManager will be authored as `class TaskManager` directly.

## Exit criteria

- Clean build, no warnings (`-Wall -Wextra -Wpedantic`).
- Behavior-preserving: boot splash fades/skips, toggles, Paint
  draw/erase/undo, PWR exit all behave as before.

## Commit

- `refactor(ui): ...`, push to `main`.
