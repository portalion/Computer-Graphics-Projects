#include "Point.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

std::vector<glm::vec3> Point::m_Vertices = std::vector<glm::vec3>();
std::vector<unsigned int> Point::m_Indices = std::vector<unsigned int>();

unsigned int Point::m_Vao = 0;
unsigned int Point::m_Vbo = 0;
unsigned int Point::m_Ibo = 0;

void Point::GenerateVertices()
{
	static const int numberOfVertices = 36;
	float angle = 360.f / numberOfVertices;
	int triangleCount = numberOfVertices - 2;

	for (int i = 0; i < numberOfVertices; i++)
	{
		float currentAngle = angle * i;
		m_Vertices.push_back({ size * cos(glm::radians(currentAngle)) + 10.f, size * sin(glm::radians(currentAngle)) + 10.f, 0.f });
	}


	for (int i = 0; i < triangleCount; i++)
	{
		m_Indices.push_back(0);
		m_Indices.push_back(i + 1);
		m_Indices.push_back(i + 2);
	}

	glGenBuffers(1, &m_Vbo);
	glGenBuffers(1, &m_Ibo);
	glGenVertexArrays(1, &m_Vao);

	glBindVertexArray(m_Vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_Vertices.size(), &m_Vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_Indices.size(), &m_Indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Point::Point(float x, float y)
	:x{x}, y{y}
{
	if(m_Vao == 0 || m_Vbo == 0 || m_Ibo == 0)
		GenerateVertices();
}

void Point::Draw()
{
	glBindVertexArray(m_Vao);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
}
