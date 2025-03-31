#ifndef TEXTURE_H
#define TEXTURE_H

#include "../types.h"
#include "image.h"
#include "glad/glad.h"

typedef u32 texture_t;

void texture_create(texture_t* texture);
void texture_createx(texture_t* texture, u32 wrap_s, u32 wrap_t, u32 min, u32 mag);
void texture_load(texture_t* texture, const char* filename);
void texture_loadx(texture_t* texture, const char* filename, u32 wrap_s, u32 wrap_t, u32 min, u32 mag);
void texture_destroy(texture_t* texture);
void texture_bind(texture_t texture);
void texture_param(u32 name, i32 param);
void texture_image(
    const void* data, 
    u64 width, u64 height, 
    i32 intern_format, u32 format, 
    u32 type, i32 level, bool mipmap
);

#endif // TEXTURE_H