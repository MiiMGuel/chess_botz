#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "main.h"
#include "chess.h"
#include "logg.h"
#include "types.h"
#include "gfx/gfx.h"
#include "SDL3/SDL.h"
#include "glad/glad.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#define CIMGUI_USE_SDL3
#include "cimgui/cimgui.h"
#include "cimgui/cimgui_impl.h"

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
    
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, app->window_width, app->window_height);

    app->ig_context = igCreateContext(NULL);
    igSetCurrentContext(app->ig_context);
    ImGuiIO* io     = igGetIO_ContextPtr(app->ig_context);
    ImFont* jmn = ImFontAtlas_AddFontFromFileTTF(io->Fonts, 
        "assets/JetBrainsMonoNerdFont-SemiBold.ttf", 32.0f, NULL, 
        ImFontAtlas_GetGlyphRangesDefault(io->Fonts)
    );
    io->FontGlobalScale = 0.6f;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    #ifdef IMGUI_HAS_DOCK
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    #endif
    ImGui_ImplSDL3_InitForOpenGL(app->window, app->gl_context);
    ImGui_ImplOpenGL3_Init("#version 460");
    igStyleColorsDark(NULL);

    ImGuiStyle* style = igGetStyle();
    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style->WindowBorderSize            = 0.0f;
        style->ChildBorderSize             = 0.0f;
        style->PopupBorderSize             = 0.0f;
        style->FrameBorderSize             = 0.0f;
        style->WindowRounding              = 4.0f;
        style->ChildRounding               = 4.0f;
        style->PopupRounding               = 4.0f;
        style->FrameRounding               = 4.0f;
        style->ScrollbarRounding           = 4.0f;
        style->GrabRounding                = 4.0f;
        style->TabBorderSize               = 0.0f;
        style->TabBarBorderSize            = 0.0f;
        style->TabBarOverlineSize          = 0.0f;
        style->TabRounding                 = 4.0f;
        style->DockingSeparatorSize        = 0.0f;
    }

    frame_create(&app->frame, app->window_width, app->window_height);

    if (fbo_check() != GL_FRAMEBUFFER_COMPLETE) {
        logg_fexit(app->flog, 1, LOGG_ERROR, "Failed to create framebuffer!\n");
    } fbo_bind(0);

    const char *vs_source = "#version 460 core\n"
    "layout (location = 0) in vec3 attr_pos;\n"
    "layout (location = 1) in vec2 attr_uv;\n"
    "out vec3 color;\n"
    "out vec2 uv_coord;\n"
    "void main() {\n"
    "   gl_Position = vec4(attr_pos.x, attr_pos.y, attr_pos.z, 1.0);\n"
    "   // color       = attr_color;\n"
    "   uv_coord    = attr_uv;\n"
    "}";

    const char *fs_source = "#version 460 core\n"
    "out vec4 frag_color;\n"
    "in vec3 color;\n"
    "in vec2 uv_coord;\n"
    "uniform sampler2D tid;\n"
    "void main() {\n"
    "   frag_color = texture(tid, uv_coord);\n"
    "}";

    shader_create(&app->shader);
    if (!shader_source(app->shader, vs_source, fs_source))
        logg_fprintf(
            app->flog, LOGG_ERROR, 
            "Failed to compile %s shader: %s\n", 
            shader_geterr_stype(), shader_geterr_msg()
        );
    
    shader_uniform1i(app->shader, "tid", 0);

    f32 vertices[] = {
        // pos               // uv
         0.5f,  0.5f,  0.0f,  1.0f, 1.0f,  // top right
         0.5f, -0.5f,  0.0f,  1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f,  0.0f,  0.0f, 0.0f,  // bottom left
        -0.5f,  0.5f,  0.0f,  0.0f, 1.0f   // top left
    };

    attribute_t attributes[] = { {   
            .norm = GL_FALSE, .type = GL_FLOAT,
            .index = 0, .offset = 0 * sizeof(f32),
            .size = 3, .stride = 5 * sizeof(f32)
        }, {
            .norm = GL_FALSE, .type = GL_FLOAT,
            .index = 1, .offset = 3 * sizeof(f32),
            .size = 2, .stride = 5 * sizeof(f32)
        }
    };

    u32 indices[] = {
       0, 1, 2,
       0, 2, 3
    }; 

    vao_create(&app->vao); vao_bind(app->vao);
    vbo_create(&app->vbo); vbo_bind(app->vbo);
    vbo_buffer(app->vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);
    ebo_create(&app->ebo); ebo_bind(app->ebo);
    ebo_buffer(app->ebo, sizeof(indices), indices, GL_STATIC_DRAW);
    vao_attributes(attributes, sizeof(attributes) / sizeof(attribute_t));

    texture_loadx(
        &app->texture, "assets/board/brown.bmp", 
        GL_REPEAT, GL_REPEAT, 
        GL_LINEAR, GL_LINEAR
    ); if (!app->texture) 
        logg_fprint(app->flog, LOGG_ERROR, "Failed to load \"assets/board/brown.bmp\"\n");

    SDL_ShowWindow(app->window);
}

static void _chess_close(void* app_data) {
    app_data_t* app = (app_data_t*)app_data;
    vbo_destroy(&app->vbo);
    ebo_destroy(&app->ebo);
    vao_destroy(&app->vao);
    shader_destroy(&app->shader);
    texture_destroy(&app->texture);
    frame_destroy(&app->frame);
    if (app->ig_context) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        igDestroyContext(NULL);
    } if (app->gl_context) SDL_GL_DestroyContext(app->gl_context);
    if (app->window) SDL_DestroyWindow(app->window);
    SDL_Quit(); fclose(app->flog); free(app);
}

static void _chess_run(void* app_data) {
    app_data_t* app   = (app_data_t*)app_data;
    ImGuiIO* io       = igGetIO_ContextPtr(app->ig_context);
    f64 freq          = (f64)SDL_GetPerformanceFrequency();
    f64 init          = (f64)SDL_GetPerformanceCounter() / freq;
    f64 curr_time     = 0.0;
    f64 prev_time     = 0.0;
    f64 delta_time    = 0.0;
    f64 cdelta_time   = 0.0;
    u64 curr_tick     = SDL_GetTicks();
    u64 prev_tick     = curr_tick;
    u64 delta_tick    = 0.0;
    u64 cdelta_tick   = 0.0;
    i64 fcount        = 0;
    i64 fps           = 0;

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
                frame_resize(&app->frame, app->window_width, app->window_height);
                glViewport(0, 0, app->event.window.data1, app->event.window.data2);
            }
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        frame_begin(&app->frame);

        shader_activate(app->shader);
        texture_activate(app->texture, 0);
        vao_bind(app->vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        frame_end(&app->frame);
        glDisable(GL_DEPTH_TEST);
        glClearColor(0.08f, 0.08f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        frame_draw(&app->frame);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        igNewFrame();

        // docking environment
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
        ImGuiViewport* viewport = igGetMainViewport();
        igSetNextWindowPos(viewport->Pos, 0, (ImVec2){0.0f, 0.0f});
        igSetNextWindowSize(viewport->Size, 0);
        igSetNextWindowViewport(viewport->ID);
        igPushStyleVar_Float(ImGuiStyleVar_WindowRounding, 0.0f);
        igPushStyleVar_Float(ImGuiStyleVar_WindowBorderSize, 0.0f);
        igPushStyleVar_Vec2(ImGuiStyleVar_WindowPadding, (ImVec2){0.0f, 0.0f});
        igBegin("InvisibleWindow", NULL, window_flags);
        igPopStyleVar(3);
        ImGuiID dockspace_id = igGetID_Str("InvisibleWindowDockSpace");
        igDockSpace(dockspace_id, (ImVec2){0.0f, 0.0f}, ImGuiDockNodeFlags_PassthruCentralNode, NULL);
        igEnd();

        igShowDemoWindow(NULL);

        igBegin("framebuffer", NULL, 0);
        ImVec2 frame_size; igGetContentRegionAvail(&frame_size);
        igImage(app->frame.texture, frame_size, (ImVec2){0, 1}, (ImVec2){1, 0});
        igEnd();

        igRender();
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