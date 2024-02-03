#include "Sphere.h"
#include <cmath>

#define PI 3.14149

Sphere::Sphere(int stackCount, int sliceCount, glm::vec3 position, float rotationAngle, glm::vec3 rotationAxis, glm::vec3 scaling)
    :stackCount{stackCount}, sliceCount{sliceCount}
{

    radius = 1.0f;
    InitializeBuffers();
}

Sphere::~Sphere() { }

float sdfSphere(glm::vec3 point)
{
    return glm::length(glm::vec3{3.f, 0.f, 0.f} - point) - 1.f;
}

glm::vec3 getNormal(glm::vec3 point)
{
    const float h = 0.1f;
    glm::vec3 normal =
    {
        sdfSphere(point + glm::vec3{ h, 0, 0 }) -
        sdfSphere(point - glm::vec3{ h, 0, 0 }),
        sdfSphere(point + glm::vec3{ 0, h, 0 }) -
        sdfSphere(point - glm::vec3{ 0, h, 0 }) ,
        sdfSphere(point + glm::vec3{ 0, 0, h }) -
        sdfSphere(point - glm::vec3{ 0, 0, h }) ,
    };

    return glm::normalize(normal);
}

void Sphere::InitializeBuffers()
{
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    for (int stack = 0; stack < stackCount; ++stack)
    {
        for (int slice = 0; slice < sliceCount; ++slice)
        {
            float phi1 = static_cast<float>(PI) * stack / stackCount;
            float phi2 = static_cast<float>(PI) * (stack + 1) / stackCount;
            float theta1 = 2.0f * static_cast<float>(PI) * slice / sliceCount;
            float theta2 = 2.0f * static_cast<float>(PI) * (slice + 1) / sliceCount;

            glm::vec3 v1(radius * std::sin(phi1) * std::cos(theta1),
                radius * std::sin(phi1) * std::sin(theta1),
                radius * std::cos(phi1));

            glm::vec3 v2(radius * std::sin(phi1) * std::cos(theta2),
                radius * std::sin(phi1) * std::sin(theta2),
                radius * std::cos(phi1));

            glm::vec3 v3(radius * std::sin(phi2) * std::cos(theta1),
                radius * std::sin(phi2) * std::sin(theta1),
                radius * std::cos(phi2));

            glm::vec3 v4(radius * std::sin(phi2) * std::cos(theta2),
                radius * std::sin(phi2) * std::sin(theta2),
                radius * std::cos(phi2));

            indices.push_back(vertices.size() / 6);
            indices.push_back(vertices.size() / 6 + 1);
            indices.push_back(vertices.size() / 6 + 2);

            indices.push_back(vertices.size() / 6 + 2);
            indices.push_back(vertices.size() / 6 + 1);
            indices.push_back(vertices.size() / 6 + 3);

            vertices.push_back(v1.x + 3.f);
            vertices.push_back(v1.y);
            vertices.push_back(v1.z);
            auto normal = getNormal({ vertices[vertices.size() - 3], vertices[vertices.size() - 2], vertices[vertices.size() - 1] });
            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);

            vertices.push_back(v2.x + 3.f);
            vertices.push_back(v2.y);
            vertices.push_back(v2.z);
            normal = getNormal({ vertices[vertices.size() - 3], vertices[vertices.size() - 2], vertices[vertices.size() - 1] });
            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);

            vertices.push_back(v3.x + 3.f);
            vertices.push_back(v3.y);
            vertices.push_back(v3.z);
             normal = getNormal({ vertices[vertices.size() - 3], vertices[vertices.size() - 2], vertices[vertices.size() - 1] });
            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);

            vertices.push_back(v4.x + 3.f);
            vertices.push_back(v4.y);
            vertices.push_back(v4.z);
             normal = getNormal({ vertices[vertices.size() - 3], vertices[vertices.size() - 2], vertices[vertices.size() - 1] });
            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    count = indices.size();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Sphere::Draw()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
