#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GL/glew.h>
#include "collider.h"
class Scene;
class Camera {
    private:
        float radius;
        glm::vec3 position;
        Scene *scene;
    public:
        Camera();
        void addScene(Scene *scene);
        void move(glm::vec3 d);
        void rotate(float dx, float dy, float rad);
        Scene *getScene() const;
        glm::vec3 getPosition() const;
        glm::mat4 getViewMatrix() const;
        glm::vec3 ray_casting(glm::vec3 start, glm::vec3 dir, float max_dist);
};
#include "scene.h"
#endif