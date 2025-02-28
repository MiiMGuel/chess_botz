#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "main.h"
#include "logg.h"
#include "event.h"
#include "types.h"
#include "SDL2/SDL.h" 

typedef struct app_data {
    int           argc;
    char**        argv;
    FILE*         flog;
    SDL_Window*   window;
    SDL_Renderer* renderer;
    SDL_Event     event;
} app_data_t;

void start(void* app_data) {
    app_data_t* data = (app_data_t*)app_data;
    data->flog = fopen("log.txt", "w+");

    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_VIDEO) < 0)
        logg_fexit(data->flog, 1, LOGG_ERROR, SDL_GetError());

    data->window = SDL_CreateWindow(
        "game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 200, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN
    ); if (data->window == NULL)
        logg_fexit(data->flog, 1, LOGG_ERROR, SDL_GetError());

    data->renderer = SDL_CreateRenderer(data->window, -1, SDL_RENDERER_ACCELERATED);
    if (data->renderer == NULL)
        logg_fexit(data->flog, 1, LOGG_ERROR, SDL_GetError());

    SDL_ShowWindow(data->window);
}

void close(void* app_data) {
    app_data_t* data = (app_data_t*)app_data;
    if (data->renderer) SDL_DestroyRenderer(data->renderer);
    if (data->window) SDL_DestroyWindow(data->window);
    SDL_Quit(); free(data);
}

void run(void* app_data) {
    app_data_t* data = (app_data_t*)app_data;

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

        while(SDL_PollEvent(&data->event))
            if (data->event.type == SDL_QUIT) exit(0);

        SDL_SetRenderDrawColor(data->renderer, 255, 255, 255, 255);
        SDL_RenderClear(data->renderer);

        SDL_SetRenderDrawColor(data->renderer, 255, 0, 255, 255);
        SDL_RenderFillRectF(data->renderer, &(SDL_FRect){ 
            .x = 0.0, 
            .y = 0.0, 
            .w = (1.0 / delta_time) / 3.0, 
            .h = 50.0
        }); SDL_SetRenderDrawColor(data->renderer, 0, 0, 0, 255);
        SDL_RenderFillRectF(data->renderer, &(SDL_FRect){ 
            .x = 0.0, 
            .y = 0.0, 
            .w = 1000.0 / 3.0, 
            .h = 10.0
        });

        SDL_SetRenderDrawColor(data->renderer, 255, 0, 0, 255);
        SDL_RenderFillRectF(data->renderer, &(SDL_FRect){ 
            .x = 0.0,
            .y = 50.0, 
            .w = fps / 3.0, 
            .h = 50.0 
        }); SDL_SetRenderDrawColor(data->renderer, 0, 0, 0, 255);
        SDL_RenderFillRectF(data->renderer, &(SDL_FRect){ 
            .x = 0.0, 
            .y = 50.0, 
            .w = 1000.0 / 3.0, 
            .h = 10.0
        });
        
        SDL_SetRenderDrawColor(data->renderer, 0, 255, 0, 255);
        SDL_RenderFillRectF(data->renderer, &(SDL_FRect){ 
            .x = 0.0, 
            .y = 100.0, 
            .w = fcount / 3.0, 
            .h = 50.0 
        }); SDL_SetRenderDrawColor(data->renderer, 0, 0, 0, 255);
        SDL_RenderFillRectF(data->renderer, &(SDL_FRect){ 
            .x = 0.0, 
            .y = 100.0, 
            .w = 1000.0 / 3.0, 
            .h = 10.0
        });

        SDL_SetRenderDrawColor(data->renderer, 0, 0, 255, 255);
        SDL_RenderFillRectF(data->renderer, &(SDL_FRect){ 
            .x = 0.0, 
            .y = 150.0, 
            .w = cdelta_time * 325.0, 
            .h = 50.0 
        }); SDL_SetRenderDrawColor(data->renderer, 0, 0, 0, 255);
        SDL_RenderFillRectF(data->renderer, &(SDL_FRect){ 
            .x = 0.0, 
            .y = 150.0,
            .w = 325.0, 
            .h = 10.0
        });

        SDL_RenderPresent(data->renderer);

        fcount++;
        if (cdelta_time >= 1.0) {
            fps = fcount;

            logg_fprintf(
                data->flog, LOGG_TRACE, 
                "curr_time = %0.6f; prev_time = %0.6f; delta_time = %0.6f; fps = %d; fps = %0.6f",
                 curr_time,         prev_time,         delta_time,         fps,      1.0 / delta_time
            );

            logg_fprintf(
                data->flog, LOGG_TRACE, 
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
    app_data_t* data = malloc(sizeof(app_data_t));
    memset(data, 0, sizeof(app_data_t));
    data->argc = argc;
    data->argv = argv;

    return (app_info_t){
        .app_data = (void*)data,
        .start = start,
        .run = run,
        .close = close
    };
}