#include "managers/ObjectManager.h"

#include <limits>

#include "objects/Object.h"
#include "systemutils/Error.h"

ObjectManager::ObjectManager():
    idSystem("Object"),
    objectsCount() {}

ObjectManager::~ObjectManager() {
    idSystem.reset();
}

auto ObjectManager::getInstance() -> ObjectManager& {
    static ObjectManager instance;
    return instance;
}

auto ObjectManager::initialize() -> void {
    objects.clear();
    objectsCount = 0;
    idSystem.reset();
}

auto ObjectManager::update() -> void {
    // Delete all Objects from queue
    while (!idSystem.toDelete.empty()) {
        ObjectID ID = idSystem.toDelete.front();
        idSystem.toDelete.pop();
        objects.erase(ID);
        idSystem.freeIDs.insert(ID);
    }

    while (!idSystem.toReuse.empty()) {
        ObjectID ID = idSystem.toReuse.front();
        idSystem.toReuse.pop();
        objects.erase(ID);
    }

    // Create all Objects from queue
    while (!idSystem.toCreate.empty()) {
        GLuint ID = idSystem.toCreate.front();
        idSystem.toCreate.pop();

        const auto object = std::make_shared<Object>(ID);

        if (!object) {
            PRINT_ERROR("Failed to create an Object with given ID", "ID: {}", ID);
            continue;
        }

        const auto [it, success] = objects.insert(std::make_pair(ID, object));
        if (!success) {
            PRINT_ERROR("Can't create new Object.", "Given ID already exists. ID: {}", ID);
        }
    }
    objectsCount = objects.size();
}

auto ObjectManager::shutdown() -> void {
    deleteAllObjects();
}

auto ObjectManager::createObject() -> ObjectID {
    return idSystem.createID();
}

auto ObjectManager::deleteObject(ObjectID ID) -> void {
    if (!objects.contains(ID)) {
        PRINT_ERROR("Can't find Object with given ID.", "ID: {}", ID);
        return;
    }
    idSystem.deleteID(ID);
}

auto ObjectManager::deleteAllObjects() -> void {
    objects.clear();
    idSystem.reset();
}

auto ObjectManager::getMaxObjects() const -> GLuint {
    return idSystem.max;
}

auto ObjectManager::getObjectsCount() const -> GLuint {
    return objectsCount;
}