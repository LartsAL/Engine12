#include "objects/Object.h"

#include "glad/glad.h"

Object::Object(ObjectID id): ID(id) {}

auto Object::getID() const -> ObjectID {
    return ID;
}
