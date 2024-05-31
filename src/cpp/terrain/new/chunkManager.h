//
// Created by marti on 22/05/2024.
//

#ifndef MAIN_CHUNKMANAGER_H
#define MAIN_CHUNKMANAGER_H


#include "../../camera.h"
#include "../../frustum.h"
#include "../cell.h"
#include "chunk.h"

class ChunkManager
{
    int m_ChunkSize = 16;

    std::vector<Chunk> m_Chunks;

public:
    ChunkManager() = default;
    void GenerateChunks(const Camera &camera);

    void DebugDraw(Shader &shader, const glm::mat4 &viewProjMatrix);

    [[nodiscard]] inline const std::vector<Chunk>& ActiveChunks() const { return m_Chunks; }

private:
};


#endif //MAIN_CHUNKMANAGER_H
