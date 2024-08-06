#pragma once

#include <set>
#include <tuple>
#include <queue>
#include <memory>
#include <vector>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "systemutils/GlobalVars.h"
#include "systemutils/IDSystem.h"

using WindowInfo = std::tuple<GLint, GLint, const char*, GLFWmonitor*>;

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

private:
    WindowManager() noexcept;
    ~WindowManager() noexcept;

    auto initializeGLAD(WindowID ID) const -> void;

    GLuint maxWindows;
    GLuint windowsCount;
    GLuint nextFreeID;
    std::set<WindowID> freeIDs;
    std::unordered_map<GLuint, std::shared_ptr<GLFWwindow>> windows;
    std::queue<WindowID> windowsIDsToDelete;
    std::queue<WindowID> windowsIDsToReuse;
    std::queue<WindowID> windowsIDsToCreate;
    std::queue<WindowInfo> windowsCreationInfo;
    std::unordered_map<GLint, GLint> hintsDefaults;
};
