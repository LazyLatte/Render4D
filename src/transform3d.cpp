#include "transform3d.h"

_Transform3D::_Transform3D(){
    this->position = glm::vec3(0.0);
    this->scale = glm::vec3(1.0);
    this->rotation = glm::quat(1.0, 0.0, 0.0, 0.0);
}

_Transform3D::_Transform3D(glm::vec3 position, glm::vec3 scale){
    this->position = position;
    this->scale = scale;
    this->rotation = glm::quat(1.0, 0.0, 0.0, 0.0);
}

_Transform3D::_Transform3D(glm::vec3 position, glm::vec3 scale, glm::quat rotation){
    this->position = position;
    this->scale = scale;
    this->rotation = rotation;
}
