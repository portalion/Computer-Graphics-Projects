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

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = Globals::Width / 2.0;
float lastY = Globals::Height / 2.0;
float fov = 45.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

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


    Globals::Width = 1000;
    Globals::Height = mode->height/2;
    Globals::ProjectionMatrix = glm::ortho(0.f, static_cast<float>(Globals::Width), 0.f, static_cast<float>(Globals::Height), -1000.f, 1000.f);
    Shape::m_Height = static_cast<int>(Globals::Height * .8f);
    Shape::m_Width = Shape::m_Height;
    Shape::m_Position = static_cast<int>((Globals::Height - Shape::m_Height) / 2.f);

    GLFWwindow* window = glfwCreateWindow(Globals::Width, Globals::Height, "3D triangles", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwSetWindowPos(window, 500, 650);

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

    glm::vec3 triangleVertices[3] =
    {
        { 600.f, 500.f, 0.f },
        { 500.f, 600.f, 0.f },
        { 700.f, 500.f, 0.f }
    };

    Shape test;
    test.GenerateTriangles();
    test.GenerateMesh();

    glfwSetCursorPosCallback(window, mouse_callback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);

        //CAMERA
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        const float cameraSpeed = 500.f * deltaTime; // adjust accordingly
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

        Globals::ViewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

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

        ImGui::SetNextWindowPos(ImVec2(Globals::Width - 300, 0), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(300, Globals::Height), ImGuiCond_FirstUseEver);
        ImGui::Begin("MainMenu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        test.DisplayMenu();
        ImGui::End();

        test.Draw();
        ControlPoint::DrawAll();

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

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2((float)xpos, (float)ypos);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

