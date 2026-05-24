#version 330 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec3 normal;

uniform mat4 uWorldToClip = mat4(1.0);
uniform mat4 uLocalToWorld = mat4(1.0);
uniform int uGridSize;

out vec3 interpNormal;
out vec3 interpPosition;
out vec3 color;

void main() {
    // TODO: Offset the instances based on gl_InstanceID and give them different colors.
    vec4 offset = vec4(0.0);
    color = vec3(0.5, 0.5, 0.5);

    gl_Position = uWorldToClip * (uLocalToWorld * vec4(position, 1.0) + offset);
    interpNormal = (uLocalToWorld * vec4(normal, 0.0)).xyz;
    interpPosition = position;
}