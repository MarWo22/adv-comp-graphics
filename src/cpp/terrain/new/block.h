//
// Created by marti on 22/05/2024.
//

#ifndef MAIN_BLOCK_H
#define MAIN_BLOCK_H

#include "glm/glm.hpp"
#include "GL/glew.h"
#include "../../shader.h"
#include <vector>
#include <iostream>

class Block {
    static int s_LODLevels;
    static GLuint s_VAO[5];
    static float s_LODDistance;

    int m_Size;
    glm::ivec3 m_Pos;

    int m_LOD;

public:
    Block() = default;
    Block(glm::ivec3 position, int size, int LOD);

    void Draw(Shader& shader, const glm::mat4& viewProjMatrix) const;

    [[nodiscard]] inline const glm::ivec3 &Pos() const { return m_Pos; }
    [[nodiscard]] inline int Size() const { return m_Size; }
    [[nodiscard]] static inline int LodLevels() { return s_LODLevels; }
    [[nodiscard]] static inline float LodDistance() { return s_LODDistance; }

private:
    static void GenerateVAO(int LOD);

};

#endif //MAIN_BLOCK_H
