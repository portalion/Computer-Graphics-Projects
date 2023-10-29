#include "Scene.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Line.h"
#include "Point.h"

Scene* Scene::m_Instance = nullptr;
Shader* Scene::currentShader = nullptr;
bool Scene::BresenhamLine = false;


void Scene::AddNewPolygon()
{
    polygons.push_back(new Polygon());
    ChangedActivePolygon(polygons.size() - 1);
}

void Scene::ChangedActivePolygon(int newPolygonIndex)
{
    if (m_ActivePolygon != -1)
        polygons[m_ActivePolygon]->currentState = UpdatingMode::NOT_ACTIVE;
    m_ActivePolygon = newPolygonIndex;
    if (m_ActivePolygon != -1)
        polygons[m_ActivePolygon]->currentState = UpdatingMode::EDIT_POLYGON;
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
    GLCall(glClearColor(0.1f, 0.2f, 0.2f, 1.0f));

    m_Projection = glm::ortho(0.f, static_cast<float>(m_Width), 0.f, static_cast<float>(m_Height));
    m_ActivePolygon = -1;
}

void Scene::Update()
{
    for(int i = 0; i < polygons.size(); i++)
        polygons[i]->Update();
}

void Scene::DisplayMenu()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoDocking;

    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, main_viewport->WorkSize.y), ImGuiCond_FirstUseEver);

    ImGui::Begin("Main Menu", nullptr, window_flags);
    ImGui::Checkbox("Use bresenhamAlgorithm", &BresenhamLine);

    ImGui::BeginTabBar("Main Polygons Bar", ImGuiTabBarFlags_TabListPopupButton);

    if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
        AddNewPolygon();

    n_HoveredPolygon = -1;

    for (int i = 0; i < polygons.size(); i++)
    {
        bool open = true;
        std::string name = "Polygon" + std::to_string(i);
        if (ImGui::BeginTabItem(name.c_str(), &open, m_ActivePolygon == i ? ImGuiTabItemFlags_SetSelected : ImGuiTabItemFlags_None))
        {
            if (ImGui::IsItemActive())
                ChangedActivePolygon(i);
            if (ImGui::IsItemHovered())
                n_HoveredPolygon = i;

            polygons[i]->DisplayMenu();

            ImGui::EndTabItem();
        }

        if (m_ActivePolygon != i && ImGui::IsItemHovered())
            n_HoveredPolygon = i;

        if (!open)
        {
            if (m_ActivePolygon > i) ChangedActivePolygon(m_ActivePolygon - 1);
            else if(m_ActivePolygon == i) ChangedActivePolygon(polygons.size() > 1 ? 0 : -1);

            delete polygons[i];
            polygons.erase(polygons.begin() + i--);
        }
    }
    ImGui::EndTabBar();
    ImGui::End();
}

void Scene::Draw()
{
    for (int i = 0; i < polygons.size(); i++)
    {
        if (n_HoveredPolygon == i) polygons[i]->Draw(true);
        else polygons[i]->Draw(false);
    }
}

Scene::~Scene()
{
    delete currentShader;
    Line::DeleteModel();
    Point::DeleteModel();
}

void Scene::Run()
{
    Init();

    currentShader = new Shader("res/shaders/Basic.shader");
    currentShader->Bind();
    currentShader->SetUniform4f("u_Color", 0.f, 1.f, 0.f, 1.0f);
    currentShader->SetUniformMat4f("u_Projection", m_Projection);

    AddNewPolygon();
    polygons[0]->AddPointAfterActive({ 500.f, 500.f });
    polygons[0]->AddPointAfterActive({ 500.f, 800.f });
    polygons[0]->AddPointAfterActive({ 800.f, 800.f });
    polygons[0]->AddPointAfterActive({ 800.f, 500.f });
    polygons[0]->m_Lines[0]->AddRelation(true);
    polygons[0]->m_Lines[1]->AddRelation(false);
    polygons[0]->m_Lines[2]->AddRelation(true);
    polygons[0]->m_Lines[3]->AddRelation(false);

    AddNewPolygon();
    polygons[1]->AddPointAfterActive({ 900.f, 500.f });
    polygons[1]->AddPointAfterActive({ 900.f, 800.f });
    polygons[1]->AddPointAfterActive({ 1200.f, 800.f });
    polygons[1]->AddPointAfterActive({ 1200.f, 500.f });
    polygons[1]->AddPointAfterActive({ 1400.f, 500.f });
    polygons[1]->m_Lines[0]->AddRelation(true);
    polygons[1]->m_Lines[1]->AddRelation(false);
    polygons[1]->m_Lines[2]->AddRelation(true);
    polygons[1]->m_Lines[3]->AddRelation(false);

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
