#include "solver.h"
#include <iostream>
#include <algorithm>
void ImpulseSolver::solve(std::vector<Collision3D4D>& collisions, AXIS axis, float dt){
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


void PositionSolver::solve(std::vector<Collision3D4D>& collisions, AXIS axis, float dt){
    float delta = 0.0;
    RigidBody3D *obj3d;
    if(collisions.size() == 0) return;
    
    for (Collision3D4D& collision : collisions) {
        obj3d = collision.obj3d->isDynamic ? (RigidBody3D*)collision.obj3d : nullptr;
        CollisionPoints points = collision.points;
        if(axis == X_AXIS){
            float depth = points.B.x - points.A.x;
            if(abs(depth) > abs(delta)){
                delta = depth;
            }
        }
        if(axis == Y_AXIS){
            float depth = points.B.y - points.A.y;
            if(abs(depth) > abs(delta)){
                delta = depth;
            }
        }
        if(axis == Z_AXIS){
            float depth = points.B.z - points.A.z;
            if(abs(depth) > abs(delta)){
                delta = depth;
            }
        }
    }
    if(axis == X_AXIS){
        obj3d->move(glm::vec3(delta, 0.0, 0.0));
    }
    if(axis == Y_AXIS){
        obj3d->move(glm::vec3(0.0, delta, 0.0));
    }
    if(axis == Z_AXIS){
        obj3d->move(glm::vec3(0.0, 0.0, delta));
    }
}