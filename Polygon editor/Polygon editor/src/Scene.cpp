#include "Scene.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Scene* Scene::m_Instance = nullptr;
Shader* Scene::currentShader = nullptr;


void Scene::AddNewPolygon()
{
    polygons.push_back(new Polygon());
    m_ActivePolygon = polygons.size() - 1;
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
    if (m_ActivePolygon != -1)
    {
        polygons[m_ActivePolygon]->Update();
        polygons[m_ActivePolygon]->UpdateExpectedPoint();
    }
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
                m_ActivePolygon = i;
            if (ImGui::IsItemHovered())
                n_HoveredPolygon = i;

            polygons[i]->DisplayMenu();

            ImGui::EndTabItem();
        }

        if (m_ActivePolygon != i && ImGui::IsItemHovered())
            n_HoveredPolygon = i;

        if (!open)
        {
            if (m_ActivePolygon > i) m_ActivePolygon--;
            else if(m_ActivePolygon == i) m_ActivePolygon = polygons.size() > 1 ? 0 : -1;

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
        if (n_HoveredPolygon == i) polygons[i]->Draw(ActivityState::WILL_BE_ACTIVE);
        else if (m_ActivePolygon == i) polygons[i]->Draw(ActivityState::ACTIVE);
        else polygons[i]->Draw( ActivityState::NOT_ACTIVE);
    }
    if (m_ActivePolygon != -1)
        polygons[m_ActivePolygon]->DrawExpectedPoint();
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
