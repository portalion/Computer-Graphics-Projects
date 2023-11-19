#include "Shape.h"
#include <glm/glm.hpp>
#include "Shader.h"

int Shape::m_Height = static_cast<int>(Globals::Height * .8f);
int Shape::m_Width = m_Height;
int Shape::m_Position = 0;

void Shape::GenerateTriangles()
{
	m_Triangles.clear();
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
			m_Triangles.push_back(Triangle(triangleVertices));
			glm::vec3 triangleVertices2[3] =
			{
				{ m_Position + i * m_Width / n, std::ceil(m_Position + (j + 1) * m_Height / m) + 1.f, 0.f },
				{ m_Position + (i + 1) * m_Width / n, std::ceil(m_Position + (j + 1) * m_Height / m) + 1.f, 0.f },
				{ m_Position + (i + 1) * m_Width / n, m_Position + j * m_Height / m, 0.f }
			};
			m_Triangles.push_back(Triangle(triangleVertices2));
		}
	}
	for (auto& triangle : m_Triangles)
		triangle.GenerateFillVertices();
}

void Shape::Draw()
{
	Shader basic;
	basic.AddShader("res/shaders/Basic.vs", ShaderType::VERTEX_SHADER);
	basic.AddShader("res/shaders/Basic.fs", ShaderType::FRAGMENT_SHADER);
	basic.CreateShader();
	basic.Bind();
	basic.SetUniformMat4f("projectionMatrix", Globals::ProjectionMatrix);
	for (auto& triangle : m_Triangles)
		triangle.Draw();
}