#include "managers/FileManager.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <SOIL/SOIL.h>
#include "Error.h"

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

auto FileManager::loadShader(const char* vertexFile, const char* fragmentFile, const char* geometryFile) -> GLuint {
    std::string vertexCodeRaw;
    std::string fragmentCodeRaw;
    std::string geometryCodeRaw;

    std::ifstream vertexFileStream;
    std::ifstream fragmentFileStream;
    std::ifstream geometryFileStream;

    vertexFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    geometryFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        std::stringstream vertexStringStream;
        std::stringstream fragmentStringStream;

        vertexFileStream.open(vertexFile);
        fragmentFileStream.open(fragmentFile);
        vertexStringStream << vertexFileStream.rdbuf();
        fragmentStringStream << fragmentFileStream.rdbuf();
        vertexFileStream.close();
        fragmentFileStream.close();

        vertexCodeRaw = vertexStringStream.str();
        fragmentCodeRaw = fragmentStringStream.str();

        if (geometryFile) {
            std::stringstream geometryStringStream;

            geometryFileStream.open(geometryFile);
            geometryStringStream << geometryFileStream.rdbuf();
            geometryFileStream.close();

            geometryCodeRaw = geometryStringStream.str();
        }
    } catch (std::ifstream::failure& err) {
        PRINT_ERROR("File reading failed.", "Details: {}\nCode: {}", err.what(), err.code().value());
        return 0;
    }

    GLint success;
    GLchar infoLog[512];

    // Vertex
    const GLchar* vertexCode = vertexCodeRaw.c_str();
    GLuint vertex;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        PRINT_ERROR("Vertex shader compilation failed.", "Log: {}", infoLog);
        return 0;
    }

    // Fragment
    const GLchar* fragmentCode = fragmentCodeRaw.c_str();
    GLuint fragment;
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        PRINT_ERROR("Fragment shader compilation failed.", "Log: {}", infoLog);
        return 0;
    }

    // Geometry
    GLuint geometry;
    if (geometryFile != nullptr) {
        const GLchar* geometry_code = geometryCodeRaw.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &geometry_code, nullptr);
        glCompileShader(geometry);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
            PRINT_ERROR("Geometry shader compilation failed.", "Log: {}", infoLog);
            return 0;
        }
    }

    // Program
    GLuint programID;
    programID = glCreateProgram();
    glAttachShader(programID, vertex);
    glAttachShader(programID, fragment);
    if (geometryFile) {
        glAttachShader(programID, geometry);
    }
    glLinkProgram(programID);
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return 0;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometryFile) {
        glDeleteShader(geometry);
    }

    return programID;
}

auto FileManager::loadTexture(const char* textureFile) -> GLuint {
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
