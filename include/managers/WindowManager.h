#pragma once

#include <set>
#include <tuple>
#include <queue>
#include <memory>
#include <vector>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "objects/Window.h"
#include "systemutils/GlobalVars.h"
#include "systemutils/IDSystem.h"

using WindowInfo = std::tuple<GLuint, GLint, GLint, const char*, GLFWmonitor*>;

class WindowManager {
public:
    static auto getInstance() noexcept -> WindowManager&;

    auto initialize() noexcept -> void;
    auto update() -> void;
    auto shutdown() noexcept -> void;

    // # WARNING: createWindow doesn't support glfwCreateWindow [share] parameter. It's always set to NULL.
    auto createWindow(SceneID linkedSceneID, GLint width, GLint height, const char* title = "",
                      GLFWmonitor* monitor = nullptr) -> WindowID;
    auto deleteWindow(WindowID ID) noexcept -> void;
    auto deleteAllWindows() noexcept -> void;
    auto setActiveWindow(WindowID ID) const noexcept -> void;
    auto enableVSync(bool state) const noexcept -> void;
    auto setWindowHint(GLint hint, GLint value) const noexcept -> void;
    auto resetWindowHints() const noexcept -> void;
    auto getWindowPtr(WindowID ID) const noexcept -> std::shared_ptr<Window>;

private:
    WindowManager() noexcept;
    ~WindowManager() noexcept;

    auto initializeGLAD(WindowID ID) const -> void;

    GLuint windowsCount;
    std::unordered_map<GLuint, std::shared_ptr<Window>> windows;
    IDSystem idSystem;
    std::queue<WindowInfo> windowsCreationInfo;
    std::unordered_map<GLint, GLint> hintsDefaults;
};
