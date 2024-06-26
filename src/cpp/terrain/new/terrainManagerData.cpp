//
// Created by marti on 30/05/2024.
//

#include "terrainManager.h"

glm::ivec4 TerrainManager::s_EdgeConnectList[256][5] = {
        { glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  8,  3, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  1,  9, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1,  8,  3, 0), glm::ivec4( 9,  8,  1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1,  2, 10, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  8,  3, 0), glm::ivec4( 1,  2, 10, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  2, 10, 0), glm::ivec4( 0,  2,  9, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 2,  8,  3, 0), glm::ivec4( 2, 10,  8, 0), glm::ivec4(10,  9,  8, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 3, 11,  2, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0, 11,  2, 0), glm::ivec4( 8, 11,  0, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1,  9,  0, 0), glm::ivec4( 2,  3, 11, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1, 11,  2, 0), glm::ivec4( 1,  9, 11, 0), glm::ivec4( 9,  8, 11, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 3, 10,  1, 0), glm::ivec4(11, 10,  3, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0, 10,  1, 0), glm::ivec4( 0,  8, 10, 0), glm::ivec4( 8, 11, 10, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 3,  9,  0, 0), glm::ivec4( 3, 11,  9, 0), glm::ivec4(11, 10,  9, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  8, 10, 0), glm::ivec4(10,  8, 11, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 4,  7,  8, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 4,  3,  0, 0), glm::ivec4( 7,  3,  4, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  1,  9, 0), glm::ivec4( 8,  4,  7, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 4,  1,  9, 0), glm::ivec4( 4,  7,  1, 0), glm::ivec4( 7,  3,  1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1,  2, 10, 0), glm::ivec4( 8,  4,  7, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 3,  4,  7, 0), glm::ivec4( 3,  0,  4, 0), glm::ivec4( 1,  2, 10, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  2, 10, 0), glm::ivec4( 9,  0,  2, 0), glm::ivec4( 8,  4,  7, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 2, 10,  9, 0), glm::ivec4( 2,  9,  7, 0), glm::ivec4( 2,  7,  3, 0), glm::ivec4( 7,  9,  4, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 8,  4,  7, 0), glm::ivec4( 3, 11,  2, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(11,  4,  7, 0), glm::ivec4(11,  2,  4, 0), glm::ivec4( 2,  0,  4, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  0,  1, 0), glm::ivec4( 8,  4,  7, 0), glm::ivec4( 2,  3, 11, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 4,  7, 11, 0), glm::ivec4( 9,  4, 11, 0), glm::ivec4( 9, 11,  2, 0), glm::ivec4( 9,  2,  1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 3, 10,  1, 0), glm::ivec4( 3, 11, 10, 0), glm::ivec4( 7,  8,  4, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1, 11, 10, 0), glm::ivec4( 1,  4, 11, 0), glm::ivec4( 1,  0,  4, 0), glm::ivec4( 7, 11,  4, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 4,  7,  8, 0), glm::ivec4( 9,  0, 11, 0), glm::ivec4( 9, 11, 10, 0), glm::ivec4(11,  0,  3, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 4,  7, 11, 0), glm::ivec4( 4, 11,  9, 0), glm::ivec4( 9, 11, 10, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  5,  4, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  5,  4, 0), glm::ivec4( 0,  8,  3, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  5,  4, 0), glm::ivec4( 1,  5,  0, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 8,  5,  4, 0), glm::ivec4( 8,  3,  5, 0), glm::ivec4( 3,  1,  5, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1,  2, 10, 0), glm::ivec4( 9,  5,  4, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 3,  0,  8, 0), glm::ivec4( 1,  2, 10, 0), glm::ivec4( 4,  9,  5, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 5,  2, 10, 0), glm::ivec4( 5,  4,  2, 0), glm::ivec4( 4,  0,  2, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 2, 10,  5, 0), glm::ivec4( 3,  2,  5, 0), glm::ivec4( 3,  5,  4, 0), glm::ivec4( 3,  4,  8, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  5,  4, 0), glm::ivec4( 2,  3, 11, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0, 11,  2, 0), glm::ivec4( 0,  8, 11, 0), glm::ivec4( 4,  9,  5, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  5,  4, 0), glm::ivec4( 0,  1,  5, 0), glm::ivec4( 2,  3, 11, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 2,  1,  5, 0), glm::ivec4( 2,  5,  8, 0), glm::ivec4( 2,  8, 11, 0), glm::ivec4( 4,  8,  5, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(10,  3, 11, 0), glm::ivec4(10,  1,  3, 0), glm::ivec4( 9,  5,  4, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 4,  9,  5, 0), glm::ivec4( 0,  8,  1, 0), glm::ivec4( 8, 10,  1, 0), glm::ivec4( 8, 11, 10, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 5,  4,  0, 0), glm::ivec4( 5,  0, 11, 0), glm::ivec4( 5, 11, 10, 0), glm::ivec4(11,  0,  3, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 5,  4,  8, 0), glm::ivec4( 5,  8, 10, 0), glm::ivec4(10,  8, 11, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  7,  8, 0), glm::ivec4( 5,  7,  9, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  3,  0, 0), glm::ivec4( 9,  5,  3, 0), glm::ivec4( 5,  7,  3, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  7,  8, 0), glm::ivec4( 0,  1,  7, 0), glm::ivec4( 1,  5,  7, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1,  5,  3, 0), glm::ivec4( 3,  5,  7, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  7,  8, 0), glm::ivec4( 9,  5,  7, 0), glm::ivec4(10,  1,  2, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(10,  1,  2, 0), glm::ivec4( 9,  5,  0, 0), glm::ivec4( 5,  3,  0, 0), glm::ivec4( 5,  7,  3, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 8,  0,  2, 0), glm::ivec4( 8,  2,  5, 0), glm::ivec4( 8,  5,  7, 0), glm::ivec4(10,  5,  2, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 2, 10,  5, 0), glm::ivec4( 2,  5,  3, 0), glm::ivec4( 3,  5,  7, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 7,  9,  5, 0), glm::ivec4( 7,  8,  9, 0), glm::ivec4( 3, 11,  2, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  5,  7, 0), glm::ivec4( 9,  7,  2, 0), glm::ivec4( 9,  2,  0, 0), glm::ivec4( 2,  7, 11, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 2,  3, 11, 0), glm::ivec4( 0,  1,  8, 0), glm::ivec4( 1,  7,  8, 0), glm::ivec4( 1,  5,  7, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(11,  2,  1, 0), glm::ivec4(11,  1,  7, 0), glm::ivec4( 7,  1,  5, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  5,  8, 0), glm::ivec4( 8,  5,  7, 0), glm::ivec4(10,  1,  3, 0), glm::ivec4(10,  3, 11, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 5,  7,  0, 0), glm::ivec4( 5,  0,  9, 0), glm::ivec4( 7, 11,  0, 0), glm::ivec4( 1,  0, 10, 0), glm::ivec4(11, 10,  0, 0) },
        { glm::ivec4(11, 10,  0, 0), glm::ivec4(11,  0,  3, 0), glm::ivec4(10,  5,  0, 0), glm::ivec4( 8,  0,  7, 0), glm::ivec4( 5,  7,  0, 0) },
        { glm::ivec4(11, 10,  5, 0), glm::ivec4( 7, 11,  5, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(10,  6,  5, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  8,  3, 0), glm::ivec4( 5, 10,  6, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  0,  1, 0), glm::ivec4( 5, 10,  6, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1,  8,  3, 0), glm::ivec4( 1,  9,  8, 0), glm::ivec4( 5, 10,  6, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1,  6,  5, 0), glm::ivec4( 2,  6,  1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1,  6,  5, 0), glm::ivec4( 1,  2,  6, 0), glm::ivec4( 3,  0,  8, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  6,  5, 0), glm::ivec4( 9,  0,  6, 0), glm::ivec4( 0,  2,  6, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 5,  9,  8, 0), glm::ivec4( 5,  8,  2, 0), glm::ivec4( 5,  2,  6, 0), glm::ivec4( 3,  2,  8, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 2,  3, 11, 0), glm::ivec4(10,  6,  5, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(11,  0,  8, 0), glm::ivec4(11,  2,  0, 0), glm::ivec4(10,  6,  5, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  1,  9, 0), glm::ivec4( 2,  3, 11, 0), glm::ivec4( 5, 10,  6, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 5, 10,  6, 0), glm::ivec4( 1,  9,  2, 0), glm::ivec4( 9, 11,  2, 0), glm::ivec4( 9,  8, 11, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 6,  3, 11, 0), glm::ivec4( 6,  5,  3, 0), glm::ivec4( 5,  1,  3, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  8, 11, 0), glm::ivec4( 0, 11,  5, 0), glm::ivec4( 0,  5,  1, 0), glm::ivec4( 5, 11,  6, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 3, 11,  6, 0), glm::ivec4( 0,  3,  6, 0), glm::ivec4( 0,  6,  5, 0), glm::ivec4( 0,  5,  9, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 6,  5,  9, 0), glm::ivec4( 6,  9, 11, 0), glm::ivec4(11,  9,  8, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 5, 10,  6, 0), glm::ivec4( 4,  7,  8, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 4,  3,  0, 0), glm::ivec4( 4,  7,  3, 0), glm::ivec4( 6,  5, 10, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1,  9,  0, 0), glm::ivec4( 5, 10,  6, 0), glm::ivec4( 8,  4,  7, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(10,  6,  5, 0), glm::ivec4( 1,  9,  7, 0), glm::ivec4( 1,  7,  3, 0), glm::ivec4( 7,  9,  4, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 6,  1,  2, 0), glm::ivec4( 6,  5,  1, 0), glm::ivec4( 4,  7,  8, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1,  2,  5, 0), glm::ivec4( 5,  2,  6, 0), glm::ivec4( 3,  0,  4, 0), glm::ivec4( 3,  4,  7, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 8,  4,  7, 0), glm::ivec4( 9,  0,  5, 0), glm::ivec4( 0,  6,  5, 0), glm::ivec4( 0,  2,  6, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 7,  3,  9, 0), glm::ivec4( 7,  9,  4, 0), glm::ivec4( 3,  2,  9, 0), glm::ivec4( 5,  9,  6, 0), glm::ivec4( 2,  6,  9, 0) },
        { glm::ivec4( 3, 11,  2, 0), glm::ivec4( 7,  8,  4, 0), glm::ivec4(10,  6,  5, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 5, 10,  6, 0), glm::ivec4( 4,  7,  2, 0), glm::ivec4( 4,  2,  0, 0), glm::ivec4( 2,  7, 11, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  1,  9, 0), glm::ivec4( 4,  7,  8, 0), glm::ivec4( 2,  3, 11, 0), glm::ivec4( 5, 10,  6, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  2,  1, 0), glm::ivec4( 9, 11,  2, 0), glm::ivec4( 9,  4, 11, 0), glm::ivec4( 7, 11,  4, 0), glm::ivec4( 5, 10,  6, 0) },
        { glm::ivec4( 8,  4,  7, 0), glm::ivec4( 3, 11,  5, 0), glm::ivec4( 3,  5,  1, 0), glm::ivec4( 5, 11,  6, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 5,  1, 11, 0), glm::ivec4( 5, 11,  6, 0), glm::ivec4( 1,  0, 11, 0), glm::ivec4( 7, 11,  4, 0), glm::ivec4( 0,  4, 11, 0) },
        { glm::ivec4( 0,  5,  9, 0), glm::ivec4( 0,  6,  5, 0), glm::ivec4( 0,  3,  6, 0), glm::ivec4(11,  6,  3, 0), glm::ivec4( 8,  4,  7, 0) },
        { glm::ivec4( 6,  5,  9, 0), glm::ivec4( 6,  9, 11, 0), glm::ivec4( 4,  7,  9, 0), glm::ivec4( 7, 11,  9, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(10,  4,  9, 0), glm::ivec4( 6,  4, 10, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 4, 10,  6, 0), glm::ivec4( 4,  9, 10, 0), glm::ivec4( 0,  8,  3, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(10,  0,  1, 0), glm::ivec4(10,  6,  0, 0), glm::ivec4( 6,  4,  0, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 8,  3,  1, 0), glm::ivec4( 8,  1,  6, 0), glm::ivec4( 8,  6,  4, 0), glm::ivec4( 6,  1, 10, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1,  4,  9, 0), glm::ivec4( 1,  2,  4, 0), glm::ivec4( 2,  6,  4, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 3,  0,  8, 0), glm::ivec4( 1,  2,  9, 0), glm::ivec4( 2,  4,  9, 0), glm::ivec4( 2,  6,  4, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  2,  4, 0), glm::ivec4( 4,  2,  6, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 8,  3,  2, 0), glm::ivec4( 8,  2,  4, 0), glm::ivec4( 4,  2,  6, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(10,  4,  9, 0), glm::ivec4(10,  6,  4, 0), glm::ivec4(11,  2,  3, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  8,  2, 0), glm::ivec4( 2,  8, 11, 0), glm::ivec4( 4,  9, 10, 0), glm::ivec4( 4, 10,  6, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 3, 11,  2, 0), glm::ivec4( 0,  1,  6, 0), glm::ivec4( 0,  6,  4, 0), glm::ivec4( 6,  1, 10, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 6,  4,  1, 0), glm::ivec4( 6,  1, 10, 0), glm::ivec4( 4,  8,  1, 0), glm::ivec4( 2,  1, 11, 0), glm::ivec4( 8, 11,  1, 0) },
        { glm::ivec4( 9,  6,  4, 0), glm::ivec4( 9,  3,  6, 0), glm::ivec4( 9,  1,  3, 0), glm::ivec4(11,  6,  3, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 8, 11,  1, 0), glm::ivec4( 8,  1,  0, 0), glm::ivec4(11,  6,  1, 0), glm::ivec4( 9,  1,  4, 0), glm::ivec4( 6,  4,  1, 0) },
        { glm::ivec4( 3, 11,  6, 0), glm::ivec4( 3,  6,  0, 0), glm::ivec4( 0,  6,  4, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 6,  4,  8, 0), glm::ivec4(11,  6,  8, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 7, 10,  6, 0), glm::ivec4( 7,  8, 10, 0), glm::ivec4( 8,  9, 10, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  7,  3, 0), glm::ivec4( 0, 10,  7, 0), glm::ivec4( 0,  9, 10, 0), glm::ivec4( 6,  7, 10, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(10,  6,  7, 0), glm::ivec4( 1, 10,  7, 0), glm::ivec4( 1,  7,  8, 0), glm::ivec4( 1,  8,  0, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(10,  6,  7, 0), glm::ivec4(10,  7,  1, 0), glm::ivec4( 1,  7,  3, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1,  2,  6, 0), glm::ivec4( 1,  6,  8, 0), glm::ivec4( 1,  8,  9, 0), glm::ivec4( 8,  6,  7, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 2,  6,  9, 0), glm::ivec4( 2,  9,  1, 0), glm::ivec4( 6,  7,  9, 0), glm::ivec4( 0,  9,  3, 0), glm::ivec4( 7,  3,  9, 0) },
        { glm::ivec4( 7,  8,  0, 0), glm::ivec4( 7,  0,  6, 0), glm::ivec4( 6,  0,  2, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 7,  3,  2, 0), glm::ivec4( 6,  7,  2, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 2,  3, 11, 0), glm::ivec4(10,  6,  8, 0), glm::ivec4(10,  8,  9, 0), glm::ivec4( 8,  6,  7, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 2,  0,  7, 0), glm::ivec4( 2,  7, 11, 0), glm::ivec4( 0,  9,  7, 0), glm::ivec4( 6,  7, 10, 0), glm::ivec4( 9, 10,  7, 0) },
        { glm::ivec4( 1,  8,  0, 0), glm::ivec4( 1,  7,  8, 0), glm::ivec4( 1, 10,  7, 0), glm::ivec4( 6,  7, 10, 0), glm::ivec4( 2,  3, 11, 0) },
        { glm::ivec4(11,  2,  1, 0), glm::ivec4(11,  1,  7, 0), glm::ivec4(10,  6,  1, 0), glm::ivec4( 6,  7,  1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 8,  9,  6, 0), glm::ivec4( 8,  6,  7, 0), glm::ivec4( 9,  1,  6, 0), glm::ivec4(11,  6,  3, 0), glm::ivec4( 1,  3,  6, 0) },
        { glm::ivec4( 0,  9,  1, 0), glm::ivec4(11,  6,  7, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 7,  8,  0, 0), glm::ivec4( 7,  0,  6, 0), glm::ivec4( 3, 11,  0, 0), glm::ivec4(11,  6,  0, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 7, 11,  6, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 7,  6, 11, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 3,  0,  8, 0), glm::ivec4(11,  7,  6, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  1,  9, 0), glm::ivec4(11,  7,  6, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 8,  1,  9, 0), glm::ivec4( 8,  3,  1, 0), glm::ivec4(11,  7,  6, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(10,  1,  2, 0), glm::ivec4( 6, 11,  7, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1,  2, 10, 0), glm::ivec4( 3,  0,  8, 0), glm::ivec4( 6, 11,  7, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 2,  9,  0, 0), glm::ivec4( 2, 10,  9, 0), glm::ivec4( 6, 11,  7, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 6, 11,  7, 0), glm::ivec4( 2, 10,  3, 0), glm::ivec4(10,  8,  3, 0), glm::ivec4(10,  9,  8, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 7,  2,  3, 0), glm::ivec4( 6,  2,  7, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 7,  0,  8, 0), glm::ivec4( 7,  6,  0, 0), glm::ivec4( 6,  2,  0, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 2,  7,  6, 0), glm::ivec4( 2,  3,  7, 0), glm::ivec4( 0,  1,  9, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1,  6,  2, 0), glm::ivec4( 1,  8,  6, 0), glm::ivec4( 1,  9,  8, 0), glm::ivec4( 8,  7,  6, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(10,  7,  6, 0), glm::ivec4(10,  1,  7, 0), glm::ivec4( 1,  3,  7, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(10,  7,  6, 0), glm::ivec4( 1,  7, 10, 0), glm::ivec4( 1,  8,  7, 0), glm::ivec4( 1,  0,  8, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  3,  7, 0), glm::ivec4( 0,  7, 10, 0), glm::ivec4( 0, 10,  9, 0), glm::ivec4( 6, 10,  7, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 7,  6, 10, 0), glm::ivec4( 7, 10,  8, 0), glm::ivec4( 8, 10,  9, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 6,  8,  4, 0), glm::ivec4(11,  8,  6, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 3,  6, 11, 0), glm::ivec4( 3,  0,  6, 0), glm::ivec4( 0,  4,  6, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 8,  6, 11, 0), glm::ivec4( 8,  4,  6, 0), glm::ivec4( 9,  0,  1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  4,  6, 0), glm::ivec4( 9,  6,  3, 0), glm::ivec4( 9,  3,  1, 0), glm::ivec4(11,  3,  6, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 6,  8,  4, 0), glm::ivec4( 6, 11,  8, 0), glm::ivec4( 2, 10,  1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1,  2, 10, 0), glm::ivec4( 3,  0, 11, 0), glm::ivec4( 0,  6, 11, 0), glm::ivec4( 0,  4,  6, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 4, 11,  8, 0), glm::ivec4( 4,  6, 11, 0), glm::ivec4( 0,  2,  9, 0), glm::ivec4( 2, 10,  9, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(10,  9,  3, 0), glm::ivec4(10,  3,  2, 0), glm::ivec4( 9,  4,  3, 0), glm::ivec4(11,  3,  6, 0), glm::ivec4( 4,  6,  3, 0) },
        { glm::ivec4( 8,  2,  3, 0), glm::ivec4( 8,  4,  2, 0), glm::ivec4( 4,  6,  2, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  4,  2, 0), glm::ivec4( 4,  6,  2, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1,  9,  0, 0), glm::ivec4( 2,  3,  4, 0), glm::ivec4( 2,  4,  6, 0), glm::ivec4( 4,  3,  8, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1,  9,  4, 0), glm::ivec4( 1,  4,  2, 0), glm::ivec4( 2,  4,  6, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 8,  1,  3, 0), glm::ivec4( 8,  6,  1, 0), glm::ivec4( 8,  4,  6, 0), glm::ivec4( 6, 10,  1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(10,  1,  0, 0), glm::ivec4(10,  0,  6, 0), glm::ivec4( 6,  0,  4, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 4,  6,  3, 0), glm::ivec4( 4,  3,  8, 0), glm::ivec4( 6, 10,  3, 0), glm::ivec4( 0,  3,  9, 0), glm::ivec4(10,  9,  3, 0) },
        { glm::ivec4(10,  9,  4, 0), glm::ivec4( 6, 10,  4, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 4,  9,  5, 0), glm::ivec4( 7,  6, 11, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  8,  3, 0), glm::ivec4( 4,  9,  5, 0), glm::ivec4(11,  7,  6, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 5,  0,  1, 0), glm::ivec4( 5,  4,  0, 0), glm::ivec4( 7,  6, 11, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(11,  7,  6, 0), glm::ivec4( 8,  3,  4, 0), glm::ivec4( 3,  5,  4, 0), glm::ivec4( 3,  1,  5, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  5,  4, 0), glm::ivec4(10,  1,  2, 0), glm::ivec4( 7,  6, 11, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 6, 11,  7, 0), glm::ivec4( 1,  2, 10, 0), glm::ivec4( 0,  8,  3, 0), glm::ivec4( 4,  9,  5, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 7,  6, 11, 0), glm::ivec4( 5,  4, 10, 0), glm::ivec4( 4,  2, 10, 0), glm::ivec4( 4,  0,  2, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 3,  4,  8, 0), glm::ivec4( 3,  5,  4, 0), glm::ivec4( 3,  2,  5, 0), glm::ivec4(10,  5,  2, 0), glm::ivec4(11,  7,  6, 0) },
        { glm::ivec4( 7,  2,  3, 0), glm::ivec4( 7,  6,  2, 0), glm::ivec4( 5,  4,  9, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  5,  4, 0), glm::ivec4( 0,  8,  6, 0), glm::ivec4( 0,  6,  2, 0), glm::ivec4( 6,  8,  7, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 3,  6,  2, 0), glm::ivec4( 3,  7,  6, 0), glm::ivec4( 1,  5,  0, 0), glm::ivec4( 5,  4,  0, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 6,  2,  8, 0), glm::ivec4( 6,  8,  7, 0), glm::ivec4( 2,  1,  8, 0), glm::ivec4( 4,  8,  5, 0), glm::ivec4( 1,  5,  8, 0) },
        { glm::ivec4( 9,  5,  4, 0), glm::ivec4(10,  1,  6, 0), glm::ivec4( 1,  7,  6, 0), glm::ivec4( 1,  3,  7, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1,  6, 10, 0), glm::ivec4( 1,  7,  6, 0), glm::ivec4( 1,  0,  7, 0), glm::ivec4( 8,  7,  0, 0), glm::ivec4( 9,  5,  4, 0) },
        { glm::ivec4( 4,  0, 10, 0), glm::ivec4( 4, 10,  5, 0), glm::ivec4( 0,  3, 10, 0), glm::ivec4( 6, 10,  7, 0), glm::ivec4( 3,  7, 10, 0) },
        { glm::ivec4( 7,  6, 10, 0), glm::ivec4( 7, 10,  8, 0), glm::ivec4( 5,  4, 10, 0), glm::ivec4( 4,  8, 10, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 6,  9,  5, 0), glm::ivec4( 6, 11,  9, 0), glm::ivec4(11,  8,  9, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 3,  6, 11, 0), glm::ivec4( 0,  6,  3, 0), glm::ivec4( 0,  5,  6, 0), glm::ivec4( 0,  9,  5, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0, 11,  8, 0), glm::ivec4( 0,  5, 11, 0), glm::ivec4( 0,  1,  5, 0), glm::ivec4( 5,  6, 11, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 6, 11,  3, 0), glm::ivec4( 6,  3,  5, 0), glm::ivec4( 5,  3,  1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1,  2, 10, 0), glm::ivec4( 9,  5, 11, 0), glm::ivec4( 9, 11,  8, 0), glm::ivec4(11,  5,  6, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0, 11,  3, 0), glm::ivec4( 0,  6, 11, 0), glm::ivec4( 0,  9,  6, 0), glm::ivec4( 5,  6,  9, 0), glm::ivec4( 1,  2, 10, 0) },
        { glm::ivec4(11,  8,  5, 0), glm::ivec4(11,  5,  6, 0), glm::ivec4( 8,  0,  5, 0), glm::ivec4(10,  5,  2, 0), glm::ivec4( 0,  2,  5, 0) },
        { glm::ivec4( 6, 11,  3, 0), glm::ivec4( 6,  3,  5, 0), glm::ivec4( 2, 10,  3, 0), glm::ivec4(10,  5,  3, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 5,  8,  9, 0), glm::ivec4( 5,  2,  8, 0), glm::ivec4( 5,  6,  2, 0), glm::ivec4( 3,  8,  2, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  5,  6, 0), glm::ivec4( 9,  6,  0, 0), glm::ivec4( 0,  6,  2, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1,  5,  8, 0), glm::ivec4( 1,  8,  0, 0), glm::ivec4( 5,  6,  8, 0), glm::ivec4( 3,  8,  2, 0), glm::ivec4( 6,  2,  8, 0) },
        { glm::ivec4( 1,  5,  6, 0), glm::ivec4( 2,  1,  6, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1,  3,  6, 0), glm::ivec4( 1,  6, 10, 0), glm::ivec4( 3,  8,  6, 0), glm::ivec4( 5,  6,  9, 0), glm::ivec4( 8,  9,  6, 0) },
        { glm::ivec4(10,  1,  0, 0), glm::ivec4(10,  0,  6, 0), glm::ivec4( 9,  5,  0, 0), glm::ivec4( 5,  6,  0, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  3,  8, 0), glm::ivec4( 5,  6, 10, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(10,  5,  6, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(11,  5, 10, 0), glm::ivec4( 7,  5, 11, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(11,  5, 10, 0), glm::ivec4(11,  7,  5, 0), glm::ivec4( 8,  3,  0, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 5, 11,  7, 0), glm::ivec4( 5, 10, 11, 0), glm::ivec4( 1,  9,  0, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(10,  7,  5, 0), glm::ivec4(10, 11,  7, 0), glm::ivec4( 9,  8,  1, 0), glm::ivec4( 8,  3,  1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(11,  1,  2, 0), glm::ivec4(11,  7,  1, 0), glm::ivec4( 7,  5,  1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  8,  3, 0), glm::ivec4( 1,  2,  7, 0), glm::ivec4( 1,  7,  5, 0), glm::ivec4( 7,  2, 11, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  7,  5, 0), glm::ivec4( 9,  2,  7, 0), glm::ivec4( 9,  0,  2, 0), glm::ivec4( 2, 11,  7, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 7,  5,  2, 0), glm::ivec4( 7,  2, 11, 0), glm::ivec4( 5,  9,  2, 0), glm::ivec4( 3,  2,  8, 0), glm::ivec4( 9,  8,  2, 0) },
        { glm::ivec4( 2,  5, 10, 0), glm::ivec4( 2,  3,  5, 0), glm::ivec4( 3,  7,  5, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 8,  2,  0, 0), glm::ivec4( 8,  5,  2, 0), glm::ivec4( 8,  7,  5, 0), glm::ivec4(10,  2,  5, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  0,  1, 0), glm::ivec4( 5, 10,  3, 0), glm::ivec4( 5,  3,  7, 0), glm::ivec4( 3, 10,  2, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  8,  2, 0), glm::ivec4( 9,  2,  1, 0), glm::ivec4( 8,  7,  2, 0), glm::ivec4(10,  2,  5, 0), glm::ivec4( 7,  5,  2, 0) },
        { glm::ivec4( 1,  3,  5, 0), glm::ivec4( 3,  7,  5, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  8,  7, 0), glm::ivec4( 0,  7,  1, 0), glm::ivec4( 1,  7,  5, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  0,  3, 0), glm::ivec4( 9,  3,  5, 0), glm::ivec4( 5,  3,  7, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  8,  7, 0), glm::ivec4( 5,  9,  7, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 5,  8,  4, 0), glm::ivec4( 5, 10,  8, 0), glm::ivec4(10, 11,  8, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 5,  0,  4, 0), glm::ivec4( 5, 11,  0, 0), glm::ivec4( 5, 10, 11, 0), glm::ivec4(11,  3,  0, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  1,  9, 0), glm::ivec4( 8,  4, 10, 0), glm::ivec4( 8, 10, 11, 0), glm::ivec4(10,  4,  5, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(10, 11,  4, 0), glm::ivec4(10,  4,  5, 0), glm::ivec4(11,  3,  4, 0), glm::ivec4( 9,  4,  1, 0), glm::ivec4( 3,  1,  4, 0) },
        { glm::ivec4( 2,  5,  1, 0), glm::ivec4( 2,  8,  5, 0), glm::ivec4( 2, 11,  8, 0), glm::ivec4( 4,  5,  8, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  4, 11, 0), glm::ivec4( 0, 11,  3, 0), glm::ivec4( 4,  5, 11, 0), glm::ivec4( 2, 11,  1, 0), glm::ivec4( 5,  1, 11, 0) },
        { glm::ivec4( 0,  2,  5, 0), glm::ivec4( 0,  5,  9, 0), glm::ivec4( 2, 11,  5, 0), glm::ivec4( 4,  5,  8, 0), glm::ivec4(11,  8,  5, 0) },
        { glm::ivec4( 9,  4,  5, 0), glm::ivec4( 2, 11,  3, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 2,  5, 10, 0), glm::ivec4( 3,  5,  2, 0), glm::ivec4( 3,  4,  5, 0), glm::ivec4( 3,  8,  4, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 5, 10,  2, 0), glm::ivec4( 5,  2,  4, 0), glm::ivec4( 4,  2,  0, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 3, 10,  2, 0), glm::ivec4( 3,  5, 10, 0), glm::ivec4( 3,  8,  5, 0), glm::ivec4( 4,  5,  8, 0), glm::ivec4( 0,  1,  9, 0) },
        { glm::ivec4( 5, 10,  2, 0), glm::ivec4( 5,  2,  4, 0), glm::ivec4( 1,  9,  2, 0), glm::ivec4( 9,  4,  2, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 8,  4,  5, 0), glm::ivec4( 8,  5,  3, 0), glm::ivec4( 3,  5,  1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  4,  5, 0), glm::ivec4( 1,  0,  5, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 8,  4,  5, 0), glm::ivec4( 8,  5,  3, 0), glm::ivec4( 9,  0,  5, 0), glm::ivec4( 0,  3,  5, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  4,  5, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 4, 11,  7, 0), glm::ivec4( 4,  9, 11, 0), glm::ivec4( 9, 10, 11, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  8,  3, 0), glm::ivec4( 4,  9,  7, 0), glm::ivec4( 9, 11,  7, 0), glm::ivec4( 9, 10, 11, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1, 10, 11, 0), glm::ivec4( 1, 11,  4, 0), glm::ivec4( 1,  4,  0, 0), glm::ivec4( 7,  4, 11, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 3,  1,  4, 0), glm::ivec4( 3,  4,  8, 0), glm::ivec4( 1, 10,  4, 0), glm::ivec4( 7,  4, 11, 0), glm::ivec4(10, 11,  4, 0) },
        { glm::ivec4( 4, 11,  7, 0), glm::ivec4( 9, 11,  4, 0), glm::ivec4( 9,  2, 11, 0), glm::ivec4( 9,  1,  2, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9,  7,  4, 0), glm::ivec4( 9, 11,  7, 0), glm::ivec4( 9,  1, 11, 0), glm::ivec4( 2, 11,  1, 0), glm::ivec4( 0,  8,  3, 0) },
        { glm::ivec4(11,  7,  4, 0), glm::ivec4(11,  4,  2, 0), glm::ivec4( 2,  4,  0, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(11,  7,  4, 0), glm::ivec4(11,  4,  2, 0), glm::ivec4( 8,  3,  4, 0), glm::ivec4( 3,  2,  4, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 2,  9, 10, 0), glm::ivec4( 2,  7,  9, 0), glm::ivec4( 2,  3,  7, 0), glm::ivec4( 7,  4,  9, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9, 10,  7, 0), glm::ivec4( 9,  7,  4, 0), glm::ivec4(10,  2,  7, 0), glm::ivec4( 8,  7,  0, 0), glm::ivec4( 2,  0,  7, 0) },
        { glm::ivec4( 3,  7, 10, 0), glm::ivec4( 3, 10,  2, 0), glm::ivec4( 7,  4, 10, 0), glm::ivec4( 1, 10,  0, 0), glm::ivec4( 4,  0, 10, 0) },
        { glm::ivec4( 1, 10,  2, 0), glm::ivec4( 8,  7,  4, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 4,  9,  1, 0), glm::ivec4( 4,  1,  7, 0), glm::ivec4( 7,  1,  3, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 4,  9,  1, 0), glm::ivec4( 4,  1,  7, 0), glm::ivec4( 0,  8,  1, 0), glm::ivec4( 8,  7,  1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 4,  0,  3, 0), glm::ivec4( 7,  4,  3, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 4,  8,  7, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9, 10,  8, 0), glm::ivec4(10, 11,  8, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 3,  0,  9, 0), glm::ivec4( 3,  9, 11, 0), glm::ivec4(11,  9, 10, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  1, 10, 0), glm::ivec4( 0, 10,  8, 0), glm::ivec4( 8, 10, 11, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 3,  1, 10, 0), glm::ivec4(11,  3, 10, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1,  2, 11, 0), glm::ivec4( 1, 11,  9, 0), glm::ivec4( 9, 11,  8, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 3,  0,  9, 0), glm::ivec4( 3,  9, 11, 0), glm::ivec4( 1,  2,  9, 0), glm::ivec4( 2, 11,  9, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  2, 11, 0), glm::ivec4( 8,  0, 11, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 3,  2, 11, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 2,  3,  8, 0), glm::ivec4( 2,  8, 10, 0), glm::ivec4(10,  8,  9, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 9, 10,  2, 0), glm::ivec4( 0,  9,  2, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 2,  3,  8, 0), glm::ivec4( 2,  8, 10, 0), glm::ivec4( 0,  1,  8, 0), glm::ivec4( 1, 10,  8, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1, 10,  2, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 1,  3,  8, 0), glm::ivec4( 9,  1,  8, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  9,  1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4( 0,  3,  8, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
        { glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0), glm::ivec4(-1, -1, -1, 0) },
};