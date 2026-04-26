#version 330 core

in vec3 interpNormal;

out vec3 fragColor;

uniform vec3 uLightDir;
uniform vec3 uLightColor;
uniform bool uShowNormals;

void main() {
    vec3 normal = normalize(interpNormal);
    vec3 lighting = uLightColor * max(dot(normal, uLightDir), 0.0);
    fragColor = uShowNormals ? normal * 0.5 + 0.5 : lighting;
}