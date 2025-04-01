#include "frame.h"
#include "../types.h"
#include "../logg.h"
#include "glad/glad.h"

void frame_create(frame_t* frame, u64 width, u64 height) {
    frame->width  = width;
    frame->height = height;

    const char *vs_source = "#version 460 core\n"
    "layout (location = 0) in vec2 attr_pos;\n"
    "layout (location = 1) in vec2 attr_uv;\n"
    "out vec2 uv_coord;\n"
    "void main() {\n"
    "   gl_Position = vec4(attr_pos.x, attr_pos.y, 0.0, 1.0);\n"
    "   uv_coord    = attr_uv;\n"
    "}";

    const char *fs_source = "#version 460 core\n"
    "out vec4 frag_color;\n"
    "in vec2 uv_coord;\n"
    "uniform sampler2D tid;\n"
    "void main() {\n"
    "   frag_color = texture(tid, uv_coord);\n"
    "}";

    shader_create(&frame->shader);
    shader_source(frame->shader, vs_source, fs_source);
    shader_uniform1i(frame->shader, "tid", 0);
    
    f32 vertices[] = {
        // pos         // uv
         1.0f,  1.0f,  1.0f, 1.0f,  // top right
         1.0f, -1.0f,  1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f,  0.0f, 0.0f,  // bottom left
        
         1.0f,  1.0f,  1.0f, 1.0f,  // top right
        -1.0f, -1.0f,  0.0f, 0.0f,  // bottom left
        -1.0f,  1.0f,  0.0f, 1.0f   // top left
    };

    attribute_t attributes[] = { {   
            .norm = GL_FALSE, .type = GL_FLOAT,
            .index = 0, .offset = 0 * sizeof(f32),
            .size = 2, .stride = 4 * sizeof(f32)
        }, {
            .norm = GL_FALSE, .type = GL_FLOAT,
            .index = 1, .offset = 2 * sizeof(f32),
            .size = 2, .stride = 4 * sizeof(f32)
        }
    };

    vao_create(&frame->vao); vao_bind(frame->vao);
    vbo_create(&frame->vbo); vbo_bind(frame->vbo);
    vbo_buffer(frame->vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);
    vao_attributes(attributes, sizeof(attributes) / sizeof(attribute_t));
    
    fbo_create(&frame->fbo); fbo_bind(frame->fbo);
    
    texture_create(&frame->texture); texture_bind(frame->texture);
    texture_param(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture_param(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture_image(NULL, frame->width, frame->height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, 0, false);
    
    fbo_texture(frame->fbo, frame->texture, GL_COLOR_ATTACHMENT0, 0);
    rbo_create(&frame->rbo); rbo_bind(frame->rbo);
    rbo_storage(frame->rbo, GL_DEPTH24_STENCIL8, frame->width, frame->height);
    fbo_rbo(frame->fbo, frame->rbo, GL_DEPTH_STENCIL_ATTACHMENT);
    fbo_bind(0);
}

void frame_destroy(frame_t* frame) {
    vao_destroy(&frame->vao);
    vbo_destroy(&frame->vbo);
    shader_destroy(&frame->shader);

    fbo_destroy(&frame->fbo);
    texture_destroy(&frame->texture);
    rbo_destroy(&frame->rbo);
}

void frame_resize(frame_t* frame, u64 width, u64 height) {
    frame->width  = width;
    frame->height = height;
    
    fbo_destroy(&frame->fbo);
    texture_destroy(&frame->texture);
    rbo_destroy(&frame->rbo);

    fbo_create(&frame->fbo); fbo_bind(frame->fbo);
    
    texture_create(&frame->texture); texture_bind(frame->texture);
    texture_param(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture_param(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture_image(NULL, frame->width, frame->height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, 0, false);
    
    fbo_texture(frame->fbo, frame->texture, GL_COLOR_ATTACHMENT0, 0);
    rbo_create(&frame->rbo); rbo_bind(frame->rbo);
    rbo_storage(frame->rbo, GL_DEPTH24_STENCIL8, frame->width, frame->height);
    fbo_rbo(frame->fbo, frame->rbo, GL_DEPTH_STENCIL_ATTACHMENT);
    fbo_bind(0);
}

void frame_begin(frame_t* frame) {
    fbo_bind(frame->fbo);
    glViewport(0, 0, frame->width, frame->height);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void frame_end(frame_t* frame) {
    fbo_bind(0);
}

void frame_draw(frame_t* frame) {
    shader_activate(frame->shader);
    texture_activate(frame->texture, 0);
    vao_bind(frame->vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}