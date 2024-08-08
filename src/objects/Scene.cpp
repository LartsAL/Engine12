#include "objects/Scene.h"

#include "systemutils/Error.h"

// TODO:
// #include "managers/ScriptManager.h"

// auto scriptManager = ScriptManager::getInstance();

Scene::Scene(SceneID ID, bool active):
    active(active),
    ID(ID),
    linkedWindow(0) {}

auto Scene::update() -> void {
    if (active) {
        for (const auto &objectID: sceneObjects) {
            // auto scripts = scriptManager.getScripts(objectID);
            // for (const auto& script : scripts) {
            //     script.update();
            // }
        }
    }
}

auto Scene::addObject(ObjectID objectID) -> void {
    const auto [it, success] = sceneObjects.insert(objectID);
    if (!success) {
        PRINT_ERROR("Can't insert Object into Scene", "Given ID already exist. ID: {}", objectID);
    }
}

auto Scene::removeObject(ObjectID objectID) -> void {
    if (sceneObjects.contains(objectID)) {
        sceneObjects.erase(objectID);
    }
}

auto Scene::isActive() -> bool {
    return active;
}

auto Scene::getID() -> SceneID {
    return ID;
}

auto Scene::getLinkedWindow() -> WindowID {
    return linkedWindow;
}
