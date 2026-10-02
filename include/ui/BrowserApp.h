#pragma once

/**
 * @brief Static web browser mock-up with preloaded tabs.
 *
 * Provides a tabbed browser window with navigation controls (Back, Forward,
 * Reload, Home, Address Bar) and preloaded static mock web pages for Google,
 * YouTube, and Wikipedia. Contains no search functionality.
 *
 * Instantiated as a member of MockApps — no standalone global.
 */
class BrowserApp
{
public:
    /**
     * @brief Renders the Web Browser window for this frame.
     *
     * Caller (MockApps) gates on AppState::show_browser; the flag
     * is passed to ImGui::Begin so the native 'X' toggles it.
     */
    void Render();

private:
    /**
     * @brief Identifier for each preloaded browser tab.
     */
    enum class Tab
    {
        Google = 0,
        YouTube,
        Wikipedia,
        Count
    };

    /**
     * @brief Draws the top navigation toolbar (history, reload, address bar).
     *
     * @param url Current page URL string to display in the address bar.
     */
    void RenderAddressBar(const char *url);

    /**
     * @brief Draws the quick-access bookmarks bar below the address bar.
     */
    void RenderBookmarksBar();

    /**
     * @brief Draws the static Google search homepage mock.
     */
    void RenderGooglePage();

    /**
     * @brief Draws the static YouTube homepage and video grid mock.
     */
    void RenderYouTubePage();

    /**
     * @brief Draws the static Wikipedia "Operating system" article mock.
     */
    void RenderWikipediaPage();

    Tab active_tab_ = Tab::Google; ///< Currently active browser tab.
    int requested_tab_ = -1;       ///< Programmatic tab selection request (-1 = none).
};
