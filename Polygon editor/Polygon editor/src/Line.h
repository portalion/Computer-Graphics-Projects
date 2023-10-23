#pragma once
#include <glm/glm.hpp>
#include "Utils.h"

#include "Point.h"

enum class LinePointPosition
{
	FIRST_LINE_POINT = 0,
	SECOND_LINE_POINT = 1
};

class Line
{
private:
	static unsigned int m_Vao;
	static unsigned int m_Vbo;

	static float m_Vertices[4];

	static void GenerateModel();

	Point* points[2];
	inline void UpdatePositionBasedOnPoints() { SetPosition(points[0]->GetPosition(), points[1]->GetPosition()); }
public:
	static void DeleteModel();

	Line(Vertex v1 = { 0.f, 0.f }, Vertex v2 = { 0.f, 0.f });
	Line(Point* first, Point* second);
	glm::mat4 model;
	
	void SetPosition(Vertex v1, Vertex v2);
	void UpdateBasedOnPointsBinded();
	void Draw();

	inline void BindPoint(Point* toInsert, LinePointPosition positionToInsert)
	{
		points[static_cast<int>(positionToInsert)] = toInsert;
		UpdatePositionBasedOnPoints();
	}
	inline void BindPoint(Point* firstPoint, Point* secondPoint)
	{
		points[0] = firstPoint; points[1] = secondPoint; UpdatePositionBasedOnPoints();
	}
};

