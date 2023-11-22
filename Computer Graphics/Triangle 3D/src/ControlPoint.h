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
	bool DisplayMenu();

	static float GetZOfPoint(float x, float y);
	static glm::vec3 GetPu(float x, float y);
	static glm::vec3 GetPv(float x, float y);

	static void DrawAll();
	static void CleanUp();
};

