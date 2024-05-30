#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexcoord;

out vec2 texcoord;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
void main(){
    gl_Position = P * V * M * vec4(aPos, 1.0);
    texcoord = aTexcoord;
}