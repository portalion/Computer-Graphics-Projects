#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

namespace Globals
{
    extern int Height;
    extern int Width;
    extern glm::mat4 ProjectionMatrix;
}