#include "imgui.h"

#include "core/AppState.h"
#include "ui/Desktop.h"
#include "ui/IconCache.h"

void RenderDesktop()
{
    // Cover the full viewport behind all other windows.
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    ImGui::Begin("##Desktop", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus |
                     ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground);

    // Wallpaper: stretch background.png over the viewport, gradient if missing.
    const ImVec2 origin = ImGui::GetWindowPos();
    const ImVec2 extent =
        ImVec2(origin.x + ImGui::GetWindowWidth(), origin.y + ImGui::GetWindowHeight());
    const AppIcon &wallpaper = GetIcon("assets/images/background.png");
    if (wallpaper.ok())
        ImGui::GetWindowDrawList()->AddImage(wallpaper.tex, origin, extent);
    else
        ImGui::GetWindowDrawList()->AddRectFilledMultiColor(origin, extent,
                                                            IM_COL32(15, 30, 70, 255), IM_COL32(25, 110, 140, 255),
                                                            IM_COL32(30, 150, 160, 255), IM_COL32(12, 45, 95, 255));

    ImGui::End();
}