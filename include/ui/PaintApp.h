#pragma once

#include <vector>

#include "imgui.h" // ImVec2, ImU32, ImVec4

/**
 * @brief MS Paint-style canvas drawing app.
 *
 * Stores completed strokes as polylines and replays them every frame;
 * the active tool picks brush/eraser color and a per-tool stroke size.
 * Instantiated as a member of MockApps — no standalone global.
 */
class PaintApp
{
public:
    /**
     * @brief Renders the Paint window (toolbar + canvas) for this frame.
     *
     * Caller (MockApps) gates on AppState::show_app_2; the flag is passed
     * to ImGui::Begin so the native 'X' toggles it.
     */
    void Render();

private:
    /// One committed or in-progress stroke.
    struct Stroke
    {
        ImU32 color;
        float thickness;
        std::vector<ImVec2> pts; // canvas-local so strokes survive window moves
    };

    /**
     * @brief Draws the tool/color/size/history controls above the canvas.
     */
    void RenderToolbar();

    /**
     * @brief Draws the white canvas, replays strokes, captures pointer drags.
     */
    void RenderCanvas();

    /**
     * @brief Packs an sRGB float triple into an IM_COL32 value.
     *
     * @param c Array of 3 floats in [0, 1].
     * @return Packed ABGR color.
     */
    static ImU32 PackColor(const float c[3]);

    /**
     * @brief Converts an IM_COL32 value into an ImVec4 for ColorButton.
     *
     * @param c Packed ABGR color.
     * @return Normalized RGBA vector.
     */
    static ImVec4 ToVec4(ImU32 c);

    std::vector<Stroke> strokes;      ///< Committed strokes, oldest first.
    float color[3] = {0.f, 0.f, 0.f}; ///< Current brush RGB for ColorEdit3.
    float brush_size = 4.0f;          ///< Brush stroke width in px.
    float eraser_size = 16.0f;        ///< Eraser stroke width in px.
    bool eraser = false;              ///< Active tool: true = eraser.
    bool drawing = false;             ///< True while a drag owns the canvas.
};
