#ifndef CHESS_H
#define CHESS_H

#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "SDL3/SDL.h"
#include "glad/glad.h"

#ifndef CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#define CIMGUI_USE_SDL3
#endif
#include "cimgui/cimgui.h"
#include "cimgui/cimgui_impl.h"

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
} app_data_t;

#endif // CHESS_H