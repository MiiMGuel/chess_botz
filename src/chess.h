#ifndef CHESS_H
#define CHESS_H

#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "gfx/image.h"
#include "gfx/texture.h"
#include "gfx/shader.h"
#include "gfx/vao.h"
#include "gfx/vbo.h"
#include "gfx/ebo.h"
#include "gfx/fbo.h"
#include "SDL3/SDL.h"
#include "glad/glad.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui/cimgui.h"

typedef struct frame {
    fbo_t     fbo;
    rbo_t     rbo;
    texture_t texture;
    u64       width;
    u64       height;
} frame_t;

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
    frame_t       frame;
    texture_t     texture;
    shader_t      shader;
    vao_t         vao;
    vbo_t         vbo;
    ebo_t         ebo;
} app_data_t;

#endif // CHESS_H