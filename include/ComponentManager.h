#pragma once

#include <unordered_map>
#include <vector>
#include <queue>
#include <bitset>
#include <limits>
#include <memory>
#include <glad/glad.h>
#include "components/Component.h"

class ComponentManager {
public:
    static auto getInstance() -> ComponentManager&;

    auto initialize() -> void;
    auto update() -> void;
    auto shutdown() -> void;

    template<class ComponentType>
    auto addComponent(GLuint ID) -> void;
    template<class ComponentType>
    auto removeComponent(GLuint ID) -> void;
    template<class ComponentType>
    auto getComponent(GLuint ID) -> std::shared_ptr<ComponentType>;
    template<class ComponentType>
    auto hasComponent(GLuint ID) -> bool;
    template<class ComponentType>
    auto getAllComponentsOfType() -> std::vector<std::shared_ptr<ComponentType>>;
    template<class ComponentType>
    auto getAllObjectsWithComponent() -> std::vector<GLuint>;
    auto clearComponents(GLuint ID) -> void;

private:
    ComponentManager();

    static GLuint nextComponentBitPos;
    GLuint maxComponents;
    std::unordered_map<GLuint, std::bitset<std::numeric_limits<GLuint>::max()>> bitmasks;                // Bitmasks to see which components Object have
    std::unordered_map<GLuint, std::unordered_map<GLuint, std::shared_ptr<Component>>> components;       // Mapping: Component ID -> pointer to Component
    std::queue<std::pair<GLuint, std::bitset<std::numeric_limits<GLuint>::max()>>> componentsToAdd;      // Components which will be added during update
    std::queue<std::pair<GLuint, std::bitset<std::numeric_limits<GLuint>::max()>>> componentsToRemove;   // Components which will be deleted during update
};

#include "ComponentManager.tpp"
