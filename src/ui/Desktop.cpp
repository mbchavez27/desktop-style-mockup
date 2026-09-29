#include <chrono>
#include <ctime>

#include "imgui.h"

#include "core/AppState.h"
#include "ui/Desktop.h"

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

    // Paint a navy-to-teal diagonal wallpaper gradient.
    const ImVec2 origin = ImGui::GetWindowPos();
    const ImVec2 extent =
        ImVec2(origin.x + ImGui::GetWindowWidth(), origin.y + ImGui::GetWindowHeight());
    ImGui::GetWindowDrawList()->AddRectFilledMultiColor(origin, extent,
                                                        IM_COL32(15, 30, 70, 255), IM_COL32(25, 110, 140, 255),
                                                        IM_COL32(30, 150, 160, 255), IM_COL32(12, 45, 95, 255));

    // Show the local time in the top-right corner.
    const std::time_t now =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char clock[16] = {};
    std::strftime(clock, sizeof(clock), "%H:%M:%S", std::localtime(&now));
    ImGui::SetCursorPos(
        ImVec2(ImGui::GetWindowWidth() - ImGui::CalcTextSize(clock).x - 16.0f, 12.0f));
    ImGui::TextUnformatted(clock);

    // Power button in the bottom-right corner; exits the main loop.
    const ImVec2 pwr_size(80.0f, 0.0f);
    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - pwr_size.x - 16.0f,
                               ImGui::GetWindowHeight() - ImGui::GetFrameHeight() - 16.0f));
    if (ImGui::Button("PWR", pwr_size))
    {
        AppState::is_running = false;
    }

    ImGui::End();
}