cmake_minimum_required(VERSION 3.30.0)

project(
    Game
    VERSION 1.0
    DESCRIPTION "Game"
    LANGUAGES C
)

add_subdirectory(lib/raylib)

add_executable(game)
target_sources(
    game 
    PRIVATE 
    ${PROJECT_SOURCE_DIR}/src/entry.c
    ${PROJECT_SOURCE_DIR}/src/game.c
) 
target_include_directories(game PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(game PRIVATE raylib)