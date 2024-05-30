#include "object3d.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
Object3D::Object3D(){}

Object3D::Object3D(glm::vec3 position, glm::vec3 scale, bool isDynamic){
    this->transform = new Transform3D(position, scale);
    this->aabbCollider = new AABBCollider;
    this->isDynamic = isDynamic;
}

Object3D::~Object3D(){
    delete this->transform;
    delete this->aabbCollider;
}

glm::mat4 Object3D::getTranslationMatrix(){
    return glm::translate(glm::mat4(1.0f), this->transform->position);
}

glm::mat4 Object3D::getScalingMatrix(){
    return glm::scale(glm::mat4(1.0f), this->transform->scale);
}

glm::mat4 Object3D::getRotationMatrix(){
    return glm::mat4_cast(this->transform->rotation);
}

glm::mat4 Object3D::getModelMatrix(){
    return this->getTranslationMatrix() * this->getRotationMatrix() * this->getScalingMatrix();
}

bool Object3D::TestCollision(const Collider* collider, const Transform3D *colliderTransform) const {
    return this->aabbCollider->TestCollision(this->transform, collider, colliderTransform).hasCollision;
}
// bool Object3D::Test3DCollision(Object3D *obj3d){
//     return this->aabbCollider->TestCollision(this->transform, obj3d->aabbCollider, obj3d->transform).hasCollision;
// }

// bool Object3D::Test4DCollision(Object4D *obj4d){
//     return this->aabbCollider->TestCollision(this->transform, obj4d->aabbCollider, &Transform3D()).hasCollision;
// }
