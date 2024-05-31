#include "solver.h"
#include <iostream>
#include <algorithm>
void ImpulseSolver::solve(std::vector<Collision3D4D>& collisions, glm::vec3 axis, float dt){
    for (Collision3D4D& collision : collisions) {
        RigidBody3D *obj3d = collision.obj3d->isDynamic ? (RigidBody3D*)collision.obj3d : nullptr;
        //assume object4d are all static
        //Object4D *obj4d = collision.obj4d;
        CollisionPoints points = collision.points;

        glm::vec3 normal_force_dir = (glm::dot(points.dir, obj3d->velocity) / glm::dot(obj3d->velocity, obj3d->velocity)) * obj3d->velocity;
        obj3d->force.x *= (normal_force_dir.x==0.0);
        obj3d->force.y *= (normal_force_dir.y==0.0);
        obj3d->force.z *= (normal_force_dir.z==0.0);

        // obj3d->velocity.x *= (points.dir.x==0.0);
        // obj3d->velocity.y *= (points.dir.y==0.0);
        // obj3d->velocity.z *= (points.dir.z==0.0);
    }
}


void PositionSolver::solve(std::vector<Collision3D4D>& collisions, glm::vec3 axis, float dt){
    // need to distinguish between dynamic obj and static obj
    float delta = 0.0;
    RigidBody3D *obj3d = NULL;
    axis = glm::normalize(axis);
    for (Collision3D4D& collision : collisions) {
        obj3d = collision.obj3d->isDynamic ? (RigidBody3D*)collision.obj3d : NULL;
        CollisionPoints points = collision.points;
        glm::vec3 normal = points.B - points.A;
        float depth = glm::dot(normal, axis);
        if(abs(depth) > abs(delta)){
            delta = depth;
            
        }
    }
    if(obj3d){
        obj3d->move(delta * axis);
        
    }
    
}