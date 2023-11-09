#include "Utils.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[Opengl Error] (" << error << "): " <<
            function << " " << file << ":" << line << '\n';
        return false;
    }
    return true;
}
int Globals::Width = 1920;
int Globals::Height = 1000;
glm::mat4 Globals::ProjectionMatrix = glm::ortho(0.f, static_cast<float>(Globals::Width), 0.f, static_cast<float>(Globals::Height), 0.f, 10.f);