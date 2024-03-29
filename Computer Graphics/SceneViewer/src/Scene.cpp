#include "Scene.h"
#include "Utils.h"
#include "glm/gtc/matrix_transform.hpp"

#include "MovingCube.h"
#include "RotatingCube.h"
#include "Plane.h"
#include "Sphere.h"	

const glm::vec2 Scene::ScreenSize = { 1000.f, 500.f };

Scene::Scene(GLFWwindow* window)
	:m_Running{ true }
{
	m_Window = window;

    m_ProjectionMatrix = glm::perspective(glm::radians(45.f), ScreenSize.x / ScreenSize.y, 0.1f, 100.f);
	m_ViewMatrix = glm::mat4(1.f);
			
	InitializeLightSources();
	InitializeShaders();
	InitializeScene();
}

void Scene::InitializeShaders()
{
	activeShaderIndex = 0;

	Shader* temporaryShader = new Shader();
	temporaryShader->AddShader("res/shaders/phongLight.vs", ShaderType::VERTEX_SHADER);
	temporaryShader->AddShader("res/shaders/phongLight.fs", ShaderType::FRAGMENT_SHADER);
	temporaryShader->CreateShader();
	shaders.push_back(temporaryShader);

	temporaryShader = new Shader();
	temporaryShader->AddShader("res/shaders/noLight.vs", ShaderType::VERTEX_SHADER);
	temporaryShader->AddShader("res/shaders/noLight.fs", ShaderType::FRAGMENT_SHADER);
	temporaryShader->CreateShader();
	shaders.push_back(temporaryShader);

	temporaryShader = new Shader();
	temporaryShader->AddShader("res/shaders/gouraudLight.vs", ShaderType::VERTEX_SHADER);
	temporaryShader->AddShader("res/shaders/gouraudLight.fs", ShaderType::FRAGMENT_SHADER);
	temporaryShader->CreateShader();
	shaders.push_back(temporaryShader);
}

void Scene::InitializeLightSources()
{
	sun.position = { 10.f, 5.f, 0.f };
	sun.color = { 1.f, 1.f, 1.f };
	sun2.position = { -10.f, 5.f, 0.f };
	sun2.color = { 1.f, 1.f, 1.f };
}

Scene::~Scene()
{
	for (auto& enitity : entities)
		delete enitity;
	for (auto& camera : cameras)
		delete camera;
	for (auto& shader : shaders)
		delete shader;
	for (auto& lightSource : lightningSources)
		delete lightSource;
}

void Scene::Update(const float& deltaTime)
{
	for (auto& enitity : entities)
		enitity->Update(deltaTime);

	m_ProjectionMatrix = glm::perspective(glm::radians(45.f), ScreenSize.x / ScreenSize.y, 0.1f, 100.f);
	if(activeCameraIndex != -1)
		m_ViewMatrix = cameras[activeCameraIndex]->GetViewMatrix();
}

void Scene::InitializeScene()
{
	entities.push_back(new Cube({ 1.f, 1.f, -1.f }));

	auto movingCube = new MovingCube({ 1.f, 0.f, -1.f });
	movingCube->Scale(0.3f);
	entities.push_back(movingCube);

	auto floor = new Plane({ 0.f, 0.f, 0.f });
	glm::mat4 floorMatrix = glm::rotate(glm::mat4(1.f), glm::radians(90.f), { 1.f, 0.f, 0.f });
	floorMatrix = glm::translate(floorMatrix, { 0.f, 0.f, 0.f });
	floorMatrix = glm::scale(floorMatrix, { 20.f, 20.f, 1.f });
	floor->SetModelMatrix(floorMatrix);
	entities.push_back(floor);

	auto rotatingObject = new RotatingCube({ 0.f, 0.f, 4.f }, 2.f);
	entities.push_back(rotatingObject);

	cameras.push_back(new Camera{ {10.f, 10.f, 10.f}, {0.f, 0.f, 0.f} });
	cameras.push_back(new Camera{ {-10.f, 10.f, -10.f}, {0.f, 0.f, 0.f} });
	cameras.push_back(new ObserverCamera{ movingCube, {-10.f, 10.f, -10.f} });
	cameras.push_back(new ObserverCamera{ movingCube, {0.f, 10.f, 0.f} });
	cameras.push_back(new MovingCamera{ movingCube });
	activeCameraIndex = 0;
}

void Scene::HandleInput(ImGuiIO& io)
{
	
	if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
	if (io.MouseClicked[0] && activeCameraIndex != -1)
		activeCameraIndex = ++activeCameraIndex % cameras.size();
	if (ImGui::IsKeyPressed(ImGuiKey_S, false) && activeShaderIndex != -1)
		activeShaderIndex = ++activeShaderIndex % shaders.size();
}

void Scene::Draw()
{
	Shader* activeShader = shaders[activeShaderIndex];
	activeShader->Bind();
	activeShader->SetUniformMat4f("u_WorldMatrix", m_ProjectionMatrix * m_ViewMatrix);
	activeShader->SetUniformVec3f("viewPos", cameras[activeCameraIndex]->GetPosition());
	activeShader->SetUniformVec3f("pointLight[0].position", sun.position);
	activeShader->SetUniformVec3f("pointLight[0].color", sun.color);
	activeShader->SetUniformVec3f("pointLight[1].position", sun2.position);
	activeShader->SetUniformVec3f("pointLight[1].color", sun2.color);
	activeShader->SetUniformVec3f("spotlight.position", static_cast<MovingCube*>(entities[1])->GetPosition());
	activeShader->SetUniformVec3f("spotlight.direction", static_cast<MovingCube*>(entities[1])->GetLookingDirection() - static_cast<MovingCube*>(entities[1])->GetPosition());
	activeShader->SetUniformVec3f("spotlight.color", {1.f, 1.f, 1.f});

	for (auto& enitity : entities)
		enitity->Draw(activeShader);
	s.Draw();
}

void Scene::Run()
{
	ImGuiIO& io = ImGui::GetIO();
	while (m_Running)
	{
		if (glfwWindowShouldClose(m_Window))
            m_Running = false;

		HandleInput(io);
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