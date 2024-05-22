#include "camera.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <iostream>

Camera::Camera(glm::vec3 position, float fov, float baseSpeed, float shiftMultiplier, float rotationSpeed)
        : m_Fov(fov)
        , m_Pos(position)
        , m_Dir(normalize(glm::vec3(0.0f) - position))
        , m_BaseSpeed(baseSpeed)
        , m_ShiftMultiplier(shiftMultiplier)
        , m_RotationSpeed(rotationSpeed)
        , m_viewMatrix(glm::lookAt(m_Pos, m_Pos + m_Dir, m_WorldUp))
        , m_mouseCoords(-1, -1)
        , m_hasMouseInput(false)
        , m_keyStatus()
{ }

void Camera::HandleKeyInput(int key, int action)
{
    KeyStatus::Status status;
    if (action == GLFW_PRESS)
        status = KeyStatus::KEY_DOWN;
    else if (action == GLFW_RELEASE)
        status = KeyStatus::KEY_UP;
    else
        return;


    switch(key)
    {
        case GLFW_KEY_LEFT_SHIFT:
            m_keyStatus.shift = status;
            break;
        case GLFW_KEY_W:
            m_keyStatus.w = status;
            break;
        case GLFW_KEY_S:
            m_keyStatus.s = status;
            break;
        case GLFW_KEY_D:
            m_keyStatus.d = status;
            break;
        case GLFW_KEY_A:
            m_keyStatus.a = status;
            break;
        case GLFW_KEY_Q:
            m_keyStatus.q = status;
            break;
        case GLFW_KEY_E:
            m_keyStatus.e = status;
            break;
        default:
            return;
    }
    UpdateViewMatrix();
}

void Camera::Update(float deltaTime)
{
    float speed = m_keyStatus.shift ? m_BaseSpeed * m_ShiftMultiplier : m_BaseSpeed;
    bool hasMoved = false;
    if (m_keyStatus.w == KeyStatus::KEY_DOWN)
    {
        hasMoved = true;
        m_Pos += speed * m_Dir * deltaTime;
    }
    if (m_keyStatus.a == KeyStatus::KEY_DOWN)
    {
        hasMoved = true;
        m_Pos -= speed * glm::cross(m_Dir, m_WorldUp) * deltaTime;
    }
    if (m_keyStatus.s == KeyStatus::KEY_DOWN)
    {
        hasMoved = true;
        m_Pos -= speed * m_Dir * deltaTime;
    }
    if (m_keyStatus.d == KeyStatus::KEY_DOWN)
    {
        hasMoved = true;
        m_Pos += speed * glm::cross(m_Dir, m_WorldUp) * deltaTime;
    }
    if (m_keyStatus.q == KeyStatus::KEY_DOWN)
    {
        hasMoved = true;
        m_Pos -= speed * m_WorldUp * deltaTime;
    }
    if (m_keyStatus.e == KeyStatus::KEY_DOWN)
    {
        hasMoved = true;
        m_Pos += speed * m_WorldUp * deltaTime;
    }

    if (hasMoved)
        UpdateViewMatrix();
}

void Camera::HandleMouseInput(float xPos, float yPos, float deltaTime)
{
    float delta_x = xPos - m_mouseCoords.x;
    float delta_y = yPos - m_mouseCoords.y;

    // Update previous mouse coordinates
    m_mouseCoords.x = xPos;
    m_mouseCoords.y = yPos;

    // Calculate yaw and pitch rotations
    glm::mat4 yaw   = glm::rotate(glm::mat4(1.0f), glm::radians(m_RotationSpeed * -delta_x), m_WorldUp);
    glm::mat4 pitch = glm::rotate(glm::mat4(1.0f), glm::radians(m_RotationSpeed * -delta_y), glm::normalize(glm::cross(m_Dir, m_WorldUp)));

    // Apply rotations to camera direction
    m_Dir = glm::vec3(pitch * yaw * glm::vec4(m_Dir, 0.0f));

    UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
    m_viewMatrix = glm::lookAt(m_Pos, m_Pos + m_Dir, m_WorldUp);
}
