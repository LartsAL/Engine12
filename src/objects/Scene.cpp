#include "objects/Scene.h"

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
