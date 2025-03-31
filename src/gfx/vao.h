#ifndef VAO_H
#define VAO_H

#include "../types.h"

typedef u32 vao_t;

typedef struct attribute {
    u8  norm;
    u32 type;
    u32 index;
    u64 offset;
    i32 size;
    u64 stride;
} attribute_t;

void vao_create(vao_t* vao);
void vao_destroy(vao_t* vao);
void vao_bind(vao_t vao);
void vao_attribute(u32 index, i32 size, u64 stride, u64 offset, u32 type, u8 norm);
void vao_attributes(attribute_t* attributes, u32 count);

#endif // VAO_H