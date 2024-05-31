//
// Created by marti on 23/05/2024.
//

#ifndef MAIN_TERRAINMANAGER_H
#define MAIN_TERRAINMANAGER_H


#include "chunkManager.h"
#include "density.h"
#include "../mapCache.h"
#include "../hashing.h"

class TerrainManager
{
//    struct Vertex
//    {
//        glm::vec4 position;
//        glm::vec4 normal;
//        glm::vec4 densitiesFront;
//        glm::vec4 densitiesBack;
//        glm::vec4 voxelInfo;
//        glm::vec4 edges;
//        glm::vec4 edgeDensities;
//        glm::vec4 localVertex;
//        glm::vec4 normalDensitiesXY;
//        glm::vec4 normalDensitiesZ;
//    };

    struct Vertex
    {
        glm::vec4 position;
        glm::vec4 normal;
    };

    struct VoxelGeometry
    {
        GLuint SSBO;
        int nVertices;
    };

    ChunkManager m_ChunkManager;
    Camera m_StaticCamera;

    Shader *m_DensityShader;
    Shader *m_VoxelShader;
    Shader *m_PrefixSumFirstPassShader;
    Shader *m_PrefixSumSecondPassShader;
    Shader *m_PrefixSumThirdPassShader;
    Shader *m_VoxelGeometryShader;
    Shader *m_TerrainShader;

    GLuint m_DensityTextures[25];
    GLuint m_PrefixSumSSBO[100];
    GLuint m_VertexCountSSBO[100];
    GLuint m_PrefixGroupSSBO;
    GLuint m_TrisRenderedSSBO;
    GLuint m_EdgeConnectListSSBO;
    GLuint m_NoiseTextures[3];

    CustomMapCache<glm::ivec4, bool, CustomIvec4Hash, Ivec4Equal> *m_EmptyBlockCache = new CustomMapCache<glm::ivec4, bool, CustomIvec4Hash, Ivec4Equal>{100000};
    CustomMapCache<glm::ivec4, VoxelGeometry, CustomIvec4Hash, Ivec4Equal> *m_GeometrySSBOCache = new CustomMapCache<glm::ivec4, VoxelGeometry, CustomIvec4Hash, Ivec4Equal>{3000};

    std::vector<VoxelGeometry> m_VoxelsToDraw;
    std::vector<std::pair<glm::vec4, VoxelGeometry>> m_NewVoxels;

    static glm::ivec4 s_EdgeConnectList[256][5];

public:
    TerrainManager() = default;
    explicit TerrainManager(Shader *densityShader,
                            Shader *voxelShader,
                            Shader *prefixSumFirstPassShader,
                            Shader *prefixSumSecondPassShader,
                            Shader *prefixSumThirdPassShader,
                            Shader *voxelGeometryShader,
                            Shader *terrainShader);

    void RenderTerrain(const Camera &camera);
    void RenderExtras(Shader &shader, const glm::mat4 &viewProjMatrix);
    void GenerateNoise();
    void FlushBuffer();

private:
    void SetupSSBO();
    void SetupTextures();
    void GenerateDensities(const std::vector<Block> &blocks);
    void GetBlocksToProcess(const std::vector<Chunk> &activeChunks, std::vector<Block> *blocksToProcess);
    void CalculateVertexCounts(int size);
    void PrefixSum(int count);
    void GenerateGeometry(const Block &block, int numVertices, int densityIndex);

};


#endif //MAIN_TERRAINMANAGER_H
