#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <Windows.h>

#include "main.h"
#include "chess.h"
#include "logg.h"
#include "types.h"
#include "gfx/gfx.h"
#include "cglm/cglm.h"
#include "SDL3/SDL.h"
#include "glad/glad.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#define CIMGUI_USE_SDL3
#include "cimgui/cimgui.h"
#include "cimgui/cimgui_impl.h"
#include "cimfilebrowser.h"

static void _igCreateDockingEnv(void) {
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
}

static void _igStylize(ImGuiIO* io) {
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
        style->DockingSeparatorSize        = 2.5f;
    }

    style->Colors[ImGuiCol_WindowBg] = (ImVec4){ 0.1f, 0.1f, 0.1f, 1.0f };

    style->Colors[ImGuiCol_Header] = (ImVec4){ 0.2f, 0.2f, 0.2f, 1.0f };
    style->Colors[ImGuiCol_HeaderHovered] = (ImVec4){ 0.3f, 0.3f, 0.3f, 1.0f };
    style->Colors[ImGuiCol_HeaderActive] = (ImVec4){ 0.275f, 0.275f, 0.275f, 1.0f };

    style->Colors[ImGuiCol_Button] = (ImVec4){ 0.2f, 0.2f, 0.2f, 1.0f };
    style->Colors[ImGuiCol_ButtonHovered] = (ImVec4){ 0.3f, 0.3f, 0.3f, 1.0f };
    style->Colors[ImGuiCol_ButtonActive] = (ImVec4){ 0.15f, 0.15f, 0.15f, 1.0f };

    style->Colors[ImGuiCol_FrameBg] = (ImVec4){ 0.2f, 0.2f, 0.2f, 1.0f };
    style->Colors[ImGuiCol_FrameBgHovered] = (ImVec4){ 0.3f, 0.3f, 0.3f, 1.0f };
    style->Colors[ImGuiCol_FrameBgActive] = (ImVec4){ 0.15f, 0.15f, 0.15f, 1.0f };

    style->Colors[ImGuiCol_Tab] = (ImVec4){ 0.15f, 0.15f, 0.15f, 1.0f };
    style->Colors[ImGuiCol_TabHovered] = (ImVec4){ 0.38f, 0.38f, 0.38f, 1.0f };
    style->Colors[ImGuiCol_TabSelected] = (ImVec4){ 0.28f, 0.28f, 0.28f, 1.0f };
    style->Colors[ImGuiCol_TabSelectedOverline] = (ImVec4){ 0.38f, 0.38f, 0.38f, 1.0f };
    style->Colors[ImGuiCol_TabDimmed] = (ImVec4){ 0.15f, 0.15f, 0.15f, 1.0f };
    style->Colors[ImGuiCol_TabDimmedSelected] = (ImVec4){ 0.2f, 0.2f, 0.2f, 1.0f };
    style->Colors[ImGuiCol_TabDimmedSelectedOverline] = (ImVec4){ 0.2f, 0.2f, 0.2f, 1.0f };

    style->Colors[ImGuiCol_TitleBg] = (ImVec4){ 0.15f, 0.15f, 0.15f, 1.0f };
    style->Colors[ImGuiCol_TitleBgActive] = (ImVec4){ 0.15f, 0.15f, 0.15f, 1.0f };
    style->Colors[ImGuiCol_TitleBgCollapsed] = (ImVec4){ 0.15f, 0.15f, 0.15f, 1.0f };
    
    style->Colors[ImGuiCol_ResizeGrip] = (ImVec4){ 0.15f, 0.15f, 0.15f, 1.0f };
    style->Colors[ImGuiCol_ResizeGripActive] = (ImVec4){ 0.38f, 0.38f, 0.38f, 1.0f };
    style->Colors[ImGuiCol_ResizeGripHovered] = (ImVec4){ 0.28f, 0.28f, 0.28f, 1.0f };

    style->Colors[ImGuiCol_CheckMark] = (ImVec4){ 1.0f, 1.0f, 1.0f, 1.0f };
    style->Colors[ImGuiCol_SliderGrab] = (ImVec4){ 0.8f, 0.8f, 0.8f, 1.0f };
    style->Colors[ImGuiCol_SliderGrabActive] = (ImVec4){ 1.0f, 1.0f, 1.0f, 1.0f };

    style->Colors[ImGuiCol_SeparatorHovered] = (ImVec4){ 0.8f, 0.8f, 0.8f, 1.0f };
    style->Colors[ImGuiCol_SeparatorActive] = (ImVec4){ 1.0f, 1.0f, 1.0f, 1.0f };
}

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
    _igStylize(io);
    
    app->img_browser = ImFileBrowser_Create(ImFileBrowserFlags_EditPathString);
    ImFileBrowser_SetTitle(app->img_browser, "select image");
    char* filter[3] = {".bmp", ".png", ".jpg"};
    ImFileBrowser_SetTypeFilters(app->img_browser, filter, 3);
    ImFileBrowser_SetDirectory(app->img_browser, "./assets");

    frame_create(&app->frame, app->window_width, app->window_height);
    fbo_bind(app->frame.fbo);
    if (fbo_check() != GL_FRAMEBUFFER_COMPLETE) {
        logg_fexit(app->flog, 1, LOGG_ERROR, "Failed to create framebuffer!\n");
    } fbo_bind(0);

    shader_create(&app->shader);
    if (!shader_load(app->shader, "assets/shaders/default.vert", "assets/shaders/default.frag"))
        logg_fprintf(
            app->flog, LOGG_ERROR, 
            "Failed to compile %s shader: %s\n", 
            shader_geterr_filename(), shader_geterr_msg()
        );
    
    shader_uniform1i(app->shader, "tid", 0);

    f32 vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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

    vao_create(&app->vao); vao_bind(app->vao);
    vbo_create(&app->vbo); vbo_bind(app->vbo);
    vbo_buffer(app->vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // ebo_create(&app->ebo); ebo_bind(app->ebo);
    // ebo_buffer(app->ebo, sizeof(indices), indices, GL_STATIC_DRAW);
    vao_attributes(attributes, sizeof(attributes) / sizeof(attribute_t));

    texture_loadx(
        &app->texture, "assets/boards/brown.bmp", 
        GL_REPEAT, GL_REPEAT, 
        GL_LINEAR, GL_LINEAR
    ); if (!app->texture) 
        logg_fprint(app->flog, LOGG_ERROR, "Failed to load \"assets/boards/brown.bmp\"\n");

    SDL_ShowWindow(app->window);
}

static void _chess_close(void* app_data) {
    app_data_t* app = (app_data_t*)app_data;
    vbo_destroy(&app->vbo);
    // ebo_destroy(&app->ebo);
    vao_destroy(&app->vao);
    shader_destroy(&app->shader);
    texture_destroy(&app->texture);
    frame_destroy(&app->frame);
    ImFileBrowser_Destroy(app->img_browser);
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
    f64 curr_time     = (f64)SDL_GetTicksNS() / 1000000000.0f;
    f64 prev_time     = 0.0;
    f64 delta_time    = 0.0;
    f64 cdelta_time   = 0.0;
    i64 fcount        = 0;
    i64 fps           = 0;
    f32 spin_speed    = 1.0f;
    f32 cam_speed     = 1.0f;
    f32 cam_sens      = 0.1f;
    bool vsync        = true;
    bool show_poly    = false;
    bool cube_spin    = false;
    mat4 model; glm_mat4_identity(model);
    camera_t cam; camera_identify(
        &cam, 1, 75.0f, 
        0.1f, 100.0f, 
        (f32)app->frame.width, (f32)app->frame.height,
        (vec3){0.0f, 0.0f, 3.0f}, (vec3){0.0f, 0.0f, -1.0f}, (vec3){0.0f, 1.0f, 0.0f}
    );

    while (true) {
        curr_time    = (f64)SDL_GetTicksNS() / 1000000000.0f;
        delta_time   = curr_time - prev_time;
        cdelta_time += delta_time;

        while(SDL_PollEvent(&app->event)) {
            ImGui_ImplSDL3_ProcessEvent(&app->event);
            if (app->event.type == SDL_EVENT_QUIT) exit(0);
            if (app->event.type == SDL_EVENT_WINDOW_RESIZED &&
                app->event.window.windowID == SDL_GetWindowID(app->window)
            ) {
                app->window_width  = app->event.window.data1;
                app->window_height = app->event.window.data2;
                // frame_resize(&app->frame, app->window_width, app->window_height);
                // glViewport(0, 0, app->event.window.data1, app->event.window.data2);
            }
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        frame_begin(&app->frame);

        if (cube_spin) glm_rotate(model, (f32)spin_speed * delta_time, (vec3){0.5f, 1.0f, 0.0f});
        camera_update(&cam);

        shader_activate(app->shader);
        shader_uniform2D(app->shader, app->texture, "tid", 0);
        shader_uniform4mfv(app->shader, "model", 1, false, (f32*)model);
        shader_uniform4mfv(app->shader, "view", 1, false, (f32*)cam.view);
        shader_uniform4mfv(app->shader, "proj", 1, false, (f32*)cam.proj);
        vao_bind(app->vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        frame_end(&app->frame);

        glClearColor(0.08f, 0.08f, 0.08f, 1.0f);
        glViewport(0, 0, app->window_width, app->window_height);
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT);

        // frame_draw(&app->frame);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        igNewFrame();

        _igCreateDockingEnv();
        igShowDemoWindow(NULL);
        
        igPushStyleVar_Vec2(ImGuiStyleVar_WindowPadding, (ImVec2){0.0f, 0.0f});
        igBegin("framebuffer", NULL, 0); igPopStyleVar(1);
        u32           frame_id  = igGetItemID();
        static bool   first_tap = true;
        static ImVec2 prev_size = {0, 0};
        static ImVec2 curr_size = {0, 0}; igGetContentRegionAvail(&curr_size);
        static ImVec2 curr_pos  = {0, 0}; igGetWindowPos(&curr_pos);
        igImage(app->frame.texture, curr_size, (ImVec2){0,1}, (ImVec2){1,0});
        if (igIsWindowFocused(0) && igIsMouseDown_ID(ImGuiMouseButton_Right, frame_id)) {
            igSetMouseCursor(ImGuiMouseCursor_None);
            SDL_HideCursor();

            vec3 cam_vspeed = {
                cam_speed * delta_time, 
                cam_speed * delta_time, 
                cam_speed * delta_time
            }; if (igIsKeyDown_ID(ImGuiKey_W, frame_id)) {
                vec3 pos; glm_vec3_mul(cam_vspeed, cam.front, pos);
                glm_vec3_add(cam.pos, pos, cam.pos);
            } if (igIsKeyDown_ID(ImGuiKey_A, frame_id)) {
                vec3 cross; glm_cross(cam.front, cam.up, cross); glm_normalize(cross);
                vec3 pos; glm_vec3_mul(cam_vspeed, cross, pos);
                glm_vec3_sub(cam.pos, pos, cam.pos);
            } if (igIsKeyDown_ID(ImGuiKey_S, frame_id)) {
                vec3 pos; glm_vec3_mul(cam_vspeed, cam.front, pos);
                glm_vec3_sub(cam.pos, pos, cam.pos);
            } if (igIsKeyDown_ID(ImGuiKey_D, frame_id)) {
                vec3 cross; glm_cross(cam.front, cam.up, cross); glm_normalize(cross);
                vec3 pos; glm_vec3_mul(cam_vspeed, cross, pos);
                glm_vec3_add(cam.pos, pos, cam.pos);
            }

            static f32 yaw = -90.0f;
            static f32 pitch = 0.0f;
            static ImVec2 mouse_pos = {0.0f, 0.0f};
            static ImVec2 center_pos = {0.0f, 0.0f};

            if (first_tap) {
                SDL_WarpMouseGlobal(curr_pos.x + curr_size.x / 2, curr_pos.y + curr_size.y / 2);
                SDL_GetGlobalMouseState(&center_pos.x, &center_pos.y);
                first_tap = false;
            } SDL_GetGlobalMouseState(&mouse_pos.x, &mouse_pos.y);
            
            ImVec2 offset = {mouse_pos.x - center_pos.x, center_pos.y - mouse_pos.y};
            offset.x *= cam_sens;
            offset.y *= cam_sens;

            yaw   += offset.x;
            pitch += offset.y;

            if(pitch > 89.0f)
                pitch = 89.0f;
            if(pitch < -89.0f)
                pitch = -89.0f;
            
            vec3 dir = {
                cos(glm_rad(yaw)) * cos(glm_rad(pitch)),
                sin(glm_rad(pitch)),
                sin(glm_rad(yaw)) * cos(glm_rad(pitch))
            }; glm_normalize(dir); glm_vec3_copy(dir, cam.front);

            SDL_WarpMouseGlobal(curr_pos.x + curr_size.x / 2, curr_pos.y + curr_size.y / 2);
        } else {
            SDL_ShowCursor();
            first_tap = true;
        } igEnd();

        igBegin("Performance", NULL, 0);
        igText("time  : %.3f", curr_time);
        igText("delta : %.3f", delta_time);
        igText("fps   : %d", fps);
        igEnd();

        igBegin("Render", NULL, 0);
        if (igCheckbox("enable vsync", &vsync))
            SDL_GL_SetSwapInterval(vsync);
        if (igCheckbox("polygon line", &show_poly))
            if (show_poly) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        igEnd();

        igBegin("Cube", NULL, 0);
        igInputFloat("speed", &spin_speed, 0.1f, 0.5f, "%.3f", 0);
        if (igCheckbox("spin", &cube_spin))
            glm_mat4_identity(model);
        if (igButton("select image", (ImVec2){.x = 0, .y = 0}))
            ImFileBrowser_Open(app->img_browser);
        igEnd();

        igBegin("Camera", NULL, 0);
        igInputFloat("FOV", &cam.fov, 0.1f, 0.5f, "%.3f", 0);
        igInputFloat("near", &cam.nearZ, 0.1f, 0.5f, "%.3f", 0);
        igInputFloat("far", &cam.farZ, 0.1f, 0.5f, "%.3f", 0);
        igInputFloat("speed", &cam_speed, 0.1f, 0.5f, "%.3f", 0);
        igInputFloat("sens", &cam_sens, 0.1f, 0.5f, "%.3f", 0);
        igSeparatorText("Position");
        igInputFloat("pos.x", &cam.pos[0], 0.1f, 0.5f, "%.3f", 0);
        igInputFloat("pos.y", &cam.pos[1], 0.1f, 0.5f, "%.3f", 0);
        igInputFloat("pos.z", &cam.pos[2], 0.1f, 0.5f, "%.3f", 0);
        igSeparatorText("Front");
        igInputFloat("front.x", &cam.front[0], 0.1f, 0.5f, "%.3f", 0);
        igInputFloat("front.y", &cam.front[1], 0.1f, 0.5f, "%.3f", 0);
        igInputFloat("front.z", &cam.front[2], 0.1f, 0.5f, "%.3f", 0);
        igSeparatorText("Up");
        igInputFloat("up.x", &cam.up[0], 0.1f, 0.5f, "%.3f", 0);
        igInputFloat("up.y", &cam.up[1], 0.1f, 0.5f, "%.3f", 0);
        igInputFloat("up.z", &cam.up[2], 0.1f, 0.5f, "%.3f", 0);
        igEnd();

        if (ImFileBrowser_HasSelected(app->img_browser)){
            logg_fprintf(app->flog, LOGG_INFO, "file %s was selected!\n", ImFileBrowser_GetSelected(app->img_browser));
            texture_destroy(&app->texture);
            texture_loadx(&app->texture, ImFileBrowser_GetSelected(app->img_browser), GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
            if (!app->texture)
                logg_fprintf(app->flog, LOGG_ERROR, "Failed to load \"%s\"\n", ImFileBrowser_GetSelected(app->img_browser));
            ImFileBrowser_ClearSelected(app->img_browser);
        }

        ImFileBrowser_Display(app->img_browser);
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
        
        if (curr_size.x != prev_size.x || curr_size.y != prev_size.y) {
            prev_size = curr_size; 
            cam.width = curr_size.x; 
            cam.height = curr_size.y;
            frame_resize(&app->frame, curr_size.x, curr_size.y);
        } SDL_GL_SwapWindow(app->window);

        fcount++;
        if (cdelta_time >= 1.0) {
            fps         = fcount;
            fcount      = 0;
            cdelta_time = 0.0;
        } prev_time = curr_time;
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