# Feature Prompt: Desktop Compositor (Base Layer)

## 1. Objective

Implement the root desktop layer that acts as the OS background. It must cover the entire viewport and sit behind all other windows.

## 2. Target Files

- `include/ui/Desktop.h`
- `src/ui/Desktop.cpp` (expose a function `void RenderDesktop()`)

## 3. UI Requirements & ImGui Directives

- **Viewport Covering:** Get the main viewport using `ImGui::GetMainViewport()`. Set the next window position to `viewport->WorkPos` and size to `viewport->WorkSize`.
- **Window Flags (CRITICAL):**
  Use `ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground`.
- **Wallpaper:**
  Since the window has `NoBackground`, use `ImGui::GetWindowDrawList()->AddRectFilledMultiColor()` to draw a visually pleasing gradient across the window coordinates to serve as the wallpaper.
- **Real-time Clock:**
  - Fetch the local system time using `and`.
  - Render the time formatted as `HH:MM:SS` (or similar) in the top-right corner.
  - Calculate X offset using `ImGui::GetWindowWidth()` and `ImGui::CalcTextSize()` to align it dynamically to the right edge.
- **Power Button:**
  - Render a distinct "PWR" button (e.g., in the bottom-right or top-right near the clock).
  - Action: `if (ImGui::Button("PWR")) { AppState::is_running = false; }`
