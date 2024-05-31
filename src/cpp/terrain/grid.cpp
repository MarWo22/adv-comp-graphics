//
// Created by marti on 08/05/2024.
//

#include <iostream>
#include "grid.h"

void Grid::GenerateCells(const Camera &camera)
{
    int count = 0;
    m_Cells.clear();

    int size = m_BlockSize;
    while (size < int(m_ViewDistance))
        size *=2;

    size *=2;

    m_Frustum.UpdateFrustumPlanes(camera.Pos(), camera.Dir(), camera.Fov(), camera.AspectRatio(),
                                  camera.ZNear(), std::min(camera.ZNear() + m_ViewDistance, camera.ZFar()));
    // Get cell camera is in
    glm::ivec3 startPos {
            (int)std::floor(camera.Pos().x / (float)m_BlockSize) * m_BlockSize - size * .5,
            (int)std::floor(camera.Pos().y / (float)m_BlockSize) * m_BlockSize - size * .5,
            (int)std::floor(camera.Pos().z / (float)m_BlockSize) * m_BlockSize - size * .5,
    };

    std::queue<BoundingBox> checkList;

    checkList.push(BoundingBox{startPos, size});

    while(!checkList.empty())
    {
        BoundingBox cube = checkList.front();
        checkList.pop();
        if (m_Frustum.CubeCheck(cube.Pos, static_cast<float>(cube.Size)) == Frustum::Result::Intersecting)
        {
            count ++;
            if (cube.Size == m_BlockSize)
            {}
            else
            {
                int newSize = cube.Size / 2;
                checkList.push(BoundingBox{cube.Pos, newSize});
                checkList.push(BoundingBox{{cube.Pos.x + newSize, cube.Pos.y, cube.Pos.z}, newSize});
                checkList.push(BoundingBox{{cube.Pos.x + newSize, cube.Pos.y + newSize, cube.Pos.z}, newSize});
                checkList.push(BoundingBox{{cube.Pos.x + newSize, cube.Pos.y, cube.Pos.z + newSize}, newSize});
                checkList.push(BoundingBox{{cube.Pos.x + newSize, cube.Pos.y + newSize, cube.Pos.z + newSize}, newSize});
                checkList.push(BoundingBox{{cube.Pos.x, cube.Pos.y + newSize, cube.Pos.z}, newSize});
                checkList.push(BoundingBox{{cube.Pos.x, cube.Pos.y + newSize, cube.Pos.z + newSize}, newSize});
                checkList.push(BoundingBox{{cube.Pos.x, cube.Pos.y, cube.Pos.z + newSize}, newSize});
            }


        }
    }
    std::cout << count << "\n";
    std::cout << size * size * size << "\n";






    std::cout << camera.Pos().x << "," << camera.Pos().y << "," << camera.Pos().z << "\n";
    std::cout << startPos.x << "," << startPos.y << "," << startPos.z << "\n";

}

void Grid::GetNewBatch(const glm::vec3 &rayOrigin, const glm::vec3 &originCell, const Camera &camera, int size)
{
    glm::vec3 startingCell = NextStartingCell(rayOrigin, originCell, camera.Dir(), size);

    glm::ivec3 startingCellIndex = {std::round((startingCell.x - originCell.x) / m_BlockSize),
                                    std::round((startingCell.y - originCell.y) / m_BlockSize),
                                    std::round((startingCell.z - originCell.z) / m_BlockSize)};


    std::unordered_map<glm::ivec3, bool> visitedCells;
    std::queue<glm::ivec3> cellsToVisit;

//    if (camera.CubeFrustumCheck(startingCell, m_BlockSize))
//        m_Cells.push_back(GenerateCell(startingCell));

    EnqueueNew(startingCellIndex, &visitedCells, &cellsToVisit, size);

    while (!cellsToVisit.empty())
    {
        glm::ivec3 cellIdx = cellsToVisit.front();
        cellsToVisit.pop();
//        if (camera.CubeFrustumCheck(startingCell, m_BlockSize))
//        {
//
//            m_Cells.push_back(GenerateCell({
//               (float)cellIdx.x * m_BlockSize + originCell.x,
//               (float)cellIdx.y * m_BlockSize + originCell.y,
//               (float)cellIdx.z * m_BlockSize + originCell.z
//            }));
//            EnqueueNew(cellIdx, &visitedCells, &cellsToVisit, size);
//        }

    }






}

void Grid::EnqueueNew(const glm::ivec3 cell, std::unordered_map<glm::ivec3, bool> *visitedCells, std::queue<glm::ivec3> *cellsToVisit, int size)
{
    glm::vec3 xMin = {cell.x - 1, cell.y, cell.z};
    glm::vec3 xMax = {cell.x + 1, cell.y, cell.z};
    glm::vec3 yMin = {cell.x, cell.y - 1, cell.z};
    glm::vec3 yMax = {cell.x, cell.y + 1, cell.z};
    glm::vec3 zMin = {cell.x, cell.y, cell.z - 1};
    glm::vec3 zMax = {cell.x, cell.y, cell.z + 1};


    if (cell.z == size || cell.z == -size)
    {
        if (!(*visitedCells)[xMin] && cell.x != -size)
        {
            cellsToVisit->push(xMin);
            (*visitedCells)[xMin] = true;
        }
        if (!(*visitedCells)[xMax] && cell.x != size)
        {
            cellsToVisit->push(xMax);
            (*visitedCells)[xMax] = true;
        }
        if (!(*visitedCells)[yMin] && cell.y != -size)
        {
            cellsToVisit->push(yMin);
            (*visitedCells)[yMin] = true;
        }
        if (!(*visitedCells)[yMax] && cell.y != size)
        {
            cellsToVisit->push(yMax);
            (*visitedCells)[yMax] = true;
        }
    }

    if (cell.y == size || cell.y == -size)
    {
        if (!(*visitedCells)[xMin] && cell.x != -size)
        {
            cellsToVisit->push(xMin);
            (*visitedCells)[xMin] = true;
        }
        if (!(*visitedCells)[xMax] && cell.x != size)
        {
            cellsToVisit->push(xMax);
            (*visitedCells)[xMax] = true;
        }
        if (!(*visitedCells)[zMin] && cell.z != -size)
        {
            cellsToVisit->push(zMin);
            (*visitedCells)[zMin] = true;
        }
        if (!(*visitedCells)[zMax] && cell.z != size)
        {
            cellsToVisit->push(zMax);
            (*visitedCells)[zMax] = true;
        }
    }

    if (cell.x == size || cell.x == -size)
    {
        if (!(*visitedCells)[zMin] && cell.z != -size)
        {
            cellsToVisit->push(zMin);
            (*visitedCells)[zMin] = true;
        }
        if (!(*visitedCells)[zMax] && cell.z != size)
        {
            cellsToVisit->push(zMax);
            (*visitedCells)[zMax] = true;
        }
        if (!(*visitedCells)[yMin] && cell.y != -size)
        {
            cellsToVisit->push(yMin);
            (*visitedCells)[yMin] = true;
        }
        if (!(*visitedCells)[yMax] && cell.y != size)
        {
            cellsToVisit->push(yMax);
            (*visitedCells)[yMax] = true;
        }
    }



}



glm::vec3 Grid::NextStartingCell(const glm::vec3 &rayOrigin, const glm::ivec3 &startCell, const glm::vec3 &cameraDir, int size) const
{
    glm::vec3 boxMin {startCell.x - size - 1, startCell.y - size - 1, startCell.z - size - 1};
    glm::vec3 boxMax {startCell.x + size, startCell.y + size, startCell.z + size};

    glm::vec3 intersection = BoxIntersectTest(rayOrigin, cameraDir, boxMin, boxMax);


    glm::ivec3 nextCell;
    if (fmod(intersection.x, m_BlockSize) == 0)
    {
//        std::cout << "x intersection\n";
        nextCell = {intersection.x > rayOrigin.x ? intersection.x : intersection.x - 1, std::floor(intersection.y),
                    std::floor(intersection.z)};
    }
    else if (fmod(intersection.y, m_BlockSize) == 0)
    {
//        std::cout << "y intersection\n";
        nextCell = {std::floor(intersection.x), intersection.y > rayOrigin.y ? intersection.y : intersection.y - 1,
                    std::floor(intersection.z)};
    }    else if (fmod(intersection.z, m_BlockSize) == 0)
    {
//        std::cout << "z intersection\n";
        nextCell = {std::floor(intersection.x), std::floor(intersection.y),
                    intersection.z > rayOrigin.z ? intersection.z : intersection.z - 1};
    }

    return nextCell;
}



glm::vec3 Grid::BoxIntersectTest(const glm::vec3& origin, const glm::vec3& direction, const glm::vec3& boxMin, const glm::vec3& boxMax) {
    float tmin = (boxMin.x - origin.x) / direction.x;
    float tmax = (boxMax.x - origin.x) / direction.x;

    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (boxMin.y - origin.y) / direction.y;
    float tymax = (boxMax.y - origin.y) / direction.y;

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return glm::vec3(NAN);

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    float tzmin = (boxMin.z - origin.z) / direction.z;
    float tzmax = (boxMax.z - origin.z) / direction.z;

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return glm::vec3(NAN);;

    if (tzmin > tmin)
        tmin = tzmin;

    if (tzmax < tmax)
        tmax = tzmax;

    // Ensure tmin is in the positive direction of the ray
    if (tmin < 0 && tmax < 0)
        return glm::vec3(NAN);

    float t = (tmin < 0) ? tmax : tmin;

    // Calculate the intersection point
    return origin + t * direction;
}

Grid::Grid()
    : m_BlockSize(1.f)
    , m_CachedCells(5000)
    , m_EmptyCells(100000)
    , m_ViewDistance(100.f)
{}

void Grid::DebugDraw(Shader &shader, const glm::mat4 &viewProjMatrix)
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    shader.Bind();
    shader.SetUniform("modelViewProjectionMatrix", viewProjMatrix);

    for (auto cell : m_Cells)
        cell->DebugDraw();

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

Cell *Grid::GenerateCell(const glm::vec3 &position)
{
//    if (m_CachedCells.Exists(position))
//        return m_CachedCells.Retrieve(position);
//
//    Cell cell = Cell{position, static_cast<float>(m_BlockSize)};
//    return m_CachedCells.Add(position, cell);
    return nullptr;
}
