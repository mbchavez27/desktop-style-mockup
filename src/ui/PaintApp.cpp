#include "imgui.h"
#include "core/AppState.h"
#include "ui/PaintApp.h"

#include <algorithm>
#include <vector>

namespace
{

constexpr float kMinThickness = 1.0f;
constexpr float kMaxThickness = 24.0f;
constexpr ImU32 kCanvasBg = IM_COL32(255, 255, 255, 255);
constexpr ImVec4 kAccent = ImVec4(90.0f / 255.0f, 180.0f / 255.0f, 255.0f / 255.0f, 1.0f);

constexpr ImU32 kSwatches[] = {
    IM_COL32(0, 0, 0, 255),
    IM_COL32(255, 0, 0, 255),
    IM_COL32(255, 165, 0, 255),
    IM_COL32(255, 255, 0, 255),
    IM_COL32(0, 128, 0, 255),
    IM_COL32(0, 100, 255, 255),
    IM_COL32(128, 0, 255, 255),
    IM_COL32(255, 255, 255, 255),
};

} // namespace

ImU32 PaintApp::PackColor(const float c[3])
{
    return IM_COL32(static_cast<int>(c[0] * 255.0f + 0.5f),
                    static_cast<int>(c[1] * 255.0f + 0.5f),
                    static_cast<int>(c[2] * 255.0f + 0.5f), 255);
}

ImVec4 PaintApp::ToVec4(ImU32 c)
{
    return ImVec4(static_cast<float>(c & 0xFF) / 255.0f,
                  static_cast<float>((c >> 8) & 0xFF) / 255.0f,
                  static_cast<float>((c >> 16) & 0xFF) / 255.0f, 1.0f);
}

void PaintApp::RenderToolbar()
{
    // Toggle buttons: highlight the active tool with the accent color.
    if (!eraser)
        ImGui::PushStyleColor(ImGuiCol_Button, kAccent);
    const bool brush_clicked = ImGui::Button("Brush");
    if (!eraser)
        ImGui::PopStyleColor();

    ImGui::SameLine();
    if (eraser)
        ImGui::PushStyleColor(ImGuiCol_Button, kAccent);
    const bool eraser_clicked = ImGui::Button("Eraser");
    if (eraser)
        ImGui::PopStyleColor();

    if (brush_clicked)
        eraser = false;
    if (eraser_clicked)
        eraser = true;

    ImGui::SameLine();
    for (size_t i = 0; i < sizeof(kSwatches) / sizeof(kSwatches[0]); ++i)
    {
        ImGui::PushID(static_cast<int>(i));
        if (ImGui::ColorButton("##swatch", ToVec4(kSwatches[i]), ImGuiColorEditFlags_NoTooltip))
        {
            const ImVec4 v = ToVec4(kSwatches[i]);
            color[0] = v.x;
            color[1] = v.y;
            color[2] = v.z;
            eraser = false; // picking a color switches back to brush
        }
        ImGui::PopID();
        ImGui::SameLine();
    }
    ImGui::ColorEdit3("Color", color);

    // Second toolbar row: size slider + history actions.
    float &size = eraser ? eraser_size : brush_size;
    ImGui::SetNextItemWidth(160.0f);
    ImGui::SliderFloat("Size##size", &size, kMinThickness, kMaxThickness, "%.0f px");

    ImGui::SameLine();
    if (ImGui::Button("Undo") && !strokes.empty())
        strokes.pop_back();
    ImGui::SameLine();
    if (ImGui::Button("Clear"))
        strokes.clear();

    ImGui::Separator();
}

void PaintApp::RenderCanvas()
{
    const ImVec2 p0 = ImGui::GetCursorScreenPos();
    const ImVec2 sz = ImGui::GetContentRegionAvail();
    if (sz.x <= 0.0f || sz.y <= 0.0f)
        return;
    const ImVec2 p1(p0.x + sz.x, p0.y + sz.y);

    ImDrawList *dl = ImGui::GetWindowDrawList();
    dl->AddRectFilled(p0, p1, kCanvasBg);
    dl->AddRect(p0, p1, IM_COL32(160, 160, 160, 255));

    ImGui::InvisibleButton("##canvas", sz);

    // Replay all strokes on top of the canvas background.
    static std::vector<ImVec2> scratch;
    for (const Stroke &s : strokes)
    {
        scratch.resize(s.pts.size());
        for (size_t i = 0; i < s.pts.size(); ++i)
            scratch[i] = ImVec2(p0.x + s.pts[i].x, p0.y + s.pts[i].y);

        if (s.pts.size() == 1)
            dl->AddCircleFilled(scratch[0], s.thickness * 0.5f, s.color);
        else
            dl->AddPolyline(scratch.data(), static_cast<int>(scratch.size()),
                            s.color, ImDrawFlags_None, s.thickness);
    }

    // Capture the drag: item-local coords, clamped so strokes stay on canvas.
    const ImVec2 m = ImGui::GetIO().MousePos;
    const ImVec2 local(std::clamp(m.x - p0.x, 0.0f, sz.x),
                       std::clamp(m.y - p0.y, 0.0f, sz.y));

    if (ImGui::IsItemActive() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        if (!drawing)
        {
            Stroke s;
            s.color = eraser ? kCanvasBg : PackColor(color);
            s.thickness = eraser ? eraser_size : brush_size;
            s.pts.push_back(local);
            strokes.push_back(std::move(s));
            drawing = true;
        }
        else if (!strokes.empty())
        {
            std::vector<ImVec2> &pts = strokes.back().pts;
            const ImVec2 &last = pts.back();
            const float dx = local.x - last.x;
            const float dy = local.y - last.y;
            if (dx * dx + dy * dy >= 1.0f) // skip duplicates while dragging
                pts.push_back(local);
        }
    }
    else
    {
        drawing = false;
    }
}

void PaintApp::Render()
{
    ImGui::SetNextWindowSize(ImVec2(600, 440), ImGuiCond_FirstUseEver);
    ImGui::Begin("Paint", &AppState::show_app_2);
    RenderToolbar();
    RenderCanvas();
    ImGui::End();
}
