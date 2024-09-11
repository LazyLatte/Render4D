#version 330 core
layout (lines_adjacency) in;
layout (triangle_strip, max_vertices = 4) out;

out vec3 vertexColor;
uniform vec4 hyperPlaneNormal;
uniform float hyperPlaneOffset;
uniform mat4 V;
uniform mat4 P;

// const uint Table[16][4][3] = {
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

const uvec4 Table[16] = uvec4[16](
    uvec4(0, 0, 0, 0), 
    uvec4(25, 27, 29, 0), 
    uvec4(17, 19, 23, 0), 
    uvec4(5, 7, 13, 15), 
    uvec4(9, 13, 15, 0), 
    uvec4(3, 7, 13, 23), 
    uvec4(3, 5, 15, 23), 
    uvec4(3, 5, 7, 0), 

    uvec4(3, 5, 7, 0), 
    uvec4(3, 5, 15, 23), 
    uvec4(3, 7, 13, 23), 
    uvec4(9, 13, 15, 0), 
    uvec4(5, 7, 13, 15), 
    uvec4(17, 19, 23, 0), 
    uvec4(25, 27, 29, 0), 
    uvec4(0, 0, 0, 0) 
);
void main() {
    bool s0 = (dot(gl_in[0].gl_Position, hyperPlaneNormal) + hyperPlaneOffset) > 0;
    bool s1 = (dot(gl_in[1].gl_Position, hyperPlaneNormal) + hyperPlaneOffset) > 0;
    bool s2 = (dot(gl_in[2].gl_Position, hyperPlaneNormal) + hyperPlaneOffset) > 0;
    bool s3 = (dot(gl_in[3].gl_Position, hyperPlaneNormal) + hyperPlaneOffset) > 0;

    uint row = uint(s0 << 3) | uint(s1 << 2) | uint(s2 << 1) | uint(s3 << 0);
    
    for(uint j=0u; j<4u && bool(Table[row][j] & 1u); j++){
        uint v_id0 = (Table[row][j] >> 3) & 0x00000003u;
        uint v_id1 = (Table[row][j] >> 1) & 0x00000003u;
        float d0 = abs(dot(gl_in[v_id0].gl_Position, hyperPlaneNormal) + hyperPlaneOffset);
        float d1 = abs(dot(gl_in[v_id1].gl_Position, hyperPlaneNormal) + hyperPlaneOffset);

        vec4 aPos = (gl_in[v_id0].gl_Position * d1 + gl_in[v_id1].gl_Position * d0) / (d0 + d1);
        vertexColor = 0.75 + sin(aPos.xyz) * 0.25;
        gl_Position = P * V * vec4(aPos.xyz, 1.0);
        EmitVertex();
    }
    EndPrimitive();

}