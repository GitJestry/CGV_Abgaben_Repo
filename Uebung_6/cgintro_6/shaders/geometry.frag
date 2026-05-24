#version 330 core

in vec3 interpNormal;
in vec3 interpPosition;
in vec3 color;

layout (location = 0) out vec4 gColor;
layout (location = 1) out vec4 gNormal;

void main() {
    gColor.xyz = (mod(floor(interpPosition.y * 15.0), 2.0) * 0.5 + 0.5) * color;
    gNormal.xyz = normalize(interpNormal);
}