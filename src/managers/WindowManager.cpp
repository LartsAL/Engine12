#include "managers/WindowManager.h"

#include <iostream>
#include "managers/SceneManager.h"
#include "EngineExceptions.h"
#include "Error.h"
#include "GlobalVars.h"

SceneManager& sceneManager = SceneManager::getInstance();

WindowManager::WindowManager() noexcept:
    maxWindows(),
    windowsCount(),
    nextFreeID() {}

WindowManager::~WindowManager() noexcept {
    shutdown();
}

auto WindowManager::getInstance() noexcept -> WindowManager& {
    static WindowManager instance;
    return instance;
}

auto WindowManager::initialize() noexcept -> void {
    maxWindows = std::numeric_limits<GLuint>::max();
    windowsCount = 0;
    nextFreeID = 1;
    freeIDs.clear();
    windows.clear();
    while (!windowsIDsToDelete.empty()) {
        windowsIDsToDelete.pop();
    }
    while (!windowsIDsToReuse.empty()) {
        windowsIDsToReuse.pop();
    }
    while (!windowsIDsToCreate.empty()) {
        windowsIDsToCreate.pop();
    }
    while (!windowsCreationInfo.empty()) {
        windowsCreationInfo.pop();
    }
    hintsDefaults = {
            {GLFW_RESIZABLE, GLFW_TRUE},
            {GLFW_VISIBLE, GLFW_TRUE},
            {GLFW_DECORATED, GLFW_TRUE},
            {GLFW_FOCUSED, GLFW_TRUE},
            {GLFW_AUTO_ICONIFY, GLFW_TRUE},
            {GLFW_FLOATING, GLFW_FALSE},
            {GLFW_MAXIMIZED, GLFW_FALSE},
            {GLFW_CENTER_CURSOR, GLFW_TRUE},
            {GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_FALSE},
            {GLFW_FOCUS_ON_SHOW, GLFW_TRUE},
            {GLFW_SCALE_TO_MONITOR, GLFW_FALSE},
            {GLFW_STEREO, GLFW_FALSE},
            {GLFW_SAMPLES, 0},
            {GLFW_SRGB_CAPABLE, GLFW_FALSE},
            {GLFW_DOUBLEBUFFER, GLFW_TRUE},
            {GLFW_REFRESH_RATE, GLFW_DONT_CARE}
    };
}

auto WindowManager::update() -> void {
    while (!windowsIDsToDelete.empty()) {
        WindowID ID = windowsIDsToDelete.front();
        windowsIDsToDelete.pop();
        windows.erase(ID);
        freeIDs.insert(ID);
    }

    while (!windowsIDsToReuse.empty()) {
        WindowID ID = windowsIDsToReuse.front();
        windowsIDsToReuse.pop();
        windows.erase(ID);
    }

    while (!windowsIDsToCreate.empty()) {
        WindowID ID = windowsIDsToCreate.front();
        windowsIDsToCreate.pop();

        auto [width, height, title,
              monitor] = windowsCreationInfo.front();
        windowsCreationInfo.pop();

        const auto window = std::shared_ptr<GLFWwindow>(
                glfwCreateWindow(width, height, title, monitor, nullptr),
                [](GLFWwindow* window) -> void { if (window) glfwDestroyWindow(window); }
        );

        if (!window) {
            throw EngineException("Window creation failed.", 1);
        }

        const auto [it, success] = windows.insert(std::make_pair(ID, window));
        if (!success) {
            PRINT_ERROR("Can't create new Window.", "Given ID already exists. ID: {}", ID);
            continue;
        }

        WindowID tmp = g_currentWindow;

        initializeGLAD(ID);

        if (tmp) {
            setActiveWindow(tmp);
        } else {
            g_currentWindow = ID;
        }
    }
}

auto WindowManager::shutdown() noexcept -> void {
    deleteAllWindows();
    while (!windowsIDsToCreate.empty()) {
        windowsIDsToCreate.pop();
    }
    while (!windowsCreationInfo.empty()) {
        windowsCreationInfo.pop();
    }
}

auto WindowManager::createWindow(SceneID linkedSceneID, GLint width, GLint height, const char* title,
                                 GLFWmonitor* monitor) -> WindowID {
    const auto scenePtr = sceneManager.getScene(linkedSceneID);
    if (!scenePtr) {
        PRINT_ERROR("Given Scene ID is invalid.", "ID: {}", linkedSceneID);
        return 0;
    }

    WindowID sceneLinkedWindow = scenePtr->getLinkedWindow();
    if (sceneLinkedWindow) {
        PRINT_ERROR("Given Scene already linked with one Window.", "Window ID: {}", sceneLinkedWindow);
        return 0;
    }

    WindowID ID;
    if (windowsCount == maxWindows) {
        if (windowsIDsToDelete.empty()) {
            PRINT_ERROR("Can't create new Window.", "Maximum number of Windows reached: {}", maxWindows);
            return 0;
        } else {
            ID = windowsIDsToDelete.front();
            windowsIDsToDelete.pop();
            windowsIDsToReuse.push(ID);
            windowsIDsToCreate.push(ID);
            return ID;
        }
    }
    if (!freeIDs.empty()) {
        ID = *freeIDs.begin();
        freeIDs.erase(freeIDs.begin());
    } else {
        ID = nextFreeID++;
    }
    windowsIDsToCreate.push(ID);
    windowsCreationInfo.emplace(width, height, title, monitor);

    return ID;
}

auto WindowManager::deleteWindow(WindowID ID) noexcept -> void {
    if (!windows.contains(ID)) {
        PRINT_ERROR("Can't find Window with given ID.", "ID: {}", ID);
        return;
    }
    windowsIDsToDelete.push(ID);
}

auto WindowManager::deleteAllWindows() noexcept -> void {
    windows.clear();
    nextFreeID = 0;
    freeIDs.clear();
    while (!windowsIDsToDelete.empty()) {
        windowsIDsToDelete.pop();
    }
    while (!windowsIDsToReuse.empty()) {
        windowsIDsToReuse.pop();
    }
}

auto WindowManager::setActiveWindow(WindowID ID) const noexcept -> void {
    if (!windows.contains(ID)) {
        PRINT_ERROR("Can't find Window with given ID.", "ID: {}", ID);
        return;
    }
    const auto window = windows.at(ID);
    glfwMakeContextCurrent(window.get());
    g_currentWindow = ID;
}

auto WindowManager::initializeGLAD(WindowID ID) const -> void {
    setActiveWindow(ID);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        throw EngineException("GLAD load failed.", 3);
    }
}

auto WindowManager::enableVSync(bool state) const noexcept -> void {
    glfwSwapInterval(state);
}

auto WindowManager::setWindowHint(GLint hint, GLint value) const noexcept -> void {
    if (hintsDefaults.contains(hint)) {
        glfwWindowHint(hint, value);
    } else {
        PRINT_ERROR("Hint is not supported.");
    }
}

auto WindowManager::resetWindowHints() const noexcept -> void {
    for (const auto [hint, value] : hintsDefaults) {
        glfwWindowHint(hint, value);
    }
}

