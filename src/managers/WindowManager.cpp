#include "managers/WindowManager.h"

#include "managers/SceneManager.h"
#include "systemutils/EngineExceptions.h"
#include "systemutils/Error.h"
#include "systemutils/GlobalVars.h"

SceneManager& w_sceneManager = SceneManager::getInstance();

WindowManager::WindowManager() noexcept:
    idSystem("Window"),
    windowsCount() {}

WindowManager::~WindowManager() noexcept {
    shutdown();
}

auto WindowManager::getInstance() noexcept -> WindowManager& {
    static WindowManager instance;
    return instance;
}

auto WindowManager::initialize() noexcept -> void {
    windowsCount = 0;
    windows.clear();
    idSystem.reset();
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
    while (!idSystem.toCreate.empty()) {
        WindowID ID = idSystem.toCreate.front();
        idSystem.toCreate.pop();

        const auto [linkedSceneID, width, height,
              title, monitor] = windowsCreationInfo.front();
        windowsCreationInfo.pop();

        const auto scenePtr = w_sceneManager.getScene(linkedSceneID);
        if (!scenePtr) {
            throw EngineException("Function returned nullptr.", ENGINE_EXCEPT_NULLPTR_RECEIVED);
        }

        WindowID sceneLinkedWindow = scenePtr->getLinkedWindow();
        if (sceneLinkedWindow) {
            throw EngineException("Given Scene already linked with one Window.", ENGINE_EXCEPT_SCENE_ALREADY_LINKED);
        }

        scenePtr->setLinkedWindow(ID);

        const auto window = std::make_shared<Window>(
                ID, linkedSceneID, width, height, title, monitor
        );

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

    while (!idSystem.toDelete.empty()) {
        WindowID ID = idSystem.toDelete.front();
        idSystem.toDelete.pop();
        windows.erase(ID);
        idSystem.freeIDs.insert(ID);
    }

    windowsCount = windows.size();
}

auto WindowManager::shutdown() noexcept -> void {
    deleteAllWindows();
}

auto WindowManager::createWindow(SceneID linkedSceneID, GLint width, GLint height, const char* title,
                                 GLFWmonitor* monitor) -> WindowID {
    WindowID ID = idSystem.createID();
    if (ID) {
        windowsCreationInfo.emplace(linkedSceneID, width, height, title, monitor);
    }
    return ID;
}

auto WindowManager::deleteWindow(WindowID ID) noexcept -> void {
    if (!windows.contains(ID)) {
        PRINT_ERROR("Can't find Window with given ID.", "ID: {}", ID);
        return;
    }
    idSystem.deleteID(ID);
}

auto WindowManager::deleteAllWindows() noexcept -> void {
    windows.clear();
    idSystem.reset();
    while (!windowsCreationInfo.empty()) {
        windowsCreationInfo.pop();
    }
}

auto WindowManager::setActiveWindow(WindowID ID) const noexcept -> void {
    if (!ID) {
        glfwMakeContextCurrent(nullptr);        // If 0 is passed, detach context
        return;
    }
    if (!windows.contains(ID)) {
        PRINT_ERROR("Can't find Window with given ID.", "ID: {}", ID);
        return;
    }
    const auto window = windows.at(ID);
    glfwMakeContextCurrent(window->getRawPointer());
    g_currentWindow = ID;
}

auto WindowManager::initializeGLAD(WindowID ID) const -> void {
    setActiveWindow(ID);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        throw EngineException("GLAD load failed.", ENGINE_EXCEPT_GLAD_INIT_FAILED);
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

auto WindowManager::getWindowPtr(WindowID ID) const noexcept -> std::shared_ptr<Window> {
    if (!windows.contains(ID)) {
        PRINT_ERROR("Can't find Window with given ID.", "ID: {}", ID);
        return nullptr;
    }
    return windows.at(ID);
}
