#version 330 core

in vec3 viewDir;
out vec3 fragColor;

uniform uint uView = 0u;
uniform vec3 uCameraPosition;
uniform float uFocalLength;
uniform float uTime;
uniform vec3 uLightDir = normalize(vec3(1.0));
uniform vec3 uLightColor = vec3(1.0);
uniform vec3 uSkyColor = vec3(0.1, 0.3, 0.6);
uniform float uAmbient = 0.3;
uniform float uNear = 0.01;
uniform float uFar = 100.0;
uniform int uSteps = 100;
uniform float uEpsilon = 0.0001;
uniform float uNormalEps = 0.0001;

const float Inf = 1.0 / 0.0;
const float NaN = 0.0 / 0.0;

vec3 proceduralSky(vec3 rayDir) {
    return exp(-abs(rayDir.y) / uSkyColor);
}

vec3 proceduralSun(vec3 rayDir) {
    return pow(max(0.0, dot(rayDir, uLightDir)), 1000) * uLightColor;
}

float sdSphere(vec3 pos, float radius) {
    return length(pos) - radius;
}

float sdBox(vec3 pos, vec3 size) {
    vec3 q = abs(pos) - size;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float sdEllipsoid(vec3 pos, vec3 radius) {
    float k0 = length(pos / radius);
    float k1 = length(pos / radius / radius);
    return k0 * (k0 - 1.0) / k1;
}

float sdBouncyBall(vec3 pos, float t) {
    float tri = abs(fract(t) - 0.5) * 2.0; // Make a triangle wave from t
    float parabola = 1.0 - pow(tri, 2.5); // Make a parabola wave from the triangle wave
                                          // 2.0 is a realistic exponent, higher values appear cartoony
    float h = 2.0 * parabola; // Animate height h, 2.0 ist the maximum height
    float squish = 0.9 + parabola * 0.2; // Animate squish
    vec3 radius = vec3(0.25 / squish, 0.25 * squish, 0.25 / squish); // Scale the sphere
    vec3 center = vec3(0.0, 0.2 + radius.y + h, 0.0); // Position the sphere h units above the floor
    return sdEllipsoid(pos - center, radius);
}

float checkerPattern(vec3 pos) {
    vec3 p = floor(pos);
    return mod(p.x + p.y + p.z, 2.0);
}

const int SKY_ID = 0;
const int BOX_ID = 1;
const int BALL_ID = 2;

/* Struct to hold the signed distance and ID of the nearest object */
struct SD {
    float dist;
    int ID;
};

SD sdUnion(SD a, SD b) {
    return a.dist < b.dist ? a : b;
}

/* Returns the signed distance to the object nearest to pos */
SD sdScene(vec3 pos) {
    SD result = SD(Inf, SKY_ID);
    result = sdUnion(result, SD(sdBox(pos, vec3(2.0, 0.2, 2.0)), BOX_ID));
    result = sdUnion(result, SD(sdBouncyBall(pos, uTime), BALL_ID));
    return result;
}

/* Returns the color of the object ID at pos */
vec3 colorScene(vec3 pos, int ID) {
    switch (ID) {
        case BOX_ID:
            // Scale the checker pattern to prevent artifacts
            return vec3(0.6, 0.4, 0.2) * (checkerPattern(pos * 4.9) * 0.5 + 0.5);
        case BALL_ID:
            return vec3(1.0, 0.2, 0.2);
        default:
            return vec3(1.0, 0.0, 1.0);
    }
}

/* Returns the normalized gradient of the signed distance field of our scene */
vec3 normalScene(vec3 pos) {
    // TODO: Implement normal calculation
    return vec3(0.0); // No solution
}

struct Intersection {
    float depth; // Current depth on the ray
    vec3 pos; // Current position on the ray
    int ID; // ID of the closest object
    int steps; // Number of steps taken
};

Intersection raymarchScene(vec3 rayOrigin, vec3 rayDir, float near, float far) {
    // TODO: Implement raymarching
    return Intersection(far, rayOrigin, SKY_ID, 0); // No solution
}

void main() {
    vec3 rayDir = normalize(viewDir); // Renormalize after interpolation
    vec3 rayOrigin = uCameraPosition;

    Intersection isec = raymarchScene(rayOrigin, rayDir, uNear, uFar);

    if (isec.depth >= uFar) {
        // No hit, render a procedural sky background
        fragColor = proceduralSky(rayDir) + proceduralSun(rayDir);
    } else {
        // We hit something
        // The normal is the normalized gradient of the signed distance field
        vec3 normal = normalScene(isec.pos);
        // Lambert lighting term
        vec3 lighting = max(dot(normal, uLightDir), 0.0) * uLightColor;
        // Test if something lies between the hit point and the light source
        float shadow = raymarchScene(isec.pos, uLightDir, uNear, uFar).depth >= uFar ? 1.0 : 0.0;
        // Get the color of the hit point
        vec3 albedo = colorScene(isec.pos, isec.ID);
        // Calculate lighting
        // To simulate a soft ambient light from all directions, we add a little bit of the sky color to the light term
        fragColor = (shadow * lighting + uAmbient * uSkyColor) * albedo;
        if (uView == 1u) fragColor = normal * 0.5 + 0.5;
        if (uView == 2u) fragColor = vec3((isec.depth - uNear) / (uFar - uNear));
    }
    if (uView == 3u) fragColor = vec3(float(isec.steps) / float(uSteps));
}