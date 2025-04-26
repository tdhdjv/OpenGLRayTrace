#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec4 uvCoord;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;

void main() {
    Normal = normal.xyz;
    gl_Position = projection * view * model * vec4(position.xyz, 1.0);
}