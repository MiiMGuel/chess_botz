#ifndef SCENE_H
#define SCENE_H

typedef struct scene_info {
    void* game_data;
    void* scene_data;
    void (*start)(void* game_data, void* scene_data);
    void (*run)(void* game_data, void* scene_data);
    void (*close)(void* game_data, void* scene_data);
} scene_info_t;

#endif // SCENE_H