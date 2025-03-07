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
add_subdirectory(lib/SDL_ttf)

add_executable(chess WIN32)
target_sources(
    chess 
    PRIVATE 
    ${PROJECT_SOURCE_DIR}/src/main.c
    ${PROJECT_SOURCE_DIR}/src/logg.c
    ${PROJECT_SOURCE_DIR}/src/board.c
    ${PROJECT_SOURCE_DIR}/src/chess.c
    ${PROJECT_SOURCE_DIR}/src/SDL_ext.c
    
    ${PROJECT_SOURCE_DIR}/lib/glad/src/glad.c
) 
target_include_directories(chess PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(chess PRIVATE ${PROJECT_SOURCE_DIR}/lib/glad/include)
target_include_directories(chess PRIVATE ${PROJECT_SOURCE_DIR}/lib/stb)
target_link_libraries(chess PRIVATE SDL3::SDL3-static)
target_link_libraries(chess PRIVATE SDL3_ttf::SDL3_ttf-static)
set_target_properties(
    chess
    PROPERTIES
    C_STANDARD 17
    CXX_STANDARD 20
)