#include "components/Transform.h"

Transform::Transform():
    position(glm::vec3(0.0)),
    rotation(glm::vec3(0.0)),
    scale(glm::vec3(1.0)) {}

Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale):
    position(position),
    rotation(rotation),
    scale(scale) {}
