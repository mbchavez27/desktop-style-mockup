cmake_minimum_required(VERSION 3.16)
project(desktop_style_mockup VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(mockup_app
  src/main.cpp
)

target_include_directories(mockup_app PRIVATE include)

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
