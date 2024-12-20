#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>

#include "entry.h"
#include "raylib.h"

typedef struct state {
    unsigned int window_width;
    unsigned int window_height;
    const char* window_title;
    bool window_vsync;
} state_t;

void init(void* data) {
    state_t* state = (state_t*)data;
}

void event(void* data) {
    state_t* state = (state_t*)data;
}

void update(void* data) {
    state_t* state = (state_t*)data;
}

void render(void* data) {
    state_t* state = (state_t*)data;

    ClearBackground(RAYWHITE);
    DrawFPS(5, 5);
}

void close(void* data) {
    state_t* state = (state_t*)data;
}

entry_data_t entry_main(int argc, char* argv[]) {
    void* data = malloc(sizeof(state_t));
    memset(data, 0, sizeof(state_t));
    
    state_t* state = (state_t*)data;
    state->window_width = 640;
    state->window_height = 480;
    state->window_title = "game";
    state->window_vsync = true;

    if (state->window_vsync) SetWindowState(FLAG_VSYNC_HINT);
    return (entry_data_t){
        .width = state->window_width,
        .height = state->window_height,
        .title = state->window_title,
        .data = data,
        .init = init,
        .event = event,
        .update = update,
        .render = render,
        .close = close
    };
}