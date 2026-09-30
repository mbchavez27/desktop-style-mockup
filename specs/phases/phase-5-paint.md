# Phase 5 — MS Paint (canvas drawing app)

Status: pending.

Depends on: Phase 4 (theme pass — rebase on latest).

## Scope

- `include/ui/PaintApp.h` / `src/ui/PaintApp.cpp`: `void RenderPaintApp()`,
  gated on `AppState::show_app_2` with the flag passed into
  `ImGui::Begin("Paint", &AppState::show_app_2)` so the native 'X' toggles
  state. `src/ui/MockApps.cpp` dispatches to it (placeholder text removed).
- Stroke model in an anonymous namespace:
  `struct Stroke { ImU32 color; float thickness; std::vector<ImVec2> points; }`
  stored in a `std::vector<Stroke>`; replayed every frame on the canvas via
  `ImDrawList::AddPolyline` (white canvas rect underneath).
- Input: `ImGui::InvisibleButton` covering the canvas; append points while
  `ImGui::IsItemActive() && ImGui::IsMouseDown(ImGuiMouseButton_Left)`,
  coordinates converted from item-local to screen space.
- Toolbar row above the canvas:
  - Brush / Eraser toggle (eraser paints the canvas background color).
  - Color: preset swatches (`IM_COL32`) + `ImGui::ColorEdit3`.
  - Stroke width: `ImGui::SliderFloat` (1–24 px).
  - Undo (pop last stroke) and Clear (empty stroke list) buttons.
- `CMakeLists.txt`: add `src/ui/PaintApp.cpp` to `mockup_app`.

## Exit criteria

- Colored strokes of adjustable width draw on the canvas; eraser covers them.
- Undo removes the last stroke; Clear empties the canvas.
- Paint window 'X' flips `show_app_2`; icon/taskbar toggles still work.
- Desktop, icons, taskbar, PWR behaviors unchanged (regression).
- Clean build, no warnings (`-Wall -Wextra -Wpedantic`).

## Commit

- `feat(paint): ...`, push to `main`.
