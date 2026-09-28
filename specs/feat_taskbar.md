# Feature Prompt: The Taskbar

## 1. Objective

Implement a persistent navigation panel anchored to the bottom (or top) of the screen, similar to the Windows Taskbar.

## 2. Target Files

- `include/ui/Taskbar.h`
- `src/ui/Taskbar.cpp` (expose a function `void RenderTaskbar()`)

## 3. UI Requirements & ImGui Directives

- **Positioning:**
  Calculate a fixed height (e.g., 40-50 pixels). Set the window position to the bottom of `ImGui::GetMainViewport()->WorkPos + WorkSize`, adjusting for the taskbar height. Span the full viewport width.
- **Window Flags:**
  `ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking`.
- **Styling:**
  Push a custom background color using `ImGui::PushStyleColor(ImGuiCol_WindowBg, ...)` to make the taskbar visually distinct from the wallpaper gradient. Remember to `ImGui::PopStyleColor()`.
- **App Buttons:**
  - Use `ImGui::SameLine()` to align buttons horizontally.
  - **Button 1 ("App 1"):** Toggles `AppState::show_app_1`. (Implement a dummy ImGui window elsewhere to show when this is true).
  - **Button 2 ("App 2"):** Toggles `AppState::show_app_2`. (Implement a dummy ImGui window elsewhere to show when this is true).
  - **Button 3 ("Task Manager"):** Toggles `AppState::show_task_mgr`.
