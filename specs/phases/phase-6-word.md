# Phase 6 — MS Word (markdown editor + live preview)

Status: pending.

Depends on: Phase 5 (Paint — rebase on latest).

## Scope

- `include/ui/WordApp.h` / `src/ui/WordApp.cpp`: `void RenderWordApp()`,
  gated on `AppState::show_app_1` with the flag passed into
  `ImGui::Begin("Word", &AppState::show_app_1)` so the native 'X' toggles
  state. `src/ui/MockApps.cpp` dispatches to it (placeholder text removed).
- Split view inside the window, 50/50 with a thin divider:
  - **Left pane** (`BeginChild`, scrollable): `ImGui::InputTextMultiline`
    over a fixed `char[]` buffer (no `imgui_stdlib` dependency) holding raw
    markdown: `#` / `##` / `###` headings, `- ` bullets, `> ` quotes,
    `**bold**`, `*italic*`, `` `code` ``.
  - **Right pane** (`BeginChild`, scrollable): live preview — line parser
    walks the buffer each frame. Headings scale via
    `ImGui::SetWindowFontScale()` (push/pop around the line), quotes and
    paragraphs via `ImGui::TextWrapped`, bullets via a bullet glyph + text,
    inline emphasis via `ImGui::TextColored` segments joined with
    `ImGui::SameLine` (no font-atlas changes, no new dependencies).
- Both panes update every frame; preview reflects typing immediately.
- `CMakeLists.txt`: add `src/ui/WordApp.cpp` to `mockup_app`.

## Exit criteria

- Typing markdown on the left updates the rendered preview on the right.
- All six token types render distinctly (headings, bullets, quotes, bold,
  italic, code).
- Word window 'X' flips `show_app_1`; icon/taskbar toggles still work.
- Desktop, icons, taskbar, PWR behaviors unchanged (regression).
- Clean build, no warnings (`-Wall -Wextra -Wpedantic`).

## Commit

- `feat(word): ...`, push to `main`.
