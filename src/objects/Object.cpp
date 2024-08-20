#include "objects/Object.h"

#include "glad/glad.h"

Object::Object(ObjectID ID, SceneID sceneID, const char* name):
    ID(ID),
    sceneID(sceneID),
    name(name) {}

auto Object::getID() const -> ObjectID {
    return ID;
}

auto Object::getSceneID() const -> ObjectID {
    return sceneID;
}

auto Object::getName() const -> const char * {
    return name;
}


