#pragma once

#include "Component.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

class Transform : public Component {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    Transform();
    explicit Transform(glm::vec3 position = glm::vec3(0.0),
                       glm::vec3 rotation = glm::vec3(0.0),
                       glm::vec3 scale    = glm::vec3(1.0));
};
