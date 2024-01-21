#include "Plane.h"

#pragma region verticesDefinition
const float Plane::m_Vertices[] =
{
    -1.0, -1.0,  1.0,
     1.0, -1.0,  1.0,
     1.0,  1.0,  1.0,
    -1.0,  1.0,  1.0,
};

const unsigned int Plane::m_Indices[] =
{
    0, 1, 2,
    2, 3, 0,
};
#pragma endregion

void Plane::InitializeModelAsset()
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

Plane::Plane(glm::vec3 startingPos, float scale)
    :DrawableEntity{startingPos, scale}
{
    InitializeObject();
}
