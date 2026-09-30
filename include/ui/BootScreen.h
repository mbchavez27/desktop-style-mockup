#pragma once

#include <chrono>

/**
 * @brief Fullscreen boot splash: fade in, hold, fade out, click-to-skip.
 *
 * Renders on top of everything; while active the caller skips the desktop
 * layers so nothing shows through or steals input.
 */
class BootScreen
{
public:
    /**
     * @brief Draws the splash for this frame.
     *
     * Starts the fade clock on the first call. Any mouse click switches to
     * a fade-out from the current alpha.
     *
     * @return true while the splash is still animating.
     */
    bool Render();

private:
    /**
     * @brief Piecewise opacity curve: linear fade in, hold, linear fade out.
     *
     * @param elapsed Seconds since the splash first rendered.
     * @return Alpha in [0, 1]; 0 once past the total duration.
     */
    static float BootAlpha(float elapsed);

    std::chrono::steady_clock::time_point start{}; ///< First-frame clock (lazy-set).
    bool started = false;                          ///< False until the first Render.
    bool skipping = false;                         ///< Click-to-skip engaged.
    float skip_from_alpha = 1.0f;                  ///< Alpha the skip began from.
    float skip_began_at = 0.0f;                    ///< Elapsed time when skip began.
};

/// Global BootScreen instance driven by the main render loop.
extern BootScreen g_boot_screen;
