#pragma once
#include "Shader.h"

class Cube
{
private:
	static const float m_Vertices[];
	static const unsigned int m_Indices[];

	unsigned int m_VAO, m_VBO, m_IBO;
	glm::mat4 m_TranslationMatrix;
	glm::mat4 m_RotationMatrix;
	glm::mat4 m_ScaleMatrix;
	glm::mat4 m_ModelMatrix;

	inline void UpdateModelMatrix()
	{
		m_ModelMatrix = m_TranslationMatrix * m_RotationMatrix * m_ScaleMatrix;
	}
public:
	Cube(glm::vec3 startingPos = {0.f, 0.f, 0.f}, float scale = 1.f);
	~Cube();

	void SetPosition(glm::vec3 position = {0.f, 0.f, 0.f});
	void Move(glm::vec3 position);
	void Scale(float scale);
	void SetScaling(float scale);
	void Draw(Shader* shader);
};

