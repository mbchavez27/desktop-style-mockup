#pragma once

#include <random>
#include <string>
#include <vector>

/**
 * @brief Floating Task Manager window with a live mock process table.
 *
 * Lists a fixed set of dummy processes (Name / CPU / Memory) whose CPU
 * values drift every frame from a class-owned RNG.
 */
class TaskManager
{
public:
    /**
     * @brief Renders the Task Manager window for this frame.
     *
     * No-op unless AppState::show_task_mgr; the flag is passed to
     * ImGui::Begin so the native 'X' toggles it.
     */
    void Render();

private:
    /**
     * @brief One row of the mock process table.
     */
    struct Process
    {
        std::string name; ///< Executable name shown in the Name column.
        float cpu;        ///< CPU usage percentage, kept in [0, 100].
        int mem_mb;       ///< Resident memory in megabytes.
    };

    /**
     * @brief Nudges every process's CPU by a small random step.
     *
     * The step is scaled by the frame's delta time; each value is clamped
     * to [0, 100] afterwards.
     */
    void PerturbCpu();

    /**
     * @brief Draws the bordered, resizable Name / CPU / Memory table.
     */
    void RenderTable();

    std::vector<Process> processes = {
        {"csopesy.exe", 12.3f, 42},
        {"dwm.exe", 4.8f, 96},
        {"explorer.exe", 2.1f, 128},
        {"svchost.exe", 1.4f, 24},
        {"System Idle", 28.0f, 0},
    }; ///< Dummy process list rendered as table rows.

    std::mt19937 rng{std::random_device{}()};                    ///< Source for CPU drift.
    std::uniform_real_distribution<float> cpu_step{-1.0f, 1.0f}; ///< Unit drift direction/size.
};

/// Global TaskManager instance driven by the main render loop.
extern TaskManager g_task_manager;
