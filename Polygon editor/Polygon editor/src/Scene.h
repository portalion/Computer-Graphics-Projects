#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Utils.h"
#include "Shader.h"
#include "Point.h"
#include "Line.h"
#include "Polygon.h"

class Scene
{
private:
	static Scene* m_Instance;

	glm::mat4 m_Projection;

	Polygon* polygon;

	

	void Init();
	void Draw();
public:
	static const int m_Width = 1024;
	static const int m_Height = 768;
	static Shader* currentShader;
	
	GLFWwindow* window;

	~Scene();

	void Run();

	static Scene* GetInstance();
};

