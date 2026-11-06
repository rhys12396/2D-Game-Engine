
cmake_minimum_required(VERSION 3.28)

project(GameEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# SFML configuration
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
)

# Dear ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.9b
)

FetchContent_MakeAvailable(SFML imgui)

# ImGui-SFML configuration
set(IMGUI_DIR ${imgui_SOURCE_DIR} CACHE PATH "" FORCE)
set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
    GIT_TAG v3.0
)

FetchContent_MakeAvailable(ImGui-SFML)

# Game engine executable
add_executable(GameEngine
    src/main.cpp
)

target_link_libraries(GameEngine PRIVATE
    SFML::Graphics
    ImGui-SFML::ImGui-SFML
)
