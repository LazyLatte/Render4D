#ifndef SOLVER_H
#define SOLVER_H
#include <vector>
#include "rigidbody3d.h"
#include "object4d.h"

enum AXIS {X_AXIS, Y_AXIS, Z_AXIS, W_AXIS};
typedef struct _Collision3D4D {
    Object3D *obj3d;
    Object4D *obj4d;
    CollisionPoints points;
    _Collision3D4D(){};
    _Collision3D4D(Object3D *_obj3d, Object4D *_obj4d, CollisionPoints _points): obj3d(_obj3d), obj4d(_obj4d), points(_points){};
}Collision3D4D;

class Solver{
    public:
        virtual void solve(std::vector<Collision3D4D>& collisions, AXIS axis, float dt) = 0;
};

class ImpulseSolver : public Solver{
    public:
        void solve(std::vector<Collision3D4D>& collisions, AXIS axis,float dt) override;
};

class PositionSolver : public Solver{
    public:
        void solve(std::vector<Collision3D4D>& collisions, AXIS axis, float dt) override;
};

#endif /* SOLVER_H */
