#version 460 core

out vec4 frag_color;

in vec2 uv_coord;

uniform sampler2D tid;

void main() {
   frag_color = texture(tid, uv_coord);
}