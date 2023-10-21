#pragma once

struct Vertex
{
	float x;
	float y;
};

class Line
{
private:
	static const int vertexSize = 2;
	
	unsigned int vao;
	unsigned int vbo;
	unsigned int ibo;

	float vertices[vertexSize * 2];
public:
	Line(Vertex v1, Vertex v2);

	void Draw();
};

