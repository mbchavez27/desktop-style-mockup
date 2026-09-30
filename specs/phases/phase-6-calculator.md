# Phase 6 — MS Calculator (basic four-function)

Status: done (`main` @ `9afc692`).

Depends on: Phase 5 (Paint — rebase on latest).

## Scope

- `include/ui/CalculatorApp.h` / `src/ui/CalculatorApp.cpp`:
  `class CalculatorApp` with a public `Render()` and Doxygen docs per
  `specs/coding_conventions.md`. File-static-free: accumulator, pending
  operator, entry buffer and flags are instance members.
- `src/ui/MockApps.h` / `src/ui/MockApps.cpp`: own a
  `CalculatorApp calculator_` member and gate it on
  `AppState::show_app_1` (the flag is passed into
  `ImGui::Begin("Calculator", &AppState::show_app_1)` so the native 'X'
  toggles it).
- Re-add the launchers removed with Word: desktop icon and taskbar
  button using `assets/images/calculator.png`, label "Calculator",
  toggling `show_app_1` (restore the 3-button centering math in
  `src/ui/Taskbar.cpp`).
- `CMakeLists.txt`: add `src/ui/CalculatorApp.cpp` to `mockup_app`.
- UI: bordered display line (right-aligned `%g` output) above a 4-column
  keypad — `C | +/- | . | /`, `7 8 9 *`, `4 5 6 -`, `1 2 3 +`,
  wide `0` and `=`. Display-only result, no history.
- Behavior: digits append to the entry; an operator commits the pending
  operation and arms the new one; `=` evaluates; `C` clears everything;
  `+/-` toggles the sign of the current entry; divide-by-zero shows
  `Error` until the next `C` or digit.

## Exit criteria

- Icon and taskbar button toggle the window; native 'X' closes it.
- All four operations, decimal entry, sign toggle, and the error state
  behave as specified above.
- Desktop, Paint, boot splash, taskbar, PWR behaviors unchanged
  (regression).
- Clean build, no warnings (`-Wall -Wextra -Wpedantic`).

## Commit

- `feat(calculator): ...`, push to `main`.
