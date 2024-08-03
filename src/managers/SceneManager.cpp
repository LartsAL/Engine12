#include "managers/SceneManager.h"

#include <limits>
#include "Error.h"

SceneManager::SceneManager():
    maxScenes(),
    scenesCount(),
    nextFreeID() {}

SceneManager::~SceneManager() {
    shutdown();
}

auto SceneManager::getInstance() -> SceneManager& {
    static SceneManager instance;
    return instance;
}

auto SceneManager::initialize() -> void {
    maxScenes = std::numeric_limits<GLuint>::max();
    scenesCount = 0;
    nextFreeID = 1;
    freeIDs.clear();
    scenes.clear();
    while (!scenesIDsToDelete.empty()) {
        scenesIDsToDelete.pop();
    }
    while (!scenesIDsToReuse.empty()) {
        scenesIDsToReuse.pop();
    }
    while (!scenesIDsToCreate.empty()) {
        scenesIDsToCreate.pop();
    }
}

auto SceneManager::update() -> void {
    while (!scenesIDsToDelete.empty()) {
        SceneID ID = scenesIDsToDelete.front();
        scenesIDsToDelete.pop();
        scenes.erase(ID);
        freeIDs.insert(ID);
    }

    while (!scenesIDsToReuse.empty()) {
        SceneID ID = scenesIDsToReuse.front();
        scenesIDsToReuse.pop();
        scenes.erase(ID);
    }

    while (!scenesIDsToCreate.empty()) {
        SceneID ID = scenesIDsToCreate.front();
        scenesIDsToCreate.pop();
        const auto scene = std::make_shared<Scene>(ID);
        const auto [it, success] = scenes.insert(std::make_pair(ID, scene));
        if (!success) {
            PRINT_ERROR("Can't create new Scene.", "Given ID already exists. ID: {}", ID);
        }
    }
    scenesCount = scenes.size();

    for (const auto& [ID, scene] : scenes) {
        scene->update();
    }
}

auto SceneManager::shutdown() -> void {
    deleteAllScenes();
    while (!scenesIDsToCreate.empty()) {
        scenesIDsToCreate.pop();
    }
}

// TODO: Make comments for autodoc?
/**
 * @return ID of the newly created Scene. If Scene was not created, returns 0.
 */
auto SceneManager::createScene() -> SceneID {
    SceneID ID;
    if (scenesCount == maxScenes) {
        if (scenesIDsToDelete.empty()) {
            PRINT_ERROR("Can't create new Scene.", "Maximum number of Scenes reached: {}", maxScenes);
            return 0;
        } else {
            ID = scenesIDsToDelete.front();
            scenesIDsToDelete.pop();
            scenesIDsToReuse.push(ID);
            scenesIDsToCreate.push(ID);
            return ID;
        }
    }
    if (!freeIDs.empty()) {
        ID = *freeIDs.begin();
        freeIDs.erase(freeIDs.begin());
    } else {
        ID = nextFreeID++;
    }
    scenesIDsToCreate.push(ID);
    return ID;
}

auto SceneManager::deleteScene(SceneID ID) -> void {
    if (!scenes.contains(ID)) {
        PRINT_ERROR("Can't find Scene with given ID.", "ID: {}", ID);
        return;
    }
    scenesIDsToDelete.push(ID);
}

auto SceneManager::deleteAllScenes() -> void {
    scenes.clear();
    nextFreeID = 0;
    freeIDs.clear();
    while (!scenesIDsToDelete.empty()) {
        scenesIDsToDelete.pop();
    }
    while (!scenesIDsToReuse.empty()) {
        scenesIDsToReuse.pop();
    }
}

auto SceneManager::getScene(SceneID ID) -> std::shared_ptr<Scene> {
    if (scenes.contains(ID)) {
        return scenes.at(ID);
    } else {
        PRINT_ERROR("Can't find Scene with given ID.", "ID: {}", ID);
        return nullptr;
    }
}
