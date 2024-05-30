#ifndef SCENE_H
#define SCENE_H
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "hyperplane.h"
#include "solver.h"
#include "tesseract.h"
#include "camera.h"
#include "player.h"
#include "shader.h"

/*
    mat3x4 hyperPlane = mat3x4(
        cos(thetaX), 0.0, 0.0, sin(thetaX),
        0.0, cos(thetaY), 0.0, sin(thetaY),
        0.0, 0.0, cos(thetaZ), sin(thetaZ)
    );
*/
class Scene {
    private:
        Hyperplane *hyperplane;
        Player *player;
        Camera *camera;

        ImpulseSolver *impulse_solver;
        PositionSolver *position_solver;
        glm::vec3 gravity;
        glm::mat4 proj_matrix;
        Shader *shader_3d;
        Shader *shader_4d;
    public:
        std::vector<Collision3D4D> coliisions;
        Tesseract *tesseracts[1000];
        unsigned numOfTesseract;
        Scene();
        ~Scene();
        void init();
        void addPlayer(Player *player);
        void addCamera(Camera *camera);
        void addTesseract(glm::vec4 pos);
        void addHyperplane(Hyperplane *hyperplane);
        void setProjectionMatrix(float viewportAspect);
        void render4D();
        void renderPlayer();
        void step(float dt);
        void resolve_key_callback(char key);
        void rotateHyperplane(float radX, float radY, float radZ);
        void applyGravity();
        void resolveCollision(float dt);
        Player *getPlayer();
        Camera *getCamera();
        Hyperplane *getHyperplane();
};

#endif