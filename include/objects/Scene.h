#pragma once

#include <vector>
#include <glad/glad.h>

using SceneID  = GLuint;
using WindowID = GLuint;
using ObjectID = GLuint;

class Scene {
public:
    explicit Scene(SceneID ID, bool active = true);

    auto update() -> void;

    auto isActive() -> bool;
    auto getID() -> SceneID;
    auto getLinkedWindow() -> WindowID;

private:
    bool                    active;
    SceneID                 ID;
    WindowID                linkedWindow;
    std::vector<ObjectID>   sceneObjects;
};