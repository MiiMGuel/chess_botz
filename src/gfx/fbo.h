#ifndef FBO_H
#define FBO_H

#include "../types.h"
#include "texture.h"
#include "rbo.h"
#include "glad/glad.h"

typedef u32 fbo_t;

void fbo_create(fbo_t* fbo);
void fbo_destroy(fbo_t* fbo);
void fbo_bind(fbo_t fbo);
void fbo_rbo(fbo_t fbo, rbo_t rbo, u32 attachment);
void fbo_texture(fbo_t fbo, texture_t texture, u32 attachment, i32 level);
u32 fbo_check(void);

#endif // FBO_H