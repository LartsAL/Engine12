#pragma once

#include <queue>
#include <bitset>
#include <limits>
#include <memory>
#include <vector>
#include <unordered_map>
#include <glad/glad.h>
#include "components/Component.h"

using ObjectID    = GLuint;    // TODO: Сделать везде
using ComponentID = GLuint;

class ComponentManager {
public:
    static auto getInstance() -> ComponentManager&;

    auto initialize() -> void;
    auto update() -> void;
    auto shutdown() -> void;

    // TODO: Achtung! Test section
    std::unordered_map<std::string, auto (*)() -> std::shared_ptr<Component>> allComponentsMap;

    template<class ComponentType>
    auto registerComponent(std::string&& componentName) -> void;

    auto addComponent(GLuint ID, std::string&& componentName) -> void;
    // TODO: Achtung! Test section

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
    // TODO: Where the fuck is destructor?

    // ! Deprecated, candidate for deletion:
    static GLuint nextComponentBitPos;
    GLuint maxComponents;
    // TODO: Уменьши размер битсета, он будет 4.2 млрд
    // TODO: Заменить bitset на set, убрать в Component.h битмаску полностью

    // ! Deprecated, candidate for deletion:
    std::unordered_map<ObjectID, std::bitset<std::numeric_limits<GLuint>::max()>> bitmasks;                // Bitmasks to see which components Object have

    std::unordered_map<ObjectID, std::unordered_map<ComponentID, std::shared_ptr<Component>>> components;       // Mapping: Component ID -> pointer to Component

    std::queue<std::pair<ObjectID, std::bitset<std::numeric_limits<GLuint>::max()>>> componentsToAdd;      // Components which will be added during update
    std::queue<std::pair<ObjectID, std::bitset<std::numeric_limits<GLuint>::max()>>> componentsToRemove;   // Components which will be deleted during update

//    std::queue<std::pair<ObjectID, ComponentID>> componentsToAdd;      // Components which will be added during update
//    std::queue<std::pair<ObjectID, ComponentID>> componentsToRemove;   // Components which will be deleted during update
};

#include "ComponentManager.tpp"
