#ifndef ENTRY_H
#define ENTRY_H

typedef struct app_info {
    void* app_data;
    void (*start)(void* app_data);
    void (*run)(void* app_data);
    void (*close)(void* app_data);
} app_info_t;

app_info_t entry_app(int argc, char* argv[]); 

#endif // ENTRY_H