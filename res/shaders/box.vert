#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 minPos;
layout(location = 2) in vec3 maxPos;

uniform mat4 view;
uniform mat4 projection;

void main() {
    vec3 Pos = (maxPos - minPos) * position + (minPos + maxPos)*0.5f;
    gl_Position = projection * view * vec4(Pos, 1.0);
}