#include <glad/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <imgui.h>

#include <framework/app.hpp>
#include <framework/mesh.hpp>
#include <framework/camera.hpp>
#include <framework/gl/program.hpp>
#include <framework/imguiutil.hpp>

struct MainApp : public App {
    Program program;
    Mesh mesh;
    Camera camera;
    unsigned int uView = 0;
    vec3 uLightDir = normalize(vec3(1.0));
    vec3 uLightColor = vec3(1.0);
    vec3 uSkyColor = vec3(0.1, 0.3, 0.6);
    float uAmbient = 0.3;
    float uNear = 0.1;
    float uFar = 100.0;
    int uSteps = 100;
    float uEpsilon = 0.0001;
    float uNormalEps = 0.0001;

    MainApp() : App(800, 600) {
        mesh.load(Mesh::FULLSCREEN_VERTICES, Mesh::FULLSCREEN_INDICES);
        program.load("shaders/raygen.vert", "shaders/raymarch.frag");
        camera.worldPosition = vec3(5.0f, 3.0f, 5.0f);

        // Init uniforms
        program.set("uLightDir", uLightDir);
        program.set("uLightColor", uLightColor);
        program.set("uSkyColor", uSkyColor);
        program.set("uAmbient", uAmbient);
        program.set("uNear", uNear);
        program.set("uFar", uFar);
        program.set("uSteps", uSteps);
        program.set("uEpsilon", uEpsilon);
        program.set("uNormalEps", uNormalEps);
        program.use();
    }

    void render() override {
        program.set("uTime", time);

        // Update camera information on change
        if (camera.updateIfChanged()) {
            program.set("uCameraMatrix", camera.cameraMatrix);
            program.set("uAspectRatio", camera.aspectRatio);
            program.set("uFocalLength", camera.focalLength);
            program.set("uCameraPosition", camera.worldPosition);
        }

        // We already bind the program in the constructor, so we don't need to bind it again here
        mesh.draw();
    }

    void buildImGui() override {
        ImGui::StatisticsWindow(App::delta, App::resolution);

        // UI to control the uniforms
        ImGui::Begin("Settings", nullptr, ImGuiChildFlags_AlwaysAutoResize);
        if (ImGui::Combo("View", &uView, {"Render", "Normals", "Depth", "Steps"})) program.set("uView", uView);
        if (ImGui::SphericalSlider("Light Dir", uLightDir)) program.set("uLightDir", uLightDir);
        if (ImGui::ColorEdit3("Light Color", value_ptr(uLightColor), ImGuiColorEditFlags_Float)) program.set("uLightColor", uLightColor);
        if (ImGui::ColorEdit3("Sky Color", value_ptr(uSkyColor), ImGuiColorEditFlags_Float)) program.set("uSkyColor", uSkyColor);
        if (ImGui::SliderFloat("Ambient", &uAmbient, 0.0, 1.0, "%.2f")) program.set("uAmbient", uAmbient);
        if (ImGui::SliderFloat("Near Plane", &uNear, 0.0, 10.0, "%.1f")) program.set("uNear", uNear);
        if (ImGui::SliderFloat("Far Plane", &uFar, 0.0, 100.0, "%.1f", ImGuiSliderFlags_Logarithmic)) program.set("uFar", uFar);
        if (ImGui::SliderInt("Max Steps", &uSteps, 0, 200)) program.set("uSteps", uSteps);
        if (ImGui::SliderFloat("Epsilon", &uEpsilon, 0.0, 1.0, "%.6f", ImGuiSliderFlags_Logarithmic)) program.set("uEpsilon", uEpsilon);
        if (ImGui::SliderFloat("Normal Epsilon", &uNormalEps, 0.0, 1.0, "%.6f", ImGuiSliderFlags_Logarithmic)) program.set("uNormalEps", uNormalEps);
        ImGui::End();
    }

    void keyCallback(Key key, Action action, Modifier modifier) override {
        if (key == Key::ESC && action == Action::PRESS) App::close();
        else if (key == Key::COMMA && action == Action::PRESS) App::imguiEnabled = !App::imguiEnabled;
    }

    void scrollCallback(float x, float y) override {
        camera.zoom(y);
    }

    void moveCallback(const vec2& movement, bool leftButton, bool rightButton, bool middleButton) override {
        if (rightButton | middleButton) camera.orbit(movement * 0.01f);
    }

    void resizeCallback(const vec2& resolution) override {
        camera.resize(resolution.x / resolution.y);
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