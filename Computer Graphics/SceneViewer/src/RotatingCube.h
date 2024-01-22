#pragma once
#include "Cube.h"

class RotatingCube : public Cube
{
private:
	const float velocity = 100.f;
	float angle;
public:
	RotatingCube(glm::vec3 startingPos = { 0.f, 0.f, 0.f }, float scale = 1.f);
	void Update(const float& deltaTime)override;
};

