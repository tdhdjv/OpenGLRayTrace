#version 330 core

uniform vec3 color;
uniform vec3 lightDir = vec3(0.0, -1.0, 0.0); 

in vec3 Normal;

out vec4 FragColor;

void main() {
    FragColor = vec4(Normal, 1.0);//vec4(vec3(max(dot(Normal, -lightDir), 0.0)), 1.0);
}