#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "Cube.h"
#include "Shader.h"

class Scene
{
private:
	GLFWwindow* m_Window;
	bool m_Running;

	//TODO: change it
	Shader temporaryShader;

	glm::mat4 m_ProjectionMatrix;

	std::vector<Cube*> cubes;

	void InitializeScene();

	void Update();
	void HandleInput();
	void Draw();
public:
	static const glm::vec2 ScreenSize;

	Scene(GLFWwindow* window);
	~Scene();

	void Run();
};

