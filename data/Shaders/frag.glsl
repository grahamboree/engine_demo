#version 330 core

uniform sampler2D mainTexture;

in vec2 TexCoord;
in vec4 VertColor;

out vec4 color;

void main() {
    color = VertColor * texture(mainTexture, TexCoord);
}