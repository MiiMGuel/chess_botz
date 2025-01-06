#include "entry.h"
#include "logg.h"

static app_info_t _entry_app = { 0 };

static void _entry_close(void) {
    _entry_app.close(_entry_app.app_data);
}

int main(int argc, char* argv[]) {
    _entry_app = entry_app(argc, argv);
    int close = (_entry_app.close != NULL) ? atexit(_entry_close) : 0;
    if (close != 0) 
        logg_print(LOGG_WARN, "atexit(_entry_close) failed! the program can't close properly");
    if (_entry_app.start != NULL) 
        _entry_app.start(_entry_app.app_data);
    if (_entry_app.run != NULL) 
        _entry_app.run(_entry_app.app_data);
    return 0;
}