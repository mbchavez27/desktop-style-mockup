#include "imgui.h"
#include "core/AppState.h"
#include "ui/DesktopIcons.h"
#include "ui/IconCache.h"

namespace
{

constexpr float kCellW = 88.0f; // icon + label cell
constexpr float kCellH = 70.0f;
constexpr float kIconPx = 40.0f;

} // namespace

DesktopIcons g_desktop_icons;

void DesktopIcons::DesktopIcon(const char *id, const char *icon_path, const char *title, bool *toggle)
{
    const AppIcon &icon = GetIcon(icon_path);
    const bool active = *toggle;

    // One invisible click target for the whole cell; icon + title are visuals.
    ImGui::InvisibleButton(id, ImVec2(kCellW, kCellH));
    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
        *toggle = !*toggle;
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
        ImGui::SetTooltip("%s", title);

    const ImVec2 min = ImGui::GetItemRectMin();
    const ImVec2 max = ImGui::GetItemRectMax();
    ImDrawList *dl = ImGui::GetWindowDrawList();

    // Active = open window (accent wash); hover = faint highlight.
    if (active)
        dl->AddRectFilled(min, max, IM_COL32(90, 180, 255, 60), 6.0f);
    else if (ImGui::IsItemHovered())
        dl->AddRectFilled(min, max, IM_COL32(255, 255, 255, 28), 6.0f);

    // Icon centered horizontally, top of cell.
    if (icon.ok())
    {
        const float ix = min.x + (kCellW - kIconPx) * 0.5f;
        dl->AddImage(icon.tex, ImVec2(ix, min.y + 4.0f), ImVec2(ix + kIconPx, min.y + 4.0f + kIconPx));
    }

    // Title centered beneath, with a shadow for wallpaper readability.
    const ImVec2 tw = ImGui::CalcTextSize(title);
    const ImVec2 tp(min.x + (kCellW - tw.x) * 0.5f, min.y + 4.0f + kIconPx + 4.0f);
    dl->AddText(ImVec2(tp.x + 1.0f, tp.y + 1.0f), IM_COL32(0, 0, 0, 200), title);
    dl->AddText(tp, IM_COL32(255, 255, 255, 255), title);
}

void DesktopIcons::Render()
{
    ImGuiViewport *vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(vp->WorkPos.x + 12.0f, vp->WorkPos.y + 12.0f));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 4.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 8.0f));
    ImGui::Begin("##DesktopIcons", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking |
                     ImGuiWindowFlags_NoBackground);

    DesktopIcon("##desk_app1", "assets/images/word.png", "Word", &AppState::show_app_1);
    DesktopIcon("##desk_app2", "assets/images/paint.png", "Paint", &AppState::show_app_2);
    DesktopIcon("##desk_taskmgr", "assets/images/taskmgr.png", "Task Manager", &AppState::show_task_mgr);

    ImGui::End();
    ImGui::PopStyleVar(2);
}
