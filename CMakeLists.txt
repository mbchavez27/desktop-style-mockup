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


# stb_image: single-header PNG loader for taskbar icons, fetched at configure time.
set(STB_IMAGE_HEADER ${CMAKE_CURRENT_BINARY_DIR}/_thirdparty/stb_image.h)
if(NOT EXISTS ${STB_IMAGE_HEADER})
  file(DOWNLOAD https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
       ${STB_IMAGE_HEADER} SHOW_PROGRESS STATUS _stb_status)
  list(GET _stb_status 0 _stb_code)
  if(NOT _stb_code EQUAL 0)
    message(FATAL_ERROR "Failed to download stb_image.h: ${_stb_status}")
  endif()
endif()

add_executable(mockup_app
  src/main.cpp
  src/core/AppState.cpp
  src/ui/Desktop.cpp
  src/ui/DesktopIcons.cpp
  src/ui/BootScreen.cpp
  src/ui/Taskbar.cpp
  src/ui/MockApps.cpp
  src/ui/PaintApp.cpp
  src/ui/CalculatorApp.cpp
  src/ui/FileExplorerApp.cpp
  src/ui/BrowserApp.cpp
  src/ui/IconCache.cpp
)

target_include_directories(mockup_app PRIVATE
  include
  ${CMAKE_CURRENT_BINARY_DIR}/_thirdparty
)

target_link_libraries(mockup_app PRIVATE imgui glfw OpenGL::GL)

if(MSVC)
  target_compile_options(mockup_app PRIVATE /W4 /utf-8)
else()
  target_compile_options(mockup_app PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Icons live in source assets/ but the app runs from the build dir.
add_custom_target(copy_assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
  COMMENT "Copying assets next to mockup_app"
)
add_dependencies(mockup_app copy_assets)

# Bundle the UCRT64 runtime DLLs next to the exe. Stale copies of these same
# DLLs ship with C:\MinGW and iVerilog, which sit earlier in a typical PATH and
# make the app die on launch (0xC0000139) from a plain shell or Explorer.
# The Windows loader checks the exe's own directory first, so bundled copies
# always win regardless of PATH order.
if(WIN32)
  # These live in the same dir as the compiler driver (e.g. C:/msys64/ucrt64/bin).
  get_filename_component(_cxx_dir ${CMAKE_CXX_COMPILER} DIRECTORY)
  set(_rt_dll_names libstdc++-6.dll libgcc_s_seh-1.dll libwinpthread-1.dll)
  set(UCRT_RUNTIME_DLLS "")
  foreach(_dll IN LISTS _rt_dll_names)
    if(EXISTS "${_cxx_dir}/${_dll}")
      list(APPEND UCRT_RUNTIME_DLLS "${_cxx_dir}/${_dll}")
    else()
      message(WARNING "Runtime DLL not found next to compiler: ${_cxx_dir}/${_dll}")
    endif()
  endforeach()
  if(UCRT_RUNTIME_DLLS)
    message(STATUS "Bundling runtime DLLs: ${UCRT_RUNTIME_DLLS}")
    add_custom_command(TARGET mockup_app POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              ${UCRT_RUNTIME_DLLS} $<TARGET_FILE_DIR:mockup_app>
      COMMENT "Bundling UCRT64 runtime DLLs next to mockup_app"
    )
  endif()
endif()

# Build + run in one step: cmake --build build --target run
add_custom_target(run
  COMMAND mockup_app
  DEPENDS mockup_app
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  USES_TERMINAL
  COMMENT "Build and run mockup_app"
)
