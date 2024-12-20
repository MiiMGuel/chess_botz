#include <stdlib.h>

#include "entry.h"
#include "raylib.h"

entry_data_t entry_main(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    entry_data_t entry = entry_main(argc, argv);

    InitWindow(entry.width, entry.height, entry.title);
    SetExitKey(KEY_NULL);
    entry.init(entry.data);

    while (!WindowShouldClose()) {
        entry.event(entry.data);
        entry.update(entry.data);
        BeginDrawing();
        entry.render(entry.data);
        EndDrawing();
    }

    entry.close(entry.data);
    if (entry.data) free(entry.data);
    CloseWindow();
    return 0;
}
