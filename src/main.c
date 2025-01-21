#include <windows.h>

#include "main.h"
#include "logg.h"

static app_info_t _main_app = { 0 };

static void _main_close(void) {
    _main_app.close(_main_app.app_data);
}

int WINAPI WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    LPSTR lpCmdLine, 
    int nCmdShow
) {
    _main_app = main_app(__argc, __argv);
    int close = (_main_app.close != NULL) ? atexit(_main_close) : 0;
    if (close != 0)
        MessageBox(NULL, "\"atexit(_main_close)\" failed! the program can't close properly.", "Error", MB_OK);
    if (_main_app.start != NULL) 
        _main_app.start(_main_app.app_data);
    if (_main_app.run != NULL) 
        _main_app.run(_main_app.app_data);
    return 0;
}