#pragma once

constexpr float kTaskbarHeight = 44.0f;

/**
 * @brief Bottom shell layer: XP-style bar with Start button, app buttons, tray clock.
 *
 * Owns the Start menu open state. The power button lives inside the
 * Start menu footer; the taskbar itself only shows the tray clock.
 */
class Taskbar
{
public:
    /**
     * @brief Draws the taskbar and, when open, the Start menu for this frame.
     *
     * The Start menu renders above the bar so it stays top-most.
     * Clicking outside the menu (or pressing Escape) closes it.
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

    /**
     * @brief Draws the green Start button at the left edge of the bar.
     *
     * @return True while the button is hovered (used for click-outside close).
     */
    bool RenderStartButton();

    /**
     * @brief Draws the XP-style Start menu floating above the bar.
     *
     * Lists every mock app as a clickable row and hosts the power
     * button in its footer. Closes itself on outside click / Escape.
     *
     * @param start_hovered True while the Start button is hovered.
     */
    void RenderStartMenu(bool start_hovered);

    /**
     * @brief Draws one full-width clickable app row inside the Start menu.
     *
     * @param id        Unique ImGui ID for the row.
     * @param icon_path PNG path under assets/.
     * @param label     Row text.
     * @param toggle    AppState flag flipped on click; closes the menu.
     */
    void StartMenuAppRow(const char *id, const char *icon_path, const char *label, bool *toggle);

    bool show_start_menu_ = false; ///< Start menu open state.
};

/// Global Taskbar instance driven by the main render loop.
extern Taskbar g_taskbar;
