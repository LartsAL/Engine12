#include "Core.h"

Core::Core():
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
    windowManager.initialize();
    fileManager.initialize();
    objectManager.initialize();
    componentManager.initialize();
    // otherManagers.initialize();
    // ...
}

auto Core::update() -> void {

}

auto Core::shutdown() -> void {
    windowManager.shutdown();
    fileManager.shutdown();
    objectManager.shutdown();
    componentManager.shutdown();
    // otherManagers.shutdown()
    // ...
}
