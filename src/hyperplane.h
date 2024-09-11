#ifndef HYPERPLANE_H
#define HYPERPLANE_H
#include <glm/glm.hpp>
class Hyperplane {
    private:
        glm::vec3 hyperplaneRad;
        float hyperplaneOffset;

        glm::vec4 hyperplaneNormal;
    public:
        Hyperplane();
        void rotate(float radX, float radY, float radZ);
        const glm::vec4 getNormal() const;
        const float getOffset() const;
};

#endif /* HYPERPLANE_H */
