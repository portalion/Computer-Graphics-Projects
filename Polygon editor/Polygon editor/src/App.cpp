#include "Scene.h"


int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Scene* scene = Scene::GetInstance();
    GLFWwindow** window = &scene->window;

    *window = glfwCreateWindow(Scene::m_Width, Scene::m_Height, "Hello World", NULL, NULL);
    if (!*window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(*window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "GLEW ERROR" << std::endl;

    scene->Run();
    
    glfwDestroyWindow(*window);
    glfwTerminate();
    return 0;
}