#include "imgui.h"
#include "core/AppState.h"
#include "ui/FileExplorerApp.h"
#include "ui/IconCache.h"

#include <cstdio>
#include <cstring>
#include <string>

namespace
{

constexpr float kFolderIconPx = 48.0f;
constexpr float kCellW = 110.0f;

} // namespace

void FileExplorerApp::OpenFolder(int idx)
{
    if (idx < 0 || idx >= static_cast<int>(folders_.size()))
        return;
    current_ = folders_[static_cast<size_t>(idx)];
    selected_ = -1;
    status_ = "Opened " + current_;
}

void FileExplorerApp::DeleteFolder(int idx)
{
    if (idx < 0 || idx >= static_cast<int>(folders_.size()))
        return;
    const std::string removed = folders_[static_cast<size_t>(idx)];
    folders_.erase(folders_.begin() + idx);
    if (current_ == removed)
        current_.clear();
    selected_ = -1;
    delete_target_ = -1;
    status_ = "Deleted " + removed;
}

bool FileExplorerApp::IsValidFolderName(const std::string &name, int ignore_idx) const
{
    if (name.empty() || name.size() >= sizeof(rename_buf_))
        return false;
    for (char c : name)
    {
        if (c == '/' || c == '\\')
            return false;
    }
    for (int i = 0; i < static_cast<int>(folders_.size()); ++i)
    {
        if (i == ignore_idx)
            continue;
        if (folders_[static_cast<size_t>(i)] == name)
            return false;
    }
    return true;
}

void FileExplorerApp::BeginRename(int idx)
{
    if (idx < 0 || idx >= static_cast<int>(folders_.size()))
        return;
    rename_target_ = idx;
    std::snprintf(rename_buf_, sizeof(rename_buf_), "%s", folders_[static_cast<size_t>(idx)].c_str());
    ImGui::OpenPopup("Rename Folder");
}

std::string FileExplorerApp::SuggestFolderName() const
{
    if (IsValidFolderName("New Folder", -1))
        return "New Folder";
    for (int n = 2; n < 100; ++n)
    {
        const std::string candidate = "New Folder (" + std::to_string(n) + ")";
        if (IsValidFolderName(candidate, -1))
            return candidate;
    }
    return "New Folder";
}

void FileExplorerApp::BeginCreate()
{
    std::snprintf(create_buf_, sizeof(create_buf_), "%s", SuggestFolderName().c_str());
    ImGui::OpenPopup("New Folder");
}

void FileExplorerApp::CreateFolder(const std::string &name)
{
    folders_.push_back(name);
    selected_ = static_cast<int>(folders_.size()) - 1;
    status_ = "Created " + name;
}

void FileExplorerApp::RenderCreatePopup()
{
    if (ImGui::BeginPopupModal("New Folder", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::TextUnformatted("Folder name:");
        ImGui::SetNextItemWidth(240.0f);
        if (ImGui::IsWindowAppearing())
            ImGui::SetKeyboardFocusHere();
        ImGui::InputText("##create", create_buf_, sizeof(create_buf_));

        const std::string candidate = create_buf_;
        const bool valid = IsValidFolderName(candidate, -1);

        if (!valid)
            ImGui::TextDisabled("Name must be unique, non-empty, no slashes.");
        else
            ImGui::TextDisabled(" ");

        if (!valid)
            ImGui::BeginDisabled();
        if (ImGui::Button("Create", ImVec2(80.0f, 0.0f)) && valid)
        {
            CreateFolder(candidate);
            ImGui::CloseCurrentPopup();
        }
        if (!valid)
            ImGui::EndDisabled();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(80.0f, 0.0f)))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}

void FileExplorerApp::RenderRenamePopup()
{
    if (ImGui::BeginPopupModal("Rename Folder", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::TextUnformatted("New name:");
        ImGui::SetNextItemWidth(240.0f);
        if (ImGui::IsWindowAppearing())
            ImGui::SetKeyboardFocusHere();
        ImGui::InputText("##rename", rename_buf_, sizeof(rename_buf_));

        const std::string candidate = rename_buf_;
        const bool valid = IsValidFolderName(candidate, rename_target_);

        if (!valid)
            ImGui::TextDisabled("Name must be unique, non-empty, no slashes.");
        else
            ImGui::TextDisabled(" ");

        if (!valid)
            ImGui::BeginDisabled();
        if (ImGui::Button("OK", ImVec2(80.0f, 0.0f)) && valid)
        {
            const std::string old = folders_[static_cast<size_t>(rename_target_)];
            folders_[static_cast<size_t>(rename_target_)] = candidate;
            if (current_ == old)
                current_ = candidate;
            status_ = "Renamed " + old + " to " + candidate;
            rename_target_ = -1;
            ImGui::CloseCurrentPopup();
        }
        if (!valid)
            ImGui::EndDisabled();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(80.0f, 0.0f)))
        {
            rename_target_ = -1;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void FileExplorerApp::RenderDeletePopup()
{
    if (ImGui::BeginPopupModal("Delete Folder", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (delete_target_ >= 0 && delete_target_ < static_cast<int>(folders_.size()))
            ImGui::Text("Delete '%s'?", folders_[static_cast<size_t>(delete_target_)].c_str());
        else
            ImGui::TextUnformatted("Delete this folder?");

        if (ImGui::Button("Delete", ImVec2(80.0f, 0.0f)))
        {
            DeleteFolder(delete_target_);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(80.0f, 0.0f)))
        {
            delete_target_ = -1;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void FileExplorerApp::RenderAddressBar()
{
    const bool at_root = current_.empty();
    if (at_root)
        ImGui::BeginDisabled();
    if (ImGui::Button("< Back"))
    {
        current_.clear();
        selected_ = -1;
        status_.clear();
    }
    if (at_root)
        ImGui::EndDisabled();

    ImGui::SameLine();
    if (at_root)
        ImGui::TextUnformatted("This PC");
    else
        ImGui::Text("This PC > %s", current_.c_str());
}

void FileExplorerApp::RenderActions()
{
    // New folders are created at root; Open/Rename/Delete need a selection.
    const bool has_selection = selected_ >= 0 && selected_ < static_cast<int>(folders_.size());
    const bool at_root = current_.empty();

    if (!at_root)
        ImGui::BeginDisabled();
    if (ImGui::Button("New Folder") && at_root)
        BeginCreate();
    if (!at_root)
        ImGui::EndDisabled();

    ImGui::SameLine();
    const bool item_actions = at_root && has_selection;
    if (!item_actions)
        ImGui::BeginDisabled();

    if (ImGui::Button("Open") && item_actions)
        OpenFolder(selected_);
    ImGui::SameLine();
    if (ImGui::Button("Rename") && item_actions)
        BeginRename(selected_);
    ImGui::SameLine();
    if (ImGui::Button("Delete") && item_actions)
    {
        delete_target_ = selected_;
        ImGui::OpenPopup("Delete Folder");
    }

    if (!item_actions)
        ImGui::EndDisabled();
}

void FileExplorerApp::RenderRootView()
{
    if (folders_.empty())
    {
        ImGui::TextDisabled("No folders. Everything was deleted.");
        if (ImGui::BeginPopupContextWindow("##root_ctx_empty", ImGuiPopupFlags_MouseButtonRight))
        {
            if (ImGui::MenuItem("New Folder"))
                BeginCreate();
            ImGui::EndPopup();
        }
        return;
    }

    const AppIcon &icon = GetIcon("assets/images/file_explorer.png");
    const float avail = ImGui::GetContentRegionAvail().x;
    int columns = static_cast<int>(avail / (kCellW + 8.0f));
    if (columns < 1)
        columns = 1;

    for (int i = 0; i < static_cast<int>(folders_.size()); ++i)
    {
        ImGui::PushID(i);
        const bool is_selected = (selected_ == i);

        ImGui::BeginGroup();
        if (icon.ok())
            ImGui::Image(icon.tex, ImVec2(kFolderIconPx, kFolderIconPx));
        else
            ImGui::Button("##fallback", ImVec2(kFolderIconPx, kFolderIconPx));

        if (is_selected)
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(90.0f / 255.0f, 180.0f / 255.0f, 255.0f / 255.0f, 1.0f));
        ImGui::TextUnformatted(folders_[static_cast<size_t>(i)].c_str());
        if (is_selected)
            ImGui::PopStyleColor();
        ImGui::EndGroup();

        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
            selected_ = i;
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            OpenFolder(i);

        if (ImGui::BeginPopupContextItem("##folder_ctx"))
        {
            if (ImGui::MenuItem("Open"))
                OpenFolder(i);
            if (ImGui::MenuItem("Rename"))
                BeginRename(i);
            if (ImGui::MenuItem("Delete"))
            {
                delete_target_ = i;
                ImGui::OpenPopup("Delete Folder");
            }
            ImGui::EndPopup();
        }

        if ((i + 1) % columns != 0)
            ImGui::SameLine(0.0f, 8.0f);

        ImGui::PopID();
    }

    if (ImGui::BeginPopupContextWindow("##root_ctx", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
    {
        if (ImGui::MenuItem("New Folder"))
            BeginCreate();
        ImGui::EndPopup();
    }
}

void FileExplorerApp::RenderOpenFolderView()
{
    ImGui::TextDisabled("2 items");
    ImGui::Separator();

    // Premade template: every folder holds one .png and one .txt file.
    const std::string png_name = current_ + ".png";
    const std::string txt_name = current_ + ".txt";
    const char *names[2] = {png_name.c_str(), txt_name.c_str()};
    const char *kinds[2] = {"PNG image", "Text file"};

    for (int i = 0; i < 2; ++i)
    {
        ImGui::PushID(i);
        ImGui::BeginGroup();
        ImGui::Bullet();
        ImGui::SameLine();
        ImGui::TextUnformatted(names[i]);
        ImGui::TextDisabled("%s", kinds[i]);
        ImGui::EndGroup();

        ImGui::PopID();
        ImGui::Spacing();
    }
}

void FileExplorerApp::Render()
{
    ImGui::SetNextWindowSize(ImVec2(520, 380), ImGuiCond_FirstUseEver);
    ImGui::Begin("File Explorer", &AppState::show_file_explorer);

    RenderAddressBar();
    ImGui::Separator();
    RenderActions();
    ImGui::Separator();

    if (current_.empty())
        RenderRootView();
    else
        RenderOpenFolderView();

    RenderRenamePopup();
    RenderDeletePopup();
    RenderCreatePopup();

    if (!status_.empty())
    {
        ImGui::Separator();
        ImGui::TextDisabled("%s", status_.c_str());
    }

    ImGui::End();
}
