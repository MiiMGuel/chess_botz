#ifndef SDL_EXT_H
#define SDL_EXT_H

#include <stdarg.h>

#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

void SDL_RenderText(
    SDL_Renderer* renderer, 
    TTF_Font* font, 
    SDL_Color color, 
    const SDL_FRect* rect,
    const char* message
);

void SDL_RenderTextf(
    SDL_Renderer* renderer, 
    TTF_Font* font, 
    SDL_Color color, 
    const SDL_FRect* rect,
    const char* format,
    ...
);

#endif // SDL_EXT_H