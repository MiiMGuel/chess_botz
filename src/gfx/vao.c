#include "vao.h"
#include "glad/glad.h"

void vao_create(vao_t* vao) {
    glGenVertexArrays(1, vao);
}

void vao_destroy(vao_t* vao) {
    glDeleteVertexArrays(1, vao);
}

void vao_bind(vao_t vao) {
    glBindVertexArray(vao);
}

void vao_attribute(u32 index, i32 size, u64 stride, u64 offset, u32 type, u8 norm) {
    glVertexAttribPointer(
        index, size, 
        type, norm, 
        stride, (void*)offset
    ); glEnableVertexAttribArray(index);
}

void vao_attributes(attribute_t* attributes, u32 count) {
    for (u32 i = 0; i < count; i++) {
        glVertexAttribPointer(
            attributes[i].index, attributes[i].size, 
            attributes[i].type, attributes[i].norm, 
            attributes[i].stride, (void*)attributes[i].offset
        ); glEnableVertexAttribArray(attributes[i].index);
    }
}