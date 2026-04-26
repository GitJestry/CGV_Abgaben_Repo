#version 330 core

in vec3 interpColor;

out vec3 fragColor;

// see this approach:
// https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB_alternative

vec3 hsv2rgb(vec3 hsv) {
    float h = hsv.x;
    float s = hsv.y;
    float v = hsv.z;

    // times 6 due to h value being normalized
    float kr = mod(5.0 + h * 6.0, 6.0);
    float kg = mod(3.0 + h * 6.0, 6.0);
    float kb = mod(1.0 + h * 6.0, 6.0);

    // the max expression can be shortened into a clamp function
    // min(k, 4-k, 1)
    float r = v - v*s* max (0, min(min(kr, 4-kr), 1));
    float g = v - v*s* max (0, min(min(kg, 4-kg), 1));
    float b = v - v*s* max (0, min(min(kb, 4-kb), 1));
    return vec3(r, g, b);
}

void main() {
    fragColor = hsv2rgb(interpColor);
}