#include "chess.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "main.h"
#include "logg.h"
#include "types.h"
#include "board.h"
#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "SDL_ext.h"

#ifndef CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#endif
#include "cimgui/cimgui.h"

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

    app->font = TTF_OpenFont("assets/JetBrainsMonoNerdFont-SemiBold.ttf", 256);
    if (!app->font)
        logg_fexit(app->flog, 1, LOGG_ERROR, SDL_GetError());

    { // Load textures from surface
        SDL_Surface* surface = SDL_LoadBMP("assets/board/brown.bmp");
        if (!surface)
            logg_fexit(app->flog, 1, LOGG_ERROR, SDL_GetError());

        app->board_img = SDL_CreateTextureFromSurface(app->renderer, surface);
        if (!app->board_img)
            logg_fexit(app->flog, 1, LOGG_ERROR, SDL_GetError());
        SDL_DestroySurface(surface);

        surface = SDL_LoadBMP("assets/pieces/cburnett.bmp");
        if (!surface)
            logg_fexit(app->flog, 1, LOGG_ERROR, SDL_GetError());

        app->pieces_img = SDL_CreateTextureFromSurface(app->renderer, surface);
        if (!app->pieces_img)
            logg_fexit(app->flog, 1, LOGG_ERROR, SDL_GetError());
        SDL_DestroySurface(surface);
    }

    app->imgui_ctx = igCreateContext(NULL);
    igSetCurrentContext(app->imgui_ctx);
    ImGuiIO io = *igGetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui_ImplSDL3_InitForSDLRenderer(app->window, app->renderer);
    ImGui_ImplSDLRenderer3_Init(app->renderer);
    igStyleColorsDark(NULL);

    SDL_SetRenderVSync(app->renderer, true);
    SDL_ShowWindow(app->window);
}

static void _chess_close(void* app_data) {
    app_data_t* app = (app_data_t*)app_data;
    if (app->imgui_ctx) {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        igDestroyContext(NULL);
    } if (app->board_img) SDL_DestroyTexture(app->board_img);
    if (app->pieces_img) SDL_DestroyTexture(app->pieces_img);
    if (app->renderer) SDL_DestroyRenderer(app->renderer);
    if (app->window) SDL_DestroyWindow(app->window);
    if (app->font) TTF_CloseFont(app->font);
    TTF_Quit(); SDL_Quit(); fclose(app->flog); free(app);
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
    board_setup(&board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    while (true) {
        curr_time    = ((f64)SDL_GetPerformanceCounter() / freq) - init;
        delta_time   = curr_time - prev_time;
        cdelta_time += delta_time;

        curr_tick    = SDL_GetTicks();
        delta_tick   = curr_tick - prev_tick;
        cdelta_tick += delta_tick;

        while(SDL_PollEvent(&app->event)) {
            ImGui_ImplSDL3_ProcessEvent(&app->event);
            if (app->event.type == SDL_EVENT_QUIT) exit(0);
        }

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        igNewFrame();

        igBegin("window", NULL, 0);
        igText("Hello from imgui");
        ImVec2 reset_button;
        reset_button.x = 0; reset_button.y = 0;
        if (igButton("reset", reset_button))
            board_setup(&board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        ImVec2 Nf3_button;
        Nf3_button.x = 0; Nf3_button.y = 0;
        if (igButton("Nf3", Nf3_button))
            board_setup(&board, "rnbqkbnr/pp1ppppp/8/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
        igEnd();

        igRender();

        SDL_SetRenderDrawColor(app->renderer, 22, 21, 18, 255);
        SDL_RenderClear(app->renderer);

        board_draw(
            app->renderer,
            app->board_img,
            app->pieces_img,
            &board,
            640
        );

        ImGui_ImplSDLRenderer3_RenderDrawData(igGetDrawData(), app->renderer);
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