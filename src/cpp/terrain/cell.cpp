//
// Created by marti on 08/05/2024.
//

#include <iostream>
#include "cell.h"
#include "GL/glew.h"

void Cell::DebugGenerateVAO()
{
    GLfloat vertices[] = {
            m_Pos.x,   m_Pos.y,   m_Pos.z,   // Vertex 0
            m_Pos.x + m_Size, m_Pos.y,   m_Pos.z,   // Vertex 1
            m_Pos.x + m_Size, m_Pos.y + m_Size, m_Pos.z,   // Vertex 2
            m_Pos.x,   m_Pos.y + m_Size, m_Pos.z,   // Vertex 3
            m_Pos.x,   m_Pos.y,   m_Pos.z + m_Size, // Vertex 4
            m_Pos.x + m_Size, m_Pos.y,   m_Pos.z + m_Size, // Vertex 5
            m_Pos.x + m_Size, m_Pos.y + m_Size, m_Pos.z + m_Size, // Vertex 6
            m_Pos.x,   m_Pos.y + m_Size, m_Pos.z + m_Size  // Vertex 7
    };

    // Define indices for the quad
    GLuint indices[] = {
            // Front face
            2, 1, 0,
            0, 3, 2,
//            // Back face
            4, 5, 6,
            6, 7, 4,
//            // Left face
            3, 0, 4,
            4, 7, 3,
//            // Right face
            6, 5, 1,
            1, 2, 6,
//            // Top face
            6, 2, 3,
            3, 7, 6,
//            // Bottom face
            1, 5, 4,
            4, 0, 1
    };
    glGenVertexArrays(1, &m_DebugVAO);
    GLuint positionBuffer, indexBuffer;

    glGenBuffers(1, &positionBuffer);
    glGenBuffers(1, &indexBuffer);
    glBindVertexArray(m_DebugVAO);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, 3 * sizeof(GLfloat), 0, nullptr);
    glEnableVertexAttribArray(0);
};


void Cell::DebugDraw()
{
    if (m_DebugVAO == 0)
        DebugGenerateVAO();

    glBindVertexArray(m_DebugVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
}

Cell::Cell(const glm::vec3 &position, float size)
    : m_Pos(position)
    , m_Size(size)
    , m_DebugVAO(0)
{ }

Cell::~Cell()
{
    if (m_DebugVAO != 0)
        glDeleteVertexArrays(1, &m_DebugVAO);
}
