#ifndef HYPERPLANE_H
#define HYPERPLANE_H
#include <glm/glm.hpp>
class Hyperplane {
    private:
        glm::vec3 hyperplaneRad;
        float hyperplaneOffset;
    public:
        Hyperplane();
        void rotate(float radX, float radY, float radZ);
        const glm::vec4 getHyperplaneNormal() const;
        const float getHyperplaneOffset() const;
};

#endif /* HYPERPLANE_H */
