//
// Created by marti on 23/05/2024.
//

#include <thread>
#include <random>
#include "terrainManager.h"
#include "../../guiVariables.h"


TerrainManager::TerrainManager(Shader *densityShader,
                               Shader *voxelShader,
                               Shader *prefixSumFirstPassShader,
                               Shader *prefixSumSecondPassShader,
                               Shader *prefixSumThirdPassShader,
                               Shader *voxelGeometryShader,
                               Shader *terrainShader)
    : m_ChunkManager()
    , m_DensityShader(densityShader)
    , m_StaticCamera(Camera(glm::vec3(-70.0f, 50.0f, 70.0f), 45.f, 0, 0, 0))
    , m_VoxelShader(voxelShader)
    , m_PrefixSumFirstPassShader(prefixSumFirstPassShader)
    , m_PrefixSumSecondPassShader(prefixSumSecondPassShader)
    , m_PrefixSumThirdPassShader(prefixSumThirdPassShader)
    , m_PrefixSumSSBO()
    , m_PrefixGroupSSBO(0)
    , m_VertexCountSSBO()
    , m_DensityTextures()
    , m_TrisRenderedSSBO(0)
    , m_VoxelGeometryShader(voxelGeometryShader)
    , m_TerrainShader(terrainShader)
    , m_VoxelsToDraw(0)
{
    SetupSSBO();
    SetupTextures();
    GenerateNoise();
}

void TerrainManager::RenderTerrain(const Camera &camera)
{
    if (GuiVariables::s_StationaryCam)
        m_ChunkManager.GenerateChunks(m_StaticCamera);
    else
        m_ChunkManager.GenerateChunks(camera);
    std::vector<Chunk> activeChunks = m_ChunkManager.ActiveChunks();

    m_VoxelsToDraw.clear();
    m_NewVoxels.clear();

    std::vector<Block> blocksToProcess;
    GetBlocksToProcess(activeChunks, &blocksToProcess);
    int nBlocks = static_cast<int>(blocksToProcess.size());

    if (nBlocks != 0)
    {
        GenerateDensities(blocksToProcess);
        CalculateVertexCounts(nBlocks);

        PrefixSum(nBlocks);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_TrisRenderedSSBO);
        auto bufferData = (GLuint *) glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

        for (int i = 0; i != nBlocks; ++i)
        {
            if (bufferData[i] == 0)
            {
                if (GuiVariables::s_EnableEmptyBlockCaching)
                    m_EmptyBlockCache->Add(glm::vec4(blocksToProcess[i].Pos(), blocksToProcess[i].Size()), true);
            }
            else
            {
                GenerateGeometry(blocksToProcess[i], bufferData[i], i);
//                shader.Bind();
//                blocksToProcess[i].Draw(shader, camera.ProjectionMatrix() * camera.ViewMatrix());
            }
        }
    }

    m_TerrainShader->Bind();
    m_TerrainShader->SetUniform("viewProjectionMatrix", camera.ProjectionMatrix() * camera.ViewMatrix());

    if (GuiVariables::s_TerrainDrawMode == GuiVariables::TerrainDrawMode::Wireframe)
        glPolygonMode(GL_FRONT, GL_LINE);

    for (auto voxel : m_VoxelsToDraw)
    {
        if (GuiVariables::s_TerrainDrawMode != GuiVariables::TerrainDrawMode::Disabled)
        {
            m_TerrainShader->SetUniform("displayNormals", GuiVariables::s_DisplayNormals);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, voxel.SSBO);
            glDrawArrays(GL_TRIANGLES, 0, voxel.nVertices);
        }
        if (!GuiVariables::s_EnableSSBOCaching)
            glDeleteBuffers(1, &voxel.SSBO);
    }

    if (GuiVariables::s_TerrainDrawMode == GuiVariables::TerrainDrawMode::Wireframe)
        glPolygonMode(GL_FRONT, GL_FILL);

    if (GuiVariables::s_EnableSSBOCaching)
    {
        for (auto &pair : m_NewVoxels)
        {
            VoxelGeometry temp;
            m_GeometrySSBOCache->Add(pair.first, pair.second, &temp);
            if (temp.SSBO != 0)
                glDeleteBuffers(1, &temp.SSBO);

        }
    }



}

void TerrainManager::RenderExtras(Shader &shader, const glm::mat4 &viewProjMatrix)
{
    shader.Bind();

    std::vector<Chunk> chunks = m_ChunkManager.ActiveChunks();

    for (const auto& chunk : chunks)
        chunk.Draw(shader, viewProjMatrix);
}

void TerrainManager::SetupSSBO()
{
    for (int i = 0; i != 100; ++i)
    {
        glGenBuffers(1, &m_VertexCountSSBO[i]);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_VertexCountSSBO[i]);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLuint) * 33 * 33 * 33, nullptr, GL_DYNAMIC_DRAW);

        glGenBuffers(1, &m_PrefixSumSSBO[i]);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_PrefixSumSSBO[i]);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLuint) * 33 * 33 * 33, nullptr, GL_DYNAMIC_DRAW);
    }

    glGenBuffers(1, &m_PrefixGroupSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_PrefixGroupSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLuint) * 16, nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &m_TrisRenderedSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_TrisRenderedSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLuint) * 100, nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &m_EdgeConnectListSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_EdgeConnectListSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::ivec4) * 256 * 5, s_EdgeConnectList, GL_STATIC_READ);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void TerrainManager::SetupTextures()
{
    for (int i = 0; i != 25; ++i)
    {
        glGenTextures(1, &m_DensityTextures[i]);
        glBindTexture(GL_TEXTURE_3D, m_DensityTextures[i]);

        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, 33, 33, 33, 0, GL_RGBA, GL_FLOAT, NULL);
    }
}

void TerrainManager::GenerateDensities(const std::vector<Block> &blocks)
{
    m_DensityShader->Bind();
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_3D, m_NoiseTextures[0]);
    glActiveTexture(GL_TEXTURE8);
    glBindTexture(GL_TEXTURE_3D, m_NoiseTextures[1]);

    glActiveTexture(GL_TEXTURE0);

    m_DensityShader->SetUniform("octaves",
                                0 | GuiVariables::s_Octaves[0] << 1
                                        | GuiVariables::s_Octaves[1] << 2
                                        | GuiVariables::s_Octaves[2] << 3
                                        | GuiVariables::s_Octaves[3] << 4
                                        | GuiVariables::s_Octaves[4] << 5
                                        | GuiVariables::s_Octaves[5] << 6);

    int index = 0;

    for (auto const &block : blocks)
    {
        int texture_channel = index % 4;

        glBindTexture(GL_TEXTURE_3D, m_DensityTextures[index / 4]);
        glBindImageTexture(0, m_DensityTextures[index / 4], 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA32F);


        m_DensityShader->SetUniform("block_position", block.Pos());
        m_DensityShader->SetUniform("block_size", static_cast<float>(block.Size()));
        m_DensityShader->SetUniform("texture_channel", texture_channel);

        glDispatchCompute(5, 5, 5);
        glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

        ++index;
    }
}

void TerrainManager::GetBlocksToProcess(const std::vector<Chunk> &activeChunks, std::vector<Block> *blocksToProcess)
{
    int count = 0;
    for (const auto& chunk: activeChunks)
    {
        for (auto const &block: chunk.Blocks())
        {
            glm::vec4 key = glm::vec4(block.Pos(), block.Size());
            if (GuiVariables::s_EnableEmptyBlockCaching && m_EmptyBlockCache->Exists(key))
                continue;

            if (GuiVariables::s_EnableSSBOCaching && m_GeometrySSBOCache->Exists(key))
            {
                m_VoxelsToDraw.push_back(m_GeometrySSBOCache->Retrieve(key));
                continue;
            }

            blocksToProcess->push_back(block);
            ++count;

            if (count == 100)
                return;
        }
    }
}

void TerrainManager::PrefixSum(int size)
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_PrefixGroupSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_TrisRenderedSSBO);


    for (int idx = 0; idx != size; ++idx)
    {

            // First pass, does prefix sum on each 2048 block, invoked 1028*16 times
        m_PrefixSumFirstPassShader->Bind();
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_VertexCountSSBO[idx]);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_PrefixSumSSBO[idx]);

        glDispatchCompute(1, 1, 16);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

            // Second pass, gets the value that should be added to each block to complete the prefix sum,
            // and stores the number of triangles this voxel will generate
        m_PrefixSumSecondPassShader->Bind();
        m_PrefixSumSecondPassShader->SetUniform("index", idx);

        glDispatchCompute(1, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

            // Third pass, applies the value to each block to complete the prefix sum
        m_PrefixSumThirdPassShader->Bind();

        glDispatchCompute(32, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
    }
}

void TerrainManager::CalculateVertexCounts(int size)
{
    glActiveTexture(GL_TEXTURE0);

    for (int idx = 0; idx != size; ++idx)
    {
        int texture_channel = idx % 4;

        glBindTexture(GL_TEXTURE_3D, m_DensityTextures[idx / 4]);
        glBindImageTexture(0, m_DensityTextures[idx / 4], 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA32F);


        m_VoxelShader->Bind();
        m_VoxelShader->SetUniform("texture_channel", texture_channel);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_VertexCountSSBO[idx]);

        glDispatchCompute(1, 1, 32);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

//        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_VertexCountSSBO[idx]);
//        auto bufferData = (float *) glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
//        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
//
//        for (int i = 0; i != 6; ++i)
//            std::cout << bufferData[i] << "\n";
//        exit(1);
    }
}

void TerrainManager::GenerateGeometry(const Block &block, int numTriangles, int index)
{

        // Generate a new geometrySSBO
    GLuint geometrySSBO;
    glGenBuffers(1, &geometrySSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, geometrySSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Vertex) * numTriangles * 3, nullptr, GL_STATIC_DRAW);

    m_VoxelGeometryShader->Bind();

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, geometrySSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, m_EdgeConnectListSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_PrefixSumSSBO[index]);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_VertexCountSSBO[index]);

    glBindTexture(GL_TEXTURE_3D, m_DensityTextures[index / 4]);
    glBindImageTexture(0, m_DensityTextures[index / 4], 0, GL_TRUE, 0, GL_READ_ONLY, GL_RGBA32F);


    m_VoxelGeometryShader->SetUniform("block_position", block.Pos());
    m_VoxelGeometryShader->SetUniform("block_size", static_cast<float>(block.Size()));
    m_VoxelGeometryShader->SetUniform("texture_channel", index % 4);

    glDispatchCompute(1, 1, 32);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

//
//    glBindBuffer(GL_SHADER_STORAGE_BUFFER, geometrySSBO);
//    auto bufferData = (Vertex *) glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
//    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
//
//    std::cout << "------------------------------------------------\n";
//    for (int i = 0; i != 6; ++i)
//    {
//        std::cout << "(Position) " << bufferData[i].position.x << "," << bufferData[i].position.y << "," << bufferData[i].position.z << "," << bufferData[i].position.w << "\n";
//        std::cout << "(Normal) " << bufferData[i].normal.x << "," << bufferData[i].normal.y << "," << bufferData[i].normal.z << "," << bufferData[i].normal.w << "\n";
//        std::cout << "(Dens Front) " <<  bufferData[i].densitiesFront.x << "," << bufferData[i].densitiesFront.y << "," << bufferData[i].densitiesFront.z << "," << bufferData[i].densitiesFront.w << "\n";
//        std::cout << "(Dens Back) " << bufferData[i].densitiesBack.x << "," << bufferData[i].densitiesBack.y << "," << bufferData[i].densitiesBack.z << "," << bufferData[i].densitiesBack.w << "\n";
//        std::cout << "(Vox Info) " << bufferData[i].voxelInfo.x << "," << bufferData[i].voxelInfo.y << "," << bufferData[i].voxelInfo.z << "," << bufferData[i].voxelInfo.w << "\n";
//        std::cout << "(Edges) " << bufferData[i].edges.x << "," << bufferData[i].edges.y << "," << bufferData[i].edges.z << "," << bufferData[i].edges.w << "\n";
//        std::cout << "(Edge Densities) " << bufferData[i].edgeDensities.x << "," << bufferData[i].edgeDensities.y << "," << bufferData[i].edgeDensities.z << "," << bufferData[i].edgeDensities.w << "\n";
//        std::cout << "(Local Vertex) " << bufferData[i].localVertex.x << "," << bufferData[i].localVertex.y << "," << bufferData[i].localVertex.z << "," << bufferData[i].localVertex.w << "\n";
//        std::cout << "(Normal Densities XY) " << bufferData[i].normalDensitiesXY.x << "," << bufferData[i].normalDensitiesXY.y << "," << bufferData[i].normalDensitiesXY.z << "," << bufferData[i].normalDensitiesXY.w << "\n";
//        std::cout << "(Normal Densities Z) " << bufferData[i].normalDensitiesZ.x << "," << bufferData[i].normalDensitiesZ.y << "," << bufferData[i].normalDensitiesZ.z << "," << bufferData[i].normalDensitiesZ.w << "\n";
//
//        std::cout << "------------------------------------------------\n";
//    }
//    exit(1);

        // Create the geometry struct
    VoxelGeometry voxelGeometry = {geometrySSBO, numTriangles * 3};
        // Store to draw next frame
    m_VoxelsToDraw.push_back(voxelGeometry);


        // Store for caching purposes
    if(GuiVariables::s_EnableSSBOCaching)
        m_NewVoxels.push_back({glm::vec4(block.Pos(), block.Size()), voxelGeometry});
}

void TerrainManager::GenerateNoise()
{
    std::vector<float> noiseData(33 * 33 * 33 * 4);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

    for (int textureIdx = 0; textureIdx != 3; ++textureIdx)
    {
        for (int i = 0; i != 33*33*33; ++i)
        {
            noiseData[i * 4] = dis(gen);
            noiseData[i * 4 + 1] = dis(gen);
            noiseData[i * 4 + 2] = dis(gen);
            noiseData[i * 4 + 3] = dis(gen);
        }

        glGenTextures(1, &m_NoiseTextures[textureIdx]);
        glBindTexture(GL_TEXTURE_3D, m_NoiseTextures[textureIdx]);

        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

        glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, 33, 33, 33, 0, GL_RGBA, GL_FLOAT, noiseData.data());

    }
}

void TerrainManager::FlushBuffer()
{
    auto keys = m_GeometrySSBOCache->Keys();
    while(keys.size() > 0)
    {
        VoxelGeometry voxel = m_GeometrySSBOCache->Retrieve(keys.front());
        glDeleteBuffers(1, &voxel.SSBO);
//        std::cout << "deleting buffer " << voxel.SSBO << "\n";
        keys.pop();
    }
    m_GeometrySSBOCache->Clear();
    m_EmptyBlockCache->Clear();

    delete m_GeometrySSBOCache;
    delete m_EmptyBlockCache;

    m_GeometrySSBOCache = new CustomMapCache<glm::ivec4, VoxelGeometry, CustomIvec4Hash, Ivec4Equal>{1000};
    m_EmptyBlockCache = new CustomMapCache<glm::ivec4, bool, CustomIvec4Hash, Ivec4Equal>{100000};
}
