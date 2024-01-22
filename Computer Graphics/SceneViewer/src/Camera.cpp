#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

#pragma region Camera

Camera::Camera(glm::vec3 position, glm::vec3 whereToLook)
    :m_Position{ position }, m_LookingPosition{ whereToLook }, m_ViewMatrix{ 1.f }
{
    m_ViewMatrix = ComputeViewMatrix();
}

glm::mat4 Camera::GetViewMatrix()
{
    return m_ViewMatrix;
}

#pragma endregion

#pragma region Observer Camera
ObserverCamera::ObserverCamera(DrawableEntity* entityToBindInto, glm::vec3 position)
    :Camera(position, { 0.f, 0.f, 0.f })
{
    m_BindedEntity = entityToBindInto;
}

glm::mat4 ObserverCamera::GetViewMatrix()
{
    m_LookingPosition = m_BindedEntity->GetPosition();
    return ComputeViewMatrix();
}
#pragma endregion


#pragma region MovingCamera

MovingCamera::MovingCamera(DrawableEntity* EntityToBindInto)
    :Camera({ 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }), m_BindedEntity{ EntityToBindInto }
{
}

glm::mat4 MovingCamera::GetViewMatrix()
{
    m_Position = m_BindedEntity->GetPosition();
    m_LookingPosition = m_BindedEntity->GetLookingDirection();
    auto lookingDir = m_Position - m_LookingPosition;
    m_Position -= lookingDir;
    m_LookingPosition -= lookingDir;
    return ComputeViewMatrix();
}

#pragma endregion


