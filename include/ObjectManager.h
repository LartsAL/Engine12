#pragma once

#include <map>
#include <set>
#include <memory>
#include <queue>
#include <glad/glad.h>
#include "Object.h"

class ObjectManager {
public:
    static auto getInstance() -> ObjectManager&;

    auto initialize() -> void;
    auto update() -> void;
    auto shutdown() -> void;

    auto createObject() -> GLuint;
    auto deleteObject(GLuint ID) -> void;
    auto deleteAllObjects() -> void;
    auto getMaxObjects() const -> GLuint;
    auto getObjectsCount() const -> GLuint;

private:
    ObjectManager();
    ~ObjectManager();

    GLuint maxObjects;                                       // Maximum existing objects
    GLuint objectsCount;                                     // Number of existing objects
    GLuint nextFreeID;                                       // Next available ID for new Object
    std::set<GLuint> freeIDs;                                // Reusable IDs freed from deleted Objects
    std::map<GLuint, std::shared_ptr<Object>> objects;       // ID -> Object, map of all existing Objects
    std::queue<GLuint> objectsIDsToDelete;                   // Objects which will be deleted during update
    std::queue<GLuint> objectsIDsToCreate;                   // Objects which will be created during update
};
