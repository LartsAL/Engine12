#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glad/glad.h>

using namespace std;

Shader::Shader(const GLchar* vertexFile, const GLchar* fragmentFile) {
    string vertexCodeRaw, fragmentCodeRaw;
    ifstream vertexShaderFileStream, fragmentShaderFileStream;

    vertexShaderFileStream.exceptions(ifstream::failbit);
    fragmentShaderFileStream.exceptions(ifstream::failbit);

    try {
        vertexShaderFileStream.open(vertexFile);
        fragmentShaderFileStream.open(fragmentFile);

        stringstream vertexShaderStringStream, fragmentShaderStringStream;
        vertexShaderStringStream << vertexShaderFileStream.rdbuf();
        fragmentShaderStringStream << fragmentShaderFileStream.rdbuf();

        vertexShaderFileStream.close();
        fragmentShaderFileStream.close();

        vertexCodeRaw = vertexShaderStringStream.str();
        fragmentCodeRaw = fragmentShaderStringStream.str();
    } catch (ifstream::failure& e) {
        cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
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
        cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
        return;
    }

    // Fragment
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
        return;
    }

    this->programID = glCreateProgram();
    glAttachShader(this->programID, vertex);
    glAttachShader(this->programID, fragment);
    glLinkProgram(this->programID);

    glGetProgramiv(this->programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->programID, 512, nullptr, infoLog);
        cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
        return;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

auto Shader::use() const -> void {
    glUseProgram(this->programID);
}
