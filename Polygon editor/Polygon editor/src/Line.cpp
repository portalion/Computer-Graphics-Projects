#include "Line.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Utils.h"
#include "Scene.h"

 unsigned int Line::m_Vao = 0;
 unsigned int Line:: m_Vbo = 0;

 float Line::m_Vertices[4];

void Line::GenerateModel()
{
	m_Vertices[0] = 0.f;
	m_Vertices[1] = 0.f;
	m_Vertices[2] = 1.f;
	m_Vertices[3] = 0.f;

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

void Line::DeleteModel()
{
	GLCall(glDeleteBuffers(1, &m_Vbo));
	GLCall(glDeleteVertexArrays(1, &m_Vao));
}

Line::Line(Vertex v1, Vertex v2)
	:model(glm::mat4(1.f))
{
	if (m_Vbo == 0 || m_Vao == 0)
		GenerateModel();

	SetPosition(v1, v2);
}

Line::Line(Point* first, Point* second)
{
	if (m_Vbo == 0 || m_Vao == 0)
		GenerateModel();

	BindPoint(first, second);
}

void Line::SetPosition(Vertex v1, Vertex v2)
{
	model = glm::mat4(1.f);
	float angle = atan2(v2.y - v1.y, v2.x - v1.x) - atan2(0.f, 1.f);
	model = glm::translate(model, glm::vec3(v1.x, v1.y, 0.f));
	model = glm::rotate(model, angle, glm::vec3(0.f, 0.f, 1.f));
	model = glm::scale(model, glm::vec3(glm::length(glm::vec2(v2.x - v1.x, v2.y - v1.y)), 1.f, 1.f));
}

void Line::UpdateBasedOnPointsBinded()
{
	if (points[0]->Moved() || points[1]->Moved())
		UpdatePositionBasedOnPoints();
}

void Line::Draw()
{
	Scene::currentShader->SetUniformMat4f("u_Model", model);
	GLCall(glBindVertexArray(m_Vao));
	GLCall(glDrawArrays(GL_LINES, 0, 2));
}
