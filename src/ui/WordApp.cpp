#include "imgui.h"
#include "core/AppState.h"
#include "ui/WordApp.h"

#include <cctype>
#include <cfloat>
#include <cstring>
#include <string>
#include <vector>

namespace
{

constexpr size_t kBufSize = 16 * 1024;
char g_buf[kBufSize] =
    "# Heading 1\n"
    "Type **markdown** on the left; the preview updates live.\n"
    "It supports *italic* and `code` too.\n"
    "\n"
    "## Heading 2\n"
    "- bullet one\n"
    "- bullet two\n"
    "\n"
    "> quoted line\n";

enum class Emphasis
{
    Plain,
    Bold,
    Italic,
    Code,
};

struct Run
{
    std::string text;
    Emphasis em;
};

ImVec4 EmColor(Emphasis em)
{
    switch (em)
    {
    case Emphasis::Bold:
        return ImVec4(0.55f, 0.78f, 1.00f, 1.0f); // accent-ish; default atlas has no bold face
    case Emphasis::Italic:
        return ImVec4(0.80f, 0.85f, 0.55f, 1.0f);
    case Emphasis::Code:
        return ImVec4(1.00f, 0.70f, 0.40f, 1.0f);
    default:
        return ImGui::GetStyleColorVec4(ImGuiCol_Text);
    }
}

std::vector<Run> TokenizeInline(const char *s, size_t len)
{
    std::vector<Run> runs;
    std::string plain;
    auto flush = [&runs, &plain]
    {
        if (!plain.empty())
        {
            runs.push_back({plain, Emphasis::Plain});
            plain.clear();
        }
    };
    auto find_char = [s, len](char a, size_t from) -> size_t
    {
        for (size_t j = from; j < len; ++j)
            if (s[j] == a)
                return j;
        return len;
    };
    auto find_pair = [s, len](char a, char b, size_t from) -> size_t
    {
        for (size_t j = from; j + 1 < len; ++j)
            if (s[j] == a && s[j + 1] == b)
                return j;
        return len;
    };

    size_t i = 0;
    while (i < len)
    {
        if (i + 1 < len && s[i] == '*' && s[i + 1] == '*')
        {
            const size_t close = find_pair('*', '*', i + 2);
            if (close < len)
            {
                flush();
                runs.push_back({std::string(s + i + 2, close - (i + 2)), Emphasis::Bold});
                i = close + 2;
                continue;
            }
        }
        if (s[i] == '*' && i + 1 < len && s[i + 1] != ' ') // '*'+' ' is a bullet, handled upstream
        {
            const size_t close = find_char('*', i + 1);
            if (close < len)
            {
                flush();
                runs.push_back({std::string(s + i + 1, close - (i + 1)), Emphasis::Italic});
                i = close + 1;
                continue;
            }
        }
        if (s[i] == '`')
        {
            const size_t close = find_char('`', i + 1);
            if (close < len)
            {
                flush();
                runs.push_back({std::string(s + i + 1, close - (i + 1)), Emphasis::Code});
                i = close + 1;
                continue;
            }
        }
        plain += s[i++];
    }
    flush();
    return runs;
}

void RenderInline(const std::vector<Run> &runs)
{
    bool first = true;
    for (const Run &r : runs)
    {
        if (!first)
            ImGui::SameLine(0.0f, 0.0f); // join segments without a gap
        ImGui::PushStyleColor(ImGuiCol_Text, EmColor(r.em));
        ImGui::TextUnformatted(r.text.c_str(), r.text.c_str() + r.text.size());
        ImGui::PopStyleColor();
        first = false;
    }
}

void RenderHeading(const char *s, size_t len, float scale)
{
    ImGui::SetWindowFontScale(scale); // applies to the current child window
    RenderInline(TokenizeInline(s, len));
    ImGui::SetWindowFontScale(1.0f);
}

bool StartsWith(const char *s, size_t len, const char *prefix)
{
    const size_t n = std::strlen(prefix);
    return len >= n && std::strncmp(s, prefix, n) == 0;
}

void RenderLine(const char *s, size_t len)
{
    if (len == 0) // blank line = paragraph break
    {
        ImGui::Dummy(ImVec2(0.0f, ImGui::GetStyle().ItemSpacing.y));
        return;
    }

    // Longest prefixes first so "### " matches before "## ".
    if (StartsWith(s, len, "### "))
        return RenderHeading(s + 4, len - 4, 1.25f);
    if (StartsWith(s, len, "## "))
        return RenderHeading(s + 3, len - 3, 1.40f);
    if (StartsWith(s, len, "# "))
        return RenderHeading(s + 2, len - 2, 1.60f);
    if (StartsWith(s, len, "- ") || StartsWith(s, len, "* "))
    {
        ImGui::Bullet();
        ImGui::SameLine(0.0f, 4.0f);
        RenderInline(TokenizeInline(s + 2, len - 2));
        return;
    }
    if (StartsWith(s, len, "> "))
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.60f, 0.65f, 0.75f, 1.0f));
        RenderInline(TokenizeInline(s + 2, len - 2));
        ImGui::PopStyleColor();
        return;
    }
    RenderInline(TokenizeInline(s, len));
}

void RenderPreview(const char *buf)
{
    const char *p = buf;
    while (*p != '\0')
    {
        const char *nl = std::strchr(p, '\n');
        const size_t len = nl ? static_cast<size_t>(nl - p) : std::strlen(p);
        RenderLine(p, len);
        if (!nl)
            break;
        p = nl + 1;
    }
}

int CountWords(const char *s)
{
    int count = 0;
    bool in_word = false;
    for (; *s != '\0'; ++s)
    {
        if (std::isspace(static_cast<unsigned char>(*s)))
            in_word = false;
        else if (!in_word)
        {
            in_word = true;
            ++count;
        }
    }
    return count;
}

} // namespace

void RenderWordApp()
{
    if (!AppState::show_app_1)
        return;

    ImGui::SetNextWindowSize(ImVec2(760, 480), ImGuiCond_FirstUseEver);
    ImGui::Begin("Word", &AppState::show_app_1, ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar())
    {
        ImGui::MenuItem("File");
        ImGui::MenuItem("Edit");
        ImGui::MenuItem("View");
        ImGui::MenuItem("Format");
        ImGui::EndMenuBar();
    }

    const ImVec2 avail = ImGui::GetContentRegionAvail();
    const float gap = ImGui::GetStyle().ItemSpacing.x;
    const float pane_w = (avail.x - gap) * 0.5f;
    const ImVec2 pane_sz(pane_w, avail.y - 24.0f); // reserve status-bar row

    ImGui::BeginChild("##source", pane_sz, ImGuiChildFlags_Borders);
    ImGui::InputTextMultiline("##markdown", g_buf, sizeof(g_buf),
                              ImVec2(-FLT_MIN, -FLT_MIN),
                              ImGuiInputTextFlags_AllowTabInput);
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("##preview", ImVec2(0.0f, pane_sz.y), ImGuiChildFlags_Borders);
    RenderPreview(g_buf);
    ImGui::EndChild();

    ImGui::Separator();
    ImGui::Text("%d words   %d characters", CountWords(g_buf),
                static_cast<int>(std::strlen(g_buf)));

    ImGui::End();
}
