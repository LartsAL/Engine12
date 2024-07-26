#include <iostream>
#include <type_traits>
#include "components/Component.h"

// TODO: Achtung! Test section
template<class ComponentType>
auto ComponentManager::registerComponent(std::string&& componentName) -> void {
    void (*pAdd)(GLuint) = &addComponent<ComponentType>;
    // other satanic stuff

    allComponentsMap.emplace(componentName, []() -> void { return std::make_shared<ComponentType>(); });
}
// TODO: Achtung! Test section

template<class ComponentType>
auto ComponentManager::addComponent(ObjectID ID) -> void {
    if (!std::is_base_of<Component, ComponentType>::value) {
        std::cerr << "ERROR::COMPONENT_MANAGER::ADD_COMPONENT::INVALID_COMPONENT_TYPE" << std::endl;
        return;
    }
    if (ComponentType::bitmask.count() == 0) {
        ComponentType::bitmask.set(nextComponentBitPos, 1);
        ComponentType::ID = nextComponentBitPos++;
    }
    components[ID].emplace(ComponentType::ID, std::make_shared<ComponentType>());
    componentsToAdd.push(std::make_pair(ID, ComponentType::bitmask));
}

template<class ComponentType>
auto ComponentManager::removeComponent(GLuint ID) -> void {
    if (!std::is_base_of<Component, ComponentType>::value) {
        std::cerr << "ERROR::COMPONENT_MANAGER::REMOVE_COMPONENT::INVALID_COMPONENT_TYPE" << std::endl;
        return;
    }
    components[ID].erase(ComponentType::ID);  // Shared_ptr will automatically delete Component object
    componentsToRemove.push(std::make_pair(ID, ComponentType::bitmask));
}

template<class ComponentType>
auto ComponentManager::getComponent(GLuint ID) -> std::shared_ptr<ComponentType> {
    return components[ID][ComponentType::ID];
}

template<class ComponentType>
auto ComponentManager::hasComponent(GLuint ID) -> bool {
    return bitmasks[ID] & ComponentType::bitmask;
}

template<class ComponentType>
auto ComponentManager::getAllComponentsOfType() -> std::vector<std::shared_ptr<ComponentType>> {
    std::vector<std::shared_ptr<ComponentType>> result;
    for (const auto& pair : components) {
        if (hasComponent<ComponentType>(pair.first)) {
            result.push_back(pair.second[ComponentType::ID]);
        }
    }
    return result;
}

template<class ComponentType>
auto ComponentManager::getAllObjectsWithComponent() -> std::vector<GLuint> {
    std::vector<GLuint> result;
    for (const auto& pair : components) {
        if (hasComponent<ComponentType>(pair.first)) {
            result.push_back(pair.first);
        }
    }
    return result;
}
