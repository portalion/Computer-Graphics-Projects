#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

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

void ControlPoint::UpdateZ()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferSubData(GL_ARRAY_BUFFER,3 * sizeof(float) * id + 2 * sizeof(float), sizeof(z), &z);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

ControlPoint::ControlPoint(int id)
: id{id}, z{0}
{
	if (m_VAO == 0 || m_VBO == 0) GeneratePoints();
}

bool ControlPoint::DisplayMenu()
{
	bool changed = false;
	ImGui::BeginChild(("Point" + std::to_string(id)).c_str(), ImVec2(250, 30));
	changed = ImGui::DragFloat(("z of Point" + std::to_string(id)).c_str(), &z, 1.f, -1000, 1000);
	ImGui::EndChild();

	if (changed)UpdateZ();

	return changed;
}

int binomialCoefficients(int n, int k) {
	if (k == 0 || k == n)
		return 1;
	return binomialCoefficients(n - 1, k - 1) + binomialCoefficients(n - 1, k);
}

float ControlPoint::GetZOfPoint(float x, float y)
{
	x -= Shape::m_Position;
	y -= Shape::m_Position;
	x /= Shape::m_Width;
	y /= Shape::m_Height;
	float result = 0;
	for (int i = 0; i < controlPointsPerSide; i++)
	{
		for (int j = 0; j < controlPointsPerSide; j++)
		{
			float Bx = binomialCoefficients(3, i) * pow(x, i) * pow(1 - x, 3 - i);
			float By = binomialCoefficients(3, j) * pow(y, j) * pow(1 - y, 3 - j);
			result += Shape::m_ControlPoints[i * controlPointsPerSide + j].z / Shape::m_Width * Bx * By;
		}
	}
	return result * Shape::m_Width;
}

void ControlPoint::DrawAll()
{
	if (m_VAO == 0 || m_VBO == 0) GeneratePoints();
	usedShader.Bind();
	usedShader.SetUniformMat4f("projectionMatrix", Globals::ProjectionMatrix);
	usedShader.SetUniformMat4f("viewMatrix", Globals::ViewMatrix);
	glBindVertexArray(m_VAO); 
	glDrawArrays(GL_POINTS, 0, static_cast<unsigned int>(m_Positions.size()));
}

void ControlPoint::CleanUp()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteVertexArrays(1, &m_VAO);
}
