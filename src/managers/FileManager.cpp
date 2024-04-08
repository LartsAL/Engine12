#include "managers/FileManager.h"

#include "glad/glad.h"
#include "Shader.h"
#include "Texture.h"

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

auto FileManager::loadShader(GLchar*&& vertexFile, GLchar*&& fragmentFile) -> Shader& {
    auto shader = new Shader(vertexFile, fragmentFile);
    return *shader;
}

auto FileManager::loadTexture(GLchar*&& textureFile) -> Texture& {
    auto texture = new Texture(textureFile);
    return *texture;
}
