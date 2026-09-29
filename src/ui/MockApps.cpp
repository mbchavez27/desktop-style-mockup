#include "imgui.h"
#include "core/AppState.h"
#include "ui/MockApps.h"

void RenderMockApps()
{
    if (AppState::show_app_1)
    {
        ImGui::Begin("App 1", &AppState::show_app_1);
        ImGui::Text("Mock App 1 placeholder.");
        ImGui::End();
    }
    if (AppState::show_app_2)
    {
        ImGui::Begin("App 2", &AppState::show_app_2);
        ImGui::Text("Mock App 2 placeholder.");
        ImGui::End();
    }
}