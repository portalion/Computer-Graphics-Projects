#include "Utils.h"
#include <iostream>
#include "LightSource.h"

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
    LightSource* lightSource = nullptr;

    int m = 1;
    float kd = 1;
    float ks = 0;
    glm::vec3 objectColor = glm::vec3(0.f, 0.f, 1.f);
    glm::vec3 lightColor = glm::vec3(1.f, 1.f, 1.f);

    bool UseTexture = false;
    unsigned int Texture = 0;
    bool UseNormalMap = false;
    unsigned int NormalMap = 0;
}