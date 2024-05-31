#include "camera.h"
#include <GL/glew.h>
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
        , m_mouseCoords(0, 0)
        , m_keyStatus()
        , m_AspectRatio(16.f/9.f)
        , m_FrustumPlanes()
        , m_LastFov(fov)
        , m_ZNear(5.f)
        , m_ZFar(2000.f)
        , m_ProjMatrix(glm::perspective(glm::radians(m_Fov), m_AspectRatio, 5.0f, 2000.0f))
{
    UpdateFrustumPlanes();
}

Camera::Plane::Plane(const glm::vec3& p1, const glm::vec3& norm)
        : normal(glm::normalize(norm)),
          distance(glm::dot(normal, p1))
{}

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

void Camera::Update(float deltaTime, float aspectRatio)
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

    bool matrixUpdate = false;

    if (hasMoved)
    {
        UpdateViewMatrix();
        matrixUpdate = true;
    }

    if (m_Fov != m_LastFov || m_AspectRatio != aspectRatio)
    {
        m_AspectRatio = aspectRatio;
        UpdateProjectionMatrix();
        matrixUpdate = true;
    }
    m_LastFov = m_Fov;

    if (matrixUpdate)
        UpdateFrustumPlanes();

}

void Camera::HandleMouseInput(float xPos, float yPos, float deltaTime)
{
    float delta_x = xPos - m_mouseCoords.x;
    float delta_y = yPos - m_mouseCoords.y;

    // Update previous mouse coordinates
    m_mouseCoords.x = xPos;
    m_mouseCoords.y = yPos;

    float pitchAngle = glm::degrees(glm::asin(m_Dir.y)); // Calculate current pitch angle
    float maxPitch = 89.5f;
    float newPitch = glm::clamp(pitchAngle - m_RotationSpeed * delta_y, -maxPitch, maxPitch);
    float deltaPitch = newPitch - pitchAngle;

    // Calculate yaw and pitch rotations
    glm::mat4 yaw   = glm::rotate(glm::mat4(1.0f), glm::radians(m_RotationSpeed * -delta_x), m_WorldUp);
    glm::mat4 pitch = glm::rotate(glm::mat4(1.0f), glm::radians(deltaPitch), glm::normalize(glm::cross(m_Dir, m_WorldUp)));

    // Apply rotations to camera direction
    m_Dir = glm::vec3(pitch * yaw * glm::vec4(m_Dir, 0.0f));
    m_Dir = glm::normalize(m_Dir);
    UpdateViewMatrix();
    UpdateFrustumPlanes();
}

void Camera::UpdateViewMatrix()
{
    m_viewMatrix = glm::lookAt(m_Pos, m_Pos + m_Dir, m_WorldUp);
}

void Camera::UpdateProjectionMatrix()
{
    m_ProjMatrix = glm::perspective(glm::radians(m_Fov), m_AspectRatio, 5.0f, 2000.0f);
}

void Camera::UpdateFrustumPlanes()
{
    const float halfVSide = m_ZFar * tanf(m_Fov * .5f);
    const float halfHSide = halfVSide * m_AspectRatio;
    glm::vec3 camRight = glm::normalize(glm::cross(m_WorldUp, -m_Dir)); // Note the negative sign for the cross product
    glm::vec3 camUp = glm::normalize(glm::cross(-m_Dir, camRight));
    const glm::vec3 frontMultFar = m_ZFar * m_Dir;

    m_FrustumPlanes[0] = { m_Pos + m_ZNear * m_Dir, m_Dir };
    m_FrustumPlanes[1] = { m_Pos + frontMultFar, -m_Dir };
    m_FrustumPlanes[2] = { m_Pos, glm::cross(frontMultFar - camRight * halfHSide, camUp) };
    m_FrustumPlanes[3] = { m_Pos, glm::cross(camUp,frontMultFar + camRight * halfHSide) };
    m_FrustumPlanes[4] = { m_Pos, glm::cross(camRight, frontMultFar - camUp * halfVSide) };
    m_FrustumPlanes[5] = { m_Pos, glm::cross(frontMultFar + camUp * halfVSide, camRight) };
}

bool Camera::CubeFrustumCheck(glm::vec3 minPos, glm::vec3 maxPos) const
{
    glm::vec3 center = (maxPos + minPos) * .5f;
    glm::vec3 extends = maxPos - center;


    for (auto plane : m_FrustumPlanes)
    {
        const float r = extends.x * std::abs(plane.normal.x) +
                        extends.y * std::abs(plane.normal.y) +
                        extends.z * std::abs(plane.normal.z);

        const float dist = glm::dot(plane.normal, center) - plane.distance;

        if (-r > dist)
            return false;
    }

    return true;
}

void Camera::DrawFrustum(Shader &shader, const Camera &camera)
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

    glDeleteVertexArrays(1, &VBO);
    glDeleteBuffers(1, &VBO);

}
