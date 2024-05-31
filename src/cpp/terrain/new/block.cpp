#include "block.h"
#include "chunk.h"
#include "glm/ext/matrix_transform.hpp"
#include "../../guiVariables.h"
#include <cmath>

float Block::s_LODDistance = 4.f;
int Block::s_LODLevels = 5;
GLuint Block::s_VAO[] = {0};

Block::Block(glm::ivec3 position, int size, int LOD)
    : m_Size(size)
    , m_Pos(position)
    , m_LOD(LOD)
{ }

void Block::GenerateVAO(int LOD) {
    if (s_VAO[LOD] != 0) {
        glDeleteVertexArrays(1, &s_VAO[LOD]);
    }

    int size = Chunk::Size() / static_cast<int>(std::pow(2, s_LODLevels - LOD - 1));

    std::vector<glm::vec3> corners = {
            {0, 0, 0}, {size, 0, 0}, {size, size, 0}, {0, size, 0},
            {0, 0, size}, {size, 0, size}, {size, size, size}, {0, size, size}
    };

    std::vector<glm::vec3> lines = {
            // minY plane
            corners[0], corners[1], corners[1], corners[2],
            corners[2], corners[3], corners[3], corners[0],
            // maxY plane
            corners[4], corners[5], corners[5], corners[6],
            corners[6], corners[7], corners[7], corners[4],
            // Connecting lines
            corners[0], corners[4], corners[1], corners[5],
            corners[2], corners[6], corners[3], corners[7]
    };

    GLuint VBO;
    glGenVertexArrays(1, &s_VAO[LOD]);
    glGenBuffers(1, &VBO);

    glBindVertexArray(s_VAO[LOD]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, lines.size() * sizeof(glm::vec3), lines.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Block::Draw(Shader& shader, const glm::mat4& viewProjMatrix) const {
    if(!GuiVariables::s_DrawLODs[m_LOD])
        return;

    glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(m_Pos));
    shader.SetUniform("modelViewProjectionMatrix", viewProjMatrix * translation);

    if (s_VAO[m_LOD] == 0)
        GenerateVAO(m_LOD);

    glBindVertexArray(s_VAO[m_LOD]);
    glDrawArrays(GL_LINES, 0, 24);
}
