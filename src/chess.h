#ifndef CHESS_H
#define CHESS_H

#include <stdio.h>

#include "types.h"
#include "gfx/gfx.h"
#include "SDL3/SDL.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui/cimgui.h"
#include "cimfilebrowser.h"

typedef struct app_data {
    int            argc;
    char**         argv;
    char*          pwd;
    u32            window_width;
    u32            window_height;
    FILE*          flog;
    SDL_Window*    window;
    SDL_GLContext  gl_context;
    SDL_Event      event;
    ImGuiContext*  ig_context;
    ImFileBrowser* img_browser;
    frame_t        frame;
    texture_t      texture;
    shader_t       shader;
    vao_t          vao;
    vbo_t          vbo;
    ebo_t          ebo;
} app_data_t;

#endif // CHESS_H