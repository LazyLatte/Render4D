#include "object4d.h"
#include <iostream>
#include <algorithm>
Object4D::Object4D(){}

Object4D::Object4D(glm::vec4 position, glm::vec4 scale, Hyperplane *hyperplane){
    // temporary constructure
    this->transform = new Transform4D(position, scale);
    this->aabbCollider = new AABBCollider;
    this->hyperplane = hyperplane;
}

Object4D::Object4D(std::vector<glm::vec4> vertices, std::vector<unsigned> indices, glm::vec4 position, glm::vec4 scale, Hyperplane *hyperplane){
    this->transform = new Transform4D(position, scale);
    this->aabbCollider = new AABBCollider;

    this->vertices = vertices;
    this->indices = indices;
    this->hyperplane = hyperplane;
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->position_buffer);
    glGenBuffers(1, &this->ebo);

    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->position_buffer);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float) * 4, &this->vertices[0].x, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);
}

glm::vec4 Object4D::getTranslation() const {
    return this->transform->position;
}

glm::mat4 Object4D::getLeftIsoclinicMatrix() const {
    glm::quat left = this->transform->left_isoclinic;
    return glm::mat4(
        left.w, left.x, left.y, left.z,
        -left.x, left.w, left.z, -left.y,
        -left.y, -left.z, left.w, left.x,
        -left.z, left.y, -left.x, left.w
    );
}

glm::mat4 Object4D::getRightIsoclinicMatrix() const {
    glm::quat right = this->transform->right_isoclinic;
    return glm::mat4(
        right.w, right.x, right.y, right.z,
        -right.x, right.w, -right.z, right.y,
        -right.y, right.z, right.w, -right.x,
        -right.z, -right.y, right.x, right.w
    );
}

Object4D::~Object4D(){
    delete this->transform;
    delete this->aabbCollider;
}

// const unsigned Table[16][4][3] = {
//     //CodeParade: https://youtu.be/dbq9uX_MycY?si=u72P-9FUHEQ_H9DU
//     //row: tetrahedron vertices A+- B+- C+- D+- (2^4)
//     //col: sliced tetragon vertices 0 1 2 3 (2^2)
//     //(r, g, b): (tetrahedron vertex 0, tetrahedron vertex 1, isValid)
//     {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, // A- B- C- D-
//     {{3, 0, 1}, {3, 1, 1}, {3, 2, 1}, {0, 0, 0}}, // A- B- C- D+
//     {{2, 0, 1}, {2, 1, 1}, {2, 3, 1}, {0, 0, 0}}, // A- B- C+ D-
//     {{0, 2, 1}, {0, 3, 1}, {1, 2, 1}, {1, 3, 1}}, // A- B- C+ D+
//     {{1, 0, 1}, {1, 2, 1}, {1, 3, 1}, {0, 0, 0}}, // A- B+ C- D-
//     {{0, 1, 1}, {0, 3, 1}, {1, 2, 1}, {2, 3, 1}}, // A- B+ C- D+
//     {{0, 1, 1}, {0, 2, 1}, {1, 3, 1}, {2, 3 ,1}}, // A- B+ C+ D-
//     {{0, 1, 1}, {0, 2, 1}, {0, 3, 1}, {0, 0, 0}}, // A- B+ C+ D+

//     {{0, 1, 1}, {0, 2, 1}, {0, 3, 1}, {0, 0, 0}}, // A+ B- C- D-
//     {{0, 1, 1}, {0, 2, 1}, {1, 3, 1}, {2, 3, 1}}, // A+ B- C- D+
//     {{0, 1, 1}, {0, 3, 1}, {1, 2, 1}, {2, 3 ,1}}, // A+ B- C+ D-
//     {{1, 0, 1}, {1, 2, 1}, {1, 3, 1}, {0, 0, 0}}, // A+ B- C+ D+
//     {{0, 2, 1}, {0, 3, 1}, {1, 2, 1}, {1, 3, 1}}, // A+ B+ C- D-
//     {{2, 0, 1}, {2, 1, 1}, {2, 3, 1}, {0, 0, 0}}, // A+ B+ C- D+
//     {{3, 0, 1}, {3, 1, 1}, {3, 2, 1}, {0, 0, 0}}, // A+ B+ C+ D-
//     {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}  // A+ B+ C+ D+
// };




// void Object4D::slice(){
//     this->sliced_vertices.clear();
//     glm::vec4 hyperPlaneNormal = this->hyperplane->getHyperplaneNormal();
//     float hyperPlaneOffset = this->hyperplane->getHyperplaneOffset();
//     float Xmin = std::numeric_limits<float>::max(), Ymin = std::numeric_limits<float>::max(), Zmin = std::numeric_limits<float>::max();
//     float Xmax = -std::numeric_limits<float>::max(), Ymax = -std::numeric_limits<float>::max(), Zmax = -std::numeric_limits<float>::max();
//     for(std::vector<unsigned>::iterator it=this->indices.begin(); it!=this->indices.end(); it+=4){
//         unsigned tetrahedron_indices[4] = {*(it), *(it+1), *(it+2), *(it+3)};
//         glm::vec4 tetrahedron_vertices[4] = {
//             this->vertex_transform(this->vertices[tetrahedron_indices[0]]), 
//             this->vertex_transform(this->vertices[tetrahedron_indices[1]]), 
//             this->vertex_transform(this->vertices[tetrahedron_indices[2]]), 
//             this->vertex_transform(this->vertices[tetrahedron_indices[3]])
//         };
//         bool s[4] = {
//             (dot(tetrahedron_vertices[0], hyperPlaneNormal) + hyperPlaneOffset) > 0, 
//             (dot(tetrahedron_vertices[1], hyperPlaneNormal) + hyperPlaneOffset) > 0, 
//             (dot(tetrahedron_vertices[2], hyperPlaneNormal) + hyperPlaneOffset) > 0, 
//             (dot(tetrahedron_vertices[3], hyperPlaneNormal) + hyperPlaneOffset) > 0
//         };
//         unsigned row = (s[0] << 3) | (s[1] << 2) | (s[2] << 1) | (s[3] << 0);

//         for(unsigned j=0; j<4 && Table[row][j][2]; j++){
//             unsigned v_id0 = Table[row][j][0];
//             unsigned v_id1 = Table[row][j][1];
//             float d0 = abs(dot(tetrahedron_vertices[v_id0], hyperPlaneNormal) + hyperPlaneOffset);
//             float d1 = abs(dot(tetrahedron_vertices[v_id1], hyperPlaneNormal) + hyperPlaneOffset);

//             glm::vec4 p = (tetrahedron_vertices[v_id0] * d1 + tetrahedron_vertices[v_id1] * d0) / (d0 + d1);

//             if(j==3){
//                 std::vector<float>::iterator iter = this->sliced_vertices.end();
//                 float prevVals[6] = {*(iter-6), *(iter-5), *(iter-4), *(iter-3), *(iter-2), *(iter-1)};
//                 this->sliced_vertices.push_back(prevVals[0]);
//                 this->sliced_vertices.push_back(prevVals[1]);
//                 this->sliced_vertices.push_back(prevVals[2]);
//                 this->sliced_vertices.push_back(prevVals[3]);
//                 this->sliced_vertices.push_back(prevVals[4]);
//                 this->sliced_vertices.push_back(prevVals[5]);
//             }
//             this->sliced_vertices.push_back(p.x);
//             this->sliced_vertices.push_back(p.y);
//             this->sliced_vertices.push_back(p.z);
//             Xmin = std::min(Xmin, p.x);
//             Ymin = std::min(Ymin, p.y);
//             Zmin = std::min(Zmin, p.z);
//             Xmax = std::max(Xmax, p.x);
//             Ymax = std::max(Ymax, p.y);
//             Zmax = std::max(Zmax, p.z);
//         }
//     }
    
//     this->aabbCollider->setAABB(Xmin, Xmax, Ymin, Ymax, Zmin, Zmax);
// }

// void Object4D::draw(){
//     this->slice();
//     if(this->sliced_vertices.size() > 0){
//         glBindVertexArray(this->vao);
//         glBindBuffer(GL_ARRAY_BUFFER, this->position_buffer);
//         glBufferData(GL_ARRAY_BUFFER, this->sliced_vertices.size()*sizeof(float), &this->sliced_vertices[0], GL_STATIC_DRAW);
//         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//         glEnableVertexAttribArray(0);
//         glDrawArrays(GL_TRIANGLES, 0, (GLsizei)this->sliced_vertices.size());
//     }
// }

void Object4D::draw(){
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glDrawElements(GL_LINES_ADJACENCY, this->indices.size(), GL_UNSIGNED_INT, (void*)0);
}

AABBCollider *Object4D::getCollider(){
    return this->aabbCollider;
}