#pragma once

#include "managers/SceneManager.h"
#include "managers/WindowManager.h"
#include "managers/FileManager.h"
#include "managers/ObjectManager.h"
#include "managers/ComponentManager.h"
// #include "AllOtherManagers.h"
// ...

class Core {
public:
    static auto getInstance() -> Core&;

    auto initialize() -> void;
    auto update() -> void;
    auto shutdown() -> void;

    auto isActive() -> bool;

private:
    Core();
    ~Core();

    bool active;

    // Managers
    SceneManager& sceneManager;
    WindowManager& windowManager;
    FileManager& fileManager;
    ObjectManager& objectManager;
    ComponentManager& componentManager;
};
