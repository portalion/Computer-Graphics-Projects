#include "Shape.h"
#include "Shader.h"

void Shape::GenerateVertices()
{
	m_Vertices.push_back({0.5f, 0.5f, 0.f});
	m_Vertices.push_back({0.5f, -0.5f, 0.f});
	m_Vertices.push_back({-0.5f, -0.5f, 0.f});
	m_Vertices.push_back({-0.5f, 0.5f, 0.f});

	m_Indices.push_back(0);
	m_Indices.push_back(1);
	m_Indices.push_back(3);
	m_Indices.push_back(1);
	m_Indices.push_back(2);
	m_Indices.push_back(3);

	GLCall(glBindVertexArray(m_VAO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(glm::vec3), m_Vertices.data(), GL_DYNAMIC_DRAW));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_DYNAMIC_DRAW));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));
}

Shape::Shape()
	:s{ 5 }, t{ 5 }
{
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);
	glGenVertexArrays(1, &m_VAO);

	GenerateVertices();
}

Shape::~Shape()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);
	glDeleteVertexArrays(1, &m_VAO);
}

void Shape::DisplayMenu()
{
}

void Shape::Draw()
{
	GenerateVertices();
	Shader tmp("res/shaders/Basic.shader");
	tmp.Bind();
	GLCall(glBindVertexArray(m_VAO));
	GLCall(glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0));
	GLCall(glBindVertexArray(0));
	tmp.Unbind();
}
