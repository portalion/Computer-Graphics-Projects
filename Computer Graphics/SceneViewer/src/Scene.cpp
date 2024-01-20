#include "Scene.h"
#include "Utils.h"

const glm::vec2 Scene::ScreenSize = { 1000.f, 500.f };

Scene::Scene(GLFWwindow* window)
	:running{true}
{
	m_Window = window;
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
}

void Scene::Run()
{
	while (running)
	{
		if (glfwWindowShouldClose(m_Window))
			running = false;

		HandleInput();
		Update();

		GLCall(glClearColor(0.1f, 0.2f, 0.2f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
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