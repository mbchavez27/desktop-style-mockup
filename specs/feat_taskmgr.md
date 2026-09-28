# Feature Prompt: Task Manager

## 1. Objective

Implement a standard, floating OS-style window that displays mock running processes using the ImGui Table API.

## 2. Target Files

- `include/ui/TaskManager.h`
- `src/ui/TaskManager.cpp` (expose a function `void RenderTaskManager()`)

## 3. UI Requirements & ImGui Directives

- **Visibility Control:**
  Wrap the entire logic in `if (AppState::show_task_mgr)`. Pass a pointer to `&AppState::show_task_mgr` into `ImGui::Begin("Task Manager", &AppState::show_task_mgr)` so the native window 'X' close button works and correctly toggles the state.
- **Window Layout:**
  Set an initial default size using `ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver)`.
- **Process Table:**
  - Create a struct for dummy processes: `struct Process { std::string name; float cpu; int mem_mb; };`
  - Initialize a static `std::vector` with 4-5 dummy entries (e.g., "csopesy.exe", "dwm.exe", "System Idle").
  - Use `ImGui::BeginTable("ProcessesTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable)`.
  - Set headers: `ImGui::TableSetupColumn("Name");`, `ImGui::TableSetupColumn("CPU");`, `ImGui::TableSetupColumn("Memory");` and call `ImGui::TableHeadersRow()`.
- **Dynamic Simulation (Optional but recommended):**
  To make it feel real, every few frames (or using `ImGui::GetTime()`), slightly perturb the `cpu` float values of the dummy processes by a small random amount so the numbers fluctuate like a real task manager. Format CPU as `%.1f%%` and Memory as `%d MB`.
