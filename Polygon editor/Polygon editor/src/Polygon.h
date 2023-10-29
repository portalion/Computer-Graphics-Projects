#pragma once
#include <vector>
#include "Utils.h"

class Line;
class Point;

enum class UpdatingMode
{
	NOT_ACTIVE = -1,
	ADD_VERTICES = 0,
	EDIT_POLYGON = 1
};

class Polygon
{
private:
	std::vector<Line*> m_Lines;
	std::vector<Point*> m_Points;

	int m_ActivePointIndex = -1;
	int m_HoveredPointIndex = -1;
	int m_HoveredLineIndex = -1;
	int m_CurrentPopupLineIndex = -1;

	Point* m_ExpectedPointPosition;
	Line* m_ExpectedLinePositions;
	bool m_IsCursorVisible;
	int m_DisplayOffset = 10;

	bool m_ShouldDisplayCursor;
	bool m_Dragging;

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

	void MoveByMouse();
	void DrawActive();
	void DrawNonActive(bool isHovered);
	void DeleteLine(unsigned int index);
	void DeletePoint(unsigned int index);
public:
	Polygon();
	~Polygon();

	UpdatingMode currentState;

	void AddPointAfterActive(Vertex position);
	void AddVertexInLine(int indexOfLine);
	void RemoveActivePoint();

	void UpdateExpectedPoint();
	void DrawExpectedPoint();

	void Update();
	void DisplayMenu();
	void Draw(bool isHovered);
};

