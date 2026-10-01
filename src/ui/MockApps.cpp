#include "imgui.h"
#include "core/AppState.h"
#include "ui/MockApps.h"

MockApps g_mock_apps;

void MockApps::Render()
{
    if (AppState::show_app_1)
        calculator_.Render();
    if (AppState::show_app_2)
        paint_.Render();
    if (AppState::show_file_explorer)
        explorer_.Render();
}
