#include "FileManager.h"

#include <glad/glad.h>
#include "Shader.h"
#include "Texture.h"

// By lvalue reference
/*
auto FileManager::loadShader(GLchar*& vertexFile, GLchar*& fragmentFile) -> Shader& {
    auto shader = new Shader(vertexFile, fragmentFile);
    return *shader;
}
*/
// By rvalue reference
auto FileManager::loadShader(GLchar*&& vertexFile, GLchar*&& fragmentFile) -> Shader& {
    auto shader = new Shader(vertexFile, fragmentFile);
    return *shader;
}

auto FileManager::loadTexture(GLchar*& textureFile) -> Texture& {
    auto texture = new Texture(textureFile);
    return *texture;
}

auto FileManager::loadTexture(GLchar*&& textureFile) -> Texture& {
    auto texture = new Texture(textureFile);
    return *texture;
}
