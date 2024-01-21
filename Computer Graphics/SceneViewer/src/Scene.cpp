#include "Scene.h"
#include "Utils.h"
#include "glm/gtc/matrix_transform.hpp"

#include "MovingCube.h"
#include "Plane.h"

const glm::vec2 Scene::ScreenSize = { 1000.f, 500.f };

Scene::Scene(GLFWwindow* window)
	:m_Running{true}
{
	m_Window = window;

    m_ProjectionMatrix = glm::perspective(glm::radians(45.f), ScreenSize.x / ScreenSize.y, 0.1f, 100.f);

	glm::mat4 view;
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 30.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	m_ProjectionMatrix = m_ProjectionMatrix * view;

	temporaryShader.AddShader("res/shaders/basic.vs", ShaderType::VERTEX_SHADER);
	temporaryShader.AddShader("res/shaders/basic.fs", ShaderType::FRAGMENT_SHADER);
	temporaryShader.CreateShader();
	temporaryShader.Bind();

	InitializeScene();
}

Scene::~Scene()
{
	for (auto& enitity : entities)
		delete enitity;
}

void Scene::Update(const float& deltaTime)
{
	for (auto& enitity : entities)
		enitity->Update(deltaTime);
}

void Scene::InitializeScene()
{
	entities.push_back(new Cube({ 1.f, 1.f, -1.f }));
	auto movingCube = new MovingCube({ 1.f, 1.f, -1.f });
	movingCube->Scale(0.3f);
	entities.push_back(movingCube);
	auto floor = new Plane({ 0.f, 0.f, 0.f });
	glm::mat4 floorMatrix = glm::rotate(glm::mat4(1.f), glm::radians(90.f), { 1.f, 0.f, 0.f });
	floorMatrix = glm::scale(floorMatrix, { 100.f, 100.f, 1.f });
	floorMatrix = glm::translate(floorMatrix, { 0.f, 0.f, 0.f });
	floor->SetModelMatrix(floorMatrix);
	entities.push_back(floor);
}

void Scene::HandleInput()
{
	if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
}

void Scene::Draw()
{
	temporaryShader.Bind();
	temporaryShader.SetUniformMat4f("u_WorldMatrix", m_ProjectionMatrix);
	for (auto& enitity : entities)
		enitity->Draw(&temporaryShader);
}

void Scene::Run()
{
	ImGuiIO& io = ImGui::GetIO();
	while (m_Running)
	{
		if (glfwWindowShouldClose(m_Window))
            m_Running = false;

		HandleInput();
		Update(io.DeltaTime);

		GLCall(glClearColor(0.1f, 0.2f, 0.2f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		Draw();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		GLCall(glfwSwapBuffers(m_Window));
		GLCall(glfwPollEvents());
	}
	glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
}