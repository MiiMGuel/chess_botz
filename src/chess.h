#ifndef CHESS_H
#define CHESS_H

#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "shader.h"
#include "SDL3/SDL.h"
#include "glad/glad.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui/cimgui.h"

typedef struct app_data {
    int           argc;
    char**        argv;
    u32           window_width;
    u32           window_height;
    FILE*         flog;
    SDL_Window*   window;
    SDL_GLContext gl_context;
    SDL_Event     event;
    ImGuiContext* ig_context;
    shader_t      shader;
    u32 vao, vbo, ebo;
} app_data_t;

#endif // CHESS_H