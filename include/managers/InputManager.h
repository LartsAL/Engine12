#pragma once

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class InputManager {
public:
    enum callbackType {
        KEY,
        CURSOR_POSITION,
        CURSOR_ENTER,
        MOUSE_BUTTON,
        SCROLL
    };

    bool pressedKeys[1024];

    static auto getInstance() -> InputManager&;

    auto initialize() -> void;
    auto update() -> void;
    auto shutdown() -> void;

    auto resetCallback(callbackType type) -> void;

private:
    InputManager();

    static auto keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) -> void;
    static auto cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) -> void;
    static auto cursorEnterCallback(GLFWwindow* window, int entered) -> void;
    static auto mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) -> void;
    static auto scrollCallback(GLFWwindow* window, double xoffset, double yoffset) -> void;
};
