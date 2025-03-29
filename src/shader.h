#ifndef SHADER_H
#define SHADER_H

#include "types.h"
#include "glad/glad.h"

typedef struct shader {
    u32 program;
    u32 vshader;
    u32 fshader;
} shader_t;

void shader_create(shader_t* shader);
i32 shader_source(shader_t* shader, const char* vs_source, const char* fs_source);
void shader_activate(shader_t* shader);
void shader_destroy(shader_t* shader);
const char* shader_geterr_msg(void);
const char* shader_geterr_stype(void);
const char* shader_geterr_filename(void);

#endif // SHADER_H