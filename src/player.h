#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <glm/glm.hpp>
#include "rigidbody3d.h"
#include "renderable.h"

class Scene;
class Player : public RigidBody3D, public Renderable {
    private:
        Scene *scene;
    public:
        static const float walking_speed;
        static const float running_speed;
        bool running;
        bool moving_forward;
        bool moving_backward;
        bool moving_right;
        bool moving_left;
        bool grounded;
        Player();
        Player(std::string filename, glm::vec3 pos, glm::vec3 scale, glm::vec3 front, float mass);
        void addScene(Scene *scene);
        Scene *getScene() const;
        glm::vec3 calculateVelocity() const;
};


#include "scene.h"
#endif