#pragma once

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "systemutils/GlobalVars.h"

class Window {
public:
    Window(WindowID ID, SceneID linkedScene);

    auto getRawPointer() -> GLFWwindow*;

private:
    WindowID                    ID;
    SceneID                     linkedScene;
    std::shared_ptr<GLFWwindow> window;
//    GLuint                      width;
//    GLuint                      height;
//    const char*                 title;
};
