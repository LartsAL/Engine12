#pragma once

#include <queue>
#include <memory>
#include <vector>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class WindowManager {
public:
    static auto getInstance() -> WindowManager&;

    auto initialize() -> void;
    // auto update() -> void;
    auto shutdown() -> void;

    auto createWindow(int width, int height, const char* title = "") -> std::shared_ptr<GLFWwindow>;
    auto destroyWindow(const std::shared_ptr<GLFWwindow>& window) -> void;
    auto setActiveWindow(const std::shared_ptr<GLFWwindow>& window) -> void;

private:
    WindowManager();
    ~WindowManager();

    auto initializeGLAD() -> int;

    GLuint maxWindows;
    GLuint windowsCount;
    GLuint nextFreeID;
    std::unordered_map<GLuint, std::shared_ptr<GLFWwindow>> windows;
    
    // Deprecated?
    std::vector<std::shared_ptr<GLFWwindow>> existingWindows;
};
