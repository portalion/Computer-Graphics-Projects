#pragma once
#include "Cube.h"

class MovingCube : public Cube
{
private:
	const glm::vec3 velocity;
public:
	MovingCube(glm::vec3 startingPos = { 0.f, 0.f, 0.f }, float scale = 1.f);
	void Update(const float& deltaTime)override;
};

