#include "vbo.h"
#include "../types.h"
#include "glad/glad.h"

void vbo_create(vbo_t* vbo) {
    glGenBuffers(1, vbo);
}

void vbo_destroy(vbo_t* vbo) {
    glDeleteBuffers(1, vbo);
}

void vbo_bind(vbo_t vbo) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void vbo_buffer(vbo_t vbo, u64 size, const void* vertices, u32 usage) {
    glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);
}