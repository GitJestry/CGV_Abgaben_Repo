#include <vector>

#include <glad/gl.h>

#include <framework/app.hpp>
#include <framework/mesh.hpp>
#include <framework/gl/program.hpp>

constexpr float OUTER = 0.5f;
constexpr float INNER = OUTER / 3.0f;

const std::vector<float> VERTICES = {
    // TODO
};

const std::vector<unsigned int> INDICES = {
    // TODO
};

struct MainApp : public App {
    Program shaderProgram;
    Mesh mesh;
    GLint lTime;

    /** 
     * In the constructor we load all assets, the members' default constructors
     * implicitly take care of the allocation of OpenGL resources.
     */
    MainApp() : App(600, 600) {
        // Load, link and compile shader program
        shaderProgram.load("shaders/transformation.vert", "shaders/white.frag");
        // Get uniform location
        lTime = glGetUniformLocation(shaderProgram.handle, "uTime");

        mesh.load(VERTICES, INDICES);

        // Set background color
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Uncomment for wireframe mode
    }

    /* Render loop */
    void render() override {
        // Fill screen with the background color (clearing the window)
        glClear(GL_COLOR_BUFFER_BIT);

        // Set the time uniform
        glProgramUniform1f(shaderProgram.handle, lTime, glfwGetTime());

        // Use our shader program
        glUseProgram(shaderProgram.handle);
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
