#ifndef RIGIDBODY3D_H
#define RIGIDBODY3D_H
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "object3d.h"

class RigidBody3D : public Object3D {
    public:
        float mass;
        glm::vec3 velocity;
        glm::vec3 force;
        glm::vec3 front;
        
        RigidBody3D();
        RigidBody3D(glm::vec3 position, glm::vec3 scale, glm::vec3 front, float mass);
        glm::vec3 getPosition() const;
        void addForce(glm::vec3 force);
        void move(glm::vec3 d);
        // void rotate(glm::quat q);
        // void rotate(glm::vec3 targetDir);
        // void rotate(float deg, glm::vec3 axis);
};

#endif /* RIGIDBODY3D_H */
