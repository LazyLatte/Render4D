#include "aabbcollider.h"
AABBCollider::AABBCollider(){
    this->min_xyz = glm::vec3(0.0);
    this->max_xyz = glm::vec3(0.0);
}

void AABBCollider::setAABB(float Xmin, float Xmax, float Ymin, float Ymax,  float Zmin, float Zmax){
    this->min_xyz = glm::vec3(Xmin, Ymin, Zmin);
    this->max_xyz = glm::vec3(Xmax, Ymax, Zmax);
}

float AABBCollider::SDF(glm::vec3 p) const {
    glm::vec3 box_center = 0.5f * (this->max_xyz + this->min_xyz);
    p-=box_center;
    glm::vec3 box = 0.5f * (this->max_xyz - this->min_xyz);
    glm::vec3 q = abs(p) - box;
    return glm::length(glm::vec3(std::max(q.x, 0.0f), std::max(q.y, 0.0f), std::max(q.z, 0.0f))) + std::min(std::max(q.x, std::max(q.y, q.z)), 0.0f);
}

CollisionPoints AABBCollider::TestCollision(const Transform3D *transform, const Collider* collider, const Transform3D *colliderTransform) const {
    return collider->TestCollision(colliderTransform, this, transform);
}

CollisionPoints AABBCollider::TestCollision(const Transform3D *transform, const AABBCollider* aabbCollider, const Transform3D *aabbTransform) const {
    return algo::FindAABBAABBCollisionPoints(this, transform, aabbCollider, aabbTransform);
}

CollisionPoints AABBCollider::TestCollision(const Transform3D *transform, const SphereCollider* sphereCollider, const Transform3D *sphereTransform) const {
    return CollisionPoints(false);
}
