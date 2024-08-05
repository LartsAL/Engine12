#pragma once

#include "glad/glad.h"
#include <unordered_set>

#include "managers/ComponentManager.h"

using ObjectID = GLuint;

class Object {
public:
    explicit Object(ObjectID ID);
    auto getID() const -> ObjectID;
    std::unordered_set<ComponentID> components; // TODO: a map in ComponentManager has the same name, rename everything
    //also unordered_set might not be memory-efficient here
private:
    ObjectID ID{};
};