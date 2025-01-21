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
        640, 480, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN
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
    while (true) {
        while(SDL_PollEvent(&data->event))
            if (data->event.type == SDL_QUIT) exit(0);

        SDL_SetRenderDrawColor(data->renderer, 255, 255, 255, 255);
        SDL_RenderClear(data->renderer);
        SDL_RenderPresent(data->renderer);
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