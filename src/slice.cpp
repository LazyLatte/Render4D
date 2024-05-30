#include <vector>
#include <glm/glm.hpp>

const unsigned Table[16][4][3] = {
    //CodeParade: https://youtu.be/dbq9uX_MycY?si=u72P-9FUHEQ_H9DU
    //row: tetrahedron vertices A+- B+- C+- D+- (2^4)
    //col: sliced tetragon vertices 0 1 2 3 (2^2)
    //(r, g, b): (tetrahedron vertex 0, tetrahedron vertex 1, isValid)
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, // A- B- C- D-
    {{3, 0, 1}, {3, 1, 1}, {3, 2, 1}, {0, 0, 0}}, // A- B- C- D+
    {{2, 0, 1}, {2, 1, 1}, {2, 3, 1}, {0, 0, 0}}, // A- B- C+ D-
    {{0, 2, 1}, {0, 3, 1}, {1, 2, 1}, {1, 3, 1}}, // A- B- C+ D+
    {{1, 0, 1}, {1, 2, 1}, {1, 3, 1}, {0, 0, 0}}, // A- B+ C- D-
    {{0, 1, 1}, {0, 3, 1}, {1, 2, 1}, {2, 3, 1}}, // A- B+ C- D+
    {{0, 1, 1}, {0, 2, 1}, {1, 3, 1}, {2, 3 ,1}}, // A- B+ C+ D-
    {{0, 1, 1}, {0, 2, 1}, {0, 3, 1}, {0, 0, 0}}, // A- B+ C+ D+

    {{0, 1, 1}, {0, 2, 1}, {0, 3, 1}, {0, 0, 0}}, // A+ B- C- D-
    {{0, 1, 1}, {0, 2, 1}, {1, 3, 1}, {2, 3, 1}}, // A+ B- C- D+
    {{0, 1, 1}, {0, 3, 1}, {1, 2, 1}, {2, 3 ,1}}, // A+ B- C+ D-
    {{1, 0, 1}, {1, 2, 1}, {1, 3, 1}, {0, 0, 0}}, // A+ B- C+ D+
    {{0, 2, 1}, {0, 3, 1}, {1, 2, 1}, {1, 3, 1}}, // A+ B+ C- D-
    {{2, 0, 1}, {2, 1, 1}, {2, 3, 1}, {0, 0, 0}}, // A+ B+ C- D+
    {{3, 0, 1}, {3, 1, 1}, {3, 2, 1}, {0, 0, 0}}, // A+ B+ C+ D-
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}  // A+ B+ C+ D+
};

void cpu_slice(std::vector<float> &triangle_vertices, glm::vec4 hyperPlaneNormal, float hyperPlaneOffset, std::vector<glm::vec4> &vertices, std::vector<unsigned> &indices){
    for(std::vector<unsigned>::iterator it=indices.begin(); it!=indices.end(); it+=4){
        unsigned tetrahedron_indices[4] = {*(it), *(it+1), *(it+2), *(it+3)};
        glm::vec4 tetrahedron_vertices[4] = {
            vertices[tetrahedron_indices[0]], 
            vertices[tetrahedron_indices[1]], 
            vertices[tetrahedron_indices[2]], 
            vertices[tetrahedron_indices[3]]
        };
        bool s[4] = {
            (dot(tetrahedron_vertices[0], hyperPlaneNormal) + hyperPlaneOffset) > 0, 
            (dot(tetrahedron_vertices[1], hyperPlaneNormal) + hyperPlaneOffset) > 0, 
            (dot(tetrahedron_vertices[2], hyperPlaneNormal) + hyperPlaneOffset) > 0, 
            (dot(tetrahedron_vertices[3], hyperPlaneNormal) + hyperPlaneOffset) > 0
        };
        unsigned row = (s[0] << 3) | (s[1] << 2) | (s[2] << 1) | (s[3] << 0);

        for(unsigned j=0; j<4 && Table[row][j][2]; j++){
            unsigned v_id0 = Table[row][j][0];
            unsigned v_id1 = Table[row][j][1];
            float d0 = abs(dot(tetrahedron_vertices[v_id0], hyperPlaneNormal) + hyperPlaneOffset);
            float d1 = abs(dot(tetrahedron_vertices[v_id1], hyperPlaneNormal) + hyperPlaneOffset);

            glm::vec4 p = (tetrahedron_vertices[v_id0] * d1 + tetrahedron_vertices[v_id1] * d0) / (d0 + d1);

            if(j==3){
                std::vector<float>::iterator iter = triangle_vertices.end();
                float prevVals[6] = {*(iter-6), *(iter-5), *(iter-4), *(iter-3), *(iter-2), *(iter-1)};
                triangle_vertices.push_back(prevVals[0]);
                triangle_vertices.push_back(prevVals[1]);
                triangle_vertices.push_back(prevVals[2]);
                triangle_vertices.push_back(prevVals[3]);
                triangle_vertices.push_back(prevVals[4]);
                triangle_vertices.push_back(prevVals[5]);
            }
            triangle_vertices.push_back(p.x);
            triangle_vertices.push_back(p.y);
            triangle_vertices.push_back(p.z);
        }

    }
}