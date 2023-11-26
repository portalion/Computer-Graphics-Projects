#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stb_image/stb_image.h>
#include <imfilebrowser.h>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Utils.h"
#include "Shader.h"
#include "Shape.h"
#include "ControlPoint.h"
#include "Triangle.h"
#include "LightSource.h"

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void setupImage(std::string path, bool isNormalMap);

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

    Globals::Width = 1300;
    Globals::Height = mode->height/2;
    Globals::ProjectionMatrix = glm::ortho(0.f, static_cast<float>(Globals::Width), 0.f, static_cast<float>(Globals::Height), -5000.f, 5000.f);
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

    Globals::ViewMatrix = glm::mat4(1.f);

    glm::vec3 triangleVertices[3] =
    {
        { 600.f, 500.f, 0.f },
        { 500.f, 600.f, 0.f },
        { 700.f, 500.f, 0.f }
    };

    Globals::lightSource = new LightSource({Shape::m_Position + Shape::m_Width / 2, Shape::m_Position + Shape::m_Height / 2, 500.f});

    bool drawMesh = false;
    bool doAnimation = true;

    Shape test; 
    test.GenerateTriangles();
    test.GenerateMesh();
    
    glfwSetScrollCallback(window, scroll_callback);

    ImGui::FileBrowser setTextureDialog;
    ImGui::FileBrowser setNormalMapDialog;

    // (optional) set browser properties
    setTextureDialog.SetTitle("Choose image file");
    setTextureDialog.SetTypeFilters({ ".png", ".jpg" }); 
    setNormalMapDialog.SetTitle("Choose image file");
    setNormalMapDialog.SetTypeFilters({ ".png", ".jpg" });

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
            Globals::ViewMatrix = glm::translate(Globals::ViewMatrix, glm::vec3(0.f, cameraSpeed, 0.f));
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            Globals::ViewMatrix = glm::translate(Globals::ViewMatrix, glm::vec3(0.f, -cameraSpeed, 0.f));
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            Globals::ViewMatrix = glm::translate(Globals::ViewMatrix, glm::vec3(-cameraSpeed, 0.f, 0.f));
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            Globals::ViewMatrix = glm::translate(Globals::ViewMatrix, glm::vec3(cameraSpeed, 0.f, 0.f));

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
        if (ImGui::SliderInt("s", &(test.n), 1, 100))
        {
            test.GenerateTriangles();
            test.GenerateMesh();
        }
        if(ImGui::SliderInt("t", &(test.m), 1, 100))
        {
            test.GenerateTriangles();
            test.GenerateMesh();
        }
        ImGui::Checkbox("Draw Mesh", &drawMesh);
        ImGui::SameLine();
        ImGui::Checkbox("Do Animation", &doAnimation);
        ImGui::SliderInt("m", &Globals::m, 1, 100);
        ImGui::SliderFloat("kd", &Globals::kd, 0, 1);
        ImGui::SliderFloat("ks", &Globals::ks, 0, 1);
        ImGui::ColorPicker3("Object Color", &Globals::objectColor.x);
        ImGui::ColorPicker3("Light Color", &Globals::lightColor.x);
        if (ImGui::Button("Reset Position"))
            Globals::ViewMatrix = glm::mat4(1.f);
        if (ImGui::SliderFloat("Light z", &Globals::lightSource->position.z, 0.f, 1000.f))
            Globals::lightSource->Update(0.f);
        ImGui::End();
        ImGui::SetNextWindowPos(ImVec2(Globals::Width - 600, 0), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(300, Globals::Height), ImGuiCond_FirstUseEver);
        ImGui::Begin("PointsMenu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        test.DisplayMenu();
        ImGui::Checkbox("Use Texture", &Globals::UseTexture);
        ImGui::SameLine();
        if (ImGui::Button("Choose Texture"))
            setTextureDialog.Open();
        ImGui::Checkbox("Use NormalMap", &Globals::UseNormalMap);
        ImGui::SameLine();
        if (ImGui::Button("Choose NormalMap"))
            setNormalMapDialog.Open();
        ImGui::End();

        setTextureDialog.Display();
        setNormalMapDialog.Display();

        if (setTextureDialog.HasSelected())
        {
            setupImage(setTextureDialog.GetSelected().string(), false);
            setTextureDialog.ClearSelected();
        }
        if (setNormalMapDialog.HasSelected())
        {
            setupImage(setNormalMapDialog.GetSelected().string(), true);
            setNormalMapDialog.ClearSelected();
        }

        if(doAnimation)Globals::lightSource->Update(deltaTime);
        Globals::lightSource->Draw();

        test.Draw();
        if (drawMesh)test.DrawMesh();
        ControlPoint::DrawAll();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }
    delete Globals::lightSource;
    Globals::lightSource = nullptr;
    ControlPoint::CleanUp();
    glfwTerminate();
    return 0;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);

    auto tmp = glm::translate(Globals::ViewMatrix, glm::vec3(Shape::m_Position + Shape::m_Width / 2,
        Shape::m_Position + Shape::m_Height / 2, 0.f));
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS 
        || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
        tmp = glm::rotate(tmp, static_cast<float>(yoffset) / 10.f, glm::vec3(0.f, 1.f, 0.f));
    else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        tmp = glm::rotate(tmp, static_cast<float>(yoffset) / 10.f, glm::vec3(0.f, 0.f, 1.f));
    else 
        tmp = glm::rotate(tmp, static_cast<float>(yoffset) / 10.f, glm::vec3(1.f, 0.f, 0.f));
    Globals::ViewMatrix = glm::translate(tmp, glm::vec3(-Shape::m_Position - Shape::m_Width / 2,
        -Shape::m_Position - Shape::m_Height / 2, 0.f));
}

void setupImage(std::string path, bool isNormalMap)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    unsigned int* desiredPlace = isNormalMap ? &Globals::NormalMap : &Globals::Texture;
    glDeleteTextures(1, desiredPlace);
    *desiredPlace = texture;
}
