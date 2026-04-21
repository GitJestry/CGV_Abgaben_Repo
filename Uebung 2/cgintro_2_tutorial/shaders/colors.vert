#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 interpColor;

uniform float uTime;

vec3 animateColor(vec3 color, float time) {
    float lerp = 0.5 * (sin(time) + 1.0);
    return mix(color.rgb, color.gbr, lerp);
}

void main() {
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
    interpColor = animateColor(color, uTime);
}
