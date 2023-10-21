#pragma once

struct Vertex
{
	float x;
	float y;
};

class Line
{
private:
	unsigned int m_Vao;
	unsigned int m_Vbo;

	float m_Vertices[4];
public:
	Line(Vertex v1, Vertex v2);

	void Draw();
};

