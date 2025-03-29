cmake_minimum_required(VERSION 3.30.0)

project(
    Game
    VERSION 1.0
    DESCRIPTION "Game"
    LANGUAGES C CXX
)

set(SDL_STATIC TRUE)
set(IMGUI_STATIC TRUE)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(lib/cimgui)
add_subdirectory(lib/SDL)
add_subdirectory(lib/SDL_ttf)

add_executable(chess WIN32)
target_sources(
    chess 
    PRIVATE 
    ${PROJECT_SOURCE_DIR}/src/main.c
    ${PROJECT_SOURCE_DIR}/src/logg.c
    ${PROJECT_SOURCE_DIR}/src/chess.c
    ${PROJECT_SOURCE_DIR}/src/SDL_ext.c
    
    ${PROJECT_SOURCE_DIR}/lib/glad/src/glad.c

    ${PROJECT_SOURCE_DIR}/lib/cimgui/imgui/backends/imgui_impl_sdl3.cpp
    ${PROJECT_SOURCE_DIR}/lib/cimgui/imgui/backends/imgui_impl_sdlrenderer3.cpp
) 
target_include_directories(chess PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(chess PRIVATE ${PROJECT_SOURCE_DIR}/lib)
target_include_directories(chess PRIVATE ${PROJECT_SOURCE_DIR}/lib/stb)
target_include_directories(chess PRIVATE ${PROJECT_SOURCE_DIR}/lib/imgui)
target_include_directories(chess PRIVATE ${PROJECT_SOURCE_DIR}/lib/glad/include)
target_link_libraries(chess PRIVATE cimgui)
target_link_libraries(chess PRIVATE SDL3::SDL3-static)
target_link_libraries(chess PRIVATE SDL3_ttf::SDL3_ttf-static)
set_target_properties(
    chess
    PROPERTIES
    C_STANDARD 17
    CXX_STANDARD 20
)