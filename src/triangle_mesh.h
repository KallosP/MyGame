#pragma once
#include <vector>

class TriangleMesh {
public:
TriangleMesh();
void draw();
~TriangleMesh();

private:
unsigned int EBO, VAO, element_count; // EBO = element buffer object (stores what we want to draw)
std::vector<unsigned int> VBOs;
};
