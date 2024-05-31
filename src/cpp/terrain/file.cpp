//
// Created by marti on 08/05/2024.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include "file.h"

int *file::ReadNumPolys(const std::string &path)
{
    int *numPolys = new int[256];
    int idx = 0;
    std::ifstream file(path);
    std::string s1, s2;
    while (std::getline(file, s1, ','))
    {
        std::istringstream iss(s1);
        while (std::getline(iss, s2, ' '))
            if (!s2.empty())
            {
                numPolys[idx] = std::stoi(s2);

                idx++;
                break;
            }
    }
    return numPolys;
}

glm::ivec3 **file::ReadTrisTable(const std::string &path)
{
    auto **numPolys = new glm::ivec3*[256];

    for (int i = 0; i < 256; ++i) {
        numPolys[i] = new glm::ivec3[5]; // Allocate memory for each row
    }

    int idx = 0;
    std::ifstream file(path);
    std::string s1, s2;
    while (std::getline(file, s1, ','))
    {
        std::istringstream iss(s1);
        int vertexIdx = 0;
        while (std::getline(iss, s2, ' '))
            if (!s2.empty())
            {
                numPolys[idx / 5][idx % 5][vertexIdx] = std::stoi(s2);

                vertexIdx++;

                if (vertexIdx == 3)
                    break;
            }
        idx++;
    }
    return numPolys;
}
