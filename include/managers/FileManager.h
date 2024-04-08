#pragma once

#include "glad/glad.h"
#include "Shader.h"
#include "Texture.h"

class FileManager {
public:
    static auto getInstance() -> FileManager&;

    auto initialize() -> void;
    // auto update() -> void;  // Maybe unused
    auto shutdown() -> void;

    auto loadShader(GLchar*&& vertexFile, GLchar*&& fragmentFile) -> Shader&;
    auto loadTexture(GLchar*&& textureFile) -> Texture&;

private:
    FileManager();
    ~FileManager();
};
