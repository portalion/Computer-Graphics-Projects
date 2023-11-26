#include "LightSource.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Utils.h"
#include "Shape.h"

LightSource::LightSource(glm::vec3 position)
{
    float vertices[] = {
        0.f, 0.f, 0.f,
        1.f, 0.f, 0.f,
        1.f, 1.f, 0.f,
        1.f, 1.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 0.f,

        0.f, 0.f, 1.f,
        1.f, 0.f, 1.f,
        1.f, 1.f, 1.f,
        1.f, 1.f, 1.f,
        0.f, 1.f, 1.f,
        0.f, 0.f, 1.f,

        0.f, 1.f, 1.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 1.f,
        0.f, 1.f, 1.f,
    
        1.f, 1.f, 1.f,
        1.f, 1.f, 0.f,
        1.f, 0.f, 0.f,
        1.f, 0.f, 0.f,
        1.f, 0.f, 1.f,
        1.f, 1.f, 1.f,
    
        0.f, 0.f, 0.f,
        1.f, 0.f, 0.f,
        1.f, 0.f, 1.f,
        1.f, 0.f, 1.f,
        0.f, 0.f, 1.f,
        0.f, 0.f, 0.f,
    
        0.f, 1.f, 0.f,
        1.f, 1.f, 0.f,
        1.f, 1.f, 1.f,
        1.f, 1.f, 1.f,
        0.f, 1.f, 1.f,
        0.f, 1.f, 0.f
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    modelMatrix = glm::scale(glm::translate(glm::mat4(1.f), position), { 100.f, 100.f, 100.f });

    shader.AddShader("res/Shaders/LightSource.vs", ShaderType::VERTEX_SHADER);
    shader.AddShader("res/Shaders/LightSource.fs", ShaderType::FRAGMENT_SHADER);
    shader.CreateShader();
    shader.Bind();
    shader.SetUniformMat4f("projectionMatrix", Globals::ProjectionMatrix);
    this->position = position;
}

LightSource::~LightSource()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void LightSource::Update(float deltaTime)
{
    angle = deltaTime * 100.f;
    float tmpX = position.x * cos(glm::radians(angle)) - position.y * sin(glm::radians(angle));
    position.y = position.x * sin(glm::radians(angle)) + position.y * cos(glm::radians(angle));
    position.x = tmpX;
    positionTranslated = position + glm::vec3{ Shape::m_Position + Shape::m_Width / 2, Shape::m_Position + Shape::m_Height / 2, 0.f };
    modelMatrix = glm::scale(glm::translate(glm::mat4(1.f), positionTranslated), { 100.f, 100.f, 100.f });
}

void LightSource::Draw()
{
    shader.Bind();
    shader.SetUniformMat4f("modelMatrix", modelMatrix);
    shader.SetUniformMat4f("viewMatrix", Globals::ViewMatrix);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
