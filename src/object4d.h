#ifndef OBJECT4D_H
#define OBJECT4D_H
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "hyperplane.h"
#include "transform4d.h"
#include "collider.h"
class Object4D {
    protected:
        Hyperplane *hyperplane;
        Transform4D *transform;
        AABBCollider *aabbCollider;
    public:
        GLuint vao;
        GLuint ebo;
        GLuint position_buffer;
        std::vector<glm::vec4> vertices;
        std::vector<unsigned> indices;
        std::vector<float> sliced_vertices;
        Object4D();
        Object4D(glm::vec4 position, glm::vec4 scale, Hyperplane *hyperplane);
        Object4D(std::vector<glm::vec4> vertices, std::vector<unsigned> indices, glm::vec4 position, glm::vec4 scale, Hyperplane *hyperplane);
        ~Object4D();
        glm::vec4 getTranslation() const;
        glm::mat4 getLeftIsoclinicMatrix() const;
        glm::mat4 getRightIsoclinicMatrix() const;
        AABBCollider *getCollider();
        void slice();
        void draw();
};

#endif