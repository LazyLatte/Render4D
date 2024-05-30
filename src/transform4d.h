#ifndef TRANSFORM4D_H
#define TRANSFORM4D_H
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

typedef struct _Transform4D {
        glm::vec4 position;        
        glm::vec4 scale;
        glm::quat left_isoclinic;
        glm::quat right_isoclinic;
        _Transform4D();
        _Transform4D(glm::vec4 position, glm::vec4 scale);
        _Transform4D(glm::vec4 position, glm::vec4 scale, glm::quat left_isoclinic, glm::quat right_isoclinic);
}Transform4D;

#endif /* TRANSFORM4D_H */
