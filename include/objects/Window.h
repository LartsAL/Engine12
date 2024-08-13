#pragma once

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "systemutils/GlobalVars.h"

class Window {
public:
    Window(WindowID ID, SceneID linkedScene, GLint width, GLint height,
           const char* title = nullptr, GLFWmonitor* monitor = nullptr);

    auto getSmartPointer() const noexcept -> std::shared_ptr<GLFWwindow>;
    auto getRawPointer() const noexcept -> GLFWwindow*;
    auto getWidth() const noexcept -> GLint;
    auto getHeight() const noexcept -> GLint;
    auto getTitle() const noexcept -> const char*;
    auto getMonitor() const noexcept -> GLFWmonitor*;

private:
    WindowID                    ID;
    SceneID                     linkedScene;
    std::shared_ptr<GLFWwindow> window;

    GLint                       width;
    GLint                       height;
    const char*                 title;
    GLFWmonitor*                monitor;
};
