#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "image.h"
#include "../types.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

image_t* image_load(const char* filename) {
    image_t* image = malloc(sizeof(image_t));
    memset(image, 0, sizeof(image_t));

    image->data = stbi_load(filename, &image->width, &image->height, &image->channels, 0);
    if (image->data == NULL) {
        free(image);
        return NULL;
    }
    
    stbi__vertical_flip(image->data, image->width, image->height, image->channels);
    return image;
}

void image_destroy(image_t* image) {
    if (image) {
        if (image->data) stbi_image_free(image->data);
        free(image);
    }
}