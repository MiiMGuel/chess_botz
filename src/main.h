#ifndef MAIN_H
#define MAIN_H

typedef struct app_info {
    void* app_data;
    void (*start)(void* app_data);
    void (*run)(void* app_data);
    void (*close)(void* app_data);
} app_info_t;

app_info_t main_app(int argc, char* argv[]); 

#endif // MAIN_H