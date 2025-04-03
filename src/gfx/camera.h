#ifndef CAMERA_H
#define CAMERA_H

#include "../types.h"
#include "cglm/cglm.h"

typedef enum camera_type {
    CAMERA_ORTHOGRAPHIC = 0,
    CAMERA_PERSPECTIVE  = 1
} camera_type_t;

typedef struct camera {
    mat4 view;
    mat4 proj;
    vec3 pos;
    vec3 front;
    vec3 up;
    f32 fov;
    f32 nearZ;
    f32 farZ;
    f32 width;
    f32 height;
    camera_type_t type;
} camera_t;

void camera_identify(
    camera_t* cam, camera_type_t type, 
    f32 fov, f32 nearZ, f32 farZ, f32 width, f32 height,
    vec3 pos, vec3 front, vec3 up
);
void camera_update(camera_t* cam);

#endif // CAMERA_H