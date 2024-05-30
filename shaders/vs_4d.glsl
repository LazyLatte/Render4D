#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 vertexColor;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
void main(){
    gl_Position = P * V * vec4(aPos, 1.0);
    vertexColor = 0.5 * sin(aPos) + vec3(0.5);
}