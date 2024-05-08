#version 330 core
layout (location = 0) in vec3 pos;
out vec3 vertexColor;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
void main(){
    gl_Position = P * V * M * vec4(pos, 1.0);
    vertexColor = pos.xyz*0.25 + vec3(0.75);
}