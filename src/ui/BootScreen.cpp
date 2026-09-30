#include <chrono>

#include "imgui.h"

#include "ui/BootScreen.h"
#include "ui/IconCache.h"

namespace
{

constexpr float kFadeIn = 1.0f; // fade 0 -> 1
constexpr float kHold = 3.0f;   // full-opacity show time
constexpr float kFadeOut = 1.0f; // fade 1 -> 0

float BootAlpha(float elapsed)
{
    const float total = kFadeIn + kHold + kFadeOut;
    if (elapsed < 0.0f || elapsed >= total)
        return 0.0f;
    if (elapsed < kFadeIn)
        return elapsed / kFadeIn;
    if (elapsed < kFadeIn + kHold)
        return 1.0f;
    return 1.0f - (elapsed - kFadeIn - kHold) / kFadeOut;
}

} // namespace

bool RenderBootScreen()
{
    // Clock starts on the first frame the splash is drawn.
    static const auto start = std::chrono::steady_clock::now();
    // Click-to-skip state: once set, we fade out from whatever alpha we had.
    static bool skipping = false;
    static float skip_from_alpha = 1.0f;
    static float skip_began_at = 0.0f;

    const float elapsed =
        std::chrono::duration<float>(std::chrono::steady_clock::now() - start).count();

    const float total = kFadeIn + kHold + kFadeOut;
    if (elapsed >= total)
        return false;
    if (skipping && (elapsed - skip_began_at) >= kFadeOut)
        return false;

    float alpha = BootAlpha(elapsed);

    // Any click skips the wait and fades out from the current alpha.
    // Checked before drawing so the very click frame already starts fading.
    // Already in the natural fade-out: let it finish on its own.
    const bool in_natural_fade_out = elapsed >= kFadeIn + kHold;
    if (!skipping && !in_natural_fade_out)
    {
        const bool clicked = ImGui::IsMouseClicked(ImGuiMouseButton_Left) ||
                             ImGui::IsMouseClicked(ImGuiMouseButton_Right) ||
                             ImGui::IsMouseClicked(ImGuiMouseButton_Middle);
        if (clicked)
        {
            skipping = true;
            skip_from_alpha = alpha;
            skip_began_at = elapsed;
        }
    }
    if (skipping)
    {
        const float t = (elapsed - skip_began_at) / kFadeOut;
        alpha = skip_from_alpha * (1.0f - t);
        if (alpha < 0.0f)
            alpha = 0.0f;
    }
    const auto alpha_u8 = static_cast<ImU32>(alpha * 255.0f);

    // Fullscreen opaque-black window that swallows all input while active.
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 255));
    ImGui::Begin("##BootScreen", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus |
                     ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDocking);

    ImDrawList *dl = ImGui::GetWindowDrawList();
    const ImVec2 origin = ImGui::GetWindowPos();
    const float vw = ImGui::GetWindowWidth();
    const float vh = ImGui::GetWindowHeight();

    const AppIcon &boot = GetIcon("assets/images/boot.png");
    if (boot.ok())
    {
        // Scale to fit while preserving aspect ratio, centered.
        const float scale =
            (vw / static_cast<float>(boot.w)) < (vh / static_cast<float>(boot.h))
                ? (vw / static_cast<float>(boot.w))
                : (vh / static_cast<float>(boot.h));
        const float dw = static_cast<float>(boot.w) * scale;
        const float dh = static_cast<float>(boot.h) * scale;
        const ImVec2 p_min(origin.x + (vw - dw) * 0.5f, origin.y + (vh - dh) * 0.5f);
        const ImVec2 p_max(p_min.x + dw, p_min.y + dh);
        dl->AddImage(boot.tex, p_min, p_max, ImVec2(0, 0), ImVec2(1, 1),
                     IM_COL32(255, 255, 255, alpha_u8));
    }
    else
    {
        // PNG missing: still show a black screen with a fading label.
        const char *msg = "Loading...";
        const ImVec2 ts = ImGui::CalcTextSize(msg);
        dl->AddText(ImVec2(origin.x + (vw - ts.x) * 0.5f, origin.y + (vh - ts.y) * 0.5f),
                    IM_COL32(255, 255, 255, alpha_u8), msg);
    }

    ImGui::End();
    ImGui::PopStyleColor();
    return true;
}
