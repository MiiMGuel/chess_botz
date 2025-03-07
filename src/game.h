#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "SDL3/SDL.h" 
#include "SDL3_ttf/SDL_ttf.h"

typedef struct game_data {
    int           argc;
    char**        argv;
    FILE*         flog;
    SDL_Window*   window;
    SDL_Renderer* renderer;
    SDL_Event     event;
    TTF_Font*     font;
} game_data_t;

#endif // GAME_H