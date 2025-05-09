#version 330 core

uniform vec3 color = vec3(1.0, 0.0, 0.0);
uniform float alpha = 0.5;

out vec4 fragColor;

void main() {
    fragColor = vec4(color, alpha);
}
