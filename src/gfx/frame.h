#ifndef FRAME_H
#define FRAME_H

#include "../types.h"
#include "fbo.h"
#include "rbo.h"
#include "vao.h"
#include "vbo.h"
#include "texture.h"
#include "shader.h"

typedef struct frame {
    fbo_t     fbo;
    rbo_t     rbo;
    texture_t texture;
    vao_t     vao;
    vbo_t     vbo;
    shader_t  shader;
    u64       width;
    u64       height;
} frame_t;

void frame_create(frame_t* frame, u64 width, u64 height);
void frame_destroy(frame_t* frame);
void frame_resize(frame_t* frame, u64 width, u64 height);
void frame_begin(frame_t* frame);
void frame_end(frame_t* frame);
void frame_draw(frame_t* frame);

#endif // FRAME_H