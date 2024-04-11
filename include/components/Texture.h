#pragma once

#include <glad/glad.h>
#include "components/Component.h"

class Texture : public Component {
public:
    GLuint textureID;

    Texture();
    explicit Texture(GLuint textureID);
};
