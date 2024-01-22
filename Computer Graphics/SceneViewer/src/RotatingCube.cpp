#include "RotatingCube.h"
#include "glm/gtc/matrix_transform.hpp"

RotatingCube::RotatingCube(glm::vec3 startingPos, float scale)
	:Cube(startingPos, scale)
{
	SetColor({ 0.4f, 0.f, 0.4f });
	angle = 0.f;
}

void RotatingCube::Update(const float& deltaTime)
{
	angle += deltaTime * velocity;
	if (angle > 360.f)angle = 0.f;
	else if (angle < 0.f) angle = 360.f;

	SetRotation(glm::rotate(glm::mat4(1.f), glm::radians(-angle), { 1.f, 1.f, 0.f }));
}
