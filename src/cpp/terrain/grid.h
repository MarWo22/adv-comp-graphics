#ifndef MAIN_GRID_H
#define MAIN_GRID_H

#include <GL/glew.h>
#include <vector>
#include <unordered_map>
#include "cell.h"
#include "../camera.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include "mapCache.h"
#include "../frustum.h"

class Grid
{
    struct BoundingBox
    {
    public:
        glm::ivec3 Pos;
        int Size;


    };


    float m_ViewDistance;
    int m_BlockSize;
    int m_MaxCells;
    Frustum m_Frustum;

    std::vector<Cell*> m_Cells;
    MapCache<glm::vec3, Cell> m_CachedCells;
    MapCache<glm::vec3, bool> m_EmptyCells;

    GLuint *m_VertexBuffers;


public:
    Grid();
    void GenerateCells(const Camera &camera);
    void DebugDraw(Shader &shader, const glm::mat4 &viewProjMatrix);

private:
    static glm::vec3 BoxIntersectTest(const glm::vec3& origin, const glm::vec3& direction, const glm::vec3& boxMin, const glm::vec3& boxMax);
    glm::vec3 NextStartingCell(const glm::vec3 &rayOrigin, const glm::ivec3 &startCell, const glm::vec3 &cameraDir, int size) const;

    Cell *GenerateCell(const glm::vec3 &position);

    void GetNewBatch(const glm::vec3 &rayOrigin, const glm::vec3 &startCell, const Camera &camera, int size);

    void EnqueueNew(const glm::ivec3 cell, std::unordered_map<glm::ivec3, bool> *visitedCells,
                    std::queue<glm::ivec3> *cellsToVisit, int size);
};


#endif //MAIN_GRID_H
