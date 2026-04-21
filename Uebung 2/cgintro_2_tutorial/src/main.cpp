#include <vector>
#include <filesystem>

#include <glad/gl.h>

#include <framework/app.hpp>
#include <framework/mesh.hpp>
#include <framework/gl/program.hpp>

const std::vector<float> VERTICES = {
    // position           // color
    -0.6f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
     0.6f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
     0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
};

const std::vector<unsigned int> INDICES = {
    0, 1, 2,
};

struct MainApp : public App {
    Program shaderProgram;
    VertexArray vao;
    Buffer<GL_ARRAY_BUFFER> vbo;
    Buffer<GL_ELEMENT_ARRAY_BUFFER> ebo;
    GLint lTime = -1;

    MainApp() : App(600, 600) {
        shaderProgram.load("shaders/colors.vert", "shaders/colors.frag");
        lTime = glGetUniformLocation(shaderProgram.handle, "uTime");

        glBindVertexArray(vao.handle);

        // Upload vertex data
        glBindBuffer(GL_ARRAY_BUFFER, vbo.handle);
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(VERTICES.size() * sizeof(float)),
            VERTICES.data(),
            GL_STATIC_DRAW
        );

        // Upload index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo.handle);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(INDICES.size() * sizeof(unsigned int)),
            INDICES.data(),
            GL_STATIC_DRAW
        );

        constexpr GLsizei stride = 6 * sizeof(float);

        // Attribute 0: position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0,                          // attribute location
            3,                          // number of components
            GL_FLOAT,                   // type
            GL_FALSE,                   // normalized?
            stride,                     // stride
            reinterpret_cast<void*>(0)  // offset
        );

        // Attribute 1: color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            stride,
            reinterpret_cast<void*>(3 * sizeof(float))
        );

        glBindVertexArray(0);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    }

    void render() override {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram.handle);

        if (lTime != -1) {
            glUniform1f(lTime, static_cast<float>(glfwGetTime()));
        }

        glBindVertexArray(vao.handle);
        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLsizei>(INDICES.size()),
            GL_UNSIGNED_INT,
            nullptr
        );
    }
};

int main() {
#ifndef NDEBUG
    std::filesystem::current_path(
        std::filesystem::path(__FILE__).parent_path().parent_path()
    );
#endif

    MainApp app;
    app.run();
}