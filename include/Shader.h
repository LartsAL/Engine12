#pragma once

#include <glad/glad.h>

class Shader {
public:
    GLuint programID;

    Shader(const GLchar* vertexFile, const GLchar* fragmentFile);
    ~Shader() = default;

    auto use() const -> void;
};