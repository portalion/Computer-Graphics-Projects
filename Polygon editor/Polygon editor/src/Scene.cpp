#include "Scene.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Scene* Scene::m_Instance = nullptr;
Shader* Scene::currentShader = nullptr;


void Scene::Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    std::cout << glGetString(GL_VERSION) << std::endl;
    GLCall(glClearColor(0.1f, 0.2f, 0.2f, 1.0f));

    m_Projection = glm::ortho(0.f, static_cast<float>(m_Width), 0.f, static_cast<float>(m_Height));


}

void Scene::Update()
{
    polygon->Update();
    polygon->UpdateExpectedPoint();
}

void Scene::DisplayMenu()
{
    polygon->DisplayMenu();
}

void Scene::Draw()
{
    polygon->Draw();
    polygon->DrawExpectedPoint();
}

Scene::~Scene()
{
    delete currentShader;
}

void Scene::Run()
{
    Init();

    currentShader = new Shader("res/shaders/Basic.shader");
    currentShader->Bind();
    currentShader->SetUniform4f("u_Color", 0.f, 1.f, 0.f, 1.0f);
    currentShader->SetUniformMat4f("u_Projection", m_Projection);

    polygon = new Polygon();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        DisplayMenu();
        Update();
        Draw();

        ImGui::ShowDemoWindow(); // Show demo window! :)
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    delete currentShader;
}

Scene* Scene::GetInstance()
{
    if (m_Instance == nullptr)
        m_Instance = new Scene;
    return m_Instance;
}
