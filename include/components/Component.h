#pragma once

#include <bitset>
#include <limits>
#include <glad/glad.h>

using ComponentID = GLuint;

class Component {
public:
    virtual ~Component() = default;

    // ! Deprecated, candidate for deletion:
    static std::bitset<std::numeric_limits<GLuint>::max()> bitmask;
    static ComponentID ID;
};
