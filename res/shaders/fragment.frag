#version 330 core

in vec2 fragCoord;
out vec4 fragColor;

void main() {
    fragColor = vec4(fragCoord, 0.0, 1.0);
}
