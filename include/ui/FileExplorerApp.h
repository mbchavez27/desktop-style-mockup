#pragma once

#include <string>
#include <vector>

/**
 * @brief Simple folder-like File Explorer mock-up.
 *
 * Root shows a fixed set of premade folders. Folders support
 * open / rename / delete plus creating new folders (no nesting).
 * Opening a folder shows a premade template of two files —
 * "<name>.png" and "<name>.txt" — which have no viewer and never open.
 *
 * Instantiated as a member of MockApps — no standalone global.
 */
class FileExplorerApp
{
public:
    /**
     * @brief Renders the File Explorer window for this frame.
     *
     * Caller (MockApps) gates on AppState::show_file_explorer; the flag
     * is passed to ImGui::Begin so the native 'X' toggles it.
     */
    void Render();

private:
    /**
     * @brief Draws the address bar (Back button + path text).
     */
    void RenderAddressBar();

    /**
     * @brief Draws the root folder grid with selection + context menu.
     */
    void RenderRootView();

    /**
     * @brief Draws the two template files inside the open folder.
     */
    void RenderOpenFolderView();

    /**
     * @brief Draws the action toolbar (New Folder / Open / Rename / Delete).
     */
    void RenderActions();

    /**
     * @brief Opens the new-folder modal with a suggested unique name.
     */
    void BeginCreate();

    /**
     * @brief Draws and handles the new-folder modal popup.
     */
    void RenderCreatePopup();

    /**
     * @brief Opens the rename modal for a folder index.
     *
     * @param idx Index into folders_.
     */
    void BeginRename(int idx);

    /**
     * @brief Draws and handles the rename modal popup.
     */
    void RenderRenamePopup();

    /**
     * @brief Draws and handles the delete-confirm modal popup.
     */
    void RenderDeletePopup();

    /**
     * @brief Opens a folder by index (enters its file view).
     *
     * @param idx Index into folders_.
     */
    void OpenFolder(int idx);

    /**
     * @brief Creates a root folder, selects it, and reports status.
     *
     * Caller must validate with IsValidFolderName first.
     *
     * @param name Validated folder name.
     */
    void CreateFolder(const std::string &name);

    /**
     * @brief Suggests a unique default name ("New Folder", "New Folder (2)", ...).
     *
     * @return Unused folder name for the create modal.
     */
    std::string SuggestFolderName() const;

    /**
     * @brief Deletes a folder by index and resets selection/navigation.
     *
     * @param idx Index into folders_.
     */
    void DeleteFolder(int idx);

    /**
     * @brief Checks a candidate folder name for emptiness/duplicates.
     *
     * @param name Candidate name.
     * @param ignore_idx Index to skip when checking duplicates (-1 = none).
     * @return True when the name is usable.
     */
    bool IsValidFolderName(const std::string &name, int ignore_idx) const;

    std::vector<std::string> folders_ = {"Documents", "Pictures", "Music", "Projects"}; ///< Premade root folders.
    int selected_ = -1;                                                                 ///< Selected root index, -1 = none.
    std::string current_;                                                               ///< Open folder name, empty = root.
    char rename_buf_[64] = {};                                                          ///< Rename modal text buffer.
    char create_buf_[64] = {};                                                          ///< New-folder modal text buffer.
    int rename_target_ = -1;                                                            ///< Folder index being renamed.
    int delete_target_ = -1;                                                            ///< Folder index pending delete.
    std::string status_;                                                                ///< Bottom status line message.
};
