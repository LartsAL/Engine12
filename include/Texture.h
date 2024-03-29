#pragma once

#include <glad/glad.h>

class Texture {
public:
    GLuint textureID;

    Texture(const GLchar* textureFile);
};
