#include <chrono>
#include <ctime>

#include "imgui.h"
#include "core/AppState.h"
#include "ui/IconCache.h"
#include "ui/Taskbar.h"

namespace
{

constexpr float kIconSize = 28.0f; // px; + FramePadding(2) fills the 44px bar exactly

} // namespace

Taskbar g_taskbar;

void Taskbar::TaskbarIconButton(const char *id, const char *icon_path, const char *tooltip, bool *toggle)
{
    const AppIcon &icon = GetIcon(icon_path);
    const bool active = *toggle;

    // Active window reads as a pressed-in key.
    if (active)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.30f, 0.36f, 0.48f, 1.00f));

    bool pressed = false;
    if (icon.ok())
        pressed = ImGui::ImageButton(id, icon.tex, ImVec2(kIconSize, kIconSize));
    else
        pressed = ImGui::Button(id, ImVec2(kIconSize, kIconSize)); // shows until the PNG lands

    if (active)
        ImGui::PopStyleColor();

    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
        ImGui::SetTooltip("%s", tooltip);

    if (pressed)
        *toggle = !*toggle;

    // Running indicator: accent underline.
    if (active)
    {
        ImDrawList *dl = ImGui::GetWindowDrawList();
        const ImVec2 min = ImGui::GetItemRectMin();
        const ImVec2 max = ImGui::GetItemRectMax();
        dl->AddLine(ImVec2(min.x + 4.0f, max.y - 2.0f), ImVec2(max.x - 4.0f, max.y - 2.0f),
                    IM_COL32(90, 180, 255, 255), 2.0f);
    }
}

void Taskbar::Render()
{
    ImGuiViewport *vp = ImGui::GetMainViewport();
    // Anchor: bottom edge, full width.
    ImGui::SetNextWindowPos(ImVec2(vp->WorkPos.x, vp->WorkPos.y + vp->WorkSize.y - kTaskbarHeight));
    ImGui::SetNextWindowSize(ImVec2(vp->WorkSize.x, kTaskbarHeight));

    // Windows XP Luna blue: flat base + painted gradient sheen. Icons stay
    // chromeless like Quick Launch: transparent until hovered/pressed.
    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(36, 93, 219, 255));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.15f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.25f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 6.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f, 0.0f));
    ImGui::Begin("##Taskbar", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking);

    // XP sheen over the base fill: bright top edge, light band, deep blue body.
    {
        ImDrawList *dl = ImGui::GetWindowDrawList();
        const ImVec2 pos = ImGui::GetWindowPos();
        const float w = ImGui::GetWindowWidth();
        const float h = ImGui::GetWindowHeight();
        const float band = h * 0.28f;
        dl->AddRectFilledMultiColor(ImVec2(pos.x, pos.y), ImVec2(pos.x + w, pos.y + band),
                                    IM_COL32(63, 140, 243, 255), IM_COL32(63, 140, 243, 255),
                                    IM_COL32(36, 93, 219, 255), IM_COL32(36, 93, 219, 255));
        dl->AddRectFilledMultiColor(ImVec2(pos.x, pos.y + band), ImVec2(pos.x + w, pos.y + h),
                                    IM_COL32(36, 93, 219, 255), IM_COL32(36, 93, 219, 255),
                                    IM_COL32(22, 60, 160, 255), IM_COL32(22, 60, 160, 255));
        dl->AddLine(ImVec2(pos.x, pos.y + 0.5f), ImVec2(pos.x + w, pos.y + 0.5f),
                    IM_COL32(140, 195, 255, 255), 1.0f);
    }

    // Center the row: content origin is WindowPadding.x, so (W - total) / 2 is exact.
    const float cell = kIconSize + 2.0f * 2.0f; // icon + FramePadding.x * 2
    const float total = 3.0f * cell + 2.0f * 10.0f;
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - total) * 0.5f);

    TaskbarIconButton("##app1", "assets/images/word.png", "Word", &AppState::show_app_1);
    ImGui::SameLine();
    TaskbarIconButton("##app2", "assets/images/paint.png", "Paint", &AppState::show_app_2);
    ImGui::SameLine();
    TaskbarIconButton("##taskmgr", "assets/images/taskmgr.png", "Task Manager", &AppState::show_task_mgr);

    // Tray group (clock + PWR) pinned to the right edge. Each item is placed
    // at its own measured height so both centers sit on the bar midline.
    const std::time_t now =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char clock[16] = {};
    std::strftime(clock, sizeof(clock), "%H:%M:%S", std::localtime(&now));
    const ImVec2 pwr_size(52.0f, 0.0f);
    const float spacing = 8.0f;
    const float mid_y = kTaskbarHeight * 0.5f;
    const float text_h = ImGui::GetTextLineHeight();
    const float btn_h = ImGui::GetFrameHeight();
    const float pwr_x = ImGui::GetWindowWidth() - pwr_size.x - 8.0f;
    const float clock_x = pwr_x - spacing - ImGui::CalcTextSize(clock).x;
    ImGui::SetCursorPos(ImVec2(clock_x, mid_y - text_h * 0.5f));
    ImGui::TextUnformatted(clock);
    ImGui::SetCursorPos(ImVec2(pwr_x, mid_y - btn_h * 0.5f));
    if (ImGui::Button("PWR", pwr_size))
        AppState::is_running = false;

    ImGui::End();
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(4);
}
