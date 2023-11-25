#include "Triangle.h"
#include <algorithm>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Utils.h"
#include "ControlPoint.h"

Triangle::~Triangle()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteVertexArrays(1, &m_VAO);
}

float GetSlopeZ(glm::vec3 p1, glm::vec3 p2)
{
	return (p2.y - p1.y) / (p2.z - p1.z);
}

float GetSlope(glm::vec3 p1, glm::vec3 p2)
{
	return (p2.y - p1.y) / (p2.x - p1.x);
}

Triangle::Triangle(glm::vec3 vertices[3])
{
	for (int i = 0; i < 3; i++)
		m_Points[i] = vertices[i];
	glGenBuffers(1, &m_VBO);
	glGenVertexArrays(1, &m_VAO);
}

glm::vec3 calculateBarycentricCoordinates(const glm::vec3& p, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
	glm::vec3 v0 = b - a;
	glm::vec3 v1 = c - a;
	glm::vec3 v2 = p - a;
	float d00 = glm::dot(v0, v0);
	float d01 = glm::dot(v0, v1);
	float d11 = glm::dot(v1, v1);
	float d20 = glm::dot(v2, v0);
	float d21 = glm::dot(v2, v1);
	float denom = d00 * d11 - d01 * d01;
	float v = (d11 * d20 - d01 * d21) / denom;
	float w = (d00 * d21 - d01 * d20) / denom;
	float u = 1.0f - v - w;

	return glm::vec3(u, v, w);
}

glm::vec3 interpolateNormals(const glm::vec3& N0, const glm::vec3& N1, const glm::vec3& N2, const glm::vec3& barycentricCoords) {
	return barycentricCoords.x * N0 + barycentricCoords.y * N1 + barycentricCoords.z * N2;
}

void Triangle::GenerateFillVertices()
{
	const float YCHANGE = 0.25f;

	struct AETPointer
	{
		float ymax;
		float x;
		float m;
		float z;
		float mz;

		bool operator==(AETPointer p)
		{
			return p.x == this->x && p.ymax == this->ymax;
		}
	};

	for (int i = 0; i < 3; i++)
	{
		auto first = ControlPoint::GetPu(m_Points[i].x, m_Points[i].y);
		auto second = ControlPoint::GetPv(m_Points[i].x, m_Points[i].y);
		m_Normals[i] = glm::normalize(glm::cross(first, second));
	}

	m_FilledLines.clear();

	int ind[3] = { 0, 1, 2 };
	std::sort(ind, ind + 3, [this](int a, int b) {return m_Points[a].y < m_Points[b].y; });
	int ymin = static_cast<int>(m_Points[ind[0]].y);
	int ymax = static_cast<int>(m_Points[ind[2]].y);

	std::vector<AETPointer> AET;

	for (float y = ymin; y <= ymax; y+=YCHANGE)
	{
		for (int i = 0; i < 3; i++)
		{
			if (m_Points[ind[i]].y == y)
			{
				int prev = ind[(i + 2) % 3];
				int next = ind[(i + 1) % 3];

				if (m_Points[prev].y >= m_Points[ind[i]].y)
					AET.push_back(AETPointer{ m_Points[prev].y, m_Points[ind[i]].x, GetSlope(m_Points[prev], m_Points[ind[i]]),
						m_Points[ind[i]].z, GetSlopeZ(m_Points[prev], m_Points[ind[i]]) });
				else AET.erase(std::remove(AET.begin(), AET.end(), AETPointer{ m_Points[prev].y, m_Points[prev].x, 0.f, 0.f, 0.f }), AET.end());

				if (m_Points[next].y >= m_Points[ind[i]].y)
					AET.push_back(AETPointer{ m_Points[next].y, m_Points[ind[i]].x, GetSlope(m_Points[ind[i]], m_Points[next]),
						m_Points[ind[i]].z, GetSlopeZ(m_Points[ind[i]], m_Points[next]) });
				else AET.erase(std::remove(AET.begin(), AET.end(), AETPointer{ m_Points[next].y, m_Points[next].x, 0.f, 0.f, 0.f }), AET.end());
			}
		}
		
		AET.erase(std::remove_if(AET.begin(), AET.end(), [&y](AETPointer p) {return p.ymax < y; }), AET.end());
		std::sort(AET.begin(), AET.end(), [](AETPointer a, AETPointer b) {return a.x < b.x; });

		for (int i = 0; i < AET.size(); i += 2)
		{
			Vertex first, second;
			first.position = { AET[i].x, y, AET[i].z };
			second.position = { AET[i + 1].x, y, AET[i + 1].z };
			first.normal = interpolateNormals(m_Normals[0], m_Normals[1], m_Normals[2], 
				calculateBarycentricCoordinates(first.position, m_Points[0], m_Points[1], m_Points[2]));
			second.normal = interpolateNormals(m_Normals[0], m_Normals[1], m_Normals[2],
				calculateBarycentricCoordinates(second.position, m_Points[0], m_Points[1], m_Points[2]));
			m_FilledLines.push_back(first);
			m_FilledLines.push_back(second);
		}
		for (int i = 0; i < AET.size(); i++)
		{
			if(AET[i].m != 0)
				AET[i].x += YCHANGE / AET[i].m;
			if (AET[i].mz != 0)
				AET[i].z += YCHANGE / AET[i].mz;
		}
	}
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_FilledLines.size() * sizeof(Vertex), m_FilledLines.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Triangle::Draw(Shader* shader)
{
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_LINES, 0, static_cast<unsigned int>(m_FilledLines.size()));
}
