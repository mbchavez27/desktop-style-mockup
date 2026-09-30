#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "imgui.h" // ImVec4

/**
 * @brief MS Word-style markdown editor with a live preview pane.
 *
 * The left pane edits a fixed 16 KB char buffer; the right pane re-parses
 * it every frame and renders headings, bullets, quotes, and inline
 * emphasis. Instantiated as a member of MockApps — no standalone global.
 */
class WordApp
{
public:
    /**
     * @brief Renders the Word window (source + preview + status bar) for this frame.
     *
     * Caller (MockApps) gates on AppState::show_app_1; the flag is passed
     * to ImGui::Begin so the native 'X' toggles it.
     */
    void Render();

private:
    /// Inline emphasis category produced by the tokenizer.
    enum class Emphasis
    {
        Plain,
        Bold,
        Italic,
        Code,
    };

    /// A contiguous run of text sharing one emphasis level.
    struct Run
    {
        std::string text;
        Emphasis em;
    };

    /**
     * @brief Maps an emphasis level to its preview text color.
     *
     * @param em Emphasis of the run.
     * @return Normalized RGBA; Plain resolves to the active text color.
     */
    static ImVec4 EmColor(Emphasis em);

    /**
     * @brief Splits one line into emphasized runs.
     *
     * Non-nested token scan bounded by len: **bold** beats *italic* beats
     * `code`; a leading "* " is a bullet and never tokenized as italic.
     *
     * @param s   Start of the line.
     * @param len Length in bytes; never scans past it.
     * @return Runs covering [s, s+len); empty when len == 0.
     */
    static auto TokenizeInline(const char *s, size_t len) -> std::vector<Run>;

    /**
     * @brief Draws runs joined with SameLine(0,0), colored by emphasis.
     *
     * @param runs Output of TokenizeInline.
     */
    static void RenderInline(const std::vector<Run> &runs);

    /**
     * @brief Draws a heading line at the given font scale, then restores 1.0.
     *
     * @param s     Heading text after the '# ' markers.
     * @param len   Text length in bytes.
     * @param scale Window font scale (e.g. 1.6 for H1).
     */
    static void RenderHeading(const char *s, size_t len, float scale);

    /**
     * @brief Reports whether a bounded line starts with a prefix.
     *
     * @param s      Start of the line.
     * @param len    Line length in bytes.
     * @param prefix NUL-terminated prefix to match.
     * @return true when the prefix fits entirely inside [s, s+len).
     */
    static bool StartsWith(const char *s, size_t len, const char *prefix);

    /**
     * @brief Classifies a source line and renders it accordingly.
     *
     * Dispatch order: ###, ##, # headings, then "- "/"* " bullets,
     * "> " quotes, and finally a plain paragraph with inline tokens.
     *
     * @param s   Start of the line.
     * @param len Line length in bytes.
     */
    static void RenderLine(const char *s, size_t len);

    /**
     * @brief Walks the buffer line by line into RenderLine.
     *
     * @param buf NUL-terminated markdown source.
     */
    static void RenderPreview(const char *buf);

    /**
     * @brief Counts whitespace-separated words.
     *
     * @param s NUL-terminated source.
     * @return Word count.
     */
    static int CountWords(const char *s);

    /// Raw markdown source; fixed buffer keeps InputText allocation-free.
    char buffer[16 * 1024] =
        "# Heading 1\n"
        "Type **markdown** on the left; the preview updates live.\n"
        "It supports *italic* and `code` too.\n"
        "\n"
        "## Heading 2\n"
        "- bullet one\n"
        "- bullet two\n"
        "\n"
        "> quoted line\n";
};
