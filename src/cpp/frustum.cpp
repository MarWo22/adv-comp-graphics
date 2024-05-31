//
// Created by marti on 20/05/2024.
//

#include <cmath>
#include "frustum.h"
#include "glm/glm.hpp"
#include "shader.h"
#include "camera.h"


Frustum::Frustum(const glm::vec3 &pos, const glm::vec3 &dir, float fov, float aspectRatio, float zNear, float zFar)
        : m_FrustumPlanes()
        , m_Pos()
        , m_Dir()
        , m_Fov()
        , m_AspectRatio()
        , m_ZNear()
        , m_ZFar()
{
    UpdateFrustumPlanes(pos, dir, fov, aspectRatio, zNear, zFar);
}


Frustum::Plane::Plane(const glm::vec3& p1, const glm::vec3& norm)
        : normal(glm::normalize(norm)),
          distance(glm::dot(normal, p1))
{}

void Frustum::UpdateFrustumPlanes(const glm::vec3 &pos, const glm::vec3 &dir, float fov, float aspectRatio, float zNear, float zFar)
{
    m_Pos = pos;
    m_Dir = dir;
    m_Fov = fov;
    m_AspectRatio = aspectRatio;
    m_ZNear = zNear;
    m_ZFar = zFar;

    const float halfVSide = zFar * tanf(fov * .5f);
    const float halfHSide = halfVSide * aspectRatio;
    glm::vec3 camRight = glm::normalize(glm::cross(m_WorldUp, -dir)); // Note the negative sign for the cross product
    glm::vec3 camUp = glm::normalize(glm::cross(-dir, camRight));
    const glm::vec3 frontMultFar = zFar * dir;

    m_FrustumPlanes[0] = { pos + zNear * dir, dir };
    m_FrustumPlanes[1] = { pos + frontMultFar, -dir };
    m_FrustumPlanes[2] = { pos, glm::cross(frontMultFar - camRight * halfHSide, camUp) };
    m_FrustumPlanes[3] = { pos, glm::cross(camUp,frontMultFar + camRight * halfHSide) };
    m_FrustumPlanes[4] = { pos, glm::cross(camRight, frontMultFar - camUp * halfVSide) };
    m_FrustumPlanes[5] = { pos, glm::cross(frontMultFar + camUp * halfVSide, camRight) };
}

Frustum::Result Frustum::CubeCheck(glm::vec3 position, float size) const
{
    glm::vec3 center = position + glm::vec3(size / 2);
    glm::vec3 extends = glm::vec3(size / 2);

    bool isFullyInside = true;

    for (const auto& plane : m_FrustumPlanes)
    {
        const float r = extends.x * std::abs(plane.normal.x) +
                        extends.y * std::abs(plane.normal.y) +
                        extends.z * std::abs(plane.normal.z);

        const float dist = glm::dot(plane.normal, center) - plane.distance;

        if (-r > dist)
        {
            return Result::Outside;  // Completely outside the frustum
        }

        if (dist < r)
        {
            isFullyInside = false;  // Intersecting the frustum
        }
    }

    return isFullyInside ? Result::Inside : Result::Intersecting;
}


void Frustum::Draw(Shader &shader, const Camera &camera)
{
    glm::vec3 camRight = glm::normalize(glm::cross(m_WorldUp, -m_Dir)); // Note the negative sign for the cross product
    glm::vec3 camUp = glm::normalize(glm::cross(-m_Dir, camRight));

    // Compute frustum corners in world space
    float tanHalfFOV = tan(glm::radians(m_Fov / 2.0f));
    float nh = m_ZNear * tanHalfFOV;
    float nw = nh * m_AspectRatio;
    float fh = m_ZFar * tanHalfFOV;
    float fw = fh * m_AspectRatio;

    std::vector<glm::vec3> frustumCorners = {
            // Near plane
            m_Pos + m_Dir * m_ZNear + camUp * nh - glm::normalize(glm::cross(camUp, m_Dir)) * nw,
            m_Pos + m_Dir * m_ZNear + camUp * nh + glm::normalize(glm::cross(camUp, m_Dir)) * nw,
            m_Pos + m_Dir * m_ZNear - camUp * nh - glm::normalize(glm::cross(camUp, m_Dir)) * nw,
            m_Pos + m_Dir * m_ZNear - camUp * nh + glm::normalize(glm::cross(camUp, m_Dir)) * nw,
            // Far plane
            m_Pos + m_Dir * m_ZFar + camUp * fh - glm::normalize(glm::cross(camUp, m_Dir)) * fw,
            m_Pos + m_Dir * m_ZFar + camUp * fh + glm::normalize(glm::cross(camUp, m_Dir)) * fw,
            m_Pos + m_Dir * m_ZFar - camUp * fh - glm::normalize(glm::cross(camUp, m_Dir)) * fw,
            m_Pos + m_Dir * m_ZFar - camUp * fh + glm::normalize(glm::cross(camUp, m_Dir)) * fw,
    };

    // Setup VBO and VAO for frustum
    GLuint VAO, VBO;
    std::vector<glm::vec3> lines = {
            // Near plane
            frustumCorners[0], frustumCorners[1], frustumCorners[1], frustumCorners[3],
            frustumCorners[3], frustumCorners[2], frustumCorners[2], frustumCorners[0],
            // Far plane
            frustumCorners[4], frustumCorners[5], frustumCorners[5], frustumCorners[7],
            frustumCorners[7], frustumCorners[6], frustumCorners[6], frustumCorners[4],
            // Connecting lines
            frustumCorners[0], frustumCorners[4], frustumCorners[1], frustumCorners[5],
            frustumCorners[2], frustumCorners[6], frustumCorners[3], frustumCorners[7]
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, lines.size() * sizeof(glm::vec3), lines.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    shader.Bind();
    shader.SetUniform("modelViewProjectionMatrix", camera.ProjectionMatrix() * camera.ViewMatrix());

    glBindVertexArray(VAO);

    glDrawArrays(GL_LINES, 0, 24);
    glBindVertexArray(0);


    glDeleteVertexArrays(1, &VAO);
}

