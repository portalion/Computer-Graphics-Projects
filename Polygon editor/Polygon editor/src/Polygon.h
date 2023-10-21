#pragma once
#include <vector>

struct Vertex
{
	float x;
	float y;
};

class Polygon
{
private:
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;

	void UpdateBuffers();
public:
	Polygon();
	~Polygon();

	void AddVertex(Vertex vertex);
	void Draw();
};

