#pragma once

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

private:
    Core();
    ~Core();

    // Managers
    WindowManager& windowManager;
    FileManager& fileManager;
    ObjectManager& objectManager;
    ComponentManager& componentManager;
};
