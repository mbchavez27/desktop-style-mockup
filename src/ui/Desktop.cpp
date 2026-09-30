#include <chrono>
#include <ctime>

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

    // Clock with the power button on the same line, pinned top-right.
    const std::time_t now =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char clock[16] = {};
    std::strftime(clock, sizeof(clock), "%H:%M:%S", std::localtime(&now));
    const ImVec2 pwr_size(80.0f, 0.0f);
    const float spacing = 8.0f;
    const float group =
        ImGui::CalcTextSize(clock).x + spacing + pwr_size.x;
    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - group - 16.0f, 12.0f));
    ImGui::TextUnformatted(clock);
    ImGui::SameLine(0.0f, spacing);
    // Power button; exits the main loop.
    if (ImGui::Button("PWR", pwr_size))
    {
        AppState::is_running = false;
    }

    ImGui::End();
}