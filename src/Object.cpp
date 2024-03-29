#include "Object.h"

#include <glad/glad.h>

Object::Object(): ID(0) {}

Object::Object(GLuint id): ID(id) {}

auto Object::getID() const -> GLuint {
    return ID;
}
