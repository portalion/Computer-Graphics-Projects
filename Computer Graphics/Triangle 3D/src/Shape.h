#pragma once
#include "Utils.h"
#include "Triangle.h"

class Shape
{
private:
	std::vector<Triangle*> m_Triangles;
	void CleanUp();
public:
	~Shape();

	static int m_Height;
	static int m_Width;

	static int m_Position;

	int n = 7, m = 6;

	void GenerateTriangles();
	void Draw();
};

