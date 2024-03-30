#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glad/glad.h>

Shader::Shader(const GLchar* vertexFile, const GLchar* fragmentFile) {
    std::string vertexCodeRaw;
    std::string fragmentCodeRaw;

    std::ifstream vertexShaderFileStream;
    std::ifstream fragmentShaderFileStream;
    vertexShaderFileStream.exceptions(std::ifstream::failbit);
    fragmentShaderFileStream.exceptions(std::ifstream::failbit);

    try {
        vertexShaderFileStream.open(vertexFile);
        fragmentShaderFileStream.open(fragmentFile);

        std::stringstream vertexShaderStringStream;
        std::stringstream fragmentShaderStringStream;
        vertexShaderStringStream << vertexShaderFileStream.rdbuf();
        fragmentShaderStringStream << fragmentShaderFileStream.rdbuf();

        vertexShaderFileStream.close();
        fragmentShaderFileStream.close();

        vertexCodeRaw = vertexShaderStringStream.str();
        fragmentCodeRaw = fragmentShaderStringStream.str();
    } catch (std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        return;
    }

    const GLchar* vertexCode = vertexCodeRaw.c_str();
    const GLchar* fragmentCode = fragmentCodeRaw.c_str();

    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    // Vertex
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, nullptr);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return;
    }

    // Fragment
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return;
    }

    this->programID = glCreateProgram();
    glAttachShader(this->programID, vertex);
    glAttachShader(this->programID, fragment);
    glLinkProgram(this->programID);

    glGetProgramiv(this->programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->programID, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

auto Shader::use() const -> void {
    glUseProgram(this->programID);
}
