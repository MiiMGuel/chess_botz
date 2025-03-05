#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "main.h"
#include "logg.h"
#include "types.h"
#include "SDL2/SDL.h"

static void _game_start(void* app_data) {
    game_data_t* game = (game_data_t*)app_data;
    game->flog = fopen("log.txt", "w+");

    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_VIDEO) < 0)
        logg_fexit(game->flog, 1, LOGG_ERROR, SDL_GetError());

    game->window = SDL_CreateWindow(
        "game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 200, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN
    ); if (game->window == NULL)
        logg_fexit(game->flog, 1, LOGG_ERROR, SDL_GetError());

    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
    if (game->renderer == NULL)
        logg_fexit(game->flog, 1, LOGG_ERROR, SDL_GetError());

    SDL_ShowWindow(game->window);
}

static void _game_close(void* app_data) {
    game_data_t* game = (game_data_t*)app_data;
    if (game->renderer) SDL_DestroyRenderer(game->renderer);
    if (game->window) SDL_DestroyWindow(game->window);
    SDL_Quit(); free(game);
}

static void _game_run(void* app_data) {
    game_data_t* game = (game_data_t*)app_data;

    f64 freq        = (f64)SDL_GetPerformanceFrequency();
    f64 init        = (f64)SDL_GetPerformanceCounter() / freq;
    f64 curr_time   = 0.0;
    f64 prev_time   = 0.0;
    f64 delta_time  = 0.0;
    f64 cdelta_time = 0.0;
    u64 curr_tick   = SDL_GetTicks64();
    u64 prev_tick   = curr_tick;
    u64 delta_tick  = 0.0;
    u64 cdelta_tick = 0.0;
    i64 fcount      = 0;
    i64 fps         = 0;

    while (true) {
        curr_time    = ((f64)SDL_GetPerformanceCounter() / freq) - init;
        delta_time   = curr_time - prev_time;
        cdelta_time += delta_time;

        curr_tick    = SDL_GetTicks64();
        delta_tick   = curr_tick - prev_tick;
        cdelta_tick += delta_tick;

        while(SDL_PollEvent(&game->event))
            if (game->event.type == SDL_QUIT) exit(0);

        SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
        SDL_RenderClear(game->renderer);

        SDL_SetRenderDrawColor(game->renderer, 255, 0, 255, 255);
        SDL_RenderFillRectF(game->renderer, &(SDL_FRect){ 
            .x = 0.0, 
            .y = 0.0, 
            .w = (1.0 / delta_time) / 3.0, 
            .h = 50.0
        }); SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderFillRectF(game->renderer, &(SDL_FRect){ 
            .x = 0.0, 
            .y = 0.0, 
            .w = 1000.0 / 3.0, 
            .h = 10.0
        });

        SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
        SDL_RenderFillRectF(game->renderer, &(SDL_FRect){ 
            .x = 0.0,
            .y = 50.0, 
            .w = fps / 3.0, 
            .h = 50.0 
        }); SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderFillRectF(game->renderer, &(SDL_FRect){ 
            .x = 0.0, 
            .y = 50.0, 
            .w = 1000.0 / 3.0, 
            .h = 10.0
        });
        
        SDL_SetRenderDrawColor(game->renderer, 0, 255, 0, 255);
        SDL_RenderFillRectF(game->renderer, &(SDL_FRect){ 
            .x = 0.0, 
            .y = 100.0, 
            .w = fcount / 3.0, 
            .h = 50.0 
        }); SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderFillRectF(game->renderer, &(SDL_FRect){ 
            .x = 0.0, 
            .y = 100.0, 
            .w = 1000.0 / 3.0, 
            .h = 10.0
        });

        SDL_SetRenderDrawColor(game->renderer, 0, 0, 255, 255);
        SDL_RenderFillRectF(game->renderer, &(SDL_FRect){ 
            .x = 0.0, 
            .y = 150.0, 
            .w = cdelta_time * 325.0, 
            .h = 50.0 
        }); SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderFillRectF(game->renderer, &(SDL_FRect){ 
            .x = 0.0, 
            .y = 150.0,
            .w = 325.0, 
            .h = 10.0
        });

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