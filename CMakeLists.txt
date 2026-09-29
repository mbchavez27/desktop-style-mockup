cmake_minimum_required(VERSION 3.16)
project(desktop_style_mockup VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
find_package(OpenGL REQUIRED)

# GLFW 3.4: fetched and built from source, no manual install needed.
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(glfw
  URL https://github.com/glfw/glfw/archive/refs/tags/3.4.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(glfw)

# Dear ImGui (docking branch): fetched at configure time into build/_deps/.
FetchContent_Declare(imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG docking
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
  ${imgui_SOURCE_DIR}
  ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui PUBLIC glfw)


add_executable(mockup_app
  src/main.cpp
  src/core/AppState.cpp
  src/ui/Desktop.cpp
  src/ui/Taskbar.cpp
  src/ui/MockApps.cpp
)

target_include_directories(mockup_app PRIVATE include)

target_link_libraries(mockup_app PRIVATE imgui glfw OpenGL::GL)

if(MSVC)
  target_compile_options(mockup_app PRIVATE /W4 /utf-8)
else()
  target_compile_options(mockup_app PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Build + run in one step: cmake --build build --target run
add_custom_target(run
  COMMAND mockup_app
  DEPENDS mockup_app
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  USES_TERMINAL
  COMMENT "Build and run mockup_app"
)
