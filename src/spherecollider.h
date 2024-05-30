#ifndef SPHERE_COLLIDER_H
#define SPHERE_COLLIDER_H
#include <glm/glm.hpp>
#include "collider.h"

class SphereCollider : public Collider {
    public:
        float radius;
        glm::vec3 center;
        SphereCollider();
        SphereCollider(glm::vec3 center, float radius);
        CollisionPoints TestCollision(const Transform3D *transform, const Collider* collider, const Transform3D *colliderTransform) const override;
        CollisionPoints TestCollision(const Transform3D *transform, const AABBCollider* aabbCollider, const Transform3D *aabbTransform) const override;
        CollisionPoints TestCollision(const Transform3D *transform, const SphereCollider* sphereCollider, const Transform3D *sphereTransform) const override;
};

#endif /* SPHERE_COLLIDER_H */
