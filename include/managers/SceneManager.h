#pragma once

#include <set>
#include <queue>
#include <memory>
#include <unordered_map>
#include <glad/glad.h>
#include "objects/Scene.h"
#include "systemutils/GlobalVars.h"
#include "systemutils/IDSystem.h"

class SceneManager {
public:
    static auto getInstance() -> SceneManager&;

    auto initialize() -> void;
    auto update() -> void;
    auto shutdown() -> void;

    auto createScene() -> SceneID;
    auto deleteScene(SceneID ID) -> void;
    auto deleteAllScenes() -> void;
    auto getScene(SceneID ID) -> std::shared_ptr<Scene>;

private:
    SceneManager();
    ~SceneManager();

    GLuint scenesCount;
    std::unordered_map<SceneID, std::shared_ptr<Scene>> scenes;
    IDSystem idSystem;
};
