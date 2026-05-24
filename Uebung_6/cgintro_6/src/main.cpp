#include <glad/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
using namespace glm;

#include <imgui.h>

#include <vector>
#include <iostream>
#include <cassert>
#include <array>

#include <framework/app.hpp>
#include <framework/camera.hpp>
#include <framework/imguiutil.hpp>
#include <framework/mesh.hpp>
#include <framework/gl/framebuffer.hpp>
#include <framework/gl/program.hpp>
#include <framework/gl/texture.hpp>

vec3 xzCircle(float r, float t) { return r * vec3(cos(t), 0.0f, sin(t)); }

void genLightPositions(std::vector<vec3>& lightPositions, float radius, float t, size_t count) {
    lightPositions.resize(count);
    srand(42);
    for (size_t i = 0; i < count; i++) {
        vec4 rand = linearRand(vec4(-1.0f), vec4(1.0f));
        lightPositions[i] = xzCircle(sqrt(abs(rand.x)), t * rand.y) + xzCircle(sqrt(abs(rand.z)), t * rand.w);
        lightPositions[i] *= radius;
        lightPositions[i].y = linearRand(0.0f, 1.0f);
    }
}

void genLightColors(std::vector<vec3>& lightColors, size_t count) {
    lightColors.reserve(count);
    for (size_t i = lightColors.size(); i < count; i++) {
        lightColors.push_back(linearRand(vec3(0.0f), vec3(1.0f)));
    }
}

struct MainApp : App {
    Camera cam;
    Mesh mesh;
    Program meshShader;
    Mesh fullscreenTriangle;
    Program deferredCompose;
    Framebuffer gBuffer;
    Texture<GL_TEXTURE_2D> colorTexture;
    Texture<GL_TEXTURE_2D> normalTexture;
    Texture<GL_TEXTURE_2D> depthTexture;
    uint uDebugView = 0;
    int uLightCount = 256;
    float uBrightness = 0.2f;
    float uLightRadius = 10.0f;
    int bunnyCount = 256;
    std::vector<vec3> uLightPositions;
    std::vector<vec3> uLightColors;

    void resizeGBuffer() {
        colorTexture = Texture<GL_TEXTURE_2D>();
        colorTexture.allocate2D(GL_RGBA32F, resolution.x, resolution.y);
        gBuffer.attach(GL_COLOR_ATTACHMENT0, colorTexture);

        normalTexture = Texture<GL_TEXTURE_2D>();
        normalTexture.allocate2D(GL_RGBA16_SNORM, resolution.x, resolution.y);
        gBuffer.attach(GL_COLOR_ATTACHMENT1, normalTexture);

        depthTexture = Texture<GL_TEXTURE_2D>();
        depthTexture.allocate2D(GL_DEPTH32F_STENCIL8, resolution.x, resolution.y);
        gBuffer.attach(GL_DEPTH_STENCIL_ATTACHMENT, depthTexture);

        colorTexture.bindTextureUnit(0);
        normalTexture.bindTextureUnit(1);
        depthTexture.bindTextureUnit(2);
    }
    
    MainApp() : App(800, 600) {
        setTitle("Bonus: Bunny Disco");
        fullscreenTriangle.load(Mesh::FULLSCREEN_VERTICES, Mesh::FULLSCREEN_INDICES);
        mesh.load("meshes/bunny.obj");

        meshShader.load("shaders/projection.vert", "shaders/geometry.frag");

        deferredCompose.load("shaders/screen.vert", "shaders/compose.frag");
        deferredCompose.bindTextureUnit("tColor", 0);
        deferredCompose.bindTextureUnit("tNormal", 1);
        deferredCompose.bindTextureUnit("tDepth", 2);
        deferredCompose.set("uDebugView", uDebugView);
        deferredCompose.set("uLightRadius", uLightRadius);
        deferredCompose.set("uLightCount", uLightCount);
        deferredCompose.set("uBrightness", uBrightness);

        genLightColors(uLightColors, uLightCount);

        resizeGBuffer();
        
        gBuffer.bind();
        const std::array<GLenum, 2> drawBuffers = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
        glDrawBuffers(drawBuffers.size(), drawBuffers.data());

        gBuffer.checkStatus(GL_DRAW_FRAMEBUFFER);

        glEnable(GL_CULL_FACE);
    }

    void render() override {
        cam.updateIfChanged();
        mat4 worldToClip = cam.projectionMatrix * cam.viewMatrix;
        mat4 localToWorld = rotate(mat4(1.0f), time, vec3(0.0f, 1.0f, 0.0f));
    
        genLightPositions(uLightPositions, uLightRadius, time * 3.0f, uLightCount);
    
        ////////////////// Geometry pass //////////////////
        gBuffer.bind();
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
        meshShader.set("uWorldToClip", worldToClip);
        meshShader.set("uLocalToWorld", localToWorld);
        meshShader.set("uGridSize", static_cast<int>(ceil(sqrt(bunnyCount))));
        meshShader.use();
        mesh.draw(bunnyCount);
    
        ////////////////// Lighting pass //////////////////
        Framebuffer::bindDefault();
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
    
        deferredCompose.set("uClipToWorld", inverse(worldToClip));
        deferredCompose.set("uResolution", resolution);
        deferredCompose.set("uLightPositions", uLightPositions);
        deferredCompose.set("uLightColors", uLightColors);
        deferredCompose.use();
        fullscreenTriangle.draw();
    }
    
    /* Catch window events by overriding the callback functions */
    void keyCallback(Key key, Action action, Modifier) override {
        if (key == Key::ESC && action == Action::PRESS) close();
        if (key == Key::COMMA && action == Action::PRESS) App::imguiEnabled = !App::imguiEnabled;
    }
    void scrollCallback(float x, float y) override {
        cam.zoom(y);
    }
    void moveCallback(const vec2& movement, bool leftButton, bool rightButton, bool middleButton) override {
        if (leftButton | rightButton | middleButton) cam.orbit(movement * 0.01f);
    }
    void resizeCallback(const vec2& resolution) override {
        cam.resize(resolution.x / resolution.y);
        resizeGBuffer();
    }
    
    /* Build GUI elements here using the functions from imgui.h and util.cpp */
    void buildImGui() override {
        ImGui::StatisticsWindow(delta, resolution);
    
        ImGui::Begin("Config", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        if (ImGui::Combo("View", &uDebugView, {"Composed", "Color", "Normal", "Depth", "Clip Position", "World Position", "Lighting"})) deferredCompose.set("uDebugView", uDebugView);
        if (ImGui::SliderFloat("Light Radius", &uLightRadius, 1.0f, 10.0f)) deferredCompose.set("uLightRadius", uLightRadius);
        if (ImGui::SliderInt("Light Count", &uLightCount, 1, 256)) {
            deferredCompose.set("uLightCount", uLightCount);
            genLightColors(uLightColors, uLightCount);
        }
        if (ImGui::SliderFloat("Brightness", &uBrightness, 0.0f, 2.0f)) deferredCompose.set("uBrightness", uBrightness);
        if (ImGui::SliderInt("Bunny Count", &bunnyCount, 1, 256)) meshShader.set("uGridSize", static_cast<int>(ceil(sqrt(bunnyCount))));
        ImGui::End();
    }
 };

#include <filesystem>

int main() {
#ifndef NDEBUG
    // cd to parent directory when in debug mode to find resources
    std::filesystem::current_path(std::filesystem::path(__FILE__).parent_path().parent_path());
#endif
    MainApp app;
    app.run();
}