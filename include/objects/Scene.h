#pragma once

#include <unordered_map>
#include <unordered_set>
#include <glad/glad.h>

#include "systemutils/GlobalVars.h"

class Scene {
public:
    explicit Scene(SceneID ID, bool active = true) noexcept;

    auto update() -> void;

    auto addObject(ObjectID objectID, const char* name = nullptr) noexcept -> void;
    auto removeObject(ObjectID objectID) noexcept -> void;
    auto isActive() const noexcept -> bool;
    auto getID() const noexcept -> SceneID;
    auto setLinkedWindow(WindowID windowID) noexcept -> void;
    auto getLinkedWindow() const noexcept -> WindowID;

    std::unordered_map<const char*, ObjectID> namesToObjects;

private:
    bool                            active;
    SceneID                         ID;
    WindowID                        linkedWindow;
    std::unordered_set<ObjectID>    sceneObjects;
};