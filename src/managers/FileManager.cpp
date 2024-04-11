#include "managers/FileManager.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <SOIL/SOIL.h>

FileManager::FileManager() = default;

FileManager::~FileManager() = default;

auto FileManager::getInstance() -> FileManager& {
    static FileManager instance;
    return instance;
}

auto FileManager::initialize() -> void {

}

auto FileManager::shutdown() -> void {

}

auto FileManager::loadShader(const char*&& vertexFile, const char*&& fragmentFile) -> GLuint {
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
        return 0;
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
        return 0;
    }
    // Fragment
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0;
    }
    // Program
    GLuint programID;
    programID = glCreateProgram();
    glAttachShader(programID, vertex);
    glAttachShader(programID, fragment);
    glLinkProgram(programID);
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return 0;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return programID;
}

auto FileManager::loadTexture(const char*&& textureFile) -> GLuint {
    GLint textureWidth, textureHeight;
    unsigned char* image = SOIL_load_image(textureFile, &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
    if (!image) {
        std::cerr << "ERROR::TEXTURE::LOAD_FAILED" << SOIL_last_result() << std::endl;
        return 0;
    }
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}
