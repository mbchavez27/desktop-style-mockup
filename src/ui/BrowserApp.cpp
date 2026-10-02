#include "ui/BrowserApp.h"

#include <algorithm>
#include <cfloat>
#include <cstdio>

#include "core/AppState.h"
#include "imgui.h"

namespace
{

constexpr float kNavButtonWidth = 28.0f;
constexpr float kNavButtonHeight = 24.0f;

// Google brand colors.
constexpr ImU32 kGoogleBlue = IM_COL32(66, 133, 244, 255);
constexpr ImU32 kGoogleRed = IM_COL32(234, 67, 53, 255);
constexpr ImU32 kGoogleYellow = IM_COL32(251, 188, 5, 255);
constexpr ImU32 kGoogleGreen = IM_COL32(52, 168, 83, 255);
constexpr ImU32 kGoogleGray = IM_COL32(95, 99, 104, 255);
constexpr ImU32 kGoogleBorder = IM_COL32(223, 225, 229, 255);

// YouTube brand colors.
constexpr ImU32 kYouTubeRed = IM_COL32(255, 0, 0, 255);
constexpr ImU32 kYouTubeDarkBg = IM_COL32(15, 15, 15, 255);
constexpr ImU32 kYouTubeChipBg = IM_COL32(39, 39, 39, 255);

// Wikipedia brand colors.
constexpr ImU32 kWikiBg = IM_COL32(255, 255, 255, 255);
constexpr ImU32 kWikiBorder = IM_COL32(162, 169, 177, 255);
constexpr ImU32 kWikiHeaderBg = IM_COL32(204, 204, 255, 255);
constexpr ImU32 kWikiTocBg = IM_COL32(248, 249, 250, 255);
constexpr ImU32 kWikiLinkBlue = IM_COL32(6, 69, 173, 255);

} // namespace

void BrowserApp::RenderAddressBar(const char *url)
{
    // History buttons (static/disabled in mock).
    ImGui::BeginDisabled(true);
    ImGui::Button("<", ImVec2(kNavButtonWidth, kNavButtonHeight));
    ImGui::SameLine();
    ImGui::Button(">", ImVec2(kNavButtonWidth, kNavButtonHeight));
    ImGui::EndDisabled();

    ImGui::SameLine();
    if (ImGui::Button("R", ImVec2(kNavButtonWidth, kNavButtonHeight)))
    {
        // Static reload button simulates refresh.
    }
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
        ImGui::SetTooltip("Reload this page");

    ImGui::SameLine();
    if (ImGui::Button("H", ImVec2(kNavButtonWidth, kNavButtonHeight)))
    {
        requested_tab_ = static_cast<int>(Tab::Google);
    }
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
        ImGui::SetTooltip("Home (Google)");

    ImGui::SameLine();

    // Address Bar / Omnibox: displays static URL with secure indicator.
    const float avail_w = ImGui::GetContentRegionAvail().x;
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.18f, 0.20f, 0.24f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8.0f, 4.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

    char display_buf[256];
    std::snprintf(display_buf, sizeof(display_buf), "[Secure] %s", url);
    ImGui::SetNextItemWidth(avail_w);
    ImGui::InputText("##omnibox", display_buf, sizeof(display_buf), ImGuiInputTextFlags_ReadOnly);

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
}

void BrowserApp::RenderBookmarksBar()
{
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.0f, 2.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6.0f, 0.0f));

    if (ImGui::SmallButton("Google"))
        requested_tab_ = static_cast<int>(Tab::Google);
    ImGui::SameLine();
    if (ImGui::SmallButton("YouTube"))
        requested_tab_ = static_cast<int>(Tab::YouTube);
    ImGui::SameLine();
    if (ImGui::SmallButton("Wikipedia (OS)"))
        requested_tab_ = static_cast<int>(Tab::Wikipedia);

    ImGui::PopStyleVar(2);
}

void BrowserApp::RenderGooglePage()
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(255, 255, 255, 255));
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(32, 33, 36, 255));
    ImGui::BeginChild("##google_scroll", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);

    const float page_w = ImGui::GetWindowWidth();

    // Top-right navigation bar.
    {
        const float tr_w = 210.0f;
        ImGui::SetCursorPos(ImVec2(std::max(10.0f, page_w - tr_w), 12.0f));
        ImGui::TextUnformatted("Gmail");
        ImGui::SameLine(0.0f, 16.0f);
        ImGui::TextUnformatted("Images");
        ImGui::SameLine(0.0f, 16.0f);
        ImGui::TextDisabled("[:::]");
        ImGui::SameLine(0.0f, 16.0f);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(26.0f / 255.0f, 115.0f / 255.0f, 232.0f / 255.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(43.0f / 255.0f, 125.0f / 255.0f, 233.0f / 255.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(15.0f / 255.0f, 95.0f / 255.0f, 200.0f / 255.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::SmallButton("Sign in");
        ImGui::PopStyleColor(4);
    }

    // Spacer down to Google logo.
    ImGui::SetCursorPosY(70.0f);

    // Google Logo: Centered colored letters.
    {
        struct Letter
        {
            char ch;
            ImU32 col;
        };
        const Letter letters[] = {
            {'G', kGoogleBlue},
            {'o', kGoogleRed},
            {'o', kGoogleYellow},
            {'g', kGoogleBlue},
            {'l', kGoogleGreen},
            {'e', kGoogleRed}
        };

        const float logo_font_size = 46.0f;
        ImFont *font = ImGui::GetFont();
        float total_logo_w = 0.0f;
        for (const auto &item : letters)
        {
            char s[2] = {item.ch, '\0'};
            total_logo_w += font->CalcTextSizeA(logo_font_size, FLT_MAX, 0.0f, s).x;
        }

        const float start_x = (page_w - total_logo_w) * 0.5f;
        const ImVec2 cur_screen = ImGui::GetCursorScreenPos();
        ImDrawList *dl = ImGui::GetWindowDrawList();
        float cur_x = cur_screen.x + start_x;
        for (const auto &item : letters)
        {
            char s[2] = {item.ch, '\0'};
            const float letter_w = font->CalcTextSizeA(logo_font_size, FLT_MAX, 0.0f, s).x;
            dl->AddText(font, logo_font_size, ImVec2(cur_x, cur_screen.y), item.col, s);
            cur_x += letter_w;
        }
        ImGui::Dummy(ImVec2(page_w, 54.0f));
    }

    ImGui::Spacing();
    ImGui::Spacing();

    // Centered Search Box (Static mockup).
    {
        const float box_w = std::clamp(page_w - 40.0f, 260.0f, 520.0f);
        const float box_h = 42.0f;
        const float box_x = (page_w - box_w) * 0.5f;
        ImGui::SetCursorPosX(box_x);

        const ImVec2 p_min = ImGui::GetCursorScreenPos();
        const ImVec2 p_max = ImVec2(p_min.x + box_w, p_min.y + box_h);
        ImDrawList *dl = ImGui::GetWindowDrawList();

        dl->AddRectFilled(p_min, p_max, IM_COL32(255, 255, 255, 255), 21.0f);
        dl->AddRect(p_min, p_max, kGoogleBorder, 21.0f, 0, 1.5f);

        dl->AddText(ImVec2(p_min.x + 16.0f, p_min.y + 13.0f), kGoogleGray, "Q");
        dl->AddText(ImVec2(p_min.x + 40.0f, p_min.y + 13.0f), IM_COL32(110, 115, 122, 255),
                    "Search Google or type a URL");
        dl->AddText(ImVec2(p_max.x - 56.0f, p_min.y + 13.0f), kGoogleBlue, "[*]");
        dl->AddText(ImVec2(p_max.x - 30.0f, p_min.y + 13.0f), kGoogleRed, "[o]");

        ImGui::Dummy(ImVec2(box_w, box_h));
    }

    ImGui::Spacing();
    ImGui::Spacing();

    // Search action buttons (Static mockup).
    {
        const float btn_w = 130.0f;
        const float btn_h = 32.0f;
        const float spacing = 12.0f;
        const float total_w = btn_w * 2.0f + spacing;
        const float start_x = std::max(10.0f, (page_w - total_w) * 0.5f);

        ImGui::SetCursorPosX(start_x);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(248.0f / 255.0f, 249.0f / 255.0f, 250.0f / 255.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(241.0f / 255.0f, 243.0f / 255.0f, 244.0f / 255.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(232.0f / 255.0f, 234.0f / 255.0f, 237.0f / 255.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(60.0f / 255.0f, 64.0f / 255.0f, 67.0f / 255.0f, 1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

        ImGui::Button("Google Search", ImVec2(btn_w, btn_h));
        ImGui::SameLine(0.0f, spacing);
        ImGui::Button("I'm Feeling Lucky", ImVec2(btn_w, btn_h));

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(4);
    }

    ImGui::Spacing();
    ImGui::Spacing();

    // Languages offered.
    {
        const char *lang_text = "Google offered in: Filipino   Cebuano";
        const float tw = ImGui::CalcTextSize(lang_text).x;
        ImGui::SetCursorPosX(std::max(10.0f, (page_w - tw) * 0.5f));
        ImGui::TextDisabled("Google offered in:");
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(26.0f / 255.0f, 13.0f / 255.0f, 171.0f / 255.0f, 1.0f));
        ImGui::TextUnformatted("Filipino");
        ImGui::SameLine();
        ImGui::TextUnformatted("Cebuano");
        ImGui::PopStyleColor();
    }

    // Bottom footer.
    {
        ImGui::SetCursorPosY(std::max(ImGui::GetCursorPosY() + 50.0f, 400.0f));
        ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(242, 242, 242, 255));
        ImGui::BeginChild("##google_footer", ImVec2(0, 72.0f), false, ImGuiWindowFlags_NoScrollbar);

        ImGui::SetCursorPos(ImVec2(16.0f, 8.0f));
        ImGui::TextDisabled("Philippines");
        ImGui::Separator();

        ImGui::SetCursorPos(ImVec2(16.0f, 38.0f));
        ImGui::TextDisabled("About    Advertising    Business    How Search works");
        const float footer_w = ImGui::GetWindowWidth();
        if (footer_w > 500.0f)
        {
            ImGui::SameLine(footer_w - 220.0f);
            ImGui::TextDisabled("Privacy    Terms    Settings");
        }

        ImGui::EndChild();
        ImGui::PopStyleColor();
    }

    ImGui::EndChild();
    ImGui::PopStyleColor(2);
}

void BrowserApp::RenderYouTubePage()
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, kYouTubeDarkBg);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(241, 241, 241, 255));
    ImGui::BeginChild("##yt_scroll", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);

    const float page_w = ImGui::GetWindowWidth();

    // Top Header.
    {
        ImGui::SetCursorPos(ImVec2(14.0f, 10.0f));
        ImGui::TextUnformatted("=");
        ImGui::SameLine(0.0f, 14.0f);

        // Red YouTube Play Icon.
        const ImVec2 logo_pos = ImGui::GetCursorScreenPos();
        ImDrawList *dl = ImGui::GetWindowDrawList();
        dl->AddRectFilled(logo_pos, ImVec2(logo_pos.x + 28.0f, logo_pos.y + 20.0f), kYouTubeRed, 5.0f);
        dl->AddTriangleFilled(ImVec2(logo_pos.x + 11.0f, logo_pos.y + 6.0f),
                              ImVec2(logo_pos.x + 11.0f, logo_pos.y + 14.0f),
                              ImVec2(logo_pos.x + 19.0f, logo_pos.y + 10.0f),
                              IM_COL32(255, 255, 255, 255));
        ImGui::Dummy(ImVec2(32.0f, 20.0f));
        ImGui::SameLine(0.0f, 4.0f);

        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
        ImGui::TextUnformatted("YouTube");
        ImGui::PopStyleColor();
        ImGui::SameLine();
        ImGui::TextDisabled("PH");

        // Center static search bar.
        const float search_w = std::clamp(page_w * 0.45f, 180.0f, 360.0f);
        const float search_x = std::max(160.0f, (page_w - search_w) * 0.5f);
        ImGui::SetCursorPos(ImVec2(search_x, 8.0f));

        ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(18, 18, 18, 255));
        ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(48, 48, 48, 255));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 16.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 4.0f));

        char yt_search[64] = "Search";
        ImGui::SetNextItemWidth(search_w - 42.0f);
        ImGui::InputText("##yt_search", yt_search, sizeof(yt_search), ImGuiInputTextFlags_ReadOnly);
        ImGui::SameLine(0.0f, 4.0f);
        ImGui::Button("Q", ImVec2(32.0f, 24.0f));

        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor(2);

        // Right side user controls.
        if (page_w > 440.0f)
        {
            ImGui::SetCursorPos(ImVec2(page_w - 110.0f, 8.0f));
            ImGui::TextUnformatted("+");
            ImGui::SameLine(0.0f, 16.0f);
            ImGui::TextUnformatted("[*]");
            ImGui::SameLine(0.0f, 16.0f);
            const ImVec2 av_pos = ImGui::GetCursorScreenPos();
            dl->AddCircleFilled(ImVec2(av_pos.x + 10.0f, av_pos.y + 10.0f), 10.0f, IM_COL32(33, 150, 243, 255));
            dl->AddText(ImVec2(av_pos.x + 7.0f, av_pos.y + 2.0f), IM_COL32(255, 255, 255, 255), "U");
            ImGui::Dummy(ImVec2(22.0f, 20.0f));
        }
    }

    ImGui::Separator();

    // Category filter chips.
    {
        const char *chips[] = {"All", "Operating Systems", "Computer Science", "Programming", "C++", "Kernels", "DLSU"};
        ImGui::SetCursorPosX(14.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 3.0f));

        for (int i = 0; i < 7; ++i)
        {
            ImGui::PushID(i);
            if (i == 0)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(255, 255, 255, 255));
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(15, 15, 15, 255));
                ImGui::Button(chips[i]);
                ImGui::PopStyleColor(2);
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_Button, kYouTubeChipBg);
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(241, 241, 241, 255));
                ImGui::Button(chips[i]);
                ImGui::PopStyleColor(2);
            }
            ImGui::SameLine(0.0f, 8.0f);
            ImGui::PopID();
        }
        ImGui::NewLine();
        ImGui::PopStyleVar(2);
    }

    // Main content: Sidebar + Video Grid.
    {
        // Mini Sidebar.
        const float sidebar_w = 88.0f;
        ImGui::BeginChild("##yt_sidebar", ImVec2(sidebar_w, 0), false, ImGuiWindowFlags_NoScrollbar);
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(241, 241, 241, 255));
        ImGui::SetCursorPosY(8.0f);
        ImGui::TextUnformatted(" Home");
        ImGui::Spacing();
        ImGui::TextUnformatted(" Shorts");
        ImGui::Spacing();
        ImGui::TextUnformatted(" Subs");
        ImGui::Separator();
        ImGui::TextUnformatted(" Library");
        ImGui::Spacing();
        ImGui::TextUnformatted(" History");
        ImGui::PopStyleColor();
        ImGui::EndChild();

        ImGui::SameLine();

        // Video Grid.
        ImGui::BeginChild("##yt_grid", ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollbar);

        struct Video
        {
            const char *title;
            const char *channel;
            const char *meta;
            const char *duration;
            ImU32 theme_col;
        };

        const Video videos[] = {
            {"CSOPESY Lecture 1: OS Principles", "DLSU CCS", "14K views - 2 weeks ago", "48:12", IM_COL32(20, 60, 100, 255)},
            {"CPU Scheduling: FCFS, SJF, RR", "TechQuickie", "180K views - 1 year ago", "14:25", IM_COL32(70, 30, 90, 255)},
            {"Processes, Threads, and Fibers", "Computerphile", "420K views - 2 years ago", "18:40", IM_COL32(20, 80, 60, 255)},
            {"Building an OS Kernel in C++", "Low Level Academy", "95K views - 3 months ago", "1:15:30", IM_COL32(110, 40, 20, 255)},
            {"Deadlocks & Banker's Algorithm", "OS Tutorials", "62K views - 8 months ago", "22:04", IM_COL32(80, 60, 20, 255)},
            {"Virtual Memory, TLB & Paging", "Code Vault", "290K views - 1 year ago", "28:55", IM_COL32(40, 40, 90, 255)},
        };

        const float grid_avail_w = ImGui::GetContentRegionAvail().x;
        const int cols = (grid_avail_w > 540.0f) ? 3 : (grid_avail_w > 320.0f ? 2 : 1);
        const float card_w = (grid_avail_w - static_cast<float>(cols - 1) * 14.0f) / static_cast<float>(cols);
        const float thumb_h = card_w * 0.5625f; // 16:9 aspect ratio.

        for (int i = 0; i < 6; ++i)
        {
            ImGui::PushID(i);
            ImGui::BeginGroup();

            // Video Thumbnail.
            const ImVec2 p_min = ImGui::GetCursorScreenPos();
            const ImVec2 p_max = ImVec2(p_min.x + card_w, p_min.y + thumb_h);
            ImDrawList *dl = ImGui::GetWindowDrawList();

            dl->AddRectFilled(p_min, p_max, videos[i].theme_col, 8.0f);
            dl->AddText(ImVec2(p_min.x + 10.0f, p_min.y + 10.0f), IM_COL32(255, 255, 255, 200), "[>] VIDEO");

            // Duration badge at bottom right.
            const ImVec2 dur_sz = ImGui::CalcTextSize(videos[i].duration);
            const ImVec2 dur_min = ImVec2(p_max.x - dur_sz.x - 8.0f, p_max.y - dur_sz.y - 6.0f);
            const ImVec2 dur_max = ImVec2(p_max.x - 4.0f, p_max.y - 2.0f);
            dl->AddRectFilled(dur_min, dur_max, IM_COL32(0, 0, 0, 220), 4.0f);
            dl->AddText(ImVec2(dur_min.x + 2.0f, dur_min.y + 1.0f), IM_COL32(255, 255, 255, 255), videos[i].duration);

            ImGui::Dummy(ImVec2(card_w, thumb_h));
            ImGui::Spacing();

            // Channel avatar icon and title.
            const ImVec2 ch_pos = ImGui::GetCursorScreenPos();
            dl->AddCircleFilled(ImVec2(ch_pos.x + 10.0f, ch_pos.y + 10.0f), 10.0f, IM_COL32(80, 80, 80, 255));
            dl->AddText(ImVec2(ch_pos.x + 7.0f, ch_pos.y + 2.0f), IM_COL32(255, 255, 255, 255), "C");

            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 26.0f);
            ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + card_w - 26.0f);
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
            ImGui::TextUnformatted(videos[i].title);
            ImGui::PopStyleColor();
            ImGui::PopTextWrapPos();

            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 26.0f);
            ImGui::TextDisabled("%s", videos[i].channel);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 26.0f);
            ImGui::TextDisabled("%s", videos[i].meta);

            ImGui::EndGroup();

            if ((i + 1) % cols != 0)
                ImGui::SameLine(0.0f, 14.0f);
            else
                ImGui::Spacing();

            ImGui::PopID();
        }

        ImGui::EndChild();
    }

    ImGui::EndChild();
    ImGui::PopStyleColor(2);
}

void BrowserApp::RenderWikipediaPage()
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, kWikiBg);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(32, 33, 34, 255));
    ImGui::BeginChild("##wiki_scroll", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);

    const float page_w = ImGui::GetWindowWidth();

    // Top action row (Article, Talk, Read, View source, View history).
    {
        ImGui::SetCursorPos(ImVec2(12.0f, 8.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, kWikiLinkBlue);
        ImGui::TextUnformatted("Article");
        ImGui::SameLine(0.0f, 12.0f);
        ImGui::TextUnformatted("Talk");
        if (page_w > 400.0f)
        {
            ImGui::SameLine(page_w - 230.0f);
            ImGui::TextUnformatted("Read");
            ImGui::SameLine(0.0f, 12.0f);
            ImGui::TextUnformatted("View source");
            ImGui::SameLine(0.0f, 12.0f);
            ImGui::TextUnformatted("View history");
        }
        ImGui::PopStyleColor();
    }

    ImGui::Separator();
    ImGui::Spacing();

    // Article Title: "Operating system".
    {
        ImGui::SetCursorPosX(16.0f);
        const float title_sz = 26.0f;
        ImFont *font = ImGui::GetFont();
        ImDrawList *dl = ImGui::GetWindowDrawList();
        const ImVec2 cur_pos = ImGui::GetCursorScreenPos();
        dl->AddText(font, title_sz, cur_pos, IM_COL32(0, 0, 0, 255), "Operating system");
        ImGui::Dummy(ImVec2(300.0f, 32.0f));

        ImGui::SetCursorPosX(16.0f);
        ImGui::TextDisabled("From Wikipedia, the free encyclopedia");
        ImGui::SetCursorPosX(16.0f);
        ImGui::Separator();
    }

    // Hatnote.
    {
        ImGui::SetCursorPosX(16.0f);
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(84, 89, 95, 255));
        ImGui::TextWrapped("This article is about the software that manages computer hardware. For other uses, see Operating system (disambiguation).");
        ImGui::PopStyleColor();
        ImGui::Spacing();
    }

    // Two-column layout: Main Article (Left) + Infobox (Right).
    const float infobox_w = 250.0f;
    const bool side_by_side = page_w > 580.0f;
    const float main_w = side_by_side ? (page_w - infobox_w - 48.0f) : (page_w - 32.0f);

    ImGui::SetCursorPosX(16.0f);
    ImGui::BeginGroup();

    // Main Column.
    ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + main_w);

    // Lead paragraphs.
    ImGui::TextUnformatted("An operating system (OS) is system software that manages computer hardware, software resources, and provides common services for computer programs.");
    ImGui::Spacing();
    ImGui::TextUnformatted("Time-sharing operating systems schedule tasks for efficient use of the system and may also include accounting software for cost allocation of processor time, mass storage, printing, and other resources.");
    ImGui::Spacing();
    ImGui::TextUnformatted("For hardware functions such as input and output and memory allocation, the operating system acts as an intermediary between programs and the computer hardware, although the application code is usually executed directly by the hardware and frequently makes system calls to an OS function or is interrupted by it.");
    ImGui::Spacing();

    // Table of contents box.
    {
        ImGui::PushStyleColor(ImGuiCol_ChildBg, kWikiTocBg);
        ImGui::PushStyleColor(ImGuiCol_Border, kWikiBorder);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 3.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 8.0f));

        ImGui::BeginChild("##wiki_toc", ImVec2(main_w, 140.0f), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::TextDisabled("Contents [hide]");
        ImGui::Separator();
        ImGui::PushStyleColor(ImGuiCol_Text, kWikiLinkBlue);
        ImGui::TextUnformatted("1 Types of operating systems");
        ImGui::TextUnformatted("    1.1 Single-tasking and multi-tasking");
        ImGui::TextUnformatted("    1.2 Real-time (RTOS)");
        ImGui::TextUnformatted("2 Components");
        ImGui::TextUnformatted("    2.1 Kernel");
        ImGui::TextUnformatted("    2.2 Memory management");
        ImGui::TextUnformatted("    2.3 Process management");
        ImGui::PopStyleColor();
        ImGui::EndChild();

        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor(2);
        ImGui::Spacing();
    }

    // Section 1: Types of operating systems.
    {
        ImFont *font = ImGui::GetFont();
        ImDrawList *dl = ImGui::GetWindowDrawList();
        const ImVec2 cur_pos = ImGui::GetCursorScreenPos();
        dl->AddText(font, 20.0f, cur_pos, IM_COL32(0, 0, 0, 255), "1. Types of operating systems");
        ImGui::Dummy(ImVec2(main_w, 26.0f));
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::TextUnformatted("Single-tasking and multi-tasking:");
        ImGui::TextUnformatted("A single-tasking system can only run one program at a time, while a multi-tasking operating system allows more than one program to be running in concurrency. This is achieved by time-sharing, where the available processor time is divided between multiple processes.");
        ImGui::Spacing();
        ImGui::TextUnformatted("Real-time:");
        ImGui::TextUnformatted("A real-time operating system (RTOS) is an operating system intended for applications with fixed deadlines. Such systems have specialized scheduling algorithms to ensure deterministic behavior.");
        ImGui::Spacing();
    }

    // Section 2: Components.
    {
        ImFont *font = ImGui::GetFont();
        ImDrawList *dl = ImGui::GetWindowDrawList();
        const ImVec2 cur_pos = ImGui::GetCursorScreenPos();
        dl->AddText(font, 20.0f, cur_pos, IM_COL32(0, 0, 0, 255), "2. Components");
        ImGui::Dummy(ImVec2(main_w, 26.0f));
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::TextUnformatted("Kernel:");
        ImGui::TextUnformatted("With the aid of firmware and device drivers, the kernel provides the most basic level of control over all the computer's hardware devices. It manages memory access for programs in RAM, determines which programs get access to hardware resources, and configures CPU states.");
        ImGui::Spacing();
        ImGui::TextUnformatted("Process management:");
        ImGui::TextUnformatted("An operating system provides the environment for programs to run. A process is a program in execution. The scheduler determines which processes receive processor time slices according to algorithms such as Round Robin or Priority Scheduling.");
        ImGui::Spacing();
        ImGui::TextUnformatted("Memory management:");
        ImGui::TextUnformatted("An operating system supervisor manages primary memory, moving processes between RAM and secondary storage using virtual memory and paging.");
        ImGui::Spacing();
    }

    ImGui::PopTextWrapPos();
    ImGui::EndGroup();

    // Right Column: Wikipedia Infobox.
    if (side_by_side)
        ImGui::SameLine(0.0f, 16.0f);
    else
        ImGui::Spacing();

    {
        ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(248, 249, 250, 255));
        ImGui::PushStyleColor(ImGuiCol_Border, kWikiBorder);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 2.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));

        ImGui::BeginChild("##wiki_infobox", ImVec2(infobox_w, 390.0f), true, ImGuiWindowFlags_NoScrollbar);

        // Infobox Header.
        ImDrawList *dl = ImGui::GetWindowDrawList();
        const ImVec2 hdr_min = ImGui::GetWindowPos();
        const ImVec2 hdr_max = ImVec2(hdr_min.x + infobox_w, hdr_min.y + 44.0f);
        dl->AddRectFilled(hdr_min, hdr_max, kWikiHeaderBg);

        ImGui::SetCursorPos(ImVec2(10.0f, 6.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
        ImGui::TextUnformatted("Operating system");
        ImGui::PopStyleColor();
        ImGui::SetCursorPos(ImVec2(10.0f, 24.0f));
        ImGui::TextDisabled("Part of a series on Software");

        ImGui::SetCursorPosY(52.0f);
        ImGui::Separator();

        // Key-Value rows.
        ImGui::Columns(2, "##infobox_cols", false);
        ImGui::SetColumnWidth(0, 85.0f);

        ImGui::TextDisabled("Type");
        ImGui::NextColumn();
        ImGui::TextUnformatted("System software");
        ImGui::NextColumn();

        ImGui::TextDisabled("Core");
        ImGui::NextColumn();
        ImGui::TextUnformatted("Kernel");
        ImGui::NextColumn();

        ImGui::TextDisabled("Common OS");
        ImGui::NextColumn();
        ImGui::PushStyleColor(ImGuiCol_Text, kWikiLinkBlue);
        ImGui::TextUnformatted("Linux\nWindows\nmacOS\nAndroid");
        ImGui::PopStyleColor();
        ImGui::NextColumn();

        ImGui::TextDisabled("Structures");
        ImGui::NextColumn();
        ImGui::TextUnformatted("Monolithic\nMicrokernel\nHybrid");
        ImGui::NextColumn();

        ImGui::TextDisabled("Standard API");
        ImGui::NextColumn();
        ImGui::TextUnformatted("POSIX, Win32");
        ImGui::NextColumn();

        ImGui::TextDisabled("First release");
        ImGui::NextColumn();
        ImGui::TextUnformatted("1956 (GM-NAA I/O)");
        ImGui::NextColumn();

        ImGui::Columns(1);
        ImGui::EndChild();

        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor(2);
    }

    ImGui::EndChild();
    ImGui::PopStyleColor(2);
}

void BrowserApp::Render()
{
    ImGui::SetNextWindowSize(ImVec2(880.0f, 580.0f), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Web Browser", &AppState::show_browser))
    {
        ImGui::End();
        return;
    }

    const int activate_tab = requested_tab_;
    requested_tab_ = -1;

    if (ImGui::BeginTabBar("##BrowserTabs", ImGuiTabBarFlags_None))
    {
        const ImGuiTabItemFlags google_flags = (activate_tab == static_cast<int>(Tab::Google))
                                                   ? ImGuiTabItemFlags_SetSelected
                                                   : ImGuiTabItemFlags_None;
        if (ImGui::BeginTabItem("Google", nullptr, google_flags))
        {
            active_tab_ = Tab::Google;
            RenderAddressBar("https://www.google.com");
            RenderBookmarksBar();
            ImGui::Separator();
            RenderGooglePage();
            ImGui::EndTabItem();
        }

        const ImGuiTabItemFlags yt_flags = (activate_tab == static_cast<int>(Tab::YouTube))
                                               ? ImGuiTabItemFlags_SetSelected
                                               : ImGuiTabItemFlags_None;
        if (ImGui::BeginTabItem("YouTube", nullptr, yt_flags))
        {
            active_tab_ = Tab::YouTube;
            RenderAddressBar("https://www.youtube.com");
            RenderBookmarksBar();
            ImGui::Separator();
            RenderYouTubePage();
            ImGui::EndTabItem();
        }

        const ImGuiTabItemFlags wiki_flags = (activate_tab == static_cast<int>(Tab::Wikipedia))
                                                 ? ImGuiTabItemFlags_SetSelected
                                                 : ImGuiTabItemFlags_None;
        if (ImGui::BeginTabItem("Operating system - Wikipedia", nullptr, wiki_flags))
        {
            active_tab_ = Tab::Wikipedia;
            RenderAddressBar("https://en.wikipedia.org/wiki/Operating_system");
            RenderBookmarksBar();
            ImGui::Separator();
            RenderWikipediaPage();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}
