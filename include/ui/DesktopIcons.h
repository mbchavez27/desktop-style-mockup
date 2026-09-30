#pragma once

/**
 * @brief Top layer of clickable desktop icons (icon + title beneath).
 *
 * Renders the top-left icon column and toggles the same AppState flags
 * as the taskbar buttons.
 */
class DesktopIcons
{
public:
    /**
     * @brief Draws the desktop icon column for this frame.
     */
    void Render();

private:
    /**
     * @brief Draws one icon cell as an invisible click target.
     *
     * @param id        Unique ImGui ID for the cell.
     * @param icon_path PNG path under assets/.
     * @param title     Tooltip and label text.
     * @param toggle    AppState flag flipped on click.
     */
    static void DesktopIcon(const char *id, const char *icon_path, const char *title, bool *toggle);
};

/// Global DesktopIcons instance driven by the main render loop.
extern DesktopIcons g_desktop_icons;
