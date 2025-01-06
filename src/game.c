#include <assert.h>
#include <memory.h>

#define ENTRY_IMPL
#include "entry.h"
#include "logg.h"
#include "types.h"
#include "SDL2/SDL.h"

typedef struct game_data {
    int argc;
    char** argv;
    bool running;
} game_data_t;

void start(void* app_data) {
    game_data_t* data = (game_data_t*)app_data;
    logg_print(LOGG_INFO, "App Started!");
}

void close(void* app_data) {
    game_data_t* data = (game_data_t*)app_data;
    free(data); logg_print(LOGG_INFO, "App Closed!");
}

void run(void* app_data) {
    game_data_t* data = (game_data_t*)app_data;
    logg_printf(LOGG_TRACE, "The Number of PI is %0.2f", 3.14);
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