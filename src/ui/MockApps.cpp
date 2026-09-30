#include "imgui.h"
#include "core/AppState.h"
#include "ui/MockApps.h"
#include "ui/PaintApp.h"

void RenderMockApps()
{
    if (AppState::show_app_1)
    {
        ImGui::Begin("Word", &AppState::show_app_1);
        ImGui::Text("Word Placeholder.");
        ImGui::End();
    }
    if (AppState::show_app_2)
        RenderPaintApp();
}
