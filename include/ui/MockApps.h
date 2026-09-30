#pragma once

#include "ui/PaintApp.h"
#include "ui/WordApp.h"

/**
 * @brief Mid layer hosting the mock application windows.
 *
 * Owns the Paint and Word components and gates them on their AppState
 * visibility flags before rendering.
 */
class MockApps
{
public:
    /**
     * @brief Renders every open mock app for this frame.
     *
     * Paint renders when AppState::show_app_2, Word when show_app_1.
     * Each window still receives its flag pointer so the native 'X' closes it.
     */
    void Render();

private:
    PaintApp paint_; ///< Canvas drawing app (show_app_2).
    WordApp word_;   ///< Markdown editor (show_app_1).
};

/// Global MockApps instance driven by the main render loop.
extern MockApps g_mock_apps;
