#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>
#include <glm/glm.hpp>

class Scene
{
private:
	GLFWwindow* m_Window;
	bool running;

	void Update();
	void HandleInput();
	void Draw();
public:
	static const glm::vec2 ScreenSize;

	Scene(GLFWwindow* window);

	void Run();
};

