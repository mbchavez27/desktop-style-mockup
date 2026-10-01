#include <chrono>
#include <ctime>

#include "imgui.h"
#include "core/AppState.h"
#include "ui/IconCache.h"
#include "ui/Taskbar.h"

namespace
{

constexpr float kIconSize = 28.0f; // px; + FramePadding(2) fills the 44px bar exactly
constexpr float kMenuWidth = 340.0f;
constexpr float kMenuHeaderH = 58.0f;
constexpr float kMenuFooterH = 48.0f;
constexpr float kMenuRowH = 46.0f;
constexpr float kMenuRowIcon = 32.0f;

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

bool Taskbar::RenderStartButton()
{
    const AppIcon &icon = GetIcon("assets/images/start_btn.png");

    // Open state reads as pressed-in so the menu feels attached.
    if (show_start_menu_)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.25f));

    bool pressed = false;
    if (icon.ok())
        pressed = ImGui::ImageButton("##start", icon.tex, ImVec2(kIconSize, kIconSize));
    else
        pressed = ImGui::Button("start", ImVec2(64.0f, kIconSize)); // fallback label until the PNG lands

    if (show_start_menu_)
        ImGui::PopStyleColor();

    const bool hovered = ImGui::IsItemHovered();
    if (hovered && ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
        ImGui::SetTooltip("Start");

    if (pressed)
        show_start_menu_ = !show_start_menu_;

    return hovered;
}

void Taskbar::StartMenuAppRow(const char *id, const char *icon_path, const char *label, bool *toggle)
{
    const float avail_w = ImGui::GetContentRegionAvail().x;
    ImGui::PushID(id);
    ImGui::InvisibleButton("##row", ImVec2(avail_w, kMenuRowH));
    const bool hovered = ImGui::IsItemHovered();
    const bool active = *toggle;
    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
    {
        *toggle = !*toggle;
        show_start_menu_ = false; // launching dismisses the menu like XP
    }
    if (hovered)
        ImGui::SetTooltip("%s", label);

    // Full-row highlight: XP hover wash, stronger wash for running apps.
    ImDrawList *dl = ImGui::GetWindowDrawList();
    const ImVec2 min = ImGui::GetItemRectMin();
    const ImVec2 max = ImGui::GetItemRectMax();
    if (active)
        dl->AddRectFilled(min, max, IM_COL32(214, 234, 252, 255));
    else if (hovered)
        dl->AddRectFilled(min, max, IM_COL32(232, 243, 255, 255));
    if (hovered || active)
        dl->AddRect(min, max, IM_COL32(153, 193, 239, 255), 3.0f);

    // Icon left, label vertically centered.
    const AppIcon &icon = GetIcon(icon_path);
    const float icon_y = min.y + (kMenuRowH - kMenuRowIcon) * 0.5f;
    if (icon.ok())
        dl->AddImage(icon.tex, ImVec2(min.x + 8.0f, icon_y),
                     ImVec2(min.x + 8.0f + kMenuRowIcon, icon_y + kMenuRowIcon));
    dl->AddText(ImVec2(min.x + 8.0f + kMenuRowIcon + 10.0f, min.y + (kMenuRowH - ImGui::GetTextLineHeight()) * 0.5f),
                IM_COL32(20, 30, 50, 255), label);
    ImGui::PopID();
}

void Taskbar::RenderStartMenu(bool start_hovered)
{
    ImGuiViewport *vp = ImGui::GetMainViewport();
    const int row_count = 4;
    const float menu_h = kMenuHeaderH + static_cast<float>(row_count) * kMenuRowH + 8.0f + kMenuFooterH;
    const ImVec2 menu_pos(vp->WorkPos.x + 2.0f, vp->WorkPos.y + vp->WorkSize.y - kTaskbarHeight - menu_h - 2.0f);

    ImGui::SetNextWindowPos(menu_pos);
    ImGui::SetNextWindowSize(ImVec2(kMenuWidth, menu_h));

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.14f, 0.36f, 0.86f, 1.00f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
    ImGui::Begin("##StartMenu", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking);

    ImDrawList *dl = ImGui::GetWindowDrawList();
    const ImVec2 pos = ImGui::GetWindowPos();
    const float w = ImGui::GetWindowWidth();

    // Header: Luna blue gradient with the signed-in user, like XP.
    dl->AddRectFilledMultiColor(pos, ImVec2(pos.x + w, pos.y + kMenuHeaderH),
                                IM_COL32(22, 60, 160, 255), IM_COL32(22, 60, 160, 255),
                                IM_COL32(63, 140, 243, 255), IM_COL32(63, 140, 243, 255));
    dl->AddLine(ImVec2(pos.x, pos.y + 0.5f), ImVec2(pos.x + w, pos.y + 0.5f),
                IM_COL32(140, 195, 255, 255), 1.0f);
    ImGui::SetCursorPos(ImVec2(12.0f, (kMenuHeaderH - ImGui::GetTextLineHeightWithSpacing()) * 0.5f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::TextUnformatted("Administrator");
    ImGui::PopStyleColor();

    // App rows on the white body.
    ImGui::SetCursorPos(ImVec2(4.0f, kMenuHeaderH + 4.0f));
    ImGui::BeginGroup();
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 2.0f));
    StartMenuAppRow("##sm_calc", "assets/images/calculator.png", "Calculator", &AppState::show_app_1);
    StartMenuAppRow("##sm_paint", "assets/images/paint.png", "Paint", &AppState::show_app_2);
    StartMenuAppRow("##sm_explorer", "assets/images/file_explorer.png", "File Explorer", &AppState::show_file_explorer);
    StartMenuAppRow("##sm_taskmgr", "assets/images/taskmgr.png", "Task Manager", &AppState::show_task_mgr);
    ImGui::PopStyleVar();
    ImGui::EndGroup();

    // Footer: Luna blue gradient hosting the power button on the right.
    const float footer_y = menu_h - kMenuFooterH;
    dl->AddRectFilledMultiColor(ImVec2(pos.x, pos.y + footer_y), ImVec2(pos.x + w, pos.y + menu_h),
                                IM_COL32(36, 93, 219, 255), IM_COL32(36, 93, 219, 255),
                                IM_COL32(22, 60, 160, 255), IM_COL32(22, 60, 160, 255));
    const AppIcon &off = GetIcon("assets/images/off_btn.png");
    const float btn_px = 28.0f;
    const float mid_y = footer_y + kMenuFooterH * 0.5f;
    ImGui::SetCursorPos(ImVec2(kMenuWidth - btn_px - 10.0f, mid_y - btn_px * 0.5f));
    bool power_pressed = false;
    if (off.ok())
        power_pressed = ImGui::ImageButton("##sm_power", off.tex, ImVec2(btn_px, btn_px));
    else
        power_pressed = ImGui::Button("##sm_power_fallback", ImVec2(btn_px, btn_px));
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
        ImGui::SetTooltip("Turn Off Computer");
    if (power_pressed)
        AppState::is_running = false;

    const bool menu_hovered =
        ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

    ImGui::End();
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);

    // Dismiss like XP: outside click or Escape.
    if (ImGui::IsKeyPressed(ImGuiKey_Escape))
        show_start_menu_ = false;
    else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !menu_hovered && !start_hovered)
        show_start_menu_ = false;
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

    // Left cluster: Start button, then Quick Launch icons to its right.
    const bool start_hovered = RenderStartButton();
    ImGui::SameLine();
    TaskbarIconButton("##app1", "assets/images/calculator.png", "Calculator", &AppState::show_app_1);
    ImGui::SameLine();
    TaskbarIconButton("##app2", "assets/images/paint.png", "Paint", &AppState::show_app_2);
    ImGui::SameLine();
    TaskbarIconButton("##explorer", "assets/images/file_explorer.png", "File Explorer", &AppState::show_file_explorer);
    ImGui::SameLine();
    TaskbarIconButton("##taskmgr", "assets/images/taskmgr.png", "Task Manager", &AppState::show_task_mgr);

    // Tray clock pinned to the right edge, centered on the bar midline.
    // Power now lives in the Start menu footer, so the tray keeps only time.
    const std::time_t now =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char clock[16] = {};
    std::strftime(clock, sizeof(clock), "%H:%M:%S", std::localtime(&now));
    const float mid_y = kTaskbarHeight * 0.5f;
    const float text_h = ImGui::GetTextLineHeight();
    const float clock_x = ImGui::GetWindowWidth() - ImGui::CalcTextSize(clock).x - 12.0f;
    ImGui::SetCursorPos(ImVec2(clock_x, mid_y - text_h * 0.5f));
    ImGui::TextUnformatted(clock);

    ImGui::End();
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(4);

    // Menu floats above the bar so it is never clipped by it.
    if (show_start_menu_)
        RenderStartMenu(start_hovered);
}
