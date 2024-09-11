#include "scene.h"
#include "slice.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#define deg2rad(x) ((x)*((3.1415926f)/(180.0f)))

glm::vec3 x_axis = glm::vec3(1.0, 0.0, 0.0);
glm::vec3 y_axis = glm::vec3(0.0, 1.0, 0.0);
glm::vec3 z_axis = glm::vec3(0.0, 0.0, 1.0);
glm::mat4 identity(1.0);
Scene::Scene(){
    this->shader3d = new Shader3D("./shaders/vs.glsl", "./shaders/fs.glsl");
    this->shader4d = new Shader4D("./shaders/vs_4d.glsl", "./shaders/fs_4d.glsl", "./shaders/slice.gs.glsl");
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
    // on all rigidbody
    glm::vec3 gravity_force = this->getPlayer()->mass * this->gravity;
    this->getPlayer()->force += gravity_force;
}

void Scene::testCollisions(){
    this->coliisions.clear();
    for(unsigned i=0; i<this->numOfTesseract; i++){
        CollisionPoints points = this->getPlayer()->TestCollision(this->tesseracts[i]->getCollider(), NULL);
        if(points.hasCollision){
            this->coliisions.emplace_back(this->getPlayer(), this->tesseracts[i], points);
        }
    }
}

void Scene::step(float dt){
    //return;
    this->applyGravity();
    this->player->velocity += (this->getPlayer()->force / this->getPlayer()->mass) * dt;
    glm::vec3 controlled_velocity = this->player->calculateVelocity();
    this->player->velocity += controlled_velocity;
    glm::vec3 delta = this->getPlayer()->velocity * dt;

    
    glm::vec3 prevPlayerPosition = this->player->getPosition();
    //solve x
    // this->getPlayer()->move(glm::vec3(delta.x, 0.0, 0.0));
    // this->testCollisions();
    // bool x_has_collision = this->coliisions.size() > 0;
    // this->position_solver->solve(this->coliisions, x_axis, dt);

    //solve y
    this->getPlayer()->move(glm::vec3(0.0, delta.y, 0.0));
    this->testCollisions();
    bool y_has_collision = this->coliisions.size() > 0;
    this->position_solver->solve(this->coliisions, y_axis, dt);
    
    //solve z
    // this->getPlayer()->move(glm::vec3(0.0, 0.0, delta.z));
    // this->testCollisions();
    // bool z_has_collision = this->coliisions.size() > 0;
    // this->position_solver->solve(this->coliisions, z_axis, dt);

    //move camera
    this->getCamera()->move(this->player->getPosition() - prevPlayerPosition);
    //clear
    this->getPlayer()->force = glm::vec3(0.0);
    this->player->velocity -= controlled_velocity;

    //this->player->velocity.x *= !x_has_collision;
    this->player->velocity.y *= !y_has_collision;
   // this->player->velocity.z *= !z_has_collision;
}

void Scene::render4D(){
    this->shader4d->setVP(&this->camera->getViewMatrix()[0][0], &this->proj_matrix[0][0]);
    this->shader4d->setHyperplane(&this->hyperplane->getNormal()[0], this->hyperplane->getOffset());
    
   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for(unsigned i=0; i<this->numOfTesseract; i++){
        this->shader4d->setTransform4D(&this->tesseracts[i]->getTranslation()[0], &this->tesseracts[i]->getLeftIsoclinicMatrix()[0][0], &this->tesseracts[i]->getRightIsoclinicMatrix()[0][0]);
        this->tesseracts[i]->draw();
    }
}

void Scene::renderPlayer(){
    this->shader3d->setMVP(&this->player->getModelMatrix()[0][0], &this->camera->getViewMatrix()[0][0], &this->proj_matrix[0][0]);
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
    delete this->shader3d;
    delete this->shader4d;
    for(unsigned i=0; i<this->numOfTesseract; i++){
        delete this->tesseracts[i];
    }
}
