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

	bool m_PointDragged;

	Point* points[2];

	Line* GetNeighbour(bool left);
public:
	static void DeleteModel();

	Line(Vertex v1 = { 0.f, 0.f }, Vertex v2 = { 0.f, 0.f });
	Line(Point* first, Point* second);
	glm::mat4 model;
	
	Vertex GetMiddlePoint();

	bool dragging;
	bool vertical = false;
	bool horizontal = false;

	void UpdatePositionBasedOnPoints();
	bool IsHovered();
	void SetPosition(Vertex v1, Vertex v2);
	void UpdateBasedOnPointsBinded();
	void DisplayMenu();
	void Update();
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

