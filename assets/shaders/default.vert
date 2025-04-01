#version 460 core

layout (location = 0) in vec3 attr_pos;
layout (location = 1) in vec2 attr_uv;

out vec2 uv_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
   gl_Position = proj * view * model * vec4(attr_pos, 1.0);
   uv_coord    = attr_uv;
}