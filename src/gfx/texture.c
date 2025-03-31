#include "texture.h"
#include "../types.h"
#include "glad/glad.h"

void texture_create(texture_t* texture) {
    glGenTextures(1, texture);
}

void texture_createx(texture_t* texture, u32 wrap_s, u32 wrap_t, u32 min, u32 mag) {
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_load(texture_t* texture, const char* filename) {
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

    image_t* image = image_load(filename);
    switch (image->channels) {
    case 3:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
        glGenerateMipmap(GL_TEXTURE_2D);
        break;
    
    case 4:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
        glGenerateMipmap(GL_TEXTURE_2D);
        break;
    
    default:
        break;
    } glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_loadx(texture_t* texture, const char* filename, u32 wrap_s, u32 wrap_t, u32 min, u32 mag) {
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);

    image_t* image = image_load(filename);
    if (!image || !image->data) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, texture);
        return;
    } switch (image->channels) {
        case 3:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
            glGenerateMipmap(GL_TEXTURE_2D);
            break;
    
        case 4:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
            glGenerateMipmap(GL_TEXTURE_2D);
            break;
    
        default:
            break;
    } 

    glBindTexture(GL_TEXTURE_2D, 0);
    image_destroy(image);
}

void texture_destroy(texture_t* texture) {
    glDeleteTextures(1, texture);
}

void texture_bind(texture_t texture) {
    glBindTexture(GL_TEXTURE_2D, texture);
}

void texture_param(u32 name, i32 param) {
    glTexParameteri(GL_TEXTURE_2D, name, param);
}
void texture_image(
    const void* data, 
    u64 width, u64 height, 
    i32 intern_format, u32 format, 
    u32 type, i32 level, bool mipmap
) {
    glTexImage2D(GL_TEXTURE_2D, level, intern_format, width, height, 0, format, type, data);
    if (mipmap) glGenerateMipmap(GL_TEXTURE_2D);
}