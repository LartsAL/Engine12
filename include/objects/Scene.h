#pragma once

#include <unordered_set>
#include <glad/glad.h>

#include "systemutils/GlobalVars.h"

class Scene {
public:
    explicit Scene(SceneID ID, bool active = true);

    auto update() -> void;

    auto addObject(ObjectID objectID) -> void;
    auto removeObject(ObjectID objectID) -> void;
    auto isActive() -> bool;
    auto getID() -> SceneID;
    auto getLinkedWindow() -> WindowID;

private:
    bool                            active;
    SceneID                         ID;
    WindowID                        linkedWindow;
    std::unordered_set<ObjectID>    sceneObjects;
};