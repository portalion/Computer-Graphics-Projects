#pragma once
#include <glm/glm.hpp>
#include "Utils.h"

class Line
{
private:
	static unsigned int m_Vao;
	static unsigned int m_Vbo;

	static float m_Vertices[4];

	static void GenerateModel();
public:
	Line(Vertex v1 = { 0.f, 0.f }, Vertex v2 = { 0.f, 0.f });
	glm::mat4 model;

	void SetPosition(Vertex v1, Vertex v2);
	void Draw();
};

