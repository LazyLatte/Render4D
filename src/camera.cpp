#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#define PI 3.14159265359
#define MIN_ANGLE 0.31415926535
#define MAX_ANGLE PI-MIN_ANGLE
const glm::vec3 y_axis = glm::vec3(0.0, 1.0, 0.0);
Camera::Camera(){
    this->position = glm::vec3(0.0);
    this->radius = 7.0f;
}

void Camera::addScene(Scene *scene){
    this->scene = scene;
    this->position = scene->getPlayer()->getPosition() + glm::vec3(2.0f, 6.0f, -3.0f);
}

void Camera::move(glm::vec3 d){
    this->position += d;
}

void Camera::rotate(float dx, float dy, float theta){
    glm::vec3 player_position = this->scene->getPlayer()->getPosition();
    glm::vec3 front = glm::normalize(player_position - this->position);
    glm::vec3 right = glm::normalize(glm::cross(front, y_axis));
    glm::vec3 up = glm::normalize(glm::cross(right, front));
    glm::vec3 axis = -(dy*right + dx*up);

    glm::mat4 rotate_matrix = glm::rotate(glm::mat4(1.0), theta, axis);
    glm::vec3 next_front = rotate_matrix * glm::vec4(front, 1.0);
    float y_axis_angle = (float)acos(dot(next_front, y_axis));
    if(y_axis_angle > MIN_ANGLE && y_axis_angle < MAX_ANGLE){
        this->position = this->ray_casting(player_position, -next_front, this->radius);
    }
}

glm::vec3 Camera::ray_casting(glm::vec3 start, glm::vec3 dir, float max_dist){
    float dist = 0.0f;
    const float min_step = 0.01f;
    while(1){
        glm::vec3 ray_pos = start + dist * dir;
        float ray_step = std::numeric_limits<float>::max();
        for(unsigned i=0; i<this->getScene()->numOfTesseract; i++){
            ray_step = std::min(ray_step, this->getScene()->tesseracts[i]->getCollider()->SDF(ray_pos));
        }
        if(ray_step < min_step){
            break;
        }
        dist += ray_step;
        if(dist >= max_dist){
            dist = max_dist;
            break;
        }
    }
    return start + dist * dir;
}

Scene *Camera::getScene() const{
    return this->scene;
}
glm::vec3 Camera::getPosition() const{
    return this->position;
}
glm::mat4 Camera::getViewMatrix() const{
    return glm::lookAt(this->position, this->scene->getPlayer()->getPosition(), y_axis);
}