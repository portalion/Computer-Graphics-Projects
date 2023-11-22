#pragma once
#include "Utils.h"
#include "Triangle.h"
#include "Shader.h"

class ControlPoint;

class Shape
{
private:
	std::vector<Triangle*> m_Triangles;
	std::vector<glm::vec3> m_MeshVertices;
	std::vector<unsigned int> m_MeshIndices;


	Shader shader;
	Shader meshShader;

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;

	bool changing = false;

	void UpdateMesh();
	void CleanUp();
public:
	Shape();
	~Shape();

	static int m_Height;
	static int m_Width;

	static int m_Position;
	static std::vector<ControlPoint> m_ControlPoints;

	int n = 7, m = 6;

	void DisplayMenu();
	void GenerateTriangles();
	void GenerateMesh();
	void Draw();
	void DrawMesh();
};

