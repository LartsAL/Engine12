#include "Texture.h"

#include <iostream>
#include <glad/glad.h>
#include <SOIL/SOIL.h>

using namespace std;

Texture::Texture(const GLchar* textureFile) {
    int textureWidth, textureHeight;
    unsigned char* image = SOIL_load_image(textureFile, &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);

    if (!image) {
        cerr << "ERROR::TEXTURE::LOAD_FAILED" << SOIL_last_result() << endl;
        return;
    }

    glGenTextures(1, &this->textureID);
    glBindTexture(GL_TEXTURE_2D, this->textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
}
