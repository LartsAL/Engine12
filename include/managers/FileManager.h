#pragma once

#include <glad/glad.h>

class FileManager {
public:
    static auto getInstance() -> FileManager&;

    auto initialize() -> void;
    // auto update() -> void;  // Maybe unused
    auto shutdown() -> void;

    // Loads vertex & fragment shaders from file and returns ID of created shader program
    auto loadShader(const char*&& vertexFile, const char*&& fragmentFile) -> GLuint;
    // Loads texture from file and returns its ID
    auto loadTexture(const char*&& textureFile) -> GLuint;

private:
    FileManager();
    ~FileManager();
};
