cmake_minimum_required(VERSION 3.30.0)

project(
    Game
    VERSION 1.0
    DESCRIPTION "Game"
    LANGUAGES C CXX
)

set(SDL_STATIC TRUE)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(lib/SDL)

add_executable(game)
target_sources(
    game 
    PRIVATE 
    ${PROJECT_SOURCE_DIR}/src/main.c
    ${PROJECT_SOURCE_DIR}/src/game.c
    ${PROJECT_SOURCE_DIR}/src/logg.c
    
    ${PROJECT_SOURCE_DIR}/lib/glad/src/glad.c
) 
target_include_directories(game PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(game PRIVATE ${PROJECT_SOURCE_DIR}/lib/glad/include)
target_include_directories(game PRIVATE ${PROJECT_SOURCE_DIR}/lib/stb)
target_link_libraries(game PRIVATE SDL2::SDL2-static)
set_target_properties(
    game
    PROPERTIES
    C_STANDARD 17
    CXX_STANDARD 20
)