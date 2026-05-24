#version 330 core

in vec2 texCoord;

uniform sampler2D tColor;
uniform sampler2D tNormal;
uniform sampler2D tDepth;

uniform mat4 uClipToWorld = mat4(1.0);
uniform vec2 uResolution = vec2(800, 600);
uniform uint uDebugView = 0U;
uniform float uBrightness = 1.0;

uniform int uLightCount;
uniform vec3 uLightPositions[256];
uniform vec3 uLightColors[256];

out vec3 fragColor;

void main() {
    vec3 color = texture(tColor, texCoord).rgb;
    vec3 normal = texture(tNormal, texCoord).xyz;
    float depth = texture(tDepth, texCoord).r;
    // TODO: Use texCoord or gl_FragCoord and depth to calculate the fragment position in clip space
    vec4 clipPos = vec4(0.0, 0.0, 0.0, 1.0);
    // TODO: Obtain the world space postion of the fragment
    vec4 worldPosition = vec4(0.0, 0.0, 0.0, 1.0);
    // NOTE: Divide worldPosition by w to get the correct position in world space

    vec3 lighting = vec3(0.0);
    if (depth < 1.0) {
        for (int i = 0; i < uLightCount; i++) {
            vec3 lightPos = uLightPositions[i];
            vec3 lightColor = uLightColors[i];

            // Calculate lighting
            vec3 lightDir = lightPos - worldPosition.xyz;
            float lightDist = length(lightDir);
            lightDir /= lightDist; // Normalize light direction
            lightColor /= lightDist * lightDist; // Light falloff
            lighting += max(dot(normal, lightDir), 0.0) * lightColor;
        }
        lighting *= uBrightness;
        fragColor = lighting * color;
    } else {
        fragColor = vec3(0.0);
    }

    switch (uDebugView) {
        case 1U: fragColor = color; break;
        case 2U: fragColor = normal * 0.5 + 0.5; break;
        case 3U: fragColor = vec3(1.0 - depth); break;
        case 4U: fragColor = clipPos.xyz * 0.5 + 0.5; break;
        case 5U: fragColor = worldPosition.xyz; break;
        case 6U: fragColor = lighting; break;
    }
}