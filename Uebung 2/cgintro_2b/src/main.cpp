#include <string>
#include <vector>

#include <glad/gl.h>

#include <framework/app.hpp>
#include <framework/mesh.hpp>
#include <framework/gl/program.hpp>

const std::vector<Mesh::VertexPC> VERTICES {
    {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
    {{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 1.0f}},
    {{ 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}},
    {{-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f, 1.0f}}
};

const std::vector<unsigned int> INDICES {
    0, 1, 2,
    2, 3, 0,
};

struct MainApp: App {
    Program program;
    Mesh mesh;

    MainApp(): App(500, 500) {
        program.load("shaders/color.vert", "shaders/hsv.frag");

        mesh.load(VERTICES, INDICES);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // Set clear color
    }

    void render() override {
        glClear(GL_COLOR_BUFFER_BIT); // Clear the screen
        program.use();
        mesh.draw();
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