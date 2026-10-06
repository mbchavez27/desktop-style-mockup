#include "imgui.h"
#include "core/AppState.h"
#include "ui/TaskManager.h"

#include <algorithm>

namespace
{

constexpr float kCpuDriftPerSec = 12.0f; // max |delta| in percentage points per second

} // namespace

TaskManager g_task_manager;

void TaskManager::PerturbCpu()
{
    // Scale by frame time so drift speed doesn't depend on the frame rate.
    const float max_step = kCpuDriftPerSec * ImGui::GetIO().DeltaTime;
    for (Process &process : processes)
        process.cpu = std::clamp(process.cpu + cpu_step(rng) * max_step, 0.0f, 100.0f);
}

void TaskManager::RenderTable()
{
    constexpr ImGuiTableFlags flags =
        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable;
    if (!ImGui::BeginTable("ProcessesTable", 3, flags))
        return;

    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("CPU");
    ImGui::TableSetupColumn("Memory");
    ImGui::TableHeadersRow();

    for (const Process &process : processes)
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::TextUnformatted(process.name.c_str());
        ImGui::TableNextColumn();
        ImGui::Text("%.1f%%", process.cpu);
        ImGui::TableNextColumn();
        ImGui::Text("%d MB", process.mem_mb);
    }

    ImGui::EndTable();
}

void TaskManager::Render()
{
    if (!AppState::show_task_mgr)
        return;

    // Drift only while open so the numbers resume where they left off.
    PerturbCpu();

    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Task Manager", &AppState::show_task_mgr))
    {
        RenderTable();

        float total_cpu = 0.0f;

        for (const auto &process : processes) {
            total_cpu += process.cpu;
        }

        total_cpu = std::clamp(total_cpu, 0.0f, 100.0f);

        ImGui::Separator();
        ImGui::Text("Processes: %zu | Total CPU: %.1f%%", processes.size(), total_cpu);
    }


    ImGui::End();
}
