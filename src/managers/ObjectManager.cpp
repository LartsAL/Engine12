#include "managers/ObjectManager.h"

#include <limits>

#include "objects/Object.h"
#include "managers/SceneManager.h"
#include "systemutils/Error.h"
#include "systemutils/EngineExceptions.h"

SceneManager& o_sceneManager = SceneManager::getInstance();

ObjectManager::ObjectManager() noexcept:
    idSystem("Object") {}

ObjectManager::~ObjectManager() noexcept {
    idSystem.reset();
}

auto ObjectManager::getInstance() noexcept -> ObjectManager& {
    static ObjectManager instance;
    return instance;
}

auto ObjectManager::initialize() noexcept -> void {
    objects.clear();
    idSystem.reset();
}

auto ObjectManager::update() -> void {
    // Nothing to do here for now
}

auto ObjectManager::shutdown() noexcept -> void {
    deleteAllObjects();
}

auto ObjectManager::createObject(SceneID sceneID) -> ObjectID {
    // Validate SceneID
    const auto scenePtr = o_sceneManager.getScene(sceneID);
    if (!scenePtr) {
        PRINT_ERROR("Given Scene ID is invalid.", "ID: {}", sceneID);
        return 0;
    }

    // Create unique ID
    ObjectID ID = idSystem.createID();
    if (ID) {
        const auto object = std::make_shared<Object>(ID);
        if (!object) {
            idSystem.deleteID(ID);
            throw EngineException("Can't create instance of Object", ENGINE_EXCEPT_OBJECT_CREATION_FAILED);
        }

        const auto [it, success] = objects.insert(std::make_pair(ID, object));
        if (!success) {
            PRINT_ERROR("Can't create new Object.", "Given ID already exists. ID: {}", ID);
            idSystem.deleteID(ID);
            return 0;
        }

        scenePtr->addObject(ID);
    }

    return ID;
}

auto ObjectManager::deleteObject(SceneID sceneID, ObjectID ID) noexcept -> void {
    if (!objects.contains(ID)) {
        PRINT_ERROR("Can't find Object with given ID.", "ID: {}", ID);
        return;
    }
    const auto scenePtr = o_sceneManager.getScene(sceneID);
    if (!scenePtr) {
        PRINT_ERROR("Given Scene ID is invalid.", "ID: {}", sceneID);
        return;
    }
    scenePtr->removeObject(ID);
    objects.erase(ID);
    idSystem.deleteID(ID);
}

auto ObjectManager::deleteAllObjects() noexcept -> void {
    objects.clear();
    idSystem.reset();
}

auto ObjectManager::getMaxObjects() const noexcept -> GLuint {
    return idSystem.getMax();
}

auto ObjectManager::getObjectsCount() const noexcept -> GLuint {
    return idSystem.getIDCount();
}