#include "Shape.h"
#include "Shader.h"

#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

void Shape::GenerateVertices()
{
	m_Vertices.clear();

	for(int i = 0; i < s + 1; i++)
		for(int j = 0; j < t + 1; j++)
			m_Vertices.push_back({ m_Width / t * j, m_Height / s * i, 0.f });

	m_Indices.clear();

	for (int i = 0; i < s; i++)
	{
		for (int j = 0; j < t; j++)
		{
			m_Indices.push_back(i * (t + 1) + j);
			m_Indices.push_back(i * (t + 1) + j + 1);
			m_Indices.push_back(i * (t + 1) + j + t + 1);
			m_Indices.push_back(i * (t + 1) + j + 1);
			m_Indices.push_back(i * (t + 1) + j + t + 1);
			m_Indices.push_back(i * (t + 1) + j + t + 2);
		}
	}

	GLCall(glBindVertexArray(m_VAO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(glm::vec3), m_Vertices.data(), GL_DYNAMIC_DRAW));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));
}

Shape::Shape()
	:s{ 100 }, t{ 100 }
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
	float margin = (Globals::Height - m_Height) / 2.f;
	auto transposeMatrix = glm::translate(Globals::ProjectionMatrix, glm::vec3(margin, margin, 0.f));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	GenerateVertices();
	Shader tmp("res/shaders/Basic.shader");
	tmp.Bind();
	tmp.SetUniformMat4f("u_Projection", transposeMatrix);
	GLCall(glBindVertexArray(m_VAO));
	GLCall(glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0));
	GLCall(glBindVertexArray(0));
	tmp.Unbind();
}
