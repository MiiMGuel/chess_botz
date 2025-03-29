#include "shader.h"
#include "logg.h"

static char* _error_stypes[] = {
    "PROGRAM",
    "VERTEX",
    "FRAGMENT"
};
static char _error_log[512];
static char _error_filename[512];
static char _error_stype;

const char* shader_error(void);

void shader_create(shader_t* shader) {
    shader->program = glCreateProgram();
    shader->vshader = glCreateShader(GL_VERTEX_SHADER);
    shader->fshader = glCreateShader(GL_FRAGMENT_SHADER);
}

i32 shader_source(shader_t* shader, const char* vs_source, const char* fs_source) {
    glShaderSource(shader->vshader, 1, &vs_source, NULL);
    glCompileShader(shader->vshader); {
        int  success;
        glGetShaderiv(shader->vshader, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(shader->vshader, 512, NULL, _error_log);
            _error_stype = 1;
            return success;
        }
    }

    glShaderSource(shader->fshader, 1, &fs_source, NULL);
    glCompileShader(shader->fshader); {
        int  success;
        glGetShaderiv(shader->fshader, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(shader->fshader, 512, NULL, _error_log);
            _error_stype = 2;
            return success;
        }
    }

    glAttachShader(shader->program, shader->vshader);
    glAttachShader(shader->program, shader->fshader);
    glLinkProgram(shader->program); {
        int  success;
        glGetProgramiv(shader->program, GL_LINK_STATUS, &success);

        if(!success) {
            glGetProgramInfoLog(shader->program, 512, NULL, _error_log);
            _error_stype = 0;
            return success;
        }
    } return 0;
}

void shader_activate(shader_t* shader) {
    glUseProgram(shader->program);
}

void shader_destroy(shader_t* shader) {
    glDeleteShader(shader->vshader);
    glDeleteShader(shader->fshader);
    glDeleteProgram(shader->program);
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