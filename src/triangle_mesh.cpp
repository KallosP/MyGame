#include "triangle_mesh.h"
#include <vector>
#include <glad/glad.h>

TriangleMesh::TriangleMesh() {
    // Hetergenous data
    std::vector<float> positions = {
         -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 
         -1.0f, 1.0f, 0.0f,
         1.0f, 1.0f, 0.0f,
    };
    std::vector<int> colorIndices = {
        0, 1, 2, 3
    };
    std::vector<int> elements = {
        0, 1, 2, 2, 1, 3
    };
    element_count = 6;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    VBOs.resize(2); // creates 2 unsigned integers in the VBOs set

    //position
    glGenBuffers(1, &VBOs[0]); 
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, (void*)0); 
    glEnableVertexAttribArray(0);

    //color indices
    glGenBuffers(1, &VBOs[1]); 
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, colorIndices.size() * sizeof(int), colorIndices.data(), GL_STATIC_DRAW);
    glVertexAttribIPointer(1, 1, GL_INT, 4, (void*)0);
    glEnableVertexAttribArray(1);

    //element buffer
    glGenBuffers(1, &EBO); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(int), elements.data(), GL_STATIC_DRAW);
}

void TriangleMesh::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, element_count, GL_UNSIGNED_INT, 0);
}

TriangleMesh::~TriangleMesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(VBOs.size(), VBOs.data());
    glDeleteBuffers(1, &EBO);
}

// NOTES (this code uses homogenous data)
/*TriangleMesh::TriangleMesh() {
    std::vector<float> data = {
         -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // format: x, y, z, r, g, b -> 1 point (vertex 1)
         1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // another point (vertex 2)
         -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f // another point (vertex 3)
         // The lines above are 3 vertices
    };
    vertex_count = 3;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO); // VBO = vertext buffer object; this function is collecting our vertices (data) and shipping it to the GPU; the GPU responds with a buffer number that is then stored in VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    size_t bufferSize = data.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, data.data(), 
            GL_STATIC_DRAW);

    //position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0); // 0 = attribute 0 i.e. position; 
                                    // number of numbers in the attribute (position is made up of 3 
                                    // floats x,y,z, so second param is 3), GL_FLOAT is the data format
                                    // (we're using floats in data), GL_FALSE is indicating NOT to normalize the data,
                                    // 24 = represents a stride of 24 bytes -> from the first vertex, it takes 24 bytes (b/c float = 4 bytes, there are 6 floats, 6*4=24 bytes) 
                                    // to get to the next vertex (in other words, we have to walk across 6 floats to get
                                    // the next vertex defined in our data vector), finally (void*)0 is the offset
    glEnableVertexAttribArray(0);

    //color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12); // 1 = attribute 1 i.e. color; (void*)12 is offset of 12 bytes (b/c first 12 bytes are for position (i.e. the first three floats), last three bytes are the color attribute) 
    glEnableVertexAttribArray(1);
}

void TriangleMesh::draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertex_count);
}

TriangleMesh::~TriangleMesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
*/