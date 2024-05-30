#ifndef COLLIDER_H
#define COLLIDER_H
#include <algorithm>
#include <glm/glm.hpp>
#include "transform3d.h"


typedef struct _CollisionPoints {
    glm::vec3 A;
    glm::vec3 B;
    glm::vec3 dir; //normalize(B-A)
    float depth;
    bool hasCollision;
    _CollisionPoints(){}
    _CollisionPoints(bool _hasCollision): hasCollision(_hasCollision){}
    _CollisionPoints(glm::vec3 _A, glm::vec3 _B, glm::vec3 _dir, float _depth, bool _hasCollision): A(_A), B(_B), dir(_dir), depth(_depth), hasCollision(_hasCollision){}
}CollisionPoints;

class AABBCollider;
class SphereCollider;
class Collider { 
    public:
        virtual float SDF(glm::vec3 p) const = 0;
        virtual CollisionPoints TestCollision(const Transform3D *transform, const Collider* collider, const Transform3D *colliderTransform) const = 0;
        virtual CollisionPoints TestCollision(const Transform3D *transform, const AABBCollider* aabbCollider, const Transform3D *aabbTransform) const = 0;
        virtual CollisionPoints TestCollision(const Transform3D *transform, const SphereCollider* sphereCollider, const Transform3D *sphereTransform) const = 0;
};

namespace algo{
    CollisionPoints FindAABBAABBCollisionPoints(const AABBCollider *a, const Transform3D *ta, const AABBCollider *b, const Transform3D *tb);
    CollisionPoints FindAABBSphereCollisionPoints(const AABBCollider *a, const Transform3D *ta, const SphereCollider *b, const Transform3D *tb);
    CollisionPoints FindSphereAABBCollisionPoints(const SphereCollider *a, const Transform3D *ta, const AABBCollider *b, const Transform3D *tb);
    CollisionPoints FindSphereSphereCollisionPoints(const SphereCollider *a, const Transform3D *ta, const SphereCollider *b, const Transform3D *tb);
}
#include "aabbcollider.h"
#include "spherecollider.h"

#endif