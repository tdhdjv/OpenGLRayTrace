#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uvCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 Normal;

void main() {
    Normal = normal.xyz;
    gl_Position = projection * view * model * vec4(position.xyz, 1.0);
}