#version 330 core

in vec4 exColor;
out vec4 outColor;

uniform vec4 Color;

void main(void) {
    outColor = Color;
}
