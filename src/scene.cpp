#include "scene.h"
#include "slice.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#define deg2rad(x) ((x)*((3.1415926f)/(180.0f)))

glm::vec3 y_axis = glm::vec3(0.0, 1.0, 0.0);
glm::mat4 identity(1.0);
Scene::Scene(){
    this->shader_3d = new Shader("./shaders/vs.glsl", "./shaders/fs.glsl");
    this->shader_4d = new Shader("./shaders/vs_4d.glsl", "./shaders/fs_4d.glsl");
    this->numOfTesseract = 0;
    this->gravity = glm::vec3(0.0, -9.81, 0.0);
    this->impulse_solver = new ImpulseSolver;
    this->position_solver = new PositionSolver;
}

void Scene::addPlayer(Player *player){
    player->addScene(this);
    this->player = player;
}

void Scene::addCamera(Camera *camera){
    camera->addScene(this);
    this->camera = camera;
}

void Scene::addTesseract(glm::vec4 pos){
    this->tesseracts[this->numOfTesseract] = new Tesseract(pos, this->hyperplane);
    this->numOfTesseract++;
}

void Scene::addHyperplane(Hyperplane *hyperplane){
    this->hyperplane = hyperplane;
}

void Scene::init(){
    // for(unsigned i=0; i<this->numOfTesseract; i++){
    //     this->tesseracts[i]->slice(this->hyperplane->getHyperplaneNormal(), this->hyperplane->getHyperplaneOffset());
    // }
}
void Scene::applyGravity(){
    glm::vec3 gravity_force = this->getPlayer()->mass * this->gravity;
    this->getPlayer()->force += gravity_force;
}

void Scene::resolveCollision(float dt){
    // for(auto &collision : this->coliisions){
        
    // }
}


void Scene::step(float dt){
    this->applyGravity();
    this->player->velocity += (this->getPlayer()->force / this->getPlayer()->mass) * dt;
    glm::vec3 controlled_velocity = this->player->calculateVelocity();
    this->player->velocity += controlled_velocity;
    glm::vec3 delta = this->getPlayer()->velocity * dt;

    //solve x
    bool x_has_collision = false;
    this->getPlayer()->move(glm::vec3(delta.x, 0.0, 0.0));
    for(unsigned i=0; i<this->numOfTesseract; i++){
        CollisionPoints points = this->getPlayer()->TestCollision(this->tesseracts[i]->getCollider(), NULL);
        if(points.hasCollision){
            x_has_collision = true;
            this->coliisions.emplace_back(this->getPlayer(), this->tesseracts[i], points);
        }
    }
    this->position_solver->solve(this->coliisions, AXIS::X_AXIS, dt);

    //solve y
    bool y_has_collision = false;
    this->getPlayer()->move(glm::vec3(0.0, delta.y, 0.0));
    for(unsigned i=0; i<this->numOfTesseract; i++){
        CollisionPoints points = this->getPlayer()->TestCollision(this->tesseracts[i]->getCollider(), NULL);
        if(points.hasCollision){
            y_has_collision = true;
            this->coliisions.emplace_back(this->getPlayer(), this->tesseracts[i], points);
        }
    }
    this->position_solver->solve(this->coliisions, AXIS::Y_AXIS, dt);

    //solve z
    bool z_has_collision = false;
    this->getPlayer()->move(glm::vec3(0.0, 0.0, delta.z));
    for(unsigned i=0; i<this->numOfTesseract; i++){
        CollisionPoints points = this->getPlayer()->TestCollision(this->tesseracts[i]->getCollider(), NULL);
        if(points.hasCollision){
            z_has_collision = true;
            this->coliisions.emplace_back(this->getPlayer(), this->tesseracts[i], points);
        }
    }
    this->position_solver->solve(this->coliisions, AXIS::Z_AXIS, dt);
   // this->getCamera()->move(d);
    this->getPlayer()->force = glm::vec3(0.0);
    this->player->velocity -= controlled_velocity;

    this->player->velocity.x *= !x_has_collision;
    this->player->velocity.y *= !y_has_collision;
    this->player->velocity.z *= !z_has_collision;

    this->coliisions.clear();
}

void Scene::render4D(){
    this->shader_4d->useShader(&identity[0][0], &this->camera->getViewMatrix()[0][0], &this->proj_matrix[0][0]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for(unsigned i=0; i<this->numOfTesseract; i++){
       // std::cout << this->tesseracts[i]->aabbCollider->Ymin << " " << this->tesseracts[i]->aabbCollider->Ymax<<std::endl;
        this->tesseracts[i]->draw();
    }
}

void Scene::renderPlayer(){
    this->shader_3d->useShader(&this->player->getModelMatrix()[0][0], &this->camera->getViewMatrix()[0][0], &this->proj_matrix[0][0]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    this->player->draw();
}

void Scene::rotateHyperplane(float radX, float radY, float radZ){
    this->hyperplane->rotate(radX, radY, radZ);
}

Player *Scene::getPlayer(){
    return this->player;
}

Camera *Scene::getCamera(){
    return this->camera;
}

Hyperplane *Scene::getHyperplane(){
    return this->hyperplane;
}
void Scene::setProjectionMatrix(float viewportAspect){
    this->proj_matrix = glm::perspective(deg2rad(60.0f), viewportAspect, 0.1f, 1000.0f);
}
Scene::~Scene(){
    delete this->impulse_solver;
    delete this->position_solver;
    delete this->shader_3d;
    delete this->shader_4d;
    for(unsigned i=0; i<this->numOfTesseract; i++){
        delete this->tesseracts[i];
    }
}

void Scene::resolve_key_callback(char key){
    // glm::vec3 player_front = this->getPlayer()->front;
    // glm::vec3 player_right = glm::cross(player_front, y_axis);
    // glm::vec3 player_up = glm::normalize(glm::cross(player_right, player_front));
    // glm::vec3 camera_front = glm::normalize(this->getCamera()->getCameraFront());
    // switch(key){
    //     case 'W': {
    //         glm::vec3 dir = glm::normalize(camera_front - (glm::dot(camera_front, player_up) / glm::dot(player_up, player_up)) * player_up);
    //         this->getPlayer()->move(dir);
    //         this->getCamera()->position += dir;

    //         float sign = glm::dot(glm::cross(player_front, dir), y_axis)>=0 ? 1.0f : -1.0f;
    //         //this->getPlayer()->transform->rotation *= glm::quat_cast(glm::rotate(glm::mat4(1.0), acos(glm::dot(player_front, dir)/(glm::length(player_front) * glm::length(dir))), sign*y_axis));
    //         this->getPlayer()->front = dir;
    //         break;
    //     }
    //     case 'S': {
    //         glm::vec3 dir = -glm::normalize(camera_front - (glm::dot(camera_front, player_up) / glm::dot(player_up, player_up)) * player_up);
    //         this->getPlayer()->move(dir);
    //         this->getCamera()->position += dir;

    //         float sign = glm::dot(glm::cross(player_front, dir), y_axis)>=0 ? 1.0f : -1.0f;
    //         //this->getPlayer()->transform->rotation *= glm::quat_cast(glm::rotate(glm::mat4(1.0), acos(glm::dot(player_front, dir)/(glm::length(player_front) * glm::length(dir))), sign*y_axis));
    //         this->getPlayer()->front = dir;
    //         break;
    //     }
    //     case 'A': {
    //         glm::vec3 camera_right = glm::normalize(glm::cross(camera_front, y_axis));
    //         glm::vec3 dir = -glm::normalize(camera_right - (glm::dot(camera_right, player_up) / glm::dot(player_up, player_up)) * player_up);
    //         this->getPlayer()->move(dir);
    //         this->getCamera()->position += dir;

    //         float sign = glm::dot(glm::cross(player_front, dir), y_axis)>=0 ? 1.0f : -1.0f;
    //         //this->getPlayer()->transform->rotation *= glm::rotate(glm::mat4(1.0), acos(glm::dot(player_front, dir)/(glm::length(player_front) * glm::length(dir))), sign*y_axis);
    //         this->getPlayer()->front = dir;
    //         break;
    //     }
    //     case 'D': {
    //         glm::vec3 camera_right = glm::normalize(glm::cross(camera_front, y_axis));
    //         glm::vec3 dir = glm::normalize(camera_right - (glm::dot(camera_right, player_up) / glm::dot(player_up, player_up)) * player_up);
    //         this->getPlayer()->move(dir);
    //         this->getCamera()->position += dir;

    //         float sign = glm::dot(glm::cross(player_front, dir), y_axis)>=0 ? 1.0f : -1.0f;
    //         //this->getPlayer()->transform->rotation *= glm::rotate(glm::mat4(1.0), acos(glm::dot(player_front, dir)/(glm::length(player_front) * glm::length(dir))), sign*y_axis);
    //         this->getPlayer()->front = dir;
    //         break;
    //     }
    //     default:
    //         break;
    // } 
}