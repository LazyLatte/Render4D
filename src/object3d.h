#ifndef OBJECT3D_H
#define OBJECT3D_H
#include <glm/glm.hpp>
#include "transform3d.h"
#include "collider.h"

class Object3D {
    protected:
        Transform3D *transform;
        AABBCollider *aabbCollider;
    public:
        bool isDynamic;
        Object3D();
        ~Object3D();
        Object3D(glm::vec3 position, glm::vec3 scale, bool isDynamic);
        glm::mat4 getTranslationMatrix();
        glm::mat4 getScalingMatrix();
        glm::mat4 getRotationMatrix();
        glm::mat4 getModelMatrix();
        bool TestCollision(const Collider* collider, const Transform3D *colliderTransform) const;
};

#endif