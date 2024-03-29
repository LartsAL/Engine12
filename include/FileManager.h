#pragma once

#include <glad/glad.h>
#include "Shader.h"
#include "Texture.h"

class FileManager {
public:
    //static auto loadShader(GLchar*& vertexFile, GLchar*& fragmentFile) -> Shader&;
    static auto loadShader(GLchar*&& vertexFile, GLchar*&& fragmentFile) -> Shader&;

    static auto loadTexture(GLchar*& textureFile) -> Texture&;
    static auto loadTexture(GLchar*&& textureFile) -> Texture&;

    FileManager() = delete;
    ~FileManager() = delete;
};
