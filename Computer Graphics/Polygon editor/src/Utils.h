#pragma once

#include <glm/glm.hpp>

struct Vertex
{
	float x;
	float y;
};

void HelpMarker(const char* desc);

enum class MouseButton
{
	LEFT = 0,
	RIGHT = 1,
	MIDDLE = 2
};

enum class ActivityState
{
	ACTIVE = 0,
	NOT_ACTIVE = 1,
	WILL_BE_ACTIVE = 2
};

glm::vec3 GetColor(ActivityState activity);