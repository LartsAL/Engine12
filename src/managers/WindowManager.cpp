#include "managers/WindowManager.h"

#include <iostream>
#include "Error.h"
#include "GlobalVars.h"

WindowManager::WindowManager() = default;

WindowManager::~WindowManager() = default;

auto WindowManager::getInstance() -> WindowManager& {
    static WindowManager instance;
    return instance;
}

auto WindowManager::initialize() -> void {

}

auto WindowManager::shutdown() -> void {

}

auto WindowManager::createWindow(int width, int height, const char* title) -> std::shared_ptr<GLFWwindow> {
    auto window = std::shared_ptr<GLFWwindow>(glfwCreateWindow(width, height, title, nullptr, nullptr),
                                              [](GLFWwindow* window) -> void { if (window) glfwDestroyWindow(window); });

    if (!window) {
        PRINT_ERROR("Window creation failed.")
        return nullptr;
    }

    auto tmp = g_currentWindow;
    setActiveWindow(window);
    initializeGLAD();

    if (tmp) {
        setActiveWindow(tmp);
    } else {
        g_currentWindow = window;
    }

    existingWindows.push_back(window);
    return window;
}

auto WindowManager::destroyWindow(const std::shared_ptr<GLFWwindow>& window) -> void {

}

auto WindowManager::setActiveWindow(const std::shared_ptr<GLFWwindow>& window) -> void {
    glfwMakeContextCurrent(window.get());
    g_currentWindow = window;
}

// ? Do smth if loading failed? Maybe destroy window?
auto WindowManager::initializeGLAD() -> int {
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        PRINT_ERROR("GLAD loading failed.")
        return 1;
    }
    return 0;
}
