//
// Created by marti on 22/05/2024.
//

#include <iostream>
#include "chunk.h"
#include "GL/glew.h"
#include "../../shader.h"
#include "../../camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "../../guiVariables.h"

Chunk::Chunk(const glm::ivec2 &position, int size, int minY, int maxY)
    : m_Pos(position)
    , m_Blocks()
{ }

void Chunk::GenerateVAO()
{
    if (s_VAO != 0)
        glDeleteVertexArrays(1, &s_VAO);

    std::vector<glm::vec3> corners = {
            {0, s_MinY, 0},
            {s_Size, s_MinY, 0},
            {s_Size, s_MaxY, 0},
            {0, s_MaxY, 0},
            {0, s_MinY, s_Size},
            {s_Size, s_MinY, s_Size},
            {s_Size, s_MaxY, s_Size},
            {0, s_MaxY, s_Size}
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

    glGenVertexArrays(1, &s_VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(s_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, lines.size() * sizeof(glm::vec3), lines.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

GLuint Chunk::s_VAO = 0;

void Chunk::Draw(Shader &shader, const glm::mat4 &viewProjMatrix) const
{

    if (GuiVariables::s_ChunkDrawMode == GuiVariables::ChunkDrawMode::Chunks)
    {
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(m_Pos.x, 0.0f, m_Pos.y));

        shader.SetUniform("modelViewProjectionMatrix", viewProjMatrix * translation);

        if (s_VAO == 0)
            GenerateVAO();
        glBindVertexArray(s_VAO);

        glDrawArrays(GL_LINES, 0, 24);

        return;
    }

    for (const auto& block : m_Blocks)
        block.Draw(shader, viewProjMatrix);
}

void Chunk::GenerateBlocks(const Camera &camera)
{
    for (int i = s_MinY; i < s_MaxY; i += s_Size)
    {
            // Check whether the block is in the frustum
        glm::vec3 pos = {m_Pos.x, i, m_Pos.y};
        glm::vec3 maxPos = pos + glm::vec3(s_Size);

        SubdivideLOD(pos, s_Size, Block::LodLevels() - 1, camera);

    }
}

void Chunk::SubdivideLOD(glm::ivec3 pos, int size, int LOD, const Camera &camera)
{
    glm::ivec3 maxPos = pos + size;

    if (camera.CubeFrustumCheck(pos, maxPos))
    {
        // Get the distance from the center of the block to the camera;
        glm::vec3 center = glm::vec3(pos) + static_cast<float>(size);
        float distance = glm::distance(center, camera.Pos());

        // Check if the block is close enough to the camera to be subdivided;
        if (LOD != 0 && distance < GuiVariables::s_LODdistance * LOD)
        {
            int newSize = size / 2;
            SubdivideLOD(pos, newSize, LOD - 1, camera);
            SubdivideLOD({pos.x + newSize, pos.y, pos.z}, newSize, LOD - 1, camera);
            SubdivideLOD({pos.x, pos.y + newSize, pos.z}, newSize, LOD - 1, camera);
            SubdivideLOD({pos.x, pos.y, pos.z + newSize}, newSize, LOD - 1, camera);
            SubdivideLOD({pos.x, pos.y + newSize, pos.z + newSize}, newSize, LOD - 1, camera);
            SubdivideLOD({pos.x + newSize, pos.y + newSize, pos.z}, newSize, LOD - 1, camera);
            SubdivideLOD({pos.x + newSize, pos.y, pos.z + newSize}, newSize, LOD - 1, camera);
            SubdivideLOD({pos.x + newSize, pos.y + newSize, pos.z + newSize}, newSize, LOD - 1, camera);

            return;
        }

        m_Blocks.push_back({pos, size, LOD});
    }
}

