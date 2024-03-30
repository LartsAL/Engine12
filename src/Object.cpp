#include "Object.h"

#include <glad/glad.h>

Object::Object(GLuint id): ID(id) {}

auto Object::getID() const -> GLuint {
    return ID;
}
