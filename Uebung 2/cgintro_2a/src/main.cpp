#include <vector>


#include <glad/gl.h>

#include <framework/app.hpp>
#include <framework/mesh.hpp>
#include <framework/gl/program.hpp>

constexpr float OUTER = 0.5f; // length
constexpr float INNER = OUTER / 3.0f; // thickness

// ------- Index Vertex lAYOUT --------

//      0     1           row 0
// 2    3     4     5     row 1
// 6    7     8     9     row 2
//      10    11          row 3
//

std::vector<float> VERTICES = {
    // row 0
    -INNER / 2.0f, OUTER, 0.0f,
    INNER / 2.0f,  OUTER, 0.0f,

    // row 1
    -OUTER,        INNER / 2.0f, 0.0f,
    -INNER / 2.0f, INNER / 2.0f, 0.0f,
    INNER / 2.0f,  INNER / 2.0f, 0.0f,
    OUTER,         INNER / 2.0f, 0.0f,

    // row 2
    -OUTER,        -INNER / 2.0f, 0.0f,
    -INNER / 2.0f, -INNER / 2.0f, 0.0f,
    INNER / 2.0f,  -INNER / 2.0f, 0.0f,
    OUTER,         -INNER / 2.0f, 0.0f,

    // row 3
    -INNER / 2.0f, -OUTER, 0.0f,
    INNER / 2.0f,  -OUTER, 0.0f
};

const std::vector<unsigned int> INDICES = {
    0, 3, 1,   1, 3, 4,   // top
    2, 6, 3,   3, 6, 7,   // left
    3, 7, 4,   4, 7, 8,   // center
    4, 8, 5,   5, 8, 9,   // right
    7, 10, 8,  8, 10, 11  // bottom
};

struct MainApp : public App {
    Program shaderProgram;
    Mesh mesh;
    GLint lTime;

    MainApp() : App(600, 600) {
        // Load, link and compile shader program
        shaderProgram.load("shaders/transformation.vert", "shaders/white.frag");
        // Get uniform location
        lTime = glGetUniformLocation(shaderProgram.handle, "uTime");
        mesh.load(VERTICES, INDICES);
        // Set background color
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    }

    /* Render loop */
    void render() override {
        // Fill screen with the background color (clearing the window)
        glClear(GL_COLOR_BUFFER_BIT);
        // Use our shader program
        glUseProgram(shaderProgram.handle);
        glUniform1f(lTime, glfwGetTime());
        
        mesh.draw();
    }
};

int main() {
    MainApp app;
    app.run();
}
