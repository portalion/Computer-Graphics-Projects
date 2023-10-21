#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Utils.h"
#include "Shader.h"
#include "Point.h"
#include "Line.h"

class Scene
{
private:
	static Scene* m_Instance;

	Shader* m_Shader;
	glm::mat4 m_Projection;
	std::vector<Point*> m_Points;
	std::vector<Line> m_Lines;

	Point* m_ExpectedPointPosition;
	Line* m_ExpectedLinePositions;
	int m_ActivePointIndex = -1;
	bool m_IsCursorVisible;

	void AddNewPoint(float x, float y);
	void RemovePoint(int index);

	void Init();
	void Draw();
public:
	static const int m_Width = 1024;
	static const int m_Height = 768;
	
	GLFWwindow* window;

	~Scene();

	void Run();

	static Scene* GetInstance();
};

