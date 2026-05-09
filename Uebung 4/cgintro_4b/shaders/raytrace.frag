#version 330 core

in vec3 viewDir;
out vec3 fragColor;

const uint MAX_TRIANGLES = 100u;
uniform vec4 uVertices[MAX_TRIANGLES * 3u * 2u];
uniform uvec3 uIndices[MAX_TRIANGLES];
uniform uint uTriangleCount;

uniform vec3 uCameraPosition;

uniform uint uView = 0u;
uniform vec3 uLightDir = normalize(vec3(1.0));
uniform vec3 uLightColor = vec3(1.0);
uniform float uNear = 0.1;
uniform float uFar = 100.0;

const float INF = 1.0 / 0.0;
const float NAN = 0.0 / 0.0;

vec3 proceduralSun(vec3 rayDir) {
    return pow(max(0.0, dot(rayDir, uLightDir)), 1000) * uLightColor;
}

vec3 calcCrossProd(vec3 a, vec3 b) {
    return vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

// Spatprodukt:
// det(a | b | c) = (a x b) · c
float spat(vec3 a, vec3 b, vec3 c) {
    return dot(calcCrossProd(a, b), c);
}

vec3 intersectTriangle(vec3 rayOrigin, vec3 rayDir, vec3 v0, vec3 v1, vec3 v2) {
    // Aus der Vorlesung:
    // Dreieck = c + u * e1 + v * e2
    // Dabei ist c ein Eckpunkt des Dreiecks.
    // e1 und e2 spannen die Dreiecksebene auf.
    vec3 c  = v0;
    vec3 e1 = v1 - v0;
    vec3 e2 = v2 - v0;

    // Die Normale der Dreiecksebene entsteht durch das Kreuzprodukt.
    vec3 n = calcCrossProd(e1, e2);

    // Falls n nahezu Länge 0 hat, ist das Dreieck degeneriert.
    // Dann liegen die drei Punkte z.B. auf einer Linie und bilden keine gültige Fläche.
    float detBase = spat(e1, e2, n);
    if (abs(detBase) < 1e-6) {
        return vec3(INF);
    }

    // Strahl in Ebene eingesetzt aus der Vorlesung
    // dot(n, rayOrigin + t * rayDir - c) = 0
    // t = dot(n, c - rayOrigin) / dot(n, rayDir)
    float denom = dot(n, rayDir);

    // Wenn denom fast 0 ist, ist der Strahl parallel zur Dreiecksebene.
    // Dann gibt es für Raytracing keinen eindeutigen gültigen Treffer.
    if (abs(denom) < 1e-6) {
        return vec3(INF);
    }

    float t = dot(n, c - rayOrigin) / denom;

    // t < 0 bedeutet: Der Ebenenschnitt liegt hinter dem Strahlursprung.
    // Für den Kamerastrahl ist dieser Treffer nicht sichtbar.
    if (t < 0.0) {
        return vec3(INF);
    }

    // Schnittpunkt des Strahls mit der unendlich großen Ebene.
    vec3 hit = rayOrigin + t * rayDir;

    // Für die Dreieckskoordinaten betrachten wir den Treffer relativ zum Aufpunkt c:
    // hit = c + u * e1 + v * e2
    // p = hit - c = u * e1 + v * e2
    vec3 p = hit - c;

    // Aus der Uebungsaufgabe:
    // u = det(p | e2 | n) / det(e1 | e2 | n)
    // v = det(e1 | p | n) / det(e1 | e2 | n)
    float u = spat(p,  e2, n) / detBase;
    float v = spat(e1, p,  n) / detBase;

    // Inside-Test:
    if (u < 0.0 || v < 0.0 || u + v > 1.0) {
        return vec3(INF);
    }

    // u und v können später für baryzentrische Interpolation genutzt werden.
    // t ist die Tiefe entlang des Strahls und wird verwendet, um den nächsten Treffer zu finden.
    return vec3(u, v, t);
}

void main() {
    // Generate camera ray
    vec3 rayDir = normalize(viewDir); // Renormalize after interpolation
    vec3 rayOrigin = uCameraPosition;

    // Clear with background
    fragColor = proceduralSun(rayDir);
    float depth = uFar;

    // Loop through each triangle uIndices[i].xyz
    for (uint i = 0u; i < uTriangleCount; i++) {

        // Fetch vertex positions
        vec3 v0 = uVertices[uIndices[i].x * 2u].xyz;
        vec3 v1 = uVertices[uIndices[i].y * 2u].xyz;
        vec3 v2 = uVertices[uIndices[i].z * 2u].xyz;

        vec3 result = intersectTriangle(rayOrigin, rayDir, v0, v1, v2);

        // Overdraw if closer
        if (result.z < depth) {
            // Unpack result
            depth = result.z;
            vec3 barycentrics = vec3(1.0 - result.x - result.y, result.xy);

            // Fetch vertex normals
            vec3 n0 = uVertices[uIndices[i].x * 2u + 1u].yzw;
            vec3 n1 = uVertices[uIndices[i].y * 2u + 1u].yzw;
            vec3 n2 = uVertices[uIndices[i].z * 2u + 1u].yzw;

            // Interpolate normals
            vec3 normal = normalize(mat3(n0, n1, n2) * barycentrics);
            // = normalize(n0 * barycentrics.x + n1 * barycentrics.y + n2 * barycentrics.z);

            // Shade
            fragColor = max(dot(normal, uLightDir), 0.0) * uLightColor + vec3(0.005);

            // Debug views
            if (uView == 1u) fragColor = normal * 0.5 + 0.5;
            if (uView == 2u) fragColor = vec3(1.0 - (depth / uFar));
            if (uView == 3u) fragColor = barycentrics;
        }
    }
}