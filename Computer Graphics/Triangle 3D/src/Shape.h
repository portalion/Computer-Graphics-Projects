#pragma once
#include "Utils.h"
#include <vector>
#include <glm/glm.hpp>

#include "Shader.h"

class Shape
{
private:
	const int m_Height = Globals::Height * .8f;
	const int m_Width = m_Height;
	int s;
	int t;

	std::vector<glm::vec3> m_Vertices;
	std::vector<unsigned int> m_Indices;

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;

	void GenerateVertices();
public:
	Shape();
	~Shape();

	void DisplayMenu();
	void Draw();
};

