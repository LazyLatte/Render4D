#include "player.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define deg2rad(x) ((x)*((3.1415926f)/(180.0f)))

const glm::vec3 y_axis = glm::vec3(0.0, 1.0, 0.0);
const float Player::walking_speed = 10.0f;
const float Player::running_speed = 15.0f;
Player::Player(){}

Player::Player(std::string filename, glm::vec3 pos, glm::vec3 scale, glm::vec3 front, float mass): RigidBody3D(pos, scale, front, mass), Renderable(filename) {
    this->running = false;
    this->moving_forward = false;
    this->moving_backward = false;
    this->moving_left = false;
    this->moving_right = false;
    this->aabbCollider->setAABB(-2.2f, 2.2f, -0.5f, 0.5f, -3.2f, 3.2f);
}
void Player::addScene(Scene *scene){
    this->scene = scene;
}

Scene *Player::getScene() const{
    return this->scene;
}



// this->move(speed * dir);
// this->getScene()->getCamera()->move(speed * dir);

// float sign = glm::dot(glm::cross(player_front, dir), y_axis)>=0 ? 1.0f : -1.0f;
// //this->getPlayer()->transform->rotation *= glm::quat_cast(glm::rotate(glm::mat4(1.0), acos(glm::dot(player_front, dir)/(glm::length(player_front) * glm::length(dir))), sign*y_axis));
// this->getPlayer()->front = dir;
glm::vec3 Player::calculateVelocity() const{
    glm::vec3 v = glm::vec3(0.0);
    if(this->moving_forward ^ this->moving_backward){
        glm::vec3 player_front = this->front;
        glm::vec3 player_right = glm::cross(player_front, y_axis);
        glm::vec3 player_up = glm::normalize(glm::cross(player_right, player_front));
        glm::vec3 camera_front = this->getPosition() - this->scene->getCamera()->getPosition();
        glm::vec3 moving_dir = this->moving_forward ? camera_front : -camera_front;

        glm::vec3 dir = glm::normalize(moving_dir - (glm::dot(moving_dir, player_up) / glm::dot(player_up, player_up)) * player_up);
        float speed = this->running ? Player::running_speed : Player::walking_speed;
        v += speed * dir;
    }
    if(this->moving_right ^ this->moving_left){
        glm::vec3 player_front = this->front;
        glm::vec3 player_right = glm::cross(player_front, y_axis);
        glm::vec3 player_up = glm::normalize(glm::cross(player_right, player_front));
        glm::vec3 camera_front = this->getPosition() - this->scene->getCamera()->getPosition();
        glm::vec3 camera_right = glm::normalize(glm::cross(camera_front, y_axis));
        glm::vec3 moving_dir = this->moving_right ? camera_right : -camera_right;

        glm::vec3 dir = glm::normalize(moving_dir - (glm::dot(moving_dir, player_up) / glm::dot(player_up, player_up)) * player_up);
        float speed = this->running ? Player::running_speed : Player::walking_speed;
        v += speed * dir; 
    }
    return v;
}
