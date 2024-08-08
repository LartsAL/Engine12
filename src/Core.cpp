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
    if (!active) {
        sceneManager.initialize();
        windowManager.initialize();
        fileManager.initialize();
        objectManager.initialize();
        componentManager.initialize();
        // otherManagers.initialize();
        // ...

        active = true;
    }
}

auto Core::update() -> void {
    if (active) {
        try {
            sceneManager.update();
            windowManager.update();
            //fileManager.update();
            objectManager.update();
            //componentManager.update();
            // otherManagers.update()
            // ...
        } catch (EngineException &exception) {
            PRINT_ERROR("Core::update() failed due to Engine exception. Shutting down.",
                        "Code: {}\nWhat: {}\n", exception.code(), exception.what());
            shutdown();
        } catch (std::exception &exception) {
            PRINT_ERROR("Core::update() failed due to std::exception. Shutting down.",
                        "What: {}", exception.what());
            shutdown();
        }
    }
}

auto Core::shutdown() -> void {
    if (active) {
        sceneManager.shutdown();
        windowManager.shutdown();
        fileManager.shutdown();
        objectManager.shutdown();
        componentManager.shutdown();
        // otherManagers.shutdown()
        // ...

        active = false;
    }
}

auto Core::isActive() -> bool {
    return active;
}
