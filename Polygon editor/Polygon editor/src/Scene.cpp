#include "Scene.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Scene* Scene::m_Instance = nullptr;

void Scene::AddNewPoint(float x, float y)
{
    m_Points.push_back(new Point(x, y));
    m_ActivePointIndex++;

    if (m_Points.size() > 1)
    {
        if(!m_Lines.empty())m_Lines.pop_back();
        m_Lines.push_back(Line(m_Points[m_ActivePointIndex], m_Points[m_ActivePointIndex - 1]));
        m_Lines.push_back(Line(m_Points[0], m_Points[m_ActivePointIndex]));
    }
}

void Scene::RemovePoint(int index)
{
    m_Points.erase(m_Points.begin() + index);
    m_Lines.pop_back();
    m_Lines.pop_back();
    m_ActivePointIndex--;
    m_Lines.push_back(Line(m_Points[0], m_Points[m_ActivePointIndex]));
}

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

    m_Projection = glm::ortho(0.f, static_cast<float>(m_Width), 0.f, static_cast<float>(m_Height));

    m_ExpectedPointPosition = new Point(0, 0);
    m_ExpectedLinePositions = new Line[2];
}

void Scene::Draw()
{
    m_Shader->SetUniform4f("u_Color", 0.5f, 0.5f, 0.5f, 1.f);
    for (int i = 0; i < m_Points.size(); i++)
    {
        m_Shader->SetUniformMat4f("u_MVP", m_Projection * m_Points[i]->model);
        if (i == m_ActivePointIndex)
        {
            m_Shader->SetUniform4f("u_Color", 0.f, 1.f, 0.f, 1.f);
            m_Points[i]->Draw();
            m_Shader->SetUniform4f("u_Color", 0.5f, 0.5f, 0.5f, 1.f);
        }
        else m_Points[i]->Draw();
    }
    m_Shader->SetUniform4f("u_Color", 0.f, 1.f, 0.f, 1.f);
    for (int i = 0; i < m_Lines.size(); i++)
    {
        m_Shader->SetUniformMat4f("u_MVP", m_Projection * m_Lines[i].model);
        m_Lines[i].Draw();
    }
    m_Shader->SetUniform4f("u_Color", 0.5f, 0.5f, 0.5f, 1.f);
    if (m_IsCursorVisible)
    {
        m_Shader->SetUniformMat4f("u_MVP", m_Projection * m_ExpectedPointPosition->model);
        m_ExpectedPointPosition->Draw();
        m_Shader->SetUniformMat4f("u_MVP", m_Projection * m_ExpectedLinePositions[0].model);
        m_ExpectedLinePositions[0].Draw();
        m_Shader->SetUniformMat4f("u_MVP", m_Projection * m_ExpectedLinePositions[1].model);
        m_ExpectedLinePositions[1].Draw();
    }

    m_Points[m_ActivePointIndex]->DisplayMenu();
}

Scene::~Scene()
{
    delete m_ExpectedPointPosition;
    delete m_Shader;
    delete[] m_ExpectedLinePositions;
}

void Scene::Run()
{
    Init();

    m_Shader = new Shader("res/shaders/Basic.shader");
    m_Shader->Bind();
    m_Shader->SetUniform4f("u_Color", 0.f, 1.f, 0.f, 1.0f);

    AddNewPoint(0, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClearColor(0.1f, 0.2f, 0.2f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow(); // Show demo window! :)
        
        auto& io = ImGui::GetIO();
        if(!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            AddNewPoint(io.MousePos.x, m_Height - io.MousePos.y);

        Vertex CurrentMousePos = { io.MousePos.x, m_Height - io.MousePos.y };

        m_ExpectedPointPosition->SetPosition(CurrentMousePos);
        m_ExpectedLinePositions[0].SetPosition(m_Points[0]->GetPosition(), CurrentMousePos);
        m_ExpectedLinePositions[1].SetPosition(m_Points[m_ActivePointIndex]->GetPosition(), CurrentMousePos);
        m_IsCursorVisible = !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);

        if (m_Points[m_ActivePointIndex]->ShouldRemove())
            RemovePoint(m_ActivePointIndex);

        for (auto& line : m_Lines)
            line.UpdateBasedOnPointsBinded();

        Draw();
;

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    for (auto& point : m_Points)
        delete point;
}

Scene* Scene::GetInstance()
{
    if (m_Instance == nullptr)
        m_Instance = new Scene;
    return m_Instance;
}
