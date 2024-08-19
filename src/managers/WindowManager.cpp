#include "managers/WindowManager.h"

#include "managers/SceneManager.h"
#include "systemutils/EngineExceptions.h"
#include "systemutils/Error.h"
#include "systemutils/GlobalVars.h"

SceneManager& w_sceneManager = SceneManager::getInstance();

WindowManager::WindowManager() noexcept:
    idSystem("Window") {}

WindowManager::~WindowManager() noexcept {
    shutdown();
}

auto WindowManager::getInstance() noexcept -> WindowManager& {
    static WindowManager instance;
    return instance;
}

auto WindowManager::initialize() noexcept -> void {
    windows.clear();
    idSystem.reset();
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
    // Nothing to do here for now
}

auto WindowManager::shutdown() noexcept -> void {
    deleteAllWindows();
}

auto WindowManager::createWindow(SceneID linkedSceneID, GLint width, GLint height, const char* title,
                                 GLFWmonitor* monitor) -> WindowID {
    // Create unique ID
    WindowID ID = idSystem.createID();

    if (ID) {
        // Link given Scene with the created Window
        const auto scenePtr = w_sceneManager.getScene(linkedSceneID);
        if (!scenePtr) {
            idSystem.deleteID(ID);
            throw EngineException("Function returned nullptr.", ENGINE_EXCEPT_NULLPTR_RECEIVED);
        }

        WindowID sceneLinkedWindow = scenePtr->getLinkedWindow();
        if (sceneLinkedWindow) {
            idSystem.deleteID(ID);
            throw EngineException("Given Scene already linked with one Window.", ENGINE_EXCEPT_SCENE_ALREADY_LINKED);
        }

        scenePtr->setLinkedWindow(ID);

        // Window creation
        const auto window = std::make_shared<Window>(ID, linkedSceneID, width, height, title, monitor);
        if (!window) {
            idSystem.deleteID(ID);
            throw EngineException("Can't create instance of Window.", ENGINE_EXCEPT_OBJECT_CREATION_FAILED);
        }

        // Trying to insert into map
        const auto [it, success] = windows.insert(std::make_pair(ID, window));
        if (!success) {
            idSystem.deleteID(ID);
            PRINT_ERROR("Can't create new Window.", "Given ID already exists. ID: {}", ID);
            return 0;
        }

        initializeGLAD(ID);
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
    WindowID tmp = g_currentWindow;

    setActiveWindow(ID);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        throw EngineException("GLAD load failed.", ENGINE_EXCEPT_GLAD_INIT_FAILED);
    }

    if (tmp) {
        setActiveWindow(tmp);
    } else {
        g_currentWindow = ID;
    }
}

auto WindowManager::enableVSync(bool state) const noexcept -> void {
    glfwSwapInterval(state);
}

auto WindowManager::setWindowHint(GLint hint, GLint value) const noexcept -> void {
    if (hintsDefaults.contains(hint)) {
        glfwWindowHint(hint, value);
    } else {
        PRINT_ERROR("This hint is not supported.");
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

auto WindowManager::getWindowsCount() const noexcept -> GLuint {
    return idSystem.getIDCount();
}
