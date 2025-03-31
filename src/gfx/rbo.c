#include "rbo.h"
#include "../types.h"
#include "texture.h"
#include "glad/glad.h"

void rbo_create(rbo_t* rbo) {
    glGenRenderbuffers(1, rbo);
}

void rbo_destroy(rbo_t* rbo) {
    glDeleteRenderbuffers(1, rbo);
}

void rbo_bind(rbo_t rbo) {
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
}

void rbo_storage(rbo_t rbo, u32 format, u64 width, u64 height) {
    glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
}