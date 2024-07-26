#include "objects/Window.h"

Window::Window(WindowID ID, SceneID linkedScene):
    ID(ID),
    linkedScene(linkedScene) {}

auto Window::getRawPointer() -> GLFWwindow* {
    return window.get();
}
