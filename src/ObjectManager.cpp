#include "ObjectManager.h"

#include <iostream>
#include <map>
#include <set>
#include <limits>
#include <glad/glad.h>
#include "Object.h"

ObjectManager::ObjectManager():
    maxObjects(),
    objectsCount(),
    nextFreeID() {}

ObjectManager::~ObjectManager() {
    freeIDs.clear();
    for (auto& pair : objects) {
        delete pair.second;
    }
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
    while (!objectsIDsToCreate.empty()) {
        objectsIDsToCreate.pop();
    }
}

auto ObjectManager::update() -> void {
    // Delete all objects from queue
    while (!objectsIDsToDelete.empty()) {
        GLuint ID = objectsIDsToDelete.front();
        objectsIDsToDelete.pop();
        Object* obj = objects[ID];
        objects.erase(ID);
        delete obj;
        freeIDs.insert(ID);
    }
    // Create all objects from queue
    while (!objectsIDsToCreate.empty()) {
        GLuint ID = objectsIDsToCreate.front();
        objectsIDsToCreate.pop();
        auto* createdObject = new Object(ID);
        objects[ID] = createdObject;
    }
    objectsCount = objects.size();
}

auto ObjectManager::createObject() -> GLuint {
    GLuint ID;
    // If we free some IDs, we can book them for new Objects
    if (objects.size() == maxObjects) {
        if (objectsIDsToDelete.empty()) {
            std::cerr << "ERROR::OBJECT_MANAGER::MAX_OBJECT_COUNT_REACHED" << std::endl;
            return 0;
        } else {
            ID = objectsIDsToDelete.front();
            objectsIDsToCreate.push(ID);
            return ID;
        }
    }
    // Generate new unique ID
    if (!freeIDs.empty()) {
        ID = *freeIDs.begin();
        freeIDs.erase(ID);
    } else {
        ID = nextFreeID++;
    }
    objectsIDsToCreate.push(ID);
    return ID;
}

auto ObjectManager::deleteObject(GLuint ID) -> void {
    if (objects.find(ID) == objects.end()) {
        std::cerr << "ERROR::OBJECT_MANAGER::INVALID_OBJECT_ID\n" << ID << std::endl;
        return;
    }
    objectsIDsToDelete.push(ID);
}

auto ObjectManager::getMaxObjects() const -> GLuint {
    return maxObjects;
}

auto ObjectManager::getObjectsCount() const -> GLuint {
    return objectsCount;
}
