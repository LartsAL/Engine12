#pragma once

#include <glad/glad.h>

class Object {
public:
    explicit Object(GLuint ID);

    auto getID() const -> GLuint;

private:
    GLuint ID;
};