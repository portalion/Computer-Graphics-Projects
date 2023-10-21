#include "Line.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Utils.h"

Line::Line(Vertex v1, Vertex v2)
{
	m_Vertices[0] = v1.x;
	m_Vertices[1] = v1.y;
	m_Vertices[2] = v2.x;
	m_Vertices[3] = v2.y;

	GLCall(glGenBuffers(1, &m_Vbo));
	GLCall(glGenVertexArrays(1, &m_Vao));

	GLCall(glBindVertexArray(m_Vao));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(m_Vertices), m_Vertices, GL_STATIC_DRAW));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
	GLCall(glEnableVertexAttribArray(0));

	GLCall(glBindVertexArray(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void Line::Draw()
{
	GLCall(glBindVertexArray(m_Vao));
	GLCall(glDrawArrays(GL_LINES, 0, 2));
}
