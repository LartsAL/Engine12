#include "managers/InputManager.h"

#include "systemutils/GlobalVars.h"

InputManager::InputManager():
    pressedKeys(0) {}

auto InputManager::getInstance() -> InputManager& {
    static InputManager instance;
    return instance;
}

auto InputManager::initialize() -> void {
//    g_keyCallback            = keyCallback;
//    g_cursorPositionCallback = cursorPositionCallback;
//    g_cursorEnterCallback    = cursorEnterCallback;
//    g_mouseButtonCallback    = mouseButtonCallback;
//    g_scrollCallback         = scrollCallback;
}

auto InputManager::update() -> void {

}

auto InputManager::shutdown() -> void {

}

auto InputManager::resetCallback(callbackType type) -> void {
//    switch (type) {
//        case KEY:
//            g_keyCallback = keyCallback;
//            break;
//        case CURSOR_POSITION:
//            g_cursorPositionCallback = cursorPositionCallback;
//            break;
//        case CURSOR_ENTER:
//            g_cursorEnterCallback = cursorEnterCallback;
//            break;
//        case MOUSE_BUTTON:
//            g_mouseButtonCallback = mouseButtonCallback;
//            break;
//        case SCROLL:
//            g_scrollCallback = scrollCallback;
//            break;
//    }
}

auto InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) -> void {
    if (action == GLFW_PRESS) {
        //pressedKeys[key] = true;
    }
    if (action == GLFW_RELEASE) {
        //pressedKeys[key] = false;
    }
}

auto InputManager::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) -> void {

}

auto InputManager::cursorEnterCallback(GLFWwindow* window, int entered) -> void {

}

auto InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) -> void {

}

auto InputManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) -> void {

}
