#ifndef AABBCOLLIDER_H
#define AABBCOLLIDER_H
#include <glm/glm.hpp>
#include "collider.h"

class AABBCollider : public Collider {
    public:
        glm::vec3 min_xyz;
        glm::vec3 max_xyz;
        AABBCollider();
        void setAABB(float Xmin, float Xmax, float Ymin, float Ymax,  float Zmin, float Zmax);
        CollisionPoints TestCollision(const Transform3D *transform, const Collider* collider, const Transform3D *colliderTransform) const override;
        CollisionPoints TestCollision(const Transform3D *transform, const AABBCollider* aabbCollider, const Transform3D *aabbTransform) const override;
        CollisionPoints TestCollision(const Transform3D *transform, const SphereCollider* sphereCollider, const Transform3D *sphereTransform) const override;
        float SDF(glm::vec3 p) const override;
};

#endif /* AABBCOLLIDER_H */
