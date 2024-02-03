#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Shader.h"

class Sphere
{
public:
    Sphere(int stackCount = 30, int sliceCount = 30, glm::vec3 position = { 3.0f, 0.f, 0.f }, float rotationAngle = 0.0f, glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 scaling = glm::vec3(1.0f));
    ~Sphere();
    void Draw();
protected:
    float radius;
    int stackCount;
    int sliceCount;

    void InitializeBuffers();

    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    unsigned int count;

   
};