#pragma once

#include "glad/glad.h"
#include <unordered_set>
#include "systemutils/GlobalVars.h"

class Object {
public:
    explicit Object(ObjectID ID);
    explicit Object(ObjectID ID, SceneID sceneID);
    auto getID() const -> ObjectID;
    std::unordered_set<ComponentID> components; // TODO: a map in ComponentManager has the same name, rename everything
    //also unordered_set might not be memory-efficient here
private:
    ObjectID ID;
    SceneID sceneID;
};
