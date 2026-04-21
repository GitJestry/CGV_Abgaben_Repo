#version 330 core
layout (location = 0) in vec3 position;

uniform float uTime;

// Translation
mat4 mov;
// Scaling
mat4 sca;
// Rotation
mat4 rot;

void main() {
    // kreisförmige translation
    float kreisRadius = 0.6;
    float tx = kreisRadius * cos(uTime);
    float ty = kreisRadius * sin(uTime);

    // zoomintervall: [0,5, 0,75]
    float zoomSpeed = 3.0;
    float scaling = 0.5 + 0.25 * sin(zoomSpeed * uTime);

    float roationtSpeed = 3;
    float angle = uTime * roationtSpeed;

    // Translation = Verschiebung
    // nutzen die homogene form endlich aus
    mov = mat4( 1.0, 0.0, 0.0, tx,
                0.0, 1.0, 0.0, ty,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0 );

    // Skaliere x und y
    sca = mat4( scaling,   0.0, 0.0, 0.0,
                0.0, scaling,   0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0 );

    // rotation wie in der Vorlesung definiert
    rot = mat4( cos(angle), -sin(angle), 0.0, 0.0,
                sin(angle),  cos(angle), 0.0, 0.0,
                0.0,         0.0,        1.0, 0.0,
                0.0,         0.0,        0.0, 1.0 );

    gl_Position = transpose(mov) * transpose(sca) * transpose(rot) * vec4(position, 1.0);
}