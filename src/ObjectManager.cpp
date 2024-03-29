#include "ObjectManager.h"

#include <iostream>
#include <map>
#include <set>
#include <limits>
#include <glad/glad.h>
#include "Object.h"

using namespace std;

ObjectManager::ObjectManager(): maxObjects(numeric_limits<GLuint>::max()), nextFreeID(1) {}

auto ObjectManager::getInstance() -> ObjectManager& {
    static ObjectManager instance;
    return instance;
}

auto ObjectManager::createObject() -> Object& {
    if (objects.size() == maxObjects) {
        cerr << "ERROR::OBJECT_MANAGER::MAX_OBJECT_COUNT_REACHED" << endl;
        auto* errorObject = new Object();
        return *errorObject;
    }

    GLuint ID;
    if (!freeIDs.empty()) {
        ID = *freeIDs.begin();
        freeIDs.erase(ID);
    } else {
        ID = nextFreeID++;
    }
    auto* createdObject = new Object(ID);
    objects[ID] = createdObject;
    return *createdObject;
}

auto ObjectManager::deleteObject(GLuint ID) -> void {
    if (objects.find(ID) == objects.end()) {
        cerr << "ERROR::OBJECT_MANAGER::INVALID_OBJECT_ID\n" << ID << endl;
        return;
    }

    Object* objectToDelete = objects[ID];
    objects.erase(ID);
    delete[] objectToDelete;
    freeIDs.insert(ID);
}
