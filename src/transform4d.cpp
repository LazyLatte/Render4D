#include "transform4d.h"

_Transform4D::_Transform4D(){
    this->position = glm::vec4(0.0);
    this->scale = glm::vec4(1.0);
    this->left_isoclinic = glm::quat(1.0, 0.0, 0.0, 0.0);
    this->right_isoclinic = glm::quat(1.0, 0.0, 0.0, 0.0);
}

_Transform4D::_Transform4D(glm::vec4 position, glm::vec4 scale){
    this->position = position;
    this->scale = scale;
    this->left_isoclinic = glm::quat(1.0, 0.0, 0.0, 0.0);
    this->right_isoclinic = glm::quat(1.0, 0.0, 0.0, 0.0);
}

_Transform4D::_Transform4D(glm::vec4 position, glm::vec4 scale, glm::quat left_isoclinic, glm::quat right_isoclinic){
    this->position = position;
    this->scale = scale;
    this->left_isoclinic = left_isoclinic;
    this->right_isoclinic = right_isoclinic;
}
