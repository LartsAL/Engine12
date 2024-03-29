#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


Renderer::Renderer() = default;

Renderer::~Renderer() {
    shutdown();
}

Renderer& Renderer::getInstance() {
    static Renderer instance;
    return instance;
}

auto Renderer::initialize() -> bool {
    // GLFW initialization
    if (!glfwInit()) {
        return false;
    }

    /*...*/

    return true;
}

auto Renderer::shutdown() -> void {

}
