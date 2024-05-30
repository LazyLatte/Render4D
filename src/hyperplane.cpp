#include "hyperplane.h"

Hyperplane::Hyperplane(){
    this->hyperplaneRad = glm::vec3(0.0);
    this->hyperplaneOffset = 0.0;
}

void Hyperplane::rotate(float radX, float radY, float radZ){
    this->hyperplaneRad += glm::vec3(radX, radY, radZ); 
}

const glm::vec4 Hyperplane::getHyperplaneNormal() const {
    const bool x_is_zero = cos(this->hyperplaneRad.x) == 0.0;
    const bool y_is_zero = cos(this->hyperplaneRad.y) == 0.0;
    const bool z_is_zero = cos(this->hyperplaneRad.z) == 0.0;
    return !x_is_zero && !y_is_zero && !z_is_zero 
        ? glm::vec4(-tan(this->hyperplaneRad.x), -tan(this->hyperplaneRad.y), -tan(this->hyperplaneRad.z), 1.0) 
        : glm::vec4(x_is_zero, y_is_zero, z_is_zero, 0);
    
}
const float Hyperplane::getHyperplaneOffset() const {
    return this->hyperplaneOffset;
}