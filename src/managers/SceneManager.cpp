#include "managers/SceneManager.h"

#include <limits>
#include "systemutils/Error.h"

SceneManager::SceneManager():
    idSystem("Scene"),
    scenesCount() {}

SceneManager::~SceneManager() {
    shutdown();
}

auto SceneManager::getInstance() -> SceneManager& {
    static SceneManager instance;
    return instance;
}

auto SceneManager::initialize() -> void {
    scenesCount = 0;
    scenes.clear();
    idSystem.reset();
}

auto SceneManager::update() -> void {
    while (!idSystem.toCreate.empty()) {
        SceneID ID = idSystem.toCreate.front();
        idSystem.toCreate.pop();

        const auto scene = std::make_shared<Scene>(ID);
        if (!scene) {
            PRINT_ERROR("Failed to create a Scene with given ID", "ID: {}", ID);
            continue;
        }

        const auto [it, success] = scenes.insert(std::make_pair(ID, scene));
        if (!success) {
            PRINT_ERROR("Can't create new Scene.", "Given ID already exists. ID: {}", ID);
        }
    }

    while (!idSystem.toDelete.empty()) {
        SceneID ID = idSystem.toDelete.front();
        idSystem.toDelete.pop();
        scenes.erase(ID);
        idSystem.freeIDs.insert(ID);
    }

    scenesCount = scenes.size();

    for (const auto& [ID, scene] : scenes) {
        scene->update();
    }
}

auto SceneManager::shutdown() -> void {
    deleteAllScenes();
    idSystem.reset();
}

// TODO: Make comments for autodoc?
/**
 * @return ID of the newly created Scene. If Scene was not created, returns 0.
 */
auto SceneManager::createScene() -> SceneID {
    return idSystem.createID();
}

auto SceneManager::deleteScene(SceneID ID) -> void {
    if (!scenes.contains(ID)) {
        PRINT_ERROR("Can't find Scene with given ID.", "ID: {}", ID);
        return;
    }
    idSystem.deleteID(ID);
}

auto SceneManager::deleteAllScenes() -> void {
    scenes.clear();
    idSystem.reset();
}

auto SceneManager::getScene(SceneID ID) -> std::shared_ptr<Scene> {
    if (scenes.contains(ID)) {
        return scenes.at(ID);
    } else {
        PRINT_ERROR("Can't find Scene with given ID.", "ID: {}", ID);
        return nullptr;
    }
}
