#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include "DrawableEntity.h"
#include "Shader.h"
#include "LightningSource.h"
#include "Camera.h"

class Scene
{
private:
	GLFWwindow* m_Window;
	bool m_Running;

	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;

	int activeShaderIndex;
	std::vector<Shader*> shaders;

	int activeCameraIndex;
	std::vector<Camera*> cameras;

	std::vector<DrawableEntity*> entities;
	std::vector<LightningSource*> lightningSources;

	LightningSource sun;

	void InitializeShaders();
	void InitializeCameras();
	void InitializeScene();

	void Update(const float& deltaTime);
	void HandleInput(ImGuiIO& io);
	void Draw();
public:
	static const glm::vec2 ScreenSize;

	Scene(GLFWwindow* window);
	~Scene();

	void Run();
};

