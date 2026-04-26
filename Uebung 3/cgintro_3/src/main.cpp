#include <glad/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
using namespace glm;

#include <framework/app.hpp>
#include <framework/camera.hpp>
#include <framework/mesh.hpp>
#include <framework/imguiutil.hpp>
#include <framework/gl/program.hpp>

const vec3 SUN_COLOR = vec3(1.0f, 0.6f, 0.0f);
const vec3 EARTH_COLOR = vec3(0.0f, 0.5f, 1.0f);
const vec3 MOON_COLOR = vec3(0.5f, 0.5f, 0.5f);

const float SUN_EARTH_DISTANCE = 5.0f; // in sun radii, in reality ca 149 000 000 km = ca 214 sun radii
const float EARTH_MOON_DISTANCE = 2.0f; // in earth radii, in reality ca 384 km = ca 60 earth radii
const float SUN_RADIUS = 1.0f; // In reality ca 696 340 km
const float SUN_EARTH_RATIO = 0.20f; // In reality ca 0.009
const float EARTH_MOON_RATIO = 0.27f; // This is actually accurate

const float TWO_PI = 2.0f * pi<float>();

struct MainApp : public App {
    Program program;
    Mesh mesh;
    Camera camera;
    
    MainApp() : App(600, 600) {
        mesh.load("meshes/highpolysphere.obj");
        program.load("shaders/projection.vert", "shaders/color.frag");
    
        camera.worldPosition = vec3(10.0f, 0.0f, 0.0f); // position camera 10 units away
        camera.minDist = SUN_RADIUS + camera.near; // don't go inside the sun

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
    }

    // Handle input
    void keyCallback(Key key, Action action, Modifier modifier) override {
        if (key == Key::ESC && action == Action::PRESS) App::close();
    }

    void scrollCallback(float xamount, float yamount) override {
        camera.zoom(yamount);
    }

    void moveCallback(const vec2& movement, bool leftButton, bool rightButton, bool middleButton) override {
        if (leftButton | rightButton | middleButton) camera.orbit(movement * 0.01f);
    }

    void resizeCallback(const vec2& resolution) override {
        camera.resize(resolution.x / resolution.y);
    }

    void render() override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        float year = App::time / 5.0f; // 5 seconds for one year
        float month = year * 13.37f; // The Moon orbits the Earth ca 13.37 times a year
    
        camera.updateIfChanged();
        mat4 worldToClip = camera.projectionMatrix * camera.viewMatrix;
    
        mat4 sun = scale(vec3(SUN_RADIUS));
    
        mat4 earth = mat4(1.0f); // TODO: Scale to the right size, translate to the right distance and rotate once every year around the sun
        mat4 moon = mat4(1.0f); // TODO: Scale to the right size, translate to the right distance and rotate once every month around the earth
    
        program.use();
        program.set("uColor", SUN_COLOR);
        program.set("uLocalToWorld", sun);
        program.set("uLocalToClip", worldToClip * sun);
        program.set("uEnableLighting", false);
        mesh.draw();
        // TODO: Draw the earth and moon
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