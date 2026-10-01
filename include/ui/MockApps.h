#pragma once

#include "ui/CalculatorApp.h"
#include "ui/FileExplorerApp.h"
#include "ui/PaintApp.h"

/**
 * @brief Mid layer hosting the mock application windows.
 *
 * Owns the Paint and Calculator components and gates them on their
 * AppState visibility flags before rendering.
 */
class MockApps
{
public:
    /**
     * @brief Renders every open mock app for this frame.
     *
     * Calculator renders when AppState::show_app_1, Paint when
     * show_app_2. Each window still receives its flag pointer so the
     * native 'X' closes it.
     */
    void Render();

private:
    CalculatorApp calculator_; ///< Four-function calculator (show_app_1).
    PaintApp paint_;           ///< Canvas drawing app (show_app_2).
    FileExplorerApp explorer_; ///< Folder mock-up (show_file_explorer).
};

/// Global MockApps instance driven by the main render loop.
extern MockApps g_mock_apps;
