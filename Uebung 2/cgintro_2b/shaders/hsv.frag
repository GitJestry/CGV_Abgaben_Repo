#version 330 core

in vec3 interpColor;

out vec3 fragColor;


vec3 hsv2rgb(vec3 hsv) {
    return vec3(0.0); // TODO
}


void main() {
    fragColor = hsv2rgb(interpColor);
}