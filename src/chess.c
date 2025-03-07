#include "chess.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <memory.h>

#include "main.h"
#include "logg.h"
#include "types.h"
#include "board.h"
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

static void _chess_start(void* app_data) {
    app_data_t* app    = (app_data_t*)app_data;
    app->flog          = fopen("log.txt", "w+");
    app->window_width  = 1280;
    app->window_height = 720;

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS))
        logg_fexit(app->flog, 1, LOGG_ERROR, SDL_GetError());
    
    if (!TTF_Init()) 
        logg_fexit(app->flog, 1, LOGG_ERROR, SDL_GetError());

    app->window = SDL_CreateWindow(
        "app", app->window_width, app->window_height, 
        SDL_WINDOW_HIDDEN
    ); if (!app->window)
        logg_fexit(app->flog, 1, LOGG_ERROR, SDL_GetError());

    app->renderer = SDL_CreateRenderer(app->window, NULL);
    if (!app->renderer)
        logg_fexit(app->flog, 1, LOGG_ERROR, SDL_GetError());

    app->font = TTF_OpenFont("Minecraft.ttf", 24);
    if (!app->font)
        logg_fexit(app->flog, 1, LOGG_ERROR, SDL_GetError());

    {
        SDL_Surface* surface = SDL_LoadBMP("pieces.bmp");
        if (!surface)
            logg_fexit(app->flog, 1, LOGG_ERROR, SDL_GetError());

        app->pieces_img = SDL_CreateTextureFromSurface(app->renderer, surface);
        if (!app->pieces_img)
            logg_fexit(app->flog, 1, LOGG_ERROR, SDL_GetError());
    }

    SDL_SetRenderVSync(app->renderer, true);
    SDL_ShowWindow(app->window);
}

static void _chess_close(void* app_data) {
    app_data_t* app = (app_data_t*)app_data;
    if (app->renderer) SDL_DestroyRenderer(app->renderer);
    if (app->window) SDL_DestroyWindow(app->window);
    if (app->font) TTF_CloseFont(app->font);
    TTF_Quit(); SDL_Quit(); free(app);
}

static void _chess_run(void* app_data) {
    app_data_t* app  = (app_data_t*)app_data;
    f64 freq         = (f64)SDL_GetPerformanceFrequency();
    f64 init         = (f64)SDL_GetPerformanceCounter() / freq;
    f64 curr_time    = 0.0;
    f64 prev_time    = 0.0;
    f64 delta_time   = 0.0;
    f64 cdelta_time  = 0.0;
    u64 curr_tick    = SDL_GetTicks();
    u64 prev_tick    = curr_tick;
    u64 delta_tick   = 0.0;
    u64 cdelta_tick  = 0.0;
    i64 fcount       = 0;
    i64 fps          = 0;
    board_t board    = {0};

    while (true) {
        curr_time    = ((f64)SDL_GetPerformanceCounter() / freq) - init;
        delta_time   = curr_time - prev_time;
        cdelta_time += delta_time;

        curr_tick    = SDL_GetTicks();
        delta_tick   = curr_tick - prev_tick;
        cdelta_tick += delta_tick;

        while(SDL_PollEvent(&app->event))
            if (app->event.type == SDL_EVENT_QUIT) exit(0);

        SDL_SetRenderDrawColor(app->renderer, 22, 21, 18, 255);
        SDL_RenderClear(app->renderer);

        board_draw(
            app->renderer,
            app->pieces_img,
            &board,
            (SDL_Color){240, 217, 181, 255},
            (SDL_Color){181, 136, 99, 255},
            640
        );

        SDL_RenderTexture(app->renderer, app->pieces_img, NULL, &(SDL_FRect){
            .w = 480,
            .h = 160,
            .x = 40,
            .y = 40
        });

        SDL_RenderPresent(app->renderer);

        fcount++;
        if (cdelta_time >= 1.0) {
            fps         = fcount;
            fcount      = 0;
            cdelta_time = 0.0;
            cdelta_tick = 0.0;
        } 
        
        prev_time = curr_time;
        prev_tick = curr_tick;
    }
}

app_info_t main_app(int argc, char* argv[]) {
    app_data_t* app = malloc(sizeof(app_data_t));
    memset(app, 0, sizeof(app_data_t));
    app->argc = argc;
    app->argv = argv;

    return (app_info_t){
        .app_data = (void*)app,
        .start = _chess_start,
        .run = _chess_run,
        .close = _chess_close
    };
}