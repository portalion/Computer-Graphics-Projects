#include "Shape.h"
#include <glm/glm.hpp>
#include "Shader.h"
#include "ControlPoint.h"

int Shape::m_Height = static_cast<int>(Globals::Height * .8f);
int Shape::m_Width = m_Height;
int Shape::m_Position = 0;

void Shape::CleanUp()
{
	for (int i = 0; i < m_Triangles.size(); i++)
		delete m_Triangles[i];
	m_Triangles.clear();
}

Shape::Shape()
{
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);
	glGenVertexArrays(1, &m_VAO);

	shader.AddShader("res/shaders/Basic.vs", ShaderType::VERTEX_SHADER);
	shader.AddShader("res/shaders/Basic.fs", ShaderType::FRAGMENT_SHADER);
	shader.CreateShader();
	shader.Bind();
	shader.SetUniformMat4f("projectionMatrix", Globals::ProjectionMatrix);

	meshShader.AddShader("res/shaders/Basic.vs", ShaderType::VERTEX_SHADER);
	meshShader.AddShader("res/shaders/BasicRed.fs", ShaderType::FRAGMENT_SHADER);
	meshShader.CreateShader();
	meshShader.Bind();
	meshShader.SetUniformMat4f("projectionMatrix", Globals::ProjectionMatrix);

	for (int i = 0; i < 16; i++)
		m_ControlPoints.push_back(ControlPoint(i));
}

Shape::~Shape()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);
	glDeleteVertexArrays(1, &m_VAO);
	CleanUp();
}

void Shape::GenerateMesh()
{
	m_MeshVertices.clear();

	for (int i = 0; i < n + 1; i++)
		for (int j = 0; j < m + 1; j++)
			m_MeshVertices.push_back({ m_Position + m_Width / n * i, m_Position + m_Height / m * j, 0.f });

	m_MeshIndices.clear();

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			m_MeshIndices.push_back(i * (m + 1) + j);
			m_MeshIndices.push_back(i * (m + 1) + j + 1);
			m_MeshIndices.push_back(i * (m + 1) + j + m + 1);
			m_MeshIndices.push_back(i * (m + 1) + j + 1);
			m_MeshIndices.push_back(i * (m + 1) + j + m + 1);
			m_MeshIndices.push_back(i * (m + 1) + j + m + 2);
		}
	}

	GLCall(glBindVertexArray(m_VAO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, m_MeshVertices.size() * sizeof(glm::vec3), m_MeshVertices.data(), GL_DYNAMIC_DRAW));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_MeshIndices.size() * sizeof(unsigned int), m_MeshIndices.data(), GL_STATIC_DRAW));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));
}

void Shape::DisplayMenu()
{
	for (int i = 0; i < m_ControlPoints.size(); i++)
		m_ControlPoints[i].DisplayMenu();
}

void Shape::GenerateTriangles()
{
	CleanUp();
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			glm::vec3 triangleVertices[3] =
			{
				{ m_Position + i * m_Width / n, m_Position + j * m_Height / m, 0.f },
				{ m_Position + (i + 1) * m_Width / n, m_Position + j * m_Height / m, 0.f },
				{ m_Position + i * m_Width / n, std::ceil(m_Position + (j + 1) * m_Height / m) + 1.f, 0.f }
			};
			m_Triangles.push_back(new Triangle(triangleVertices));
			glm::vec3 triangleVertices2[3] =
			{
				{ m_Position + i * m_Width / n, std::ceil(m_Position + (j + 1) * m_Height / m) + 1.f, 0.f },
				{ m_Position + (i + 1) * m_Width / n, std::ceil(m_Position + (j + 1) * m_Height / m) + 1.f, 0.f },
				{ m_Position + (i + 1) * m_Width / n, m_Position + j * m_Height / m, 0.f }
			};
			m_Triangles.push_back(new Triangle(triangleVertices2));
		}
	}
	for (auto& triangle : m_Triangles)
		triangle->GenerateFillVertices();
}

void Shape::Draw()
{
	shader.Bind();
	shader.SetUniformMat4f("viewMatrix", Globals::ViewMatrix);

	for (auto& triangle : m_Triangles)
		triangle->Draw();

	meshShader.Bind();
	meshShader.SetUniformMat4f("viewMatrix", Globals::ViewMatrix);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	GLCall(glBindVertexArray(m_VAO));
	GLCall(glDrawElements(GL_TRIANGLES, m_MeshIndices.size(), GL_UNSIGNED_INT, 0));
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}