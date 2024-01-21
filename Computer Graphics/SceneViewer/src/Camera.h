#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "DrawableEntity.h"

class Camera
{
protected:
	glm::vec3 m_Position;
	glm::vec3 m_LookingPosition;
	glm::mat4 m_ViewMatrix;
	inline glm::mat4 ComputeViewMatrix() { return glm::lookAt(m_Position, m_LookingPosition, { 0.f, 1.f, 0.f }); }
public:
	Camera(glm::vec3 position, glm::vec3 whereToLook);
	virtual glm::mat4 GetViewMatrix();
	inline glm::vec3 GetPosition() { return m_Position; }
};


class ObserverCamera : public Camera
{
protected:
	DrawableEntity* m_BindedEntity;
public:
	ObserverCamera(DrawableEntity* EntityToBindInto, glm::vec3 position);
	glm::mat4 GetViewMatrix() override;
	inline void BindEntity(DrawableEntity* toBind) { m_BindedEntity = toBind; }
};