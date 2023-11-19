#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ControlPoint.h"
#include "Shape.h"
#include "Shader.h"

unsigned int ControlPoint::m_VAO = 0;
unsigned int ControlPoint::m_VBO = 0;
Shader ControlPoint::usedShader;

std::vector<glm::vec3> ControlPoint::m_Positions;

void ControlPoint::GeneratePoints()
{
	for (int i = 0; i < controlPointsPerSide; i++)
	{
		for (int j = 0; j < controlPointsPerSide; j++)
		{
			m_Positions.push_back({
				Shape::m_Position + Shape::m_Height / (controlPointsPerSide - 1) * i,
				Shape::m_Position + Shape::m_Height / (controlPointsPerSide - 1) * j, 0.f });
		}
	}
	
	usedShader.AddShader("res/shaders/PointToCircle.vs", ShaderType::VERTEX_SHADER);
	usedShader.AddShader("res/shaders/PointToCircle.gs", ShaderType::GEOMETRY_SHADER);
	usedShader.AddShader("res/shaders/PointToCircle.fs", ShaderType::FRAGMENT_SHADER);
	usedShader.CreateShader();

	glGenBuffers(1, &m_VBO);
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Positions.size() * sizeof(glm::vec3), m_Positions.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ControlPoint::DrawAll()
{
	if (m_VAO == 0 || m_VBO == 0) GeneratePoints();
	glBindVertexArray(m_VAO);
	usedShader.Bind();
	usedShader.SetUniformMat4f("projectionMatrix", Globals::ProjectionMatrix);
	glDrawArrays(GL_POINTS, 0, static_cast<unsigned int>(m_Positions.size()));
}

void ControlPoint::CleanUp()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteVertexArrays(1, &m_VAO);
}
