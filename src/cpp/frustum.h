//
// Created by marti on 20/05/2024.
//

#ifndef MAIN_FRUSTUM_H
#define MAIN_FRUSTUM_H


#include "glm/vec3.hpp"
#include "shader.h"
#include "camera.h"

class Frustum
{
private:
    struct Plane
    {
        glm::vec3 normal;
        float distance;

        Plane() = default;
        Plane(const glm::vec3& p1, const glm::vec3& norm);
    };
public:
    enum Result
    {
        Outside,
        Inside,
        Intersecting
    };
private:

    float m_Fov;
    glm::vec3 m_Pos;
    glm::vec3 m_Dir;
    float m_AspectRatio;
    float m_ZNear;
    float m_ZFar;


    Plane m_FrustumPlanes[6];
    constexpr const static glm::vec3 m_WorldUp = {0.0f, 1.0f, 0.0f};

public:
    Frustum() = default;

    Frustum(const glm::vec3 &pos, const glm::vec3 &dir, float fov, float aspectRatio, float zNear,
                        float zFar);


    void UpdateFrustumPlanes(const glm::vec3 &pos, const glm::vec3 &dir, float fov, float aspectRatio, float zNear, float zFar);

    Result CubeCheck(glm::vec3 position, float size) const;

    void Draw(Shader &shader, const Camera &camera);
};


#endif //MAIN_FRUSTUM_H
