#ifndef MAIN_CAMERA_H
#define MAIN_CAMERA_H

#include "glm/glm.hpp"

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

    float m_Fov;
    glm::vec3 m_Pos;
    glm::vec3 m_Dir;
    float m_BaseSpeed;
    float m_ShiftMultiplier;
    float m_RotationSpeed;
    glm::mat4 m_viewMatrix;

    glm::vec2 m_mouseCoords;
    bool m_hasMouseInput;
    KeyStatus m_keyStatus;

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

    inline void Fov(float fov) { m_Fov = fov; }
    inline void BaseSpeed(float baseSpeed) { m_BaseSpeed = baseSpeed; }
    inline void ShiftMultiplier(float shiftMultiplier) { m_ShiftMultiplier = shiftMultiplier; }

    void HandleKeyInput(int key, int action);
    void Update(float deltaTime);
    void HandleMouseInput(float xPos, float yPos, float deltaTime);

private:
    void UpdateViewMatrix();
};


#endif //MAIN_CAMERA_H
