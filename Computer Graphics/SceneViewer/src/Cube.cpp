#include "Cube.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#pragma region verticesDefinition
const float Cube::m_Vertices[] =
{
    -1.0, -1.0,  1.0,
     1.0, -1.0,  1.0,
     1.0,  1.0,  1.0,
    -1.0,  1.0,  1.0,
    // back
    -1.0, -1.0, -1.0,
     1.0, -1.0, -1.0,
     1.0,  1.0, -1.0,
    -1.0,  1.0, -1.0
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

Cube::Cube(glm::vec3 startingPos, float scale)
    :m_ModelMatrix{ 1.f }, m_RotationMatrix{1.f}, 
    m_ScaleMatrix{1.f}, m_TranslationMatrix{1.f}
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_IBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_Vertices), m_Vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_Indices), m_Indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    SetPosition(startingPos);
    SetScaling(scale);
}

Cube::~Cube()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_IBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void Cube::SetPosition(glm::vec3 position)
{
    m_TranslationMatrix = glm::translate(glm::mat4(1.f), position);
    UpdateModelMatrix();
}

void Cube::Move(glm::vec3 position)
{
    m_TranslationMatrix = glm::translate(m_TranslationMatrix, position);
    UpdateModelMatrix();
}

void Cube::Scale(float scale)
{
    m_ScaleMatrix = glm::scale(m_ScaleMatrix, {scale, scale, scale});
    UpdateModelMatrix();
}

void Cube::SetScaling(float scale)
{
    m_ScaleMatrix = glm::scale(glm::mat4(1.f), { scale, scale, scale });
    UpdateModelMatrix();
}

void Cube::Draw(Shader* shader)
{
    shader->SetUniformMat4f("u_ModelMatrix", m_ModelMatrix);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
