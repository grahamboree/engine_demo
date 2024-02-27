#version 330 core

uniform mat4 MVP;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 norm;
layout(location = 3) in vec4 color;

out vec2 TexCoord;
out vec4 VertColor;

void main() {
    VertColor = color;
    TexCoord = uv;
    gl_Position = MVP * vec4(pos, 1.0);
}