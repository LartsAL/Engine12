#include "objects/Scene.h"

#include "managers/WindowManager.h"
#include "systemutils/Error.h"

// TODO:
// #include "managers/ScriptManager.h"

// auto scriptManager = ScriptManager::getInstance();

WindowManager& windowManager = WindowManager::getInstance();

Scene::Scene(SceneID ID, bool active) noexcept:
    active(active),
    ID(ID),
    linkedWindow(0) {}

auto Scene::update() -> void {
    if (active) {
        for (const auto& objectID: sceneObjects) {
            // auto scripts = scriptManager.getScripts(objectID);
            // for (const auto& script : scripts) {
            //     script.update();
            // }
        }
    }
}

auto Scene::addObject(ObjectID objectID, const char* name) noexcept -> void {
    if(name && namesToObjects.contains(name)) {
        PRINT_ERROR("Can't insert Object into Scene", "Object with given name already exist. name: {}", name);
        return;
    }
    const auto [it, success] = sceneObjects.insert(objectID);
    if (!success) {
        PRINT_ERROR("Can't insert Object into Scene", "Given ID already exist. ID: {}", objectID);
        return;
    }
    if(name) {
        namesToObjects.insert(std::make_pair(name, objectID));
    }
}

auto Scene::removeObject(ObjectID objectID) noexcept -> void {
    if (sceneObjects.contains(objectID)) {
        sceneObjects.erase(objectID);
    }
}

auto Scene::isActive() const noexcept -> bool {
    return active;
}

auto Scene::getID() const noexcept -> SceneID {
    return ID;
}

auto Scene::setLinkedWindow(WindowID windowID) noexcept -> void {
    linkedWindow = windowID;
}

auto Scene::getLinkedWindow() const noexcept -> WindowID {
    return linkedWindow;
}
