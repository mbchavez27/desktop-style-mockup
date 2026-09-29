#include "imgui.h"
#include "core/AppState.h"
#include "ui/Taskbar.h"

void RenderTaskbar()
{
    ImGuiViewport *vp = ImGui::GetMainViewport();
    // Anchor: bottom edge, full width.
    ImGui::SetNextWindowPos(ImVec2(vp->WorkPos.x, vp->WorkPos.y + vp->WorkSize.y - kTaskbarHeight));
    ImGui::SetNextWindowSize(ImVec2(vp->WorkSize.x, kTaskbarHeight));

    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(24, 26, 34, 255)); // dark vs wallpaper
    ImGui::Begin("##Taskbar", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking);

    if (ImGui::Button("App 1"))
        AppState::show_app_1 = !AppState::show_app_1;
    ImGui::SameLine();
    if (ImGui::Button("App 2"))
        AppState::show_app_2 = !AppState::show_app_2;
    ImGui::SameLine();
    if (ImGui::Button("Task Manager"))
        AppState::show_task_mgr = !AppState::show_task_mgr;

    ImGui::End();
    ImGui::PopStyleColor();
}