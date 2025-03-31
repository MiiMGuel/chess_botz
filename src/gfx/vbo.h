#ifndef VBO_H
#define VBO_H

#include "../types.h"
#include "glad/glad.h"

typedef u32 vbo_t;

void vbo_create(vbo_t* vbo);
void vbo_destroy(vbo_t* vbo);
void vbo_bind(vbo_t vbo);
void vbo_buffer(vbo_t vbo, u64 size, const void* vertices, u32 usage);

#endif // VBO_H