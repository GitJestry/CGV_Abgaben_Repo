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
    vec3 normal = normalize(texture(tNormal, texCoord).xyz);
    float depth = texture(tDepth, texCoord).r;

    vec3 ndcPos = vec3(
        texCoord * 2.0 - 1.0,
        depth * 2.0 - 1.0
    );

    vec4 clipPos = vec4(ndcPos, 1.0);

    vec4 worldPosition = uClipToWorld * clipPos;
    worldPosition /= worldPosition.w;

    vec3 lighting = vec3(0.0);

    if (depth < 1.0) {
        for (int i = 0; i < uLightCount; i++) {
            vec3 lightPos = uLightPositions[i];
            vec3 lightColor = uLightColors[i];

            vec3 lightDir = lightPos - worldPosition.xyz;
            float lightDist = length(lightDir);
            lightDir /= lightDist;

            lightColor /= lightDist * lightDist;

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