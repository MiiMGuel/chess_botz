#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "main.h"
#include "chess.h"
#include "logg.h"
#include "types.h"
#include "SDL3/SDL.h"
#include "glad/glad.h"

static void _chess_start(void* app_data) {
    app_data_t* app    = (app_data_t*)app_data;
    app->flog          = fopen("log.txt", "w+");
    app->window_width  = 1280;
    app->window_height = 720;

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS))
        logg_fexit(app->flog, 1, LOGG_ERROR, SDL_GetError());

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    app->window = SDL_CreateWindow(
        "app", app->window_width, app->window_height, 
        SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
    ); if (!app->window)
        logg_fexit(app->flog, 1, LOGG_ERROR, SDL_GetError());

    
    app->gl_context = SDL_GL_CreateContext(app->window);
    if (!app->gl_context)
        logg_fexit(app->flog, 1, LOGG_ERROR, SDL_GetError());
    
    SDL_GL_MakeCurrent(app->window, app->gl_context);
    SDL_GL_SetSwapInterval(1); 

    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress))
        logg_fexit(app->flog, 1, LOGG_ERROR, "Failed to initialize GLAD\n");
    
    glViewport(0, 0, app->window_width, app->window_height);

    app->ig_context = igCreateContext(NULL);
    igSetCurrentContext(app->ig_context);
    ImGuiIO* io     = igGetIO_ContextPtr(app->ig_context);
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    #ifdef IMGUI_HAS_DOCK
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    #endif
    ImGui_ImplSDL3_InitForOpenGL(app->window, app->gl_context);
    ImGui_ImplOpenGL3_Init("#version 460");
    igStyleColorsDark(NULL);

    SDL_ShowWindow(app->window);
}

static void _chess_close(void* app_data) {
    app_data_t* app = (app_data_t*)app_data;
    if (app->ig_context) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        igDestroyContext(NULL);
    } if (app->gl_context) SDL_GL_DestroyContext(app->gl_context);
    if (app->window) SDL_DestroyWindow(app->window);
    SDL_Quit(); fclose(app->flog); free(app);
}

static void _chess_run(void* app_data) {
    app_data_t* app  = (app_data_t*)app_data;
    ImGuiIO* io      = igGetIO_ContextPtr(app->ig_context);
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
            if (app->event.type == SDL_EVENT_WINDOW_RESIZED) {
                app->window_width  = app->event.window.data1;
                app->window_height = app->event.window.data2;
                glViewport(0, 0, app->event.window.data1, app->event.window.data2);
            }
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        igNewFrame();

        igShowDemoWindow(NULL);
        igShowStyleEditor(igGetStyle());

        igRender();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
        #ifdef IMGUI_HAS_DOCK
	    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            igUpdatePlatformWindows();
            igRenderPlatformWindowsDefault(NULL,NULL);
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }
        #endif
        SDL_GL_SwapWindow(app->window);

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