cmake_minimum_required(VERSION 3.15)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "Disable Wayland support for GLFW")
project(TerrainGen)

set(CMAKE_CXX_STANDARD 17)

# Common source files
set(COMMON_SRC
    ${PROJECT_SOURCE_DIR}/src/tilemap.cpp
    ${PROJECT_SOURCE_DIR}/src/zone.cpp
    ${PROJECT_SOURCE_DIR}/src/zone_planner.cpp
)

# Include dirs (for all targets)
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external/FastNoiseLite/include
    ${PROJECT_SOURCE_DIR}/external/stb/include
)

# ---- Terrain CLI (2D PNG generator etc.) ----
add_executable(terrain_2d
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${COMMON_SRC}
)

# ---- Terrain GUI (3D Viewer with ImGui + OpenGL) ----

# GLAD
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# ImGui
set(IMGUI_SRC
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_tables.cpp
)

# GLFW
add_subdirectory(external/glfw)

# GUI source files
add_executable(terrain_gui
    ${PROJECT_SOURCE_DIR}/gui/main.cpp
    ${PROJECT_SOURCE_DIR}/gui/terrain_renderer.cpp
    ${PROJECT_SOURCE_DIR}/gui/viewport_navigator.cpp
    ${COMMON_SRC}
    ${IMGUI_SRC}
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
 "gui/shader.cpp")

target_include_directories(terrain_gui PRIVATE
    external/glad/include
    external/glfw/include
    external/glm
    external/imgui
    external/imgui/backends
)

find_package(OpenGL REQUIRED)

target_link_libraries(terrain_gui
    glad
    glfw
    OpenGL::GL
)

# Set output dir to project root/build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Copy shaders folder after build
add_custom_command(
    TARGET terrain_gui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/resources
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources
)