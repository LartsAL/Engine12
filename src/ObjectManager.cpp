#include "ObjectManager.h"

#include <iostream>
#include <map>
#include <set>
#include <limits>
#include <glad/glad.h>
#include "Object.h"

using namespace std;

ObjectManager::ObjectManager():
    maxObjects(numeric_limits<GLuint>::max()),
    objectsCount(0),
    nextFreeID(1) {}

auto ObjectManager::getInstance() -> ObjectManager& {
    static ObjectManager instance;
    return instance;
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
            cerr << "ERROR::OBJECT_MANAGER::MAX_OBJECT_COUNT_REACHED" << endl;
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
        cerr << "ERROR::OBJECT_MANAGER::INVALID_OBJECT_ID\n" << ID << endl;
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
