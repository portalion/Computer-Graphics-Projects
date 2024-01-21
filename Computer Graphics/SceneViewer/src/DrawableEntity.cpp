#include "DrawableEntity.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

void DrawableEntity::InitializeObject()
{
    InitializeModelAsset();
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_IBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_ModelAsset->m_VerticesSize, m_ModelAsset->m_Vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_ModelAsset->m_IndicesSize, m_ModelAsset->m_Indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

DrawableEntity::DrawableEntity(glm::vec3 startingPos, float scale)
    :m_ModelMatrix{ 1.f }, m_RotationMatrix{ 1.f },
    m_ScaleMatrix{ 1.f }, m_TranslationMatrix{ 1.f }
{
    color = { 0.3f, 0.3f, 0.f };
    SetPosition(startingPos);
    SetScaling(scale);
    UpdateModelMatrix();
}

DrawableEntity::~DrawableEntity()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_IBO);
    glDeleteVertexArrays(1, &m_VAO);

    delete m_ModelAsset;
}

void DrawableEntity::SetPosition(glm::vec3 position)
{
    m_TranslationMatrix = glm::translate(glm::mat4(1.f), position);
    UpdateModelMatrix();
}

void DrawableEntity::Move(glm::vec3 position)
{
    m_TranslationMatrix = glm::translate(m_TranslationMatrix, position);
    UpdateModelMatrix();
}

void DrawableEntity::Scale(float scale)
{
    m_ScaleMatrix = glm::scale(m_ScaleMatrix, { scale, scale, scale });
    UpdateModelMatrix();
}

void DrawableEntity::SetScaling(float scale)
{
    m_ScaleMatrix = glm::scale(glm::mat4(1.f), { scale, scale, scale });
    UpdateModelMatrix();
}

void DrawableEntity::Rotate(glm::mat4 rotationMatrix)
{
    m_RotationMatrix *= rotationMatrix;
    UpdateModelMatrix();
}

void DrawableEntity::SetRotation(glm::mat4 rotationMatrix)
{
    m_RotationMatrix = rotationMatrix;
    UpdateModelMatrix();
}

void DrawableEntity::Update(const float& deltaTime)
{
}


void DrawableEntity::Draw(Shader* shader)
{
    shader->SetUniformMat4f("u_ModelMatrix", m_ModelMatrix);
    shader->SetUniformVec3f("u_Color", color);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_ModelAsset->m_IndicesCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
