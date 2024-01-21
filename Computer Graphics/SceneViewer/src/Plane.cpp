#include "Plane.h"

#pragma region verticesDefinition
const float Plane::m_Vertices[] =
{
    -1.0, -1.0,  1.0, 0.0, 0.0, -1.0,
     1.0, -1.0,  1.0, 0.0, 0.0, -1.0,
     1.0,  1.0,  1.0, 0.0, 0.0, -1.0,

     1.0,  1.0,  1.0, 0.0, 0.0, -1.0,
    -1.0,  1.0,  1.0, 0.0, 0.0, -1.0,
    -1.0, -1.0,  1.0, 0.0, 0.0, -1.0
};
#pragma endregion

void Plane::InitializeModelAsset()
{
    m_ModelAsset = new ModelAsset
    {
        m_Vertices,
        sizeof(m_Vertices),
        sizeof(m_Vertices) / sizeof(float) / 6
    };
}

Plane::Plane(glm::vec3 startingPos, float scale)
    :DrawableEntity{startingPos, scale}
{
    InitializeObject();
}
