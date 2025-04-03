#include "camera.h"

void camera_identify(
    camera_t* cam, camera_type_t type, 
    f32 fov, f32 nearZ, f32 farZ, f32 width, f32 height,
    vec3 pos, vec3 front, vec3 up
) {
    glm_mat4_identity(cam->view);
    glm_mat4_identity(cam->proj);
    glm_vec3_copy(pos, cam->pos);
    glm_vec3_copy(front, cam->front);
    glm_vec3_copy(up, cam->up);
    cam->fov = fov;
    cam->nearZ = nearZ;
    cam->farZ = farZ;
    cam->width = width;
    cam->height = height;
    cam->type = type;

    camera_update(cam);
}

void camera_update(camera_t* cam) {
    vec3 center; glm_vec3_add(cam->pos, cam->front, center);
    glm_lookat_rh(cam->pos, center, cam->up, cam->view);
    
    if (cam->type) glm_perspective(glm_rad(cam->fov), cam->width / cam->height, cam->nearZ, cam->farZ, cam->proj);
    else glm_ortho(cam->width / -2, cam->width / 2, cam->height / -2, cam->height / 2, cam->nearZ, cam->farZ, cam->proj);
}