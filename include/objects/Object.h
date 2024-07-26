#pragma once

#include "glad/glad.h"

using ObjectID = GLuint;

class Object {
public:
    explicit Object(ObjectID ID);

    auto getID() const -> ObjectID;

private:
    ObjectID ID;
};