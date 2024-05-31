#include "rigidbody3d.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

RigidBody3D::RigidBody3D(){
    this->velocity = glm::vec3(0.0);
    this->force = glm::vec3(0.0);
    this->mass = 0.0;
}

RigidBody3D::RigidBody3D(glm::vec3 position, glm::vec3 scale, float mass) : Object3D(position, scale, true) {
    this->velocity = glm::vec3(0.0);
    this->force = glm::vec3(0.0);
    this->mass = mass;
}

glm::vec3 RigidBody3D::getPosition() const{
    return this->transform->position;
}

void RigidBody3D::addForce(glm::vec3 force){
    this->force += force;
}

void RigidBody3D::move(glm::vec3 d){
    this->transform->position += d;
    //this->velocity = delta;
}

// void RigidBody3D::rotate(glm::quat q){
//     this->transform->rotation *= q;
// }

// void RigidBody3D::rotate(glm::vec3 targetDir){
//     glm::vec3 axis = glm::normalize(glm::cross(this->front, targetDir));
//     float rad = acos(glm::dot(this->front, targetDir) / length(this->front)) * 0.5;
//     this->transform->rotation = glm::quat(cos(rad), sin(rad)*axis);
// }

// void RigidBody3D::rotate(float deg, glm::vec3 axis){
//     this->transform->rotation = glm::rotate(this->transform->rotation, deg, axis);
// }
