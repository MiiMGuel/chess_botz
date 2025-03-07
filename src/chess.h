#ifndef CHESS_H
#define CHESS_H

#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "SDL3/SDL.h" 
#include "SDL3_ttf/SDL_ttf.h"

typedef struct app_data {
    int           argc;
    char**        argv;
    u32           window_width;
    u32           window_height;
    FILE*         flog;
    SDL_Window*   window;
    SDL_Renderer* renderer;
    SDL_Texture*  pieces_img;
    SDL_Event     event;
    TTF_Font*     font;
} app_data_t;

#endif // CHESS_H