#version 330 core

in vec4 inPosition;

uniform mat4 Matrix;

void main(void) {
    gl_Position = Matrix * inPosition;
}
