#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class LightSource;

namespace Globals
{
    extern bool UseTexture;
    extern std::string TexturePath;
    extern bool UseNormalMap;
    extern std::string NormalMapPath;
    extern int Height;
    extern int Width;
    extern glm::mat4 ProjectionMatrix;
    extern glm::mat4 ViewMatrix;
    extern LightSource* lightSource;

    extern int m;
    extern float kd;
    extern float ks;
    extern glm::vec3 objectColor;
    extern glm::vec3 lightColor;
}