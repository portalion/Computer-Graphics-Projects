#pragma once
#include <glm/glm.hpp>
#include <vector>

class Point
{
private:
	static unsigned int m_Vao;
	static unsigned int m_Vbo;
	static unsigned int m_Ibo;

	static const int size = 5;

	static std::vector<glm::vec3> m_Vertices;
	static std::vector<unsigned int> m_Indices;

	static void GenerateVertices();

	float x;
	float y;

	void UpdateMatrices();
public:
	Point(float x, float y);
	glm::mat4 model;

	void SetPosition(float x, float y);
	void Draw();
	void DisplayMenu();
};

