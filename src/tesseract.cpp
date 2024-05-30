#include "tesseract.h"
#define CUBE_TETRAHEDRON_INDICIES(v0, v1, v2, v3, v4, v5, v6, v7) \
    v0,v1,v3,v4,\
    v2,v1,v3,v6,\
    v7,v3,v4,v6,\
    v5,v1,v4,v6,\
    v6,v1,v3,v4
    
glm::vec4 tesseract_vertices[] = {
    glm::vec4(1.0, 1.0, 1.0, 1.0),
    glm::vec4(1.0, -1.0, 1.0, 1.0),
    glm::vec4(-1.0, -1.0, 1.0, 1.0),
    glm::vec4(-1.0, 1.0, 1.0, 1.0),

    glm::vec4(1.0, 1.0, -1.0, 1.0),
    glm::vec4(1.0, -1.0, -1.0, 1.0),
    glm::vec4(-1.0, -1.0, -1.0, 1.0),
    glm::vec4(-1.0, 1.0, -1.0, 1.0),

    glm::vec4(1.0, 1.0, 1.0, -1.0),
    glm::vec4(1.0, -1.0, 1.0, -1.0),
    glm::vec4(-1.0, -1.0, 1.0, -1.0),
    glm::vec4(-1.0, 1.0, 1.0, -1.0),

    glm::vec4(1.0, 1.0, -1.0, -1.0),
    glm::vec4(1.0, -1.0, -1.0, -1.0),
    glm::vec4(-1.0, -1.0, -1.0, -1.0),
    glm::vec4(-1.0, 1.0, -1.0, -1.0)
};

unsigned tesseract_indices[] = {  
    //cube order -> in out top down right front left back
    CUBE_TETRAHEDRON_INDICIES(0, 1, 2, 3, 4, 5, 6, 7),
    CUBE_TETRAHEDRON_INDICIES(8, 9, 10, 11, 12, 13, 14, 15),     
    CUBE_TETRAHEDRON_INDICIES(8, 9, 10, 11, 0, 1, 2, 3),    
    CUBE_TETRAHEDRON_INDICIES(4, 5, 6, 7, 12, 13, 14, 15), 
    CUBE_TETRAHEDRON_INDICIES(8, 0, 3, 11, 12, 4, 7 ,15),
    CUBE_TETRAHEDRON_INDICIES(8, 9, 1, 0, 12, 13, 5, 4),
    CUBE_TETRAHEDRON_INDICIES(1, 9, 10, 2, 5, 13, 14, 6),
    CUBE_TETRAHEDRON_INDICIES(3, 2, 10, 11, 7, 6, 14, 15) 
};
Tesseract::Tesseract(){
    std::vector<glm::vec4> v(tesseract_vertices, tesseract_vertices + (sizeof(tesseract_vertices)/sizeof(tesseract_vertices[0])));
    std::vector<unsigned> idx(tesseract_indices, tesseract_indices + (sizeof(tesseract_indices)/sizeof(tesseract_indices[0])));
    this->vertices = v;
    this->indices = idx;
}

Tesseract::Tesseract(glm::vec4 position, Hyperplane *hyperplane): Object4D(
    std::vector<glm::vec4>(tesseract_vertices, tesseract_vertices + (sizeof(tesseract_vertices)/sizeof(tesseract_vertices[0]))),
    std::vector<unsigned>(tesseract_indices, tesseract_indices + (sizeof(tesseract_indices)/sizeof(tesseract_indices[0]))),
    position, glm::vec4(1.0f), hyperplane)
{
    this->aabbCollider->setAABB(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
}