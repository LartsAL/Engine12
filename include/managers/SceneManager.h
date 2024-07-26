#pragma once

#include <set>
#include <queue>
#include <memory>
#include <unordered_map>
#include <glad/glad.h>
#include "objects/Scene.h"

using SceneID = GLuint;

class SceneManager {
public:
    static auto getInstance() -> SceneManager&;

    auto initialize() -> void;
    auto update() -> void;
    auto shutdown() -> void;

    auto createScene() -> SceneID;
    auto deleteScene(SceneID ID) -> void;
    auto deleteAllScenes() -> void;

private:
    SceneManager();
    ~SceneManager();

    GLuint maxScenes;
    GLuint scenesCount;
    GLuint nextFreeID;
    std::set<SceneID> freeIDs;
    std::unordered_map<SceneID, std::shared_ptr<Scene>> scenes;
    std::queue<SceneID> scenesIDsToDelete;
    std::queue<SceneID> scenesIDsToReuse;       // Who the fuck can reach 4.7 billion scenes?
    std::queue<SceneID> scenesIDsToCreate;
};
