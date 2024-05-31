//
// Created by marti on 30/05/2024.
//

#ifndef MAIN_HASHING_H
#define MAIN_HASHING_H

#include <glm/glm.hpp>

struct CustomIvec4Hash {
    std::size_t operator()(const glm::ivec4& v) const {
        std::size_t h1 = std::hash<int>{}(v.x);
        std::size_t h2 = std::hash<int>{}(v.y);
        std::size_t h3 = std::hash<int>{}(v.z);
        std::size_t h4 = std::hash<int>{}(v.w);

        // Combine the hash values of each component using a prime number
        std::size_t hash = h1;
        hash = hash * 31 + h2;
        hash = hash * 31 + h3;
        hash = hash * 31 + h4;
        return hash;
    }
};

// Custom equality function for glm::ivec4
struct Ivec4Equal {
    bool operator()(const glm::ivec4& lhs, const glm::ivec4& rhs) const {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
    }
};

#endif //MAIN_HASHING_H
