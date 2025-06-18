cmake_minimum_required(VERSION 3.15)
project(TerrainGen)

set(CMAKE_CXX_STANDARD 17)

# 2D Terrain Generation Prototype
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external/FastNoiseLite/include
    ${PROJECT_SOURCE_DIR}/external/stb/include
)

set(SRC_FILES     
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/tilemap.cpp
    ${PROJECT_SOURCE_DIR}/src/zone.cpp
    ${PROJECT_SOURCE_DIR}/src/zone_planner.cpp
)

add_executable(terrain_2d ${SRC_FILES} )


