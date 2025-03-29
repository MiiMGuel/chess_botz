#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "main.h"
#include "chess.h"
#include "logg.h"
#include "types.h"
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
        style->Colors[ImGuiCol_WindowBg].w = 0.8f;
    }

    SDL_ShowWindow(app->window);
}

static void _chess_close(void* app_data) {
    app_data_t* app = (app_data_t*)app_data;
    glDeleteVertexArrays(1, &app->vao);
    glDeleteBuffers(1, &app->vbo);
    glDeleteBuffers(1, &app->ebo);
    glDeleteProgram(app->shader);
    glDeleteShader(app->vshader);
    glDeleteShader(app->fshader);
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

    f32 vertices[] = {
        // pos               // color
         0.0f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f,  // top 
         0.5f, -0.5f,  0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 0.0f   // bottom left
    };

    u32 indices[] = {
       0, 1, 2,
    }; 

    const char *vshader_source = "#version 460 core\n"
    "layout (location = 0) in vec3 attr_pos;\n"
    "layout (location = 1) in vec3 attr_color;\n"
    "out vec3 color;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(attr_pos.x, attr_pos.y, attr_pos.z, 1.0);\n"
    "   color = attr_color;"
    "}\0";

    const char *fshader_source = "#version 460 core\n"
    "out vec4 frag_color;\n"
    "in vec3 color;\n"
    "void main()\n"
    "{\n"
    "   frag_color = vec4(color, 1.0f);\n"
    "}\0";

    app->vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(app->vshader, 1, &vshader_source, NULL);
    glCompileShader(app->vshader); {
        int  success;
        char info_log[512];
        glGetShaderiv(app->vshader, GL_COMPILE_STATUS, &success);

        if(!success) {
            glGetShaderInfoLog(app->vshader, 512, NULL, info_log);
            logg_fprintf(app->flog, LOGG_ERROR, "Failed compile vshader: %s\n", info_log);
        }
    }

    app->fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(app->fshader, 1, &fshader_source, NULL);
    glCompileShader(app->fshader); {
        int  success;
        char info_log[512];
        glGetShaderiv(app->fshader, GL_COMPILE_STATUS, &success);

        if(!success) {
            glGetShaderInfoLog(app->fshader, 512, NULL, info_log);
            logg_fprintf(app->flog, LOGG_ERROR, "Failed compile fshader: %s\n", info_log);
        }
    }

    app->shader = glCreateProgram();
    glAttachShader(app->shader, app->vshader);
    glAttachShader(app->shader, app->fshader);
    glLinkProgram(app->shader); {
        int  success;
        char info_log[512];
        glGetProgramiv(app->shader, GL_LINK_STATUS, &success);

        if(!success) {
            glGetProgramInfoLog(app->shader, 512, NULL, info_log);
            logg_fprintf(app->flog, LOGG_ERROR, "Failed compile shader: %s\n", info_log);
        }
    } glUseProgram(app->shader);

    glGenVertexArrays(1, &app->vao);
    glBindVertexArray(app->vao);

    glGenBuffers(1, &app->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, app->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &app->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

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

        igRender();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(app->shader);
        glBindVertexArray(app->vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app->ebo);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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