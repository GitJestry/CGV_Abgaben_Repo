#include <glad/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <imgui.h>

#include <vector>
#include <iostream>

#include <framework/app.hpp>
#include <framework/camera.hpp>
#include <framework/mesh.hpp>
#include <framework/imguiutil.hpp>
#include <framework/gl/program.hpp>

struct MainApp : public App {
    Camera camera;
    Mesh mesh;
    Program program;
    vec3 lightDir = normalize(vec3(1.0f, 1.0f, 1.0f));
    vec3 lightColor = vec3(1.0f);
    bool showNormals = false;

    MainApp() : App(800, 600) {
        mesh.load("meshes/suzanne.obj");
        program.load("shaders/projection.vert", "shaders/lambert.frag");
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
    }

    void render() override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.updateIfChanged();

        mat4 model = rotate(mat4(1.0f), time, vec3(0.0f, 1.0f, 0.0f));
        mat4 view = camera.viewMatrix;
        mat4 projection = camera.projectionMatrix;

        mat4 localToClip = projection * view * model;
        mat4 localToWorld = model;

        program.set("uLocalToClip", localToClip);
        program.set("uLocalToWorld", localToWorld);
        program.set("uLightDir", lightDir);
        program.set("uLightColor", lightColor);
        program.set("uShowNormals", showNormals);

        program.use();
        mesh.draw();
    }

    void buildImGui() override {
        ImGui::StatisticsWindow(App::delta, App::resolution);
        ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::SphericalSlider("Light Direction", lightDir);
        ImGui::ColorEdit3("Light Color", value_ptr(lightColor), ImGuiColorEditFlags_Float);
        ImGui::Checkbox("Show Normals", &showNormals);
        if (ImGui::SliderAngle("Field Of View", &camera.fov, 0.0f, 180.0f)) camera.invalidate();
        if (ImGui::Button("Load Bunny")) mesh.load("meshes/bunny.obj");
        if (ImGui::Button("Load Suzanne")) mesh.load("meshes/suzanne.obj");
        if (ImGui::Button("Load Sphere")) mesh.load("meshes/highpolysphere.obj");
        ImGui::End();
    }

    void keyCallback(Key key, Action action, Modifier modifier) override {
        // Close the application when pressing ESC
        if (key == Key::ESC && action == Action::PRESS) App::close();
        // Toggle GUI with COMMA
        if (key == Key::COMMA && action == Action::PRESS) App::imguiEnabled = !App::imguiEnabled;
    }

    // void clickCallback(Button button, Action action, const vec2& position) override;

    void scrollCallback(float x, float y) override {
        camera.zoom(y);
    }

    void moveCallback(const vec2& movement, bool leftButton, bool rightButton, bool middleButton) override {
        if (rightButton || leftButton || middleButton) camera.orbit(movement * 0.01f);
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