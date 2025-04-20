#version 330 core

in vec2 fragCoord;

out vec4 fragColor;

uniform sampler2D image;

void main() {
    fragColor = texture2D(image, 0.5*(fragCoord+1.0));
}