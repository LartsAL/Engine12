#pragma once

#include <glad/glad.h>
#include "components/Component.h"

class Shader : public Component {
public:
    GLuint shaderProgramID;

    Shader();
    explicit Shader(GLuint shaderProgramID);
};