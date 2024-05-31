//
// Created by marti on 22/05/2024.
//

#ifndef MAIN_CHUNK_H
#define MAIN_CHUNK_H


#include "glm/glm.hpp"
#include "block.h"
#include "GL/glew.h"
#include "../../shader.h"
#include "../../camera.h"

class Chunk
{
    static const int s_Size = 16;
    static const int s_MinY = 0;
    static const int s_MaxY = 256;
    static GLuint s_VAO;

    glm::ivec2 m_Pos;

    std::vector<Block> m_Blocks;


public:
    Chunk(const glm::ivec2 &position, int size, int minY, int maxY);
    void Draw(Shader &shader, const glm::mat4 &viewProjMatrix) const;

    void GenerateBlocks(const Camera &camera);

    inline static int Size() {return s_Size; }
    inline const std::vector<Block> &Blocks() const { return m_Blocks; }

private:
    static void GenerateVAO();


    void SubdivideLOD(glm::ivec3 pos, int size, int LOD, const Camera &camera);
};


#endif //MAIN_CHUNK_H
