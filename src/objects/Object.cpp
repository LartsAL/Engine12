#include "objects/Object.h"

#include "glad/glad.h"

Object::Object(ObjectID ID): ID(ID) {}

Object::Object(ObjectID ID, SceneID sceneID):
    ID(ID),
    sceneID(sceneID) {}

auto Object::getID() const -> ObjectID {
    return ID;
}
