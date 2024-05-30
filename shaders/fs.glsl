#version 330 core
out vec4 fragColor;

in vec2 texcoord;

uniform sampler2D tex_color;

void main(){
    vec3 diff = texture(tex_color, texcoord).rgb;
    fragColor = vec4(diff, 1.0);
}