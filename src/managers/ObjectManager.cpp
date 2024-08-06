#include "managers/ObjectManager.h"

#include <limits>

#include "objects/Object.h"
#include "systemutils/Error.h"

ObjectManager::ObjectManager():
    maxObjects(),
    objectsCount(),
    nextFreeID() {}

ObjectManager::~ObjectManager() {
    freeIDs.clear();
    objects.clear();
    while (!objectsIDsToDelete.empty()) {
        objectsIDsToDelete.pop();
    }
    while (!objectsIDsToCreate.empty()) {
        objectsIDsToCreate.pop();
    }
}

auto ObjectManager::getInstance() -> ObjectManager& {
    static ObjectManager instance;
    return instance;
}

auto ObjectManager::initialize() -> void {
    maxObjects = std::numeric_limits<GLuint>::max();
    objectsCount = 0;
    nextFreeID = 1;
    freeIDs.clear();
    objects.clear();
    while (!objectsIDsToDelete.empty()) {
        objectsIDsToDelete.pop();
    }
    while (!objectsIDsToReuse.empty()) {
        objectsIDsToReuse.pop();
    }
    while (!objectsIDsToCreate.empty()) {
        objectsIDsToCreate.pop();
    }
}

auto ObjectManager::update() -> void {
    // Delete all Objects from queue
    while (!objectsIDsToDelete.empty()) {
        ObjectID ID = objectsIDsToDelete.front();
        objectsIDsToDelete.pop();
        objects.erase(ID);
        freeIDs.insert(ID);
    }

    while (!objectsIDsToReuse.empty()) {
        ObjectID ID = objectsIDsToReuse.front();
        objectsIDsToReuse.pop();
        objects.erase(ID);
    }

    // Create all Objects from queue
    while (!objectsIDsToCreate.empty()) {
        GLuint ID = objectsIDsToCreate.front();
        objectsIDsToCreate.pop();

        const auto object = std::make_shared<Object>(ID);

        if (!object) {
            PRINT_ERROR("Failed to create an Object with given ID", "ID: {}", ID);
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
    while (!objectsIDsToCreate.empty()) {
        objectsIDsToCreate.pop();
    }
}

auto ObjectManager::createObject() -> ObjectID {
    ObjectID ID;
    // If we free some IDs, we can book them for new Objects
    if (objectsCount == maxObjects) {
        if (objectsIDsToDelete.empty()) {
            PRINT_ERROR("Can't create new Object.", "Maximum number of Objects reached: {}", maxObjects);
            return 0;
        } else {
            ID = objectsIDsToDelete.front();
            objectsIDsToDelete.pop();
            objectsIDsToReuse.push(ID);     // Now we're sure we can't book this ID again
            objectsIDsToCreate.push(ID);
            return ID;
        }
    }
    // Generate new unique ID
    if (!freeIDs.empty()) {
        ID = *freeIDs.begin();
        freeIDs.erase(freeIDs.begin());
    } else {
        ID = nextFreeID++;
    }
    objectsIDsToCreate.push(ID);
    return ID;
}

auto ObjectManager::deleteObject(ObjectID ID) -> void {
    if (!objects.contains(ID)) {
        PRINT_ERROR("Can't find Object with given ID.", "ID: {}", ID);
        return;
    }
    objectsIDsToDelete.push(ID);
}

auto ObjectManager::deleteAllObjects() -> void {
    objects.clear();
    nextFreeID = 0;
    freeIDs.clear();
    while (!objectsIDsToDelete.empty()) {
        objectsIDsToDelete.pop();
    }
    while (!objectsIDsToReuse.empty()) {
        objectsIDsToReuse.pop();
    }
}

auto ObjectManager::getMaxObjects() const -> GLuint {
    return maxObjects;
}

auto ObjectManager::getObjectsCount() const -> GLuint {
    return objectsCount;
}
