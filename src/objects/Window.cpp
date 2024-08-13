#include "objects/Window.h"

#include "systemutils/EngineExceptions.h"

Window::Window(WindowID ID, SceneID linkedScene, GLint width, GLint height,
               const char* title, GLFWmonitor* monitor) {
    this->ID = ID;
    this->linkedScene = linkedScene;
    this->width = width;
    this->height = height;
    this->title = title;
    this->monitor = monitor;

    this->window = std::shared_ptr<GLFWwindow>(
            glfwCreateWindow(width, height, title, monitor, nullptr),
            [](GLFWwindow* window) -> void { if (window) glfwDestroyWindow(window); }
    );

    if (!this->window) {
        throw EngineException("Window creation failed.", ENGINE_EXCEPT_WINDOW_CREATION_FAILED);
    }
}

auto Window::getSmartPointer() const noexcept -> std::shared_ptr<GLFWwindow> {
    return window;
}

auto Window::getRawPointer() const noexcept -> GLFWwindow* {
    return window.get();
}

auto Window::getWidth() const noexcept -> GLint {
    return width;
}

auto Window::getHeight() const noexcept -> GLint {
    return height;
}

auto Window::getTitle() const noexcept -> const char* {
    return title;
}

auto Window::getMonitor() const noexcept -> GLFWmonitor* {
    return monitor;
}
