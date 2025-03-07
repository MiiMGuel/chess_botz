#include "SDL_ext.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

void SDL_RenderText(
    SDL_Renderer* renderer, 
    TTF_Font* font, 
    SDL_Color color, 
    const SDL_FRect* rect,
    const char* msg
) {
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(
        font, 
        msg, 
        0,
        color
    ); 
    
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); 
    SDL_SetTextureScaleMode(Message, SDL_SCALEMODE_NEAREST);

    SDL_RenderTexture(renderer, Message, NULL, rect);
    SDL_DestroySurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

void SDL_RenderTextf(
    SDL_Renderer* renderer, 
    TTF_Font* font, 
    SDL_Color color, 
    const SDL_FRect* rect,
    const char* fmt,
    ...
) {
    va_list args; va_start(args, fmt);
    char buffer[256];
    vsnprintf(buffer, 256, fmt, args);
    va_end(args);
    SDL_RenderText(renderer, font, color, rect, buffer);
}