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
    static auto getInstance() noexcept -> ObjectManager&;

    auto initialize() noexcept -> void;
    auto update() -> void;
    auto shutdown() noexcept -> void;

    auto createObject(SceneID sceneID, const char* name = nullptr) -> ObjectID;
    auto findObjectByName(SceneID sceneID, const char* name) -> ObjectID;
    auto deleteObject(SceneID sceneID, ObjectID ID) noexcept -> void;
    auto deleteAllObjects() noexcept -> void;
    auto getMaxObjects() const noexcept -> GLuint;
    auto getObjectsCount() const noexcept -> GLuint;
    auto getObject(ObjectID ID) const noexcept -> std::shared_ptr<Object>;

private:
    ObjectManager() noexcept;
    ~ObjectManager() noexcept;

    IDSystem idSystem;
    std::unordered_map<ObjectID, std::shared_ptr<Object>> objects;
};
