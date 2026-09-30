#pragma once

/**
 * @brief Base shell layer: wallpaper with gradient fallback.
 *
 * Occupies the full viewport behind every other layer as a borderless,
 * non-interactive ImGui window.
 */
class Desktop
{
public:
    /**
     * @brief Draws the desktop background for this frame.
     *
     * Stretches assets/images/background.png over the viewport when the
     * texture loads; otherwise paints a teal gradient fallback.
     */
    void Render();
};

/// Global Desktop instance driven by the main render loop.
extern Desktop g_desktop;
