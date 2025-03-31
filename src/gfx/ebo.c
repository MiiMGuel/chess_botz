#include "ebo.h"
#include "../types.h"
#include "glad/glad.h"

void ebo_create(ebo_t* ebo) {
    glGenBuffers(1, ebo);
}

void ebo_destroy(ebo_t* ebo) {
    glDeleteBuffers(1, ebo);
}

void ebo_bind(ebo_t ebo) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void ebo_buffer(ebo_t ebo, u64 size, const void* indices, u32 usage) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, usage);
}
