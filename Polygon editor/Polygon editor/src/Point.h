#pragma once
#include <glm/glm.hpp>
#include <vector>

struct Vertex;

class Point
{
private:
	static unsigned int m_Vao;
	static unsigned int m_Vbo;
	static unsigned int m_Ibo;

	static const int size = 5;

	static std::vector<glm::vec3> m_Vertices;
	static std::vector<unsigned int> m_Indices;

	static void GenerateVertices();

	bool m_Remove;
	bool m_Moved;

	float x;
	float y;

	void UpdateMatrices();
public:
	static void DeleteModel();

	Point(float x, float y);
	Point(Vertex position);
	glm::mat4 model;

	inline const bool ShouldRemove() const { return m_Remove; }
	inline const bool Moved() const { return m_Moved; }

	Vertex GetPosition();
	bool IsHovered();
	void SetPosition(float x, float y);
	void SetPosition(Vertex position);
	void Draw();
	void DisplayMenu();
};

