#version 330 core

layout(location = 0) in vec2 position;

out vec2 fragCoord;

void main() {
    gl_Position = vec4(position, 0.0, 1.0);
    fragCoord = position;
}