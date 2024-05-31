#ifndef MAIN_CAMERA_H
#define MAIN_CAMERA_H

#include "glm/glm.hpp"
#include "shader.h"

class Camera
{
private:
    struct KeyStatus
    {
        enum Status{KEY_UP, KEY_DOWN};

        Status w;
        Status a;
        Status s;
        Status d;
        Status q;
        Status e;
        Status shift;
    };

    struct Plane
    {
        glm::vec3 normal;
        float distance;

        Plane() = default;
        Plane(const glm::vec3& p1, const glm::vec3& norm);
    };

    float m_Fov;
    float m_LastFov;
    glm::vec3 m_Pos;
    glm::vec3 m_Dir;
    float m_BaseSpeed;
    float m_ShiftMultiplier;
    float m_RotationSpeed;
    glm::mat4 m_viewMatrix;

    glm::vec2 m_mouseCoords;
    KeyStatus m_keyStatus;

    Plane m_FrustumPlanes[6];
    float m_AspectRatio;
    glm::mat4 m_ProjMatrix;

    float m_ZNear;
    float m_ZFar;

    constexpr const static glm::vec3 m_WorldUp = {0.0f, 1.0f, 0.0f};

public:
    Camera() = default;
    Camera(glm::vec3 position, float fov, float baseSpeed, float shiftMultiplier, float rotationSpeed);
    inline float Fov() const { return m_Fov; }
    inline const glm::vec3 &Pos() const { return m_Pos; }
    inline const glm::vec3 &Dir() const { return m_Dir; }
    inline float BaseSpeed() const { return m_BaseSpeed; }
    inline float ShiftMultiplier() const {return m_ShiftMultiplier; }
    inline const glm::mat4 &ViewMatrix() const {return m_viewMatrix; }
    inline const glm::mat4 &ProjectionMatrix() const { return m_ProjMatrix; }
    inline float AspectRatio() const { return m_AspectRatio; }
    inline float ZNear() const { return m_ZNear; }
    inline float ZFar() const { return m_ZFar; }


    inline void Fov(float fov) { m_Fov = fov; }
    inline void BaseSpeed(float baseSpeed) { m_BaseSpeed = baseSpeed; }
    inline void ShiftMultiplier(float shiftMultiplier) { m_ShiftMultiplier = shiftMultiplier; }

    void HandleKeyInput(int key, int action);
    void Update(float deltaTime, float aspectRatio);
    void HandleMouseInput(float xPos, float yPos, float deltaTime);
    bool CubeFrustumCheck(glm::vec3 minPos, glm::vec3 maxPos) const;

    void DrawFrustum(Shader &shader, const Camera &camera);

private:
    void UpdateViewMatrix();

    void UpdateProjectionMatrix();

    void UpdateFrustumPlanes();

};


#endif //MAIN_CAMERA_H
