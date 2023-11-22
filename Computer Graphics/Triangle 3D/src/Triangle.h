#pragma once
#include <glm/glm.hpp>
#include <vector>

class Shader;
class Triangle
{
private:
	unsigned int m_VAO;
	unsigned int m_VBO;

	glm::vec3 m_Points[3];
	glm::vec3 m_Normals[3];

	std::vector<glm::vec3> m_FilledLines;
public:
	Triangle(glm::vec3 vertices[3]);
	~Triangle();

	void GenerateFillVertices();
	void Draw(Shader* shader);
};