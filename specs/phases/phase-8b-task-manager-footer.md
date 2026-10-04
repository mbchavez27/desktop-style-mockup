# Phase 8b — Task Manager footer (groupmate)

Status: pending.

Spec: `specs/feat/feat_taskmgr.md` (footer bullet).

Depends on: Phase 8a pushed to `main` — `git pull` before starting.

## Scope (groupmate — small)

- `src/ui/TaskManager.cpp` only: after `ImGui::EndTable()`, a separator
  plus a summary line — `Processes: <n>  |  Total CPU: <sum>` — with the
  sum formatted `%.1f%%`, computed fresh each frame from the `processes`
  vector and clamped to [0, 100]. Stays inside the existing
  `AppState::show_task_mgr` / `ImGui::Begin()` block.
- Docs: `README.md` drop "(WIP)" (line 73), insert
  `g_task_manager.Render()` into the z-order block between
  `g_mock_apps.Render()` and `g_taskbar.Render()`, add a `TaskManager`
  row to the Layers table; flip this file's `Status:` → `done.`.
- Must not change: `TaskManager.h`, table columns/formats, `AppState`
  flags, `CMakeLists.txt`, `src/main.cpp`.

## Exit criteria

- Footer tracks the table every frame, hides with the window, never
  leaves [0, 100]. 8a behavior unchanged. No new warnings.

## Commit

- `feat(taskmgr): add total cpu footer` + `docs(specs): mark phase 8 done`,
  push to `main`.
