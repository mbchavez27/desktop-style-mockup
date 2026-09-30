#include "imgui.h"
#include "core/AppState.h"
#include "ui/MockApps.h"
#include "ui/PaintApp.h"
#include "ui/WordApp.h"

void RenderMockApps()
{
    if (AppState::show_app_1)
        RenderWordApp();
    if (AppState::show_app_2)
        RenderPaintApp();
}
