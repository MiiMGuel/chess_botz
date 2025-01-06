#include <memory.h>

#include "entry.h"
#include "logg.h"
#include "types.h"
#include "SDL2/SDL.h"

typedef struct game_data {
    int           argc;
    char**        argv;
    bool          running;
    SDL_Window*   window;
    SDL_Renderer* renderer;
    SDL_Event     event;
} game_data_t;

void start(void* app_data) {
    game_data_t* data = (game_data_t*)app_data;
    data->window = SDL_CreateWindow(
        "game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        640, 480, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN
    ); if (data->window == NULL)
        logg_exit(1, LOGG_ERROR, SDL_GetError());

    data->renderer = SDL_CreateRenderer(data->window, -1, SDL_RENDERER_ACCELERATED); 
    if (data->renderer == NULL) 
        logg_exit(1, LOGG_ERROR, SDL_GetError());
        
    SDL_ShowWindow(data->window);
}

void close(void* app_data) {
    game_data_t* data = (game_data_t*)app_data;
    if (data->renderer) SDL_DestroyRenderer(data->renderer);
    if (data->window) SDL_DestroyWindow(data->window);
    free(data);
}

void run(void* app_data) {
    game_data_t* data = (game_data_t*)app_data;
    while (data->running) {
        while(SDL_PollEvent(&data->event)) {
            if (data->event.type == SDL_QUIT) exit(0);
        }

        SDL_SetRenderDrawColor(data->renderer, 255, 255, 255, 255);
        SDL_RenderClear(data->renderer);
        SDL_RenderPresent(data->renderer);
    }
}

app_info_t entry_app(int argc, char* argv[]) {
    game_data_t* data = malloc(sizeof(game_data_t));
    memset(data, 0, sizeof(game_data_t));
    data->argc = argc;
    data->argv = argv;
    data->running = true;

    return (app_info_t){
        .app_data = (void*)data,
        .start = start,
        .run = run,
        .close = close
    };
}