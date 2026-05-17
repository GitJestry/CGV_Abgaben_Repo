#version 330 core

in vec3 interpNormal;

out vec3 fragColor;

uniform vec3 uLightDir = normalize(vec3(1.0));
uniform vec3 uColor = vec3(1.0);

void main() {
    vec3 normal = normalize(interpNormal);
    vec3 lighting = uColor * max(dot(normal, uLightDir), 0.0);
    fragColor = lighting;
}