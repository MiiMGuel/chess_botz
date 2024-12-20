#pragma once

#ifdef __cplusplus 
extern "C" {
#endif

typedef struct entry_data {
    unsigned int width;
    unsigned int height;
    const char* title;
    void* data;
    void (*init)(void* data);
    void (*event)(void* data);
    void (*update)(void* data);
    void (*render)(void* data);
    void (*close)(void* data);
} entry_data_t;

#ifdef __cplusplus 
}
#endif