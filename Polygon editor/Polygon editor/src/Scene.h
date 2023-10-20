#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Utils.h"
#include "Shader.h"

class Scene
{
private:
	static Scene* m_Instance;

	Shader* shader;

	glm::mat4 proj;

	void Init();
	void Draw();
public:
	static const int m_Width = 1024;
	static const int m_Height = 768;
	
	GLFWwindow* window;
	
	void Run();

	static Scene* GetInstance();
};

