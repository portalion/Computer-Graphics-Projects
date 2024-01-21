#include "MovingCube.h"

MovingCube::MovingCube(glm::vec3 startingPos, float scale)
	:Cube(startingPos, scale),
	velocity{1.f, 1.f, 1.f}
{
}

void MovingCube::Update(const float& deltaTime)
{
	Move({ -velocity.x * deltaTime, 0.f, 0.f });
}
