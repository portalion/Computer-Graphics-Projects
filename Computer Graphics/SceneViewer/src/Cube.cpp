#include "Cube.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#pragma region verticesDefinition
const float Cube::m_Vertices[] =
{
    -1.f, -1.f,  1.f,     0.f,  0.f,  1.f,
     1.f, -1.f,  1.f,     1.f, -1.f,  1.f,
     1.f,  1.f,  1.f,     1.f,  1.f,  1.f,
    -1.f,  1.f,  1.f,    -1.f,  1.f,  1.f,

    -1.f, -1.f, -1.f,    -1.f, -1.f, -1.f,
     1.f, -1.f, -1.f,     1.f, -1.f, -1.f,
     1.f,  1.f, -1.f,     1.f,  1.f, -1.f,
    -1.f,  1.f, -1.f,    -1.f,  1.f, -1.f
};

const unsigned int Cube::m_Indices[] =
{
	0, 1, 2,
    2, 3, 0,
    // right
    1, 5, 6,
    6, 2, 1,
    // back
    7, 6, 5,
    5, 4, 7,
    // left
    4, 0, 3,
    3, 7, 4,
    // bottom
    4, 5, 1,
    1, 0, 4,
    // top
    3, 2, 6,
    6, 7, 3
};
#pragma endregion

void Cube::InitializeModelAsset()
{
    m_ModelAsset = new ModelAsset
    {
        m_Vertices,
        m_Indices,
        sizeof(m_Vertices),
        sizeof(m_Indices),
        sizeof(m_Indices) / sizeof(unsigned int),
    };
}

Cube::Cube(glm::vec3 startingPos, float scale)
    :DrawableEntity(startingPos, scale)
{
    InitializeObject();
    SetColor({ 0.f, 1.f, 0.f });
}

