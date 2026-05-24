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
    int id = gl_InstanceID;

    int gridX = id % uGridSize;
    int gridZ = id / uGridSize;

    float spacing = 1.6;

    vec2 grid = vec2(gridX, gridZ);
    vec2 centeredGrid = grid - vec2(uGridSize - 1) * 0.5;

    vec4 offset = vec4(
        centeredGrid.x * spacing,
        0.0,
        centeredGrid.y * spacing,
        0.0
    );

    float diag = (centeredGrid.x + centeredGrid.y) / float(uGridSize - 1);
    float t = clamp(diag * 0.5 + 0.5, 0.0, 1.0);

    // farbverlauf wie im originalbild
    vec3 pink   = vec3(1.00, 0.18, 0.80);
    vec3 yellow = vec3(1.00, 0.88, 0.35);
    vec3 green  = vec3(0.35, 1.00, 0.65);

    vec3 baseColor = mix(pink, green, t);

    // etwas weniger gelb in der mitte
    float yellowAmount = 1.0 - abs(t - 0.5) / 0.14;
    yellowAmount = clamp(yellowAmount, 0.0, 1.0);
    yellowAmount *= 0.5;

    color = mix(baseColor, yellow, yellowAmount);

    vec4 worldPosition = uLocalToWorld * vec4(position, 1.0) + offset;

    gl_Position = uWorldToClip * worldPosition;

    interpNormal = normalize((uLocalToWorld * vec4(normal, 0.0)).xyz);
    interpPosition = worldPosition.xyz;
}