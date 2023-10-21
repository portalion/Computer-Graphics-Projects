#include "Point.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Utils.h"
#include "Scene.h"

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
		m_Vertices.push_back({ size * cos(glm::radians(currentAngle)), size * sin(glm::radians(currentAngle)), 0.f });
	}

	for (int i = 0; i < triangleCount; i++)
	{
		m_Indices.push_back(0);
		m_Indices.push_back(i + 1);
		m_Indices.push_back(i + 2);
	}

	GLCall(glGenBuffers(1, &m_Vbo));
	GLCall(glGenBuffers(1, &m_Ibo));
	GLCall(glGenVertexArrays(1, &m_Vao));

	GLCall(glBindVertexArray(m_Vao));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_Vertices.size(), &m_Vertices[0], GL_STATIC_DRAW));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_Indices.size(), &m_Indices[0], GL_STATIC_DRAW));

	GLCall(glBindVertexArray(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

Point::Point(float x, float y)
	:model(glm::mat4(1.f))
{
	if(m_Vao == 0 || m_Vbo == 0 || m_Ibo == 0)
		GenerateVertices();

	SetPosition(x, y);
}

void Point::UpdateMatrices()
{
	model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(this->x, this->y, 0.f));
}

void Point::Draw()
{
	GLCall(glBindVertexArray(m_Vao));
	GLCall(glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0));
}

void Point::DisplayMenu()
{
	if (!ImGui::Begin("Active point menu", nullptr))
	{
		ImGui::End();
		return;
	}
	ImGui::PushItemWidth(80);
	bool changed = ImGui::SliderFloat("X", &x, 0.0f, Scene::m_Width); ImGui::SameLine();
	changed = changed || ImGui::SliderFloat("Y", &y, 0.0f, Scene::m_Height);
	if (changed) UpdateMatrices();
	ImGui::PopItemWidth();
	ImGui::End();
}

void Point::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
	UpdateMatrices();
}
