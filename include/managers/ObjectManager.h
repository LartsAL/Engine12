#pragma once

#include <set>
#include <queue>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <glad/glad.h>
#include "objects/Object.h"
#include "systemutils/GlobalVars.h"
#include "systemutils/IDSystem.h"

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

private:
    ObjectManager();
    ~ObjectManager();

    IDSystem idSystem;
    GLuint objectsCount;
    std::unordered_map<ObjectID, std::shared_ptr<Object>> objects;
};
