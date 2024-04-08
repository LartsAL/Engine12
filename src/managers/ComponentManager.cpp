#include "managers/ComponentManager.h"

#include <limits>

GLuint ComponentManager::nextComponentBitPos = 0;

ComponentManager::ComponentManager():
    maxComponents() {}

auto ComponentManager::getInstance() -> ComponentManager& {
    static ComponentManager instance;
    return instance;
}

auto ComponentManager::initialize() -> void {
    maxComponents = std::numeric_limits<GLuint>::max();
    nextComponentBitPos = 0;
    bitmasks.clear();
    while (!componentsToAdd.empty()) {
        componentsToAdd.pop();
    }
    while (!componentsToRemove.empty()) {
        componentsToRemove.pop();
    }
}

auto ComponentManager::update() -> void {
    while (!componentsToAdd.empty()) {
        std::pair pair = componentsToAdd.front();
        bitmasks[pair.first] |= pair.second;
        componentsToAdd.pop();
    }
    while (!componentsToRemove.empty()) {
        std::pair pairInfo = componentsToRemove.front();
        bitmasks[pairInfo.first] &= ~pairInfo.second;
        componentsToRemove.pop();
    }
}

auto ComponentManager::shutdown() -> void {
    bitmasks.clear();
    for (auto& pair : components) {
        pair.second.clear();
    }
    components.clear();
    while (!componentsToAdd.empty()) {
        componentsToAdd.pop();
    }
    while (!componentsToRemove.empty()) {
        componentsToRemove.pop();
    }
}

auto ComponentManager::clearComponents(GLuint ID) -> void {
    components[ID].clear();
}

// Template functions
//     auto addComponent(GLuint ID) -> void;
//     auto removeComponent(GLuint ID) -> void;
//     auto getComponent(GLuint ID) -> std::shared_ptr<ComponentType>;
//     auto hasComponent(GLuint ID) -> bool;
//     auto getAllComponentsOfType() -> std::vector<std::shared_ptr<ComponentType>>;
//     auto getAllObjectsWithComponent() -> std::vector<GLuint>;
// are defined in ComponentManager.tpp
