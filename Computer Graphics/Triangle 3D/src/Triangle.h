#pragma once
#include <glm/glm.hpp>
#include <vector>

class Triangle
{
private:
	static unsigned int m_VAO;
	
	glm::vec3 m_Points[3];

	std::vector<glm::vec3> m_FilledLines;
public:
	Triangle(glm::vec3 vertices[3]);

	void GenerateFillVertices();
	void GenerateTriangleBorder();
	void Draw();
};