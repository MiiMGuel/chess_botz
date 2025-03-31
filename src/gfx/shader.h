#ifndef SHADER_H
#define SHADER_H

#include "../types.h"

typedef u32 shader_t;

void shader_create(shader_t* shader);
void shader_destroy(shader_t* shader);
void shader_activate(shader_t shader);
i32 shader_source(shader_t shader, const char* vs_source, const char* fs_source);
const char* shader_geterr_msg(void);
const char* shader_geterr_stype(void);
const char* shader_geterr_filename(void);
void shader_uniform1i(shader_t shader, const char* location, i32 x);
void shader_uniform1f(shader_t shader, const char* location, f32 x);
void shader_uniform2f(shader_t shader, const char* location, f32 x, f32 y);
void shader_uniform3f(shader_t shader, const char* location, f32 x, f32 y, f32 z);
void shader_uniform4f(shader_t shader, const char* location, f32 x, f32 y, f32 z, f32 w);
void shader_uniform1fv(shader_t shader, const char* location, u64 count, f32* x);
void shader_uniform2fv(shader_t shader, const char* location, u64 count, f32* x);
void shader_uniform3fv(shader_t shader, const char* location, u64 count, f32* x);
void shader_uniform4fv(shader_t shader, const char* location, u64 count, f32* x);
void shader_uniform4mfv(shader_t shader, const char* location, u64 count, bool transpose, f32* mat);

#endif // SHADER_H