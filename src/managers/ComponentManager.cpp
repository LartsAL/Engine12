#include "managers/ComponentManager.h"

#include <limits>

// ! Deprecated, candidate for deletion:
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
        auto pair = componentsToAdd.front();
        bitmasks.at(pair.first) |= pair.second;
        componentsToAdd.pop();
    }
    while (!componentsToRemove.empty()) {
        auto pairInfo = componentsToRemove.front();
        bitmasks.at(pairInfo.first) &= ~pairInfo.second;
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

// TODO: Achtung! Test section
auto ComponentManager::addComponent(GLuint ID, std::string&& componentName) -> void {
    std::shared_ptr<Component> component = allComponentsMap.at(componentName)();
    components.at(ID).emplace(component->ID, component);
    componentsToAdd.emplace(ID, component->bitmask);
}
// TODO: Achtung! Test section

auto ComponentManager::clearComponents(GLuint ID) -> void {
    components.at(ID).clear();
}
