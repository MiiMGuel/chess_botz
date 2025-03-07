#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <memory.h>

#include "main.h"
#include "logg.h"
#include "types.h"
#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

static void _SDL_RenderText(
    SDL_Renderer* renderer, 
    TTF_Font* font, 
    SDL_Color color, 
    const SDL_FRect* rect,
    const char* message
) {
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(
        font, 
        message, 
        0,
        color
    ); 
    
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); 
    SDL_SetTextureScaleMode(Message, SDL_SCALEMODE_NEAREST);

    SDL_RenderTexture(renderer, Message, NULL, rect);
    SDL_DestroySurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

static void _SDL_RenderTextf(
    SDL_Renderer* renderer, 
    TTF_Font* font, 
    SDL_Color color, 
    const SDL_FRect* rect,
    const char* format,
    ...
) {
    va_list args; va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, 256, format, args);
    va_end(args);
    _SDL_RenderText(renderer, font, color, rect, buffer);
}

static void _game_start(void* app_data) {
    game_data_t* game = (game_data_t*)app_data;
    game->flog = fopen("log.txt", "w+");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS))
        logg_fexit(game->flog, 1, LOGG_ERROR, SDL_GetError());
    
    if (!TTF_Init())
        logg_fexit(game->flog, 1, LOGG_ERROR, SDL_GetError());

    game->window = SDL_CreateWindow(
        "game", 800, 600, 
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN
    ); if (game->window == NULL)
        logg_fexit(game->flog, 1, LOGG_ERROR, SDL_GetError());

    game->renderer = SDL_CreateRenderer(game->window, NULL);
    if (game->renderer == NULL)
        logg_fexit(game->flog, 1, LOGG_ERROR, SDL_GetError());

    game->font = TTF_OpenFont("Minecraft.ttf", 24);
    if (!game->font)
        logg_fexit(game->flog, 1, LOGG_ERROR, SDL_GetError());

    SDL_ShowWindow(game->window);
}

static void _game_close(void* app_data) {
    game_data_t* game = (game_data_t*)app_data;
    if (game->renderer) SDL_DestroyRenderer(game->renderer);
    if (game->window) SDL_DestroyWindow(game->window);
    if (game->font) TTF_CloseFont(game->font);
    TTF_Quit(); SDL_Quit(); free(game);
}

static void _game_run(void* app_data) {
    game_data_t* game = (game_data_t*)app_data;
    f64 freq        = (f64)SDL_GetPerformanceFrequency();
    f64 init        = (f64)SDL_GetPerformanceCounter() / freq;
    f64 curr_time   = 0.0;
    f64 prev_time   = 0.0;
    f64 delta_time  = 0.0;
    f64 cdelta_time = 0.0;
    u64 curr_tick   = SDL_GetTicks();
    u64 prev_tick   = curr_tick;
    u64 delta_tick  = 0.0;
    u64 cdelta_tick = 0.0;
    i64 fcount      = 0;
    i64 fps         = 0;

    while (true) {
        curr_time    = ((f64)SDL_GetPerformanceCounter() / freq) - init;
        delta_time   = curr_time - prev_time;
        cdelta_time += delta_time;

        curr_tick    = SDL_GetTicks();
        delta_tick   = curr_tick - prev_tick;
        cdelta_tick += delta_tick;

        while(SDL_PollEvent(&game->event))
            if (game->event.type == SDL_EVENT_QUIT) exit(0);

        SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
        SDL_RenderClear(game->renderer);

        _SDL_RenderTextf(
            game->renderer,
            game->font,
            (SDL_Color){.r = 0, .g = 0, .b = 0, .a = 255},
            &(SDL_FRect){.w = 120, .h = 60, .x = 0, .y = 0},
            "%d", fps
        );

        SDL_RenderPresent(game->renderer);

        fcount++;
        if (cdelta_time >= 1.0) {
            fps = fcount;

            logg_fprintf(
                game->flog, LOGG_TRACE, 
                "curr_time = %0.6f; prev_time = %0.6f; delta_time = %0.6f; fps = %d; fps = %0.6f",
                 curr_time,         prev_time,         delta_time,         fps,      1.0 / delta_time
            );

            logg_fprintf(
                game->flog, LOGG_TRACE, 
                "curr_tick = %d; prev_tick = %d; delta_tick = %d; fps = %d\n",
                 curr_tick,         prev_tick,         delta_tick,         fps
            );

            fcount = 0;
            cdelta_time = 0.0;
            cdelta_tick = 0.0;
        } 
        
        prev_time = curr_time;
        prev_tick = curr_tick;
    }
}

app_info_t main_app(int argc, char* argv[]) {
    game_data_t* game = malloc(sizeof(game_data_t));
    memset(game, 0, sizeof(game_data_t));
    game->argc = argc;
    game->argv = argv;

    return (app_info_t){
        .app_data = (void*)game,
        .start = _game_start,
        .run = _game_run,
        .close = _game_close
    };
}