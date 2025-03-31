#ifndef IMAGE_H
#define IMAGE_H

#include "../types.h"

typedef struct image {
    u32 width;
    u32 height;
    u32 channels;
    u8* data;
} image_t;

image_t* image_load(const char* filename);
void image_destroy(image_t* image);

#endif // IMAGE_H