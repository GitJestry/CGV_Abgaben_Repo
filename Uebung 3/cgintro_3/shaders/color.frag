#version 330 core

in vec3 interpNormal;
in vec3 worldPosition;

out vec3 fragColor;

uniform bool uEnableLighting;
uniform vec3 uColor;

void main() {
    if (uEnableLighting) { 
        vec3 normal = normalize(interpNormal);
        vec3 lightDir = normalize(vec3(0.0) - worldPosition); // Sun is at the origin
        fragColor = max(dot(normal, lightDir), 0.0) * uColor;
    } else {
        fragColor = uColor;
    }
}