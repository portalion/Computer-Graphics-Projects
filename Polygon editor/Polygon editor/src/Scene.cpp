#include "Scene.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Point.h"

Scene* Scene::m_Instance = nullptr;


void Scene::Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    std::cout << glGetString(GL_VERSION) << std::endl;
}

void Scene::Draw()
{
    float positions[] =
    {
        100.0f, 100.0f, 
        200.0f, 100.0f, 
        200.0f, 200.0f, 
        ImGui::GetIO().MousePos.x,  m_Height - ImGui::GetIO().MousePos.y
    };

    unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glEnableVertexAttribArray(0);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    GLCall(glBindVertexArray(vao));

    GLCall(glDrawElements(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, nullptr));


    shader->Bind();
}

void Scene::Run()
{
    Init();

    shader = new Shader("res/shaders/Basic.shader");
    shader->Bind();
    shader->SetUniform4f("u_Color", 0.f, 1.f, 0.f, 1.0f);
    shader->SetUniformMat4f("u_MVP", glm::ortho(0.f, static_cast<float>(m_Width), 0.f, static_cast<float>(m_Height)));

    GLCall(glBindVertexArray(0));
    //shader->Unbind();
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow(); // Show demo window! :)
        
        Point test(100.f, 100.f);
        Draw();
        test.Draw();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

Scene* Scene::GetInstance()
{
    if (m_Instance == nullptr)
        m_Instance = new Scene;
    return m_Instance;
}
