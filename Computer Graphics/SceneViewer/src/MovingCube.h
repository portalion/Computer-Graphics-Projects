#pragma once
#include "Cube.h"

class MovingCube : public Cube
{
private:
	const float velocity = 40.f;
	glm::vec3 position;
	float angle;
	const float radius = 4.f;
public:
	MovingCube(glm::vec3 startingPos = { 0.f, 0.f, 0.f }, float scale = 1.f);
	void Update(const float& deltaTime)override;

	inline virtual glm::vec3 GetPosition() const override { return position; }
	inline virtual glm::vec3 GetLookingDirection() const override 
	{
		return {
			radius * glm::cos(glm::radians(angle + 5.f)),
			position.y,
			radius * glm::sin(glm::radians(angle + 5.f)) };
	}
};

