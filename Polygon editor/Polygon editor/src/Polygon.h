#pragma once
#include <vector>
#include "Utils.h"

class Line;
class Point;

class Polygon
{
private:
	std::vector<Line*> m_Lines;
	std::vector<Point*> m_Points;

	int m_ActivePointIndex = -1;

	Point* m_ExpectedPointPosition;
	Line* m_ExpectedLinePositions;
	bool m_IsCursorVisible;

	inline unsigned int GetPreviousPointIndex(unsigned int index) 
	{
		if (index <= 0) return static_cast<unsigned int>(m_Points.size() - 1);
		else return index - 1;
	}
	inline unsigned int GetNextPointIndex(unsigned int index)
	{
		if (index + 1 >= m_Points.size()) return 0;
		else return index + 1;
	}

	void DeleteLine(unsigned int index);
	void DeletePoint(unsigned int index);
public:
	Polygon();
	~Polygon();

	void AddPointAfterActive(Vertex position);
	void RemoveActivePoint();

	void UpdateExpectedPoint();
	void DrawExpectedPoint();

	void Update();
	void DisplayMenu();
	void Draw(ActivityState activity = ActivityState::NOT_ACTIVE);
};

