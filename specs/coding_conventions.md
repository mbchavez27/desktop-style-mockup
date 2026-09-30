# Coding Conventions

Status: active. Applies to all UI code from Phase 8 (OOP refactor)
onward. Derive new code from these rules; review diffs against them.

## 1. Architecture

- One class per UI layer: `include/ui/<Name>.h` + `src/ui/<Name>.cpp`.
  Public API is a per-frame `Render()` method; only `BootScreen` returns
  `bool` ("still animating" — `main.cpp` skips the layers while true).
- Singleton instances: header declares `extern <Name> g_<name>;`, the cpp
  defines it. `src/main.cpp` is the only place that calls `g_*.Render()`;
  layer z-order lives only there.
- `MockApps` owns its app components as private members (`paint_`, and
  `calculator_` once Phase 6 lands); visibility gating
  (`show_app_1` / `show_app_2`) happens in `MockApps::Render()`.
- `AppState` statics are global state per `specs/main_agenda.md` §2 and
  are not converted to instances. App windows receive `&flag` in
  `ImGui::Begin()` so the native 'X' toggles them.
- `IconCache` stays free functions (`GetIcon`) — it is a helper, not a
  UI layer.
- New `.cpp` files must be added to `add_executable` in `CMakeLists.txt`.

## 2. Documentation

- Doxygen on every class and every method, public or private:
  `/** @brief ... */` with `@param` / `@return` where non-obvious.
- Trailing `///<` comments on class data members.
- Inline comments explain _why_, never _what_. No commented-out code.
- Phase specs follow the house format: `Status` / `Depends on` /
  `Scope` / `Exit criteria` / `Commit`.

## 3. Naming

| Kind | Style | Example |
|---|---|---|
| Classes / methods (public) | `PascalCase` | `RenderCanvas`, `BootScreen` |
| Functions, locals, members | `snake_case`, no trailing underscore | `brush_size`, `skip_began_at` |
| File-local constants | `kPascalCase`, `constexpr` | `kFadeIn`, `kIconSize` |
| Globals | `g_` prefix | `g_desktop`, `g_mock_apps` |
| ImGui IDs | `##` prefix for label-less widgets | `##canvas`, `##swatch` |

- Repeated widgets (swatches, icon grids) must wrap in `PushID(i)` /
  `PopID()` to avoid ID collisions.

## 4. State rules

- Instance members over file/function statics. File-local statics are a
  smell — state belongs to the owning class.
- Constants stay file-scope `constexpr` inside an anonymous namespace
  unless they are part of a header contract (e.g. `kTaskbarHeight`).
- Stateless helpers are `private static` member functions.
- Per-frame derived values (clocks, layouts) are computed fresh each
  `Render()` — no caching.

## 5. ImGui conventions

- Scope all style overrides: matching `PushStyleColor`/`PopStyleColor`
  and `PushStyleVar`/`PopStyleVar`; evaluate the guard condition before
  any click mutates the state it reads.
- Colors: `IM_COL32(...)` for `ImDrawList` calls, `ImVec4` (0–1) for
  style APIs. Accent blue is `IM_COL32(90, 180, 255, ...)`.
- Full-screen / non-interactive layers use the chromeless flag set:
  `NoTitleBar | NoResize | NoMove | NoScrollbar | NoSavedSettings |
  NoDocking` (+ `NoBackground` for wallpaper/icons).
- Drawing goes through `ImDrawList`; there is no retained scene graph.
- Text with emphasis uses `PushStyleColor(ImGuiCol_Text, ...)` around
  `TextUnformatted` — the default font atlas has no bold/italic faces.

## 6. Code style

- C++17, 4-space indent, Allman braces, single blank line between
  functions.
- `const` by default; `static_cast` over C casts; `std::clamp` over
  hand-rolled min/max.
- Narrow with `std::isspace(static_cast<unsigned char>(...))` and bound
  every scan by an explicit length — never `str*` past a known end.
- No third-party deps beyond the CMake `FetchContent` set (GLFW, ImGui,
  stb_image). Ask before adding one.

## 7. Quality gates

- Clean build with zero warnings:
  `-Wall -Wextra -Wpedantic` (GCC/Clang) / `/W4` (MSVC).
- Behavior-preserving refactors must smoke-test: boot splash, toggles,
  Paint draw/erase/undo, Word typing, tray clock, PWR exit.
- Commits follow Conventional Commits (`feat:`, `fix:`, `refactor:`,
  `docs:`, `chore:`), pushed to `main`.
