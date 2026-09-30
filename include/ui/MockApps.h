#pragma once

#include "ui/PaintApp.h"

/**
 * @brief Mid layer hosting the mock application windows.
 *
 * Owns the Paint component and gates it on its AppState visibility flag
 * before rendering. show_app_1 is reserved for the calculator (Phase 6).
 */
class MockApps
{
public:
    /**
     * @brief Renders every open mock app for this frame.
     *
     * Paint renders when AppState::show_app_2. Each window still receives
     * its flag pointer so the native 'X' closes it.
     */
    void Render();

private:
    PaintApp paint_; ///< Canvas drawing app (show_app_2).
};

/// Global MockApps instance driven by the main render loop.
extern MockApps g_mock_apps;
