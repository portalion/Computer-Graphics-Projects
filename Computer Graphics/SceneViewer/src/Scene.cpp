#include "Scene.h"
#include "Utils.h"
#include "glm/gtc/matrix_transform.hpp"

#include "Cube.h"

const glm::vec2 Scene::ScreenSize = { 1000.f, 500.f };

Scene::Scene(GLFWwindow* window)
	:m_Running{true}
{
	m_Window = window;

    m_ProjectionMatrix = glm::perspective(90.f, ScreenSize.x / ScreenSize.y, 0.1f, 100.f);

	temporaryShader.AddShader("res/shaders/basic.vs", ShaderType::VERTEX_SHADER);
	temporaryShader.AddShader("res/shaders/basic.fs", ShaderType::FRAGMENT_SHADER);
	temporaryShader.CreateShader();
	temporaryShader.Bind();
}

void Scene::Update()
{

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
	Cube c;
	c.Draw(&temporaryShader);
}

void Scene::Run()
{
	while (m_Running)
	{
		if (glfwWindowShouldClose(m_Window))
            m_Running = false;

		HandleInput();
		Update();

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