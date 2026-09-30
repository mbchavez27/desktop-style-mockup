#include "imgui.h"
#include "core/AppState.h"
#include "ui/MockApps.h"

void RenderMockApps()
{
    if (AppState::show_app_1)
    {
        ImGui::Begin("Word", &AppState::show_app_1);
        ImGui::Text("Word Placeholder.");
        ImGui::End();
    }
    if (AppState::show_app_2)
    {
        ImGui::Begin("Paint", &AppState::show_app_2);
        ImGui::Text("Paint Placeholder");
        ImGui::End();
    }
}