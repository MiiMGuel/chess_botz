#include <raylib.h>

int main(void) {
    InitWindow(640, 480, "game");
    SetTargetFPS(60);
    
    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawFPS(5, 5);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
