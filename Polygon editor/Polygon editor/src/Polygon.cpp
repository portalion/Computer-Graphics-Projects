#include "Polygon.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Utils.h"

void Polygon::UpdateBuffers()
{
	if (m_Vertices.size() == 0 || m_Indices.size() == 0) 
		return;
	GLCall(glBindVertexArray(vao));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_Vertices.size() * 2, &m_Vertices[0], GL_DYNAMIC_DRAW));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_Indices.size(), &m_Indices[0], GL_DYNAMIC_DRAW));
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	//GLCall(glEnableVertexAttribArray(0));
	//GLCall(glBindVertexArray(0));
	//GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	//GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

Polygon::Polygon()
{
	glGenBuffers(1, &ebo);
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	m_Vertices.push_back({ 10.f, 10.f  });
	m_Vertices.push_back({ 100.f, 10.f, });
	m_Vertices.push_back({ 10.f, 100.f });
	//m_Vertices.push_back({ 100.f, 100.f, });
	m_Indices.push_back(2);
	m_Indices.push_back(1);
	m_Indices.push_back(0);

	//m_Indices.push_back(1);
	//m_Indices.push_back(2);
	//m_Indices.push_back(3);

	UpdateBuffers();
}

Polygon::~Polygon()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void Polygon::AddVertex(Vertex vertex)
{
	m_Vertices.push_back(vertex);
	
	unsigned int n = m_Vertices.size();

	if(n >= 2)
		m_Indices.push_back(n - 2);
	if(n >= 1)
		m_Indices.push_back(n - 1);

	m_Indices.push_back(0);
	UpdateBuffers();
}

void Polygon::Draw()
{
	GLCall(glBindVertexArray(vao));
	glDrawArrays(GL_LINE_STRIP, 0, 3);
}
