#pragma once

constexpr float kTaskbarHeight = 44.0f;

/**
 * @brief Top-most shell layer: XP-style bar with app buttons, tray clock, PWR.
 */
class Taskbar
{
public:
    /**
     * @brief Draws the taskbar for this frame.
     *
     * Anchored to the bottom edge at kTaskbarHeight. PWR sets
     * AppState::is_running = false; the native 'X' is intercepted elsewhere.
     */
    void Render();

private:
    /**
     * @brief Draws one taskbar app button.
     *
     * @param id        Unique ImGui ID for the button.
     * @param icon_path PNG path under assets/.
     * @param tooltip   Hover text.
     * @param toggle    AppState flag flipped on press.
     */
    static void TaskbarIconButton(const char *id, const char *icon_path, const char *tooltip, bool *toggle);
};

/// Global Taskbar instance driven by the main render loop.
extern Taskbar g_taskbar;
