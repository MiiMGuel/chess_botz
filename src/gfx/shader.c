#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shader.h"
#include "../logg.h"
#include "glad/glad.h"

static char* _error_stypes[] = {
    "PROGRAM",
    "VERTEX",
    "FRAGMENT",
};
static char _error_log[512];
static char _error_filename[512];
static char _error_stype;

static char* _file_getstr(const char* filename) {
    size_t size = 0;
    char* str   = NULL;
    FILE* file  = fopen(filename, "rb");
    if (!file) return NULL;
    
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (size < 0) {
        fclose(file);
        return NULL;
    }

    str          = malloc(size + 1);
    size_t rsize = fread(str, 1, size, file);
    if (rsize != size) {
        free(str);
        fclose(file);
        return NULL;
    } 
    
    str[size] = '\0';
    fclose(file);
    return str;
}

void shader_create(shader_t* shader) {
    *shader = glCreateProgram();
}

void shader_destroy(shader_t* shader) {
    glDeleteProgram(*shader);
    *shader = GL_INVALID_VALUE;
}

void shader_activate(shader_t shader) {
    glUseProgram(shader);
}

i32 shader_source(shader_t shader, const char* vs_source, const char* fs_source) {
    i32 success;

    u32 vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, &vs_source, NULL);
    glCompileShader(vshader);
    glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);
    if (!success) {
        _error_stype = 0;
        glGetShaderInfoLog(vshader, 512, NULL, _error_log);
        glDeleteShader(vshader);
        return success;
    }

    u32 fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, &fs_source, NULL);
    glCompileShader(fshader);
    glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);
    if (!success) {
        _error_stype = 2;
        glGetShaderInfoLog(fshader, 512, NULL, _error_log);
        glDeleteShader(fshader);
        return success;
    }

    glAttachShader(shader, vshader);
    glAttachShader(shader, fshader);
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if(!success) {
        _error_stype = 0;
        glGetProgramInfoLog(shader, 512, NULL, _error_log);
        glDeleteShader(vshader);
        glDeleteShader(fshader);
        return success;
    }
    
    glDeleteShader(vshader);
    glDeleteShader(fshader);
    return success;
}

i32 shader_load(shader_t shader, const char* vs_filename, const char* fs_filename) {
    i32 success;
    char* vs_source = _file_getstr(vs_filename);
    char* fs_source = _file_getstr(fs_filename);

    if (vs_source == NULL) {
        strcpy(_error_log, "file not found!");
        strcpy(_error_filename, vs_filename);
        return 0;
    } if (fs_source == NULL) {
        strcpy(_error_log, "file not found!");
        strcpy(_error_filename, fs_filename);
        free(vs_source);
        return 0;
    }

    u32 vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, &vs_source, NULL);
    glCompileShader(vshader);
    glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);
    if (!success) {
        _error_stype = 0;
        glGetShaderInfoLog(vshader, 512, NULL, _error_log);
        glDeleteShader(vshader);
        strcpy(_error_filename, vs_filename);
        free(vs_source);
        free(fs_source);
        return success;
    }

    u32 fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, &fs_source, NULL);
    glCompileShader(fshader);
    glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);
    if (!success) {
        _error_stype = 2;
        glGetShaderInfoLog(fshader, 512, NULL, _error_log);
        glDeleteShader(fshader);
        strcpy(_error_filename, fs_filename);
        free(vs_source);
        free(fs_source);
        return success;
    }

    glAttachShader(shader, vshader);
    glAttachShader(shader, fshader);
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if(!success) {
        _error_stype = 0;
        glGetProgramInfoLog(shader, 512, NULL, _error_log);
        glDeleteShader(vshader);
        glDeleteShader(fshader);
        free(vs_source);
        free(fs_source);
        return success;
    }
    
    glDeleteShader(vshader);
    glDeleteShader(fshader);
    free(vs_source);
    free(fs_source);
    return success;
}

const char* shader_geterr_msg(void) {
    return _error_log;
}

const char* shader_geterr_stype(void) {
    return _error_stypes[_error_stype];
}

const char* shader_geterr_filename(void) {
    return _error_filename;
}

void shader_uniform2D(shader_t shader, texture_t texture, const char* location, u32 id) {
    texture_activate(texture, id);
    glUniform1f(glGetUniformLocation(shader, location), id);
}

void shader_uniform1i(shader_t shader, const char* location, i32 x) {
    glUniform1f(glGetUniformLocation(shader, location), x);
}

void shader_uniform1f(shader_t shader, const char* location, f32 x) {
    glUniform1f(glGetUniformLocation(shader, location), x);
}

void shader_uniform2f(shader_t shader, const char* location, f32 x, f32 y) {
    glUniform2f(glGetUniformLocation(shader, location), x, y);
}

void shader_uniform3f(shader_t shader, const char* location, f32 x, f32 y, f32 z) {
    glUniform3f(glGetUniformLocation(shader, location), x, y, z);
}

void shader_uniform4f(shader_t shader, const char* location, f32 x, f32 y, f32 z, f32 w) {
    glUniform4f(glGetUniformLocation(shader, location), x, y, z, w);
}

void shader_uniform1fv(shader_t shader, const char* location, u64 count, f32* x) {
    glUniform1fv(glGetUniformLocation(shader, location), count, x);
}

void shader_uniform2fv(shader_t shader, const char* location, u64 count, f32* x) {
    glUniform2fv(glGetUniformLocation(shader, location), count, x);
}

void shader_uniform3fv(shader_t shader, const char* location, u64 count, f32* x) {
    glUniform3fv(glGetUniformLocation(shader, location), count, x);
}

void shader_uniform4fv(shader_t shader, const char* location, u64 count, f32* x) {
    glUniform4fv(glGetUniformLocation(shader, location), count, x);
}

void shader_uniform4mfv(shader_t shader, const char* location, u64 count, bool transpose, f32* mat) {
    glUniformMatrix4fv(glGetUniformLocation(shader, location), count, transpose, mat);
}