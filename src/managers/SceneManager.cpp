#include "managers/SceneManager.h"

#include <limits>
#include "systemutils/Error.h"
#include "systemutils/EngineExceptions.h"

SceneManager::SceneManager() noexcept:
    idSystem("Scene") {}

SceneManager::~SceneManager() noexcept {
    shutdown();
}

auto SceneManager::getInstance() noexcept -> SceneManager& {
    static SceneManager instance;
    return instance;
}

auto SceneManager::initialize() noexcept -> void {
    scenes.clear();
    idSystem.reset();
}

auto SceneManager::update() -> void {
    for (const auto& [ID, scene] : scenes) {
        scene->update();
    }
}

auto SceneManager::shutdown() noexcept -> void {
    deleteAllScenes();
    idSystem.reset();
}

// TODO: Make comments for autodoc?
/**
 * @return ID of the newly created Scene. If Scene was not created, returns 0.
 */
auto SceneManager::createScene() -> SceneID {
    // Create unique ID
    SceneID ID = idSystem.createID();

    if (ID) {
        // Scene creation
        const auto scene = std::make_shared<Scene>(ID);
        if (!scene) {
            throw EngineException("Cannot create instance of Scene.", ENGINE_EXCEPT_OBJECT_CREATION_FAILED);
        }

        // Trying to insert into map
        const auto [it, success] = scenes.insert(std::make_pair(ID, scene));
        if (!success) {
            PRINT_ERROR("Can't create new Scene.", "Given ID already exists. ID: {}", ID);
            return 0;
        }
    }

    return ID;
}

auto SceneManager::deleteScene(SceneID ID) noexcept -> void {
    if (!scenes.contains(ID)) {
        PRINT_ERROR("Can't find Scene with given ID.", "ID: {}", ID);
        return;
    }
    idSystem.deleteID(ID);
}

auto SceneManager::deleteAllScenes() noexcept -> void {
    scenes.clear();
    idSystem.reset();
}

auto SceneManager::getScene(SceneID ID) const noexcept -> std::shared_ptr<Scene> {
    if (scenes.contains(ID)) {
        return scenes.at(ID);
    } else {
        PRINT_ERROR("Can't find Scene with given ID.", "ID: {}", ID);
        return nullptr;
    }
}

auto SceneManager::getScenesCount() const noexcept -> GLuint {
    return idSystem.getIDCount();
}
