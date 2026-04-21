#version 330 core

in vec3 interpColor;

out vec3 fragColor;

void main() {
    fragColor = interpColor;
}
