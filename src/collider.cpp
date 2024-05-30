#include "collider.h"

CollisionPoints algo::FindAABBAABBCollisionPoints(const AABBCollider *a, const Transform3D *ta, const AABBCollider *b, const Transform3D *tb){
    glm::vec3 aMin = ta ? /*ta->scale * */a->min_xyz + ta->position : a->min_xyz;
    glm::vec3 aMax = ta ? /*ta->scale * */a->max_xyz + ta->position : a->max_xyz;

    glm::vec3 bMin = tb ? tb->scale * b->min_xyz + tb->position : b->min_xyz;
    glm::vec3 bMax = tb ? tb->scale * b->max_xyz + tb->position : b->max_xyz;
    bool hasCollision = (aMin.x <= bMax.x && aMax.x >= bMin.x) && (aMin.y <= bMax.y && aMax.y >= bMin.y) && (aMin.z <= bMax.z && aMax.z >= bMin.z);
    if(hasCollision){
        // glm::vec3 A, B;
        // if(aMin.x <= bMin.x && bMax.x <= aMax.x){
        //     A.x = (aMin.x + aMax.x) * 0.5f;
        //     B.x = (bMin.x + bMax.x) * 0.5f;
        // }
        glm::vec3 A = glm::vec3(
            aMax.x>=bMax.x ? aMin.x : aMax.x,
            aMax.y>=bMax.y ? aMin.y : aMax.y,
            aMax.z>=bMax.z ? aMin.z : aMax.z
        ); 
        glm::vec3 B = glm::vec3(
            bMax.x>=aMax.x ? bMin.x : bMax.x,
            bMax.y>=aMax.y ? bMin.y : bMax.y,
            bMax.z>=aMax.z ? bMin.z : bMax.z
        ); 
        return CollisionPoints(A, B, glm::normalize(B-A), glm::length(B-A), hasCollision);
    }
    return CollisionPoints(false);
}

CollisionPoints algo::FindAABBSphereCollisionPoints(const AABBCollider *a, const Transform3D *ta, const SphereCollider *b, const Transform3D *tb){
    glm::vec3 aMin = ta ? ta->scale * a->min_xyz + ta->position : a->min_xyz;
    glm::vec3 aMax = ta ? ta->scale * a->max_xyz + ta->position : a->max_xyz;

    glm::vec3 bCenter = b->center + tb->position;
    float bRadius = b->radius * std::max(std::max(tb->scale.x, tb->scale.y), tb->scale.z);

    glm::vec3 closest_point_on_box = glm::vec3(
        std::max(aMin.x, std::min(bCenter.x, aMax.x)), 
        std::max(aMin.y, std::min(bCenter.y, aMax.y)), 
        std::max(aMin.z, std::min(bCenter.z, aMax.z))
    );


    float dist2 = glm::dot(glm::vec3(bCenter - closest_point_on_box), glm::vec3(bCenter - closest_point_on_box));
    bool hasCollision = dist2 <= bRadius * bRadius;

    if(hasCollision){

    }
    return CollisionPoints(false);
}

CollisionPoints algo::FindSphereAABBCollisionPoints(const SphereCollider *a, const Transform3D *ta, const AABBCollider *b, const Transform3D *tb){
    return CollisionPoints(false);
}

CollisionPoints algo::FindSphereSphereCollisionPoints(const SphereCollider *a, const Transform3D *ta, const SphereCollider *b, const Transform3D *tb){
    return CollisionPoints(false);
}