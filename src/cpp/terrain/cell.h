//
// Created by marti on 08/05/2024.
//

#ifndef MAIN_CELL_H
#define MAIN_CELL_H


#include "glm/vec3.hpp"
#include "../shader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

class Cell
{
    glm::vec3 m_Pos;
    float m_Size;
    GLuint m_DebugVAO;

public:
    Cell(const glm::vec3 &position, float size);
    Cell() = default;
    void DebugDraw();
    [[nodiscard]] inline glm::vec3 Pos() const {return m_Pos; };
    ~Cell();

private:
    void DebugGenerateVAO();
};


#endif //MAIN_CELL_H
