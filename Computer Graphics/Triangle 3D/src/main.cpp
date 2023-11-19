#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Utils.h"
#include "Shader.h"
#include "Shape.h"
#include "ControlPoint.h"
#include "Triangle.h"

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);

    int monitorsCount;
    auto monitors = glfwGetMonitors(&monitorsCount);
    GLFWmonitor* monitor = monitors[monitorsCount - 1];
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    Globals::Width = mode->width;
    Globals::Height = mode->height;
    Globals::ProjectionMatrix = glm::ortho(0.f, static_cast<float>(Globals::Width), 0.f, static_cast<float>(Globals::Height), -10.f, 10.f);
    Shape::m_Height = static_cast<int>(Globals::Height * .8f);
    Shape::m_Width = Shape::m_Height;
    Shape::m_Position = static_cast<int>((Globals::Height - Shape::m_Height) / 2.f);

    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "3D triangles", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "GLEW ERROR" << std::endl;

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
    
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

    Shader basic;
    basic.AddShader("res/shaders/Basic.vs", ShaderType::VERTEX_SHADER);
    basic.AddShader("res/shaders/Basic.fs", ShaderType::FRAGMENT_SHADER);
    basic.CreateShader();

    glm::vec3 triangleVertices[3] =
    {
        { 600.f, 500.f, 0.f },
        { 500.f, 600.f, 0.f },
        { 700.f, 500.f, 0.f }
    };

    Triangle test(triangleVertices);
    test.GenerateFillVertices();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);

        /* Render here */
        GLCall(glClearColor(0.1f, 0.2f, 0.2f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_FirstUseEver);
        ImGui::Begin("Fps Display", nullptr, 
            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDocking
        );

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();

        ControlPoint::DrawAll();
        test.Draw();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }

    ControlPoint::CleanUp();
    glfwTerminate();
    return 0;
}