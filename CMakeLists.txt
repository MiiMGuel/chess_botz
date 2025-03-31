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
option(CGLM_SHARED "Shared build" OFF)
option(CGLM_STATIC "Static build" ON)
option(CGLM_USE_C99 "" OFF) # C11 
option(CGLM_USE_TEST "Enable Tests" OFF) # for make check - make test

add_subdirectory(lib/cglm)
add_subdirectory(lib/cimgui)
add_subdirectory(lib/SDL)

add_executable(chess_botz WIN32)

target_sources(
    chess_botz
    PRIVATE 
    ${PROJECT_SOURCE_DIR}/src/main.c
    ${PROJECT_SOURCE_DIR}/src/chess.c
    ${PROJECT_SOURCE_DIR}/src/logg.c
    ${PROJECT_SOURCE_DIR}/src/gfx/image.c
    ${PROJECT_SOURCE_DIR}/src/gfx/shader.c
    ${PROJECT_SOURCE_DIR}/src/gfx/texture.c
    ${PROJECT_SOURCE_DIR}/src/gfx/vao.c
    ${PROJECT_SOURCE_DIR}/src/gfx/vbo.c
    ${PROJECT_SOURCE_DIR}/src/gfx/ebo.c
    ${PROJECT_SOURCE_DIR}/src/gfx/fbo.c
    ${PROJECT_SOURCE_DIR}/src/gfx/rbo.c
    ${PROJECT_SOURCE_DIR}/src/gfx/frame.c
    
    ${PROJECT_SOURCE_DIR}/lib/glad/src/glad.c

    ${PROJECT_SOURCE_DIR}/lib/cimgui/imgui/backends/imgui_impl_sdl3.cpp
    ${PROJECT_SOURCE_DIR}/lib/cimgui/imgui/backends/imgui_impl_opengl3.cpp
) 

target_include_directories(
    chess_botz 
    PRIVATE 
    ${PROJECT_SOURCE_DIR}/src 
    ${PROJECT_SOURCE_DIR}/lib
    ${PROJECT_SOURCE_DIR}/lib/glad/include
)

target_link_libraries(
    chess_botz 
    PRIVATE
    cglm_headers
    cimgui 
    SDL3::SDL3-static
)

set_target_properties(
    chess_botz
    PROPERTIES
    C_STANDARD 17
    CXX_STANDARD 20
)