#include "MovingCube.h"
#include "glm/gtc/matrix_transform.hpp"

MovingCube::MovingCube(glm::vec3 startingPos, float scale)
	:Cube(startingPos, scale),
	position{startingPos}
{
	SetColor({ 1.f, 0.f, 0.f });
}

void MovingCube::Update(const float& deltaTime)
{
	angle += deltaTime * velocity;
	if (angle > 360.f)angle = 0.f;
	else if (angle < 0.f) angle = 360.f;

	glm::vec3 nextPos = 
	{ 
		radius * glm::cos(glm::radians(angle)),
		position.y,
		radius * glm::sin(glm::radians(angle))
	};

	SetPosition(nextPos);
	SetRotation(glm::rotate(glm::mat4(1.f), glm::radians(-angle), { 0.f, 1.f, 0.f }));

	position = nextPos;
}
