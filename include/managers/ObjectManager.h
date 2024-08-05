#pragma once

#include <set>
#include <queue>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <glad/glad.h>
#include "objects/Object.h"

using ObjectID = GLuint;

class ObjectManager {
public:
    static auto getInstance() -> ObjectManager&;

    auto initialize() -> void;
    auto update() -> void;
    auto shutdown() -> void;

    auto createObject() -> ObjectID;
    auto deleteObject(ObjectID ID) -> void;
    auto deleteAllObjects() -> void;
    auto getMaxObjects() const -> GLuint;
    auto getObjectsCount() const -> GLuint;

    std::unordered_map<ObjectID, std::shared_ptr<Object>> objects;

private:
    ObjectManager();
    ~ObjectManager();

    GLuint maxObjects;                          // Maximum existing objects
    GLuint objectsCount;                        // Number of existing objects
    ObjectID nextFreeID;                        // Next available ID for new Object
    std::set<ObjectID> freeIDs;                 // Pool of reusable IDs freed from deleted Objects
    std::queue<ObjectID> objectsIDsToDelete;    // Objects which will be deleted during update
    std::queue<ObjectID> objectsIDsToReuse;     // If we book some IDs from objectsIDsToDelete, they'll go here to prevent double book of the same ID
    std::queue<ObjectID> objectsIDsToCreate;    // Objects which will be created during update
};
