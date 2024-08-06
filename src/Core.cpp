#include "Core.h"

#include "systemutils/EngineExceptions.h"
#include "systemutils/Error.h"

Core::Core():
    sceneManager(SceneManager::getInstance()),
    windowManager(WindowManager::getInstance()),
    fileManager(FileManager::getInstance()),
    objectManager(ObjectManager::getInstance()),
    componentManager(ComponentManager::getInstance()) {}

Core::~Core() {
    shutdown();
}

auto Core::getInstance() -> Core& {
    static Core instance;
    return instance;
}

auto Core::initialize() -> void {
    sceneManager.initialize();
    windowManager.initialize();
    fileManager.initialize();
    objectManager.initialize();
    componentManager.initialize();
    // otherManagers.initialize();
    // ...
}

auto Core::update() -> void {
    try {
        sceneManager.update();
        windowManager.update();
        //fileManager.update();
        objectManager.update();
        componentManager.update();
        // otherManagers.update()
        // ...
    } catch (EngineException& exception) {
        PRINT_ERROR("Core::update() failed due to Engine exception. Shutting down.",
                    "Code: {}\nWhat: {}\n", exception.code(), exception.what());
        shutdown();
    } catch (std::exception& exception) {
        PRINT_ERROR("Core::update() failed due to std::exception.",
                    "What: {}", exception.what());
    }
}

auto Core::shutdown() -> void {
    sceneManager.shutdown();
    windowManager.shutdown();
    fileManager.shutdown();
    objectManager.shutdown();
    componentManager.shutdown();
    // otherManagers.shutdown()
    // ...
}
