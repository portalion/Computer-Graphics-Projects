#pragma once
#include <glm/glm.hpp>
#include "Shader.h"

class LightSource
{
private:
	unsigned int VBO;
	unsigned int VAO;
	Shader shader;
	glm::mat4 modelMatrix;
public:
	LightSource(glm::vec3 position = glm::vec3(0));
	~LightSource();
	glm::vec3 position;

	void Draw();
};

