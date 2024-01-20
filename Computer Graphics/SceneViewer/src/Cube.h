#pragma once
#include "Shader.h"

class Cube
{
private:
	static const float m_Vertices[];
	static const unsigned int m_Indices[];

	unsigned int m_VAO, m_VBO, m_IBO;
public:
	Cube();
	~Cube();
	void Draw(Shader* shader);
};

