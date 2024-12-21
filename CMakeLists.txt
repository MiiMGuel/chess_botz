cmake_minimum_required(VERSION 3.30.0)

project(
    Game
    VERSION 1.0
    DESCRIPTION "Game"
    LANGUAGES C CXX
)

set(SDL_SHARED FALSE)
set(SDL_STATIC TRUE)
add_subdirectory(lib/SDL)

add_executable(game)
target_sources(
    game 
    PRIVATE 
    ${PROJECT_SOURCE_DIR}/src/entry.cpp
    ${PROJECT_SOURCE_DIR}/src/game.cpp
) 
target_include_directories(game PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(game PRIVATE ${PROJECT_SOURCE_DIR}/lib/stb)
target_link_libraries(game PRIVATE SDL2::SDL2-static)