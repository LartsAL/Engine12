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
    static auto getInstance() noexcept -> SceneManager&;

    auto initialize() noexcept -> void;
    auto update() -> void;
    auto shutdown() noexcept -> void;

    auto createScene() -> SceneID;
    auto deleteScene(SceneID ID) noexcept -> void;
    auto deleteAllScenes() noexcept -> void;
    auto getScene(SceneID ID) const noexcept -> std::shared_ptr<Scene>;
    auto getScenesCount() const noexcept -> GLuint;

private:
    SceneManager() noexcept;
    ~SceneManager() noexcept;

    IDSystem idSystem;
    std::unordered_map<SceneID, std::shared_ptr<Scene>> scenes;
};
