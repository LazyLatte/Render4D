
#ifndef SLICE_H
#define SLICE_H

#include <vector>
#include <glm/glm.hpp>
void cpu_slice(std::vector<float> &triangle_vertices, glm::vec4 hyperPlaneNormal, float hyperPlaneOffset, std::vector<glm::vec4> &vertices, std::vector<unsigned> &indices, glm::vec4 vertexOffsets);
#endif