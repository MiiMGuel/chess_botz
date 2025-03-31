#include "fbo.h"

void fbo_create(fbo_t* fbo) {
    glGenFramebuffers(1, fbo);
}

void fbo_destroy(fbo_t* fbo) {
    glDeleteFramebuffers(1, fbo);
}

void fbo_bind(fbo_t fbo) {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void fbo_rbo(fbo_t fbo, rbo_t rbo, u32 attachment) {
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbo);
}

void fbo_texture(fbo_t fbo, texture_t texture, u32 attachment, i32 level) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, level);
}

u32 fbo_check(void) {
    return glCheckFramebufferStatus(GL_FRAMEBUFFER);
}