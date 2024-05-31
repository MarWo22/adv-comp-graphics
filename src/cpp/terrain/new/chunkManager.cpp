//
// Created by marti on 22/05/2024.
//

#include <iostream>
#include "chunkManager.h"
#include "../../guiVariables.h"

void ChunkManager::GenerateChunks(const Camera &camera)
{
    m_Chunks.clear();

    glm::ivec2 startPos {
            static_cast<int>(std::floor(camera.Pos().x / static_cast<float>(m_ChunkSize))) * m_ChunkSize,
            static_cast<int>(std::floor(camera.Pos().z / static_cast<float>(m_ChunkSize))) * m_ChunkSize,
    };

    for (int i = -GuiVariables::s_RenderDistance; i != GuiVariables::s_RenderDistance; ++i)
        for (int j = -GuiVariables::s_RenderDistance; j != GuiVariables::s_RenderDistance; ++j)
        {
            glm::ivec3 chunkPos = {startPos.x + (i * m_ChunkSize), startPos.y + (j * m_ChunkSize), 0};
            glm::ivec3 chunkMax = {chunkPos.x + m_ChunkSize, chunkPos.z + m_ChunkSize, 256};
            if(camera.CubeFrustumCheck(chunkPos, chunkMax))
            {
                Chunk chunk = {chunkPos, m_ChunkSize, 0, 256};
                chunk.GenerateBlocks(camera);
                if (!chunk.Blocks().empty())
                    m_Chunks.push_back(chunk);
            }
        }
}

void ChunkManager::DebugDraw(Shader &shader, const glm::mat4 &viewProjMatrix)
{
    shader.Bind();

    for (const auto& chunk : m_Chunks)
        chunk.Draw(shader, viewProjMatrix);

}
