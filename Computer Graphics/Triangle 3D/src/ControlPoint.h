#pragma once
#include <glm/glm.hpp>
#include <vector>

class Shader;

class ControlPoint
{
private:
	static const int controlPointsPerSide = 4;

	static unsigned int m_VAO;
	static unsigned int m_VBO;
	static Shader usedShader;

	static std::vector<glm::vec3> m_Positions;

	static void GeneratePoints();

	const int id;	
	float z;
	void UpdateZ();
public:
	ControlPoint(int id);
	void DisplayMenu();

	static void DrawAll();
	static void CleanUp();
};

