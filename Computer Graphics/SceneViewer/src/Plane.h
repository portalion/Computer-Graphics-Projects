#pragma once
#include "DrawableEntity.h"

class Plane : public DrawableEntity
{
private:
	static const float m_Vertices[];
	static const unsigned int m_Indices[];
protected:
	void InitializeModelAsset() override;
public:
	Plane(glm::vec3 startingPos = { 0.f, 0.f, 0.f }, float scale = 1.f);
};

