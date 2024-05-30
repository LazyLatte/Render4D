#ifndef TRANSFORM3D_H
#define TRANSFORM3D_H
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
typedef struct _Transform3D {
        glm::vec3 position;        
        glm::vec3 scale;
        glm::quat rotation;
        _Transform3D();
        _Transform3D(glm::vec3 position, glm::vec3 scale);
        _Transform3D(glm::vec3 position, glm::vec3 scale, glm::quat rotation);
}Transform3D;

#endif /* TRANSFORM3D_H */
