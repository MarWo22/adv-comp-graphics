//
// Created by marti on 08/05/2024.
//

#ifndef MAIN_FILE_H
#define MAIN_FILE_H


#include <string>
#include "glm/glm.hpp"

namespace file
{
    int *ReadNumPolys(const std::string &path);
    glm::ivec3 **ReadTrisTable(const std::string &path);
};


#endif //MAIN_FILE_H
