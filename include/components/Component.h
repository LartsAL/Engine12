#pragma once

#include <bitset>
#include <limits>
#include <glad/glad.h>

class Component {
public:
    virtual ~Component() = default;

    static std::bitset<std::numeric_limits<GLuint>::max()> bitmask;
    static GLuint ID;
};
