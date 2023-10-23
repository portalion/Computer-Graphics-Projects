#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Utils.h"
#include "Shader.h"
#include "Polygon.h"

class Scene
{
private:
	static Scene* m_Instance;

	glm::mat4 m_Projection;

	std::vector<Polygon*> polygons;
	unsigned int m_ActivePolygon;
	unsigned int n_HoveredPolygon;

	void AddNewPolygon();

	void Init();
	void Update();
	void DisplayMenu();
	void Draw();
public:
	static const int m_Width = 1600;
	static const int m_Height = 900;
	static Shader* currentShader;
	
	GLFWwindow* window;

	~Scene();

	void Run();

	static Scene* GetInstance();
};

