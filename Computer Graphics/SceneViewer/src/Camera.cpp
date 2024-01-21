#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 whereToLook)
    :m_Position{position}, m_LookingPosition{whereToLook}, m_ViewMatrix{1.f}
{
    m_ViewMatrix = ComputeViewMatrix();
}

glm::mat4 Camera::GetViewMatrix()
{
    return m_ViewMatrix;
}

ObserverCamera::ObserverCamera(DrawableEntity* entityToBindInto, glm::vec3 position)
    :Camera(position, {0.f, 0.f, 0.f})
{
    m_BindedEntity = entityToBindInto;
}

glm::mat4 ObserverCamera::GetViewMatrix()
{
    m_LookingPosition = m_BindedEntity->GetPosition();
    return ComputeViewMatrix();
}
