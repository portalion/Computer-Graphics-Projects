#include "Utils.h"
#include <iostream>

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

namespace Globals
{
    int Height = 0;
    int Width = 0;
    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;
}