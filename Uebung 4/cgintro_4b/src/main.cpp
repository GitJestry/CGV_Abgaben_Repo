#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
using namespace glm;

#include <imgui.h>

#include <iostream>
#include <vector>

#include <framework/app.hpp>
#include <framework/mesh.hpp>
#include <framework/camera.hpp>
#include <framework/gl/program.hpp>
#include <framework/imguiutil.hpp>
#include <framework/objparser.hpp>

struct MainApp : public App {
    Program program;
    Mesh mesh;
    Camera camera;

    // Uniforms
    unsigned int uView = 0;
    vec3 uLightDir = normalize(vec3(1.0));
    vec3 uLightColor = vec3(1.0);
    float uNear = 0.1;
    float uFar = 100.0;

    void loadObj(Program& program, const std::string& filename) {
        std::vector<Mesh::VertexPTN> vertices;
        std::vector<unsigned int> indices;
        ObjParser::parse(filename, vertices, indices);
    
        // Output mesh size
        GLuint triangleCount = indices.size() / 3;
        std::cout << "Vertices: " << vertices.size() << "\tIndices: " << indices.size() << "\tFaces: " << triangleCount << std::endl;
    
        // Pass the vertices to the shader as vec4 array
        // Each vertex is 2 vec4s arranged as:
        // (vec4(Px, Py, Pz, Cx), vec4(Cy, Nx, Ny, Nz)) with P = Position, C = Texture Coordinate, N = Normal
        glProgramUniform4fv(program.handle, program.uniform("uVertices"), vertices.size() * 2, value_ptr(vertices[0].position));
    
        // Pass the indices to the shader as uvec3 array, each uvec3 being a triangle
        glProgramUniform3uiv(program.handle, program.uniform("uIndices"), triangleCount, indices.data());
    
        // Pass triangle count to the shader
        program.set("uTriangleCount", triangleCount);
    }

    MainApp(): App(800, 600) {
        mesh.load(Mesh::FULLSCREEN_VERTICES, Mesh::FULLSCREEN_INDICES);
        program.load("shaders/raygen.vert", "shaders/raytrace.frag");

        GLint maxFragmentUniformComponents;
        glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxFragmentUniformComponents);
        std::cout << "Max fragment uniform components: " << maxFragmentUniformComponents << std::endl;

        program.set("uView", uView);
        program.set("uLightDir", uLightDir);
        program.set("uLightColor", uLightColor);
        program.set("uNear", uNear);
        program.set("uFar", uFar);
        loadObj(program, "meshes/lowpolysphere.obj");
        program.use();
    }

    void render() override {
        if (camera.updateIfChanged()) {
            program.set("uCameraMatrix", camera.cameraMatrix);
            program.set("uAspectRatio", camera.aspectRatio);
            program.set("uFocalLength", camera.focalLength);
            program.set("uCameraPosition", camera.worldPosition);
        }
    
        mesh.draw();
    }

    void buildImGui() override {
        ImGui::StatisticsWindow(App::delta, App::resolution);
    
        ImGui::Begin("Settings", nullptr, ImGuiChildFlags_AlwaysAutoResize);
        if (ImGui::Combo("View", &uView, {"Render", "Normals", "Depth", "Barycentrics"})) program.set("uView", uView);
        if (ImGui::SphericalSlider("Light Dir", uLightDir)) program.set("uLightDir", uLightDir);
        if (ImGui::ColorEdit3("Light Color", value_ptr(uLightColor), ImGuiColorEditFlags_Float)) program.set("uLightColor", uLightColor);
        if (ImGui::SliderFloat("Near Plane", &uNear, 0.0, 10.0, "%.1f")) program.set("uNear", uNear);
        if (ImGui::SliderFloat("Far Plane", &uFar, 0.0, 100.0, "%.1f", ImGuiSliderFlags_Logarithmic)) program.set("uFar", uFar);
        if (ImGui::Button("Load Plane")) loadObj(program, "meshes/plane.obj");
        if (ImGui::Button("Load Cube")) loadObj(program, "meshes/cube.obj");
        if (ImGui::Button("Load Sphere")) loadObj(program, "meshes/lowpolysphere.obj");
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