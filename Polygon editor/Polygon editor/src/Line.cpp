#include "Line.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Utils.h"
#include "Scene.h"

 unsigned int Line::m_Vao = 0;
 unsigned int Line:: m_Vbo = 0;

 float Line::m_Vertices[4];

void Line::GenerateModel()
{
	m_Vertices[0] = 0.f;
	m_Vertices[1] = 0.f;
	m_Vertices[2] = 1.f;
	m_Vertices[3] = 0.f;

	GLCall(glGenBuffers(1, &m_Vbo));
	GLCall(glGenVertexArrays(1, &m_Vao));

	GLCall(glBindVertexArray(m_Vao));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(m_Vertices), m_Vertices, GL_STATIC_DRAW));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
	GLCall(glEnableVertexAttribArray(0));

	GLCall(glBindVertexArray(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

Line* Line::GetNeighbour(bool left)
{
	int index = left ? 0 : 1;
	return points[index]->GetLines()[0] == this ?
		points[index]->GetLines()[1] : points[index]->GetLines()[0];;
}

void Line::DeleteModel()
{
	GLCall(glDeleteBuffers(1, &m_Vbo));
	GLCall(glDeleteVertexArrays(1, &m_Vao));
}

Line::Line(Vertex v1, Vertex v2)
	:model(glm::mat4(1.f)), m_PointDragged{false}, dragging{ false }
{
	if (m_Vbo == 0 || m_Vao == 0)
		GenerateModel();

	SetPosition(v1, v2);
}

Line::Line(Point* first, Point* second)
	:model(glm::mat4(1.f)), m_PointDragged{ false }, dragging{false}
{
	if (m_Vbo == 0 || m_Vao == 0)
		GenerateModel();

	BindPoint(first, second);
}

Vertex Line::GetMiddlePoint()
{
	return 
	{
		(points[0]->GetPosition().x + points[1]->GetPosition().x) / 2,
		(points[0]->GetPosition().y + points[1]->GetPosition().y) / 2
	};
}

float area(int x1, int y1, int x2, int y2,
	int x3, int y3)
{
	return abs((x1 * (y2 - y3) + x2 * (y3 - y1) +
		x3 * (y1 - y2)) / 2.0);
}

void Line::UpdatePositionBasedOnPoints()
{
	 int movedPointIndex = points[0]->Moved() || points[0]->dragging ? 0 : 1;
	 int otherPointIndex = 1 - movedPointIndex;

	 if (horizontal)
		 points[otherPointIndex]->SetPosition(points[otherPointIndex]->GetPosition().x, points[movedPointIndex]->GetPosition().y);
	 
	 if(vertical)
		 points[otherPointIndex]->SetPosition(points[movedPointIndex]->GetPosition().x, points[otherPointIndex]->GetPosition().y);

	 SetPosition(points[0]->GetPosition(), points[1]->GetPosition());
}

bool Line::IsHovered()
{
	if (points[0]->IsHovered() || points[1]->IsHovered()
		|| points[0]->dragging || points[1]->dragging) return false;
	
	glm::vec2 first(points[0]->GetPosition().x, points[0]->GetPosition().y);
	glm::vec2 second(points[1]->GetPosition().x, points[1]->GetPosition().y);
	glm::vec2 mousePos(ImGui::GetMousePos().x, Scene::m_Height - ImGui::GetMousePos().y);

	int offset = 5;

	glm::vec2 lineDirection = second - first;
	glm::vec2 pointToLineStart = mousePos - first;

	float t = glm::dot(pointToLineStart, lineDirection) / glm::length2(lineDirection);
	t = glm::clamp(t, 0.0f, 1.0f);

	glm::vec2 closestPoint = first + t * lineDirection;

	return glm::length(mousePos - closestPoint) < offset;
}

void Line::SetPosition(Vertex v1, Vertex v2)
{
	model = glm::mat4(1.f);
	float angle = atan2(v2.y - v1.y, v2.x - v1.x) - atan2(0.f, 1.f);
	model = glm::translate(model, glm::vec3(v1.x, v1.y, 0.f));
	model = glm::rotate(model, angle, glm::vec3(0.f, 0.f, 1.f));
	model = glm::scale(model, glm::vec3(glm::length(glm::vec2(v2.x - v1.x, v2.y - v1.y)), 1.f, 1.f));
}

void Line::UpdateBasedOnPointsBinded()
{
	if (points[0]->Moved() || points[1]->Moved() ||
		points[0]->dragging || points[1]->dragging)
		m_PointDragged = true;
	else
		m_PointDragged = false;
	
	if(m_PointDragged)
		UpdatePositionBasedOnPoints();
}

void Line::DisplayMenu()
{
	Line* left = GetNeighbour(true);
	Line* right = GetNeighbour(false);
	if (ImGui::MenuItem("Vertical", NULL, vertical, !(right->vertical || left->vertical)))
	{
		vertical = !vertical;
		horizontal = false;
		if (vertical) points[0]->SetPosition(points[1]->GetPosition().x, points[0]->GetPosition().y);
	}
	if (ImGui::MenuItem("Horizontal", NULL, horizontal, !(right->horizontal || left->horizontal)))
	{
		horizontal = !horizontal;
		vertical = false;
		if (horizontal) points[0]->SetPosition(points[0]->GetPosition().x, points[1]->GetPosition().y);
	}
}

void Line::Update()
{
	if (dragging)
	{
		auto tmp = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left, 0.f);
		ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
		Vertex delta = { tmp.x, tmp.y };
		Vertex first = points[0]->GetPosition();
		first.x = first.x + delta.x;
		first.y = first.y - delta.y;
		Vertex second = points[1]->GetPosition();
		second.x = second.x + delta.x;
		second.y = second.y - delta.y;
		points[0]->SetPosition(first);
		points[1]->SetPosition(second);

		UpdatePositionBasedOnPoints();
		points[0]->GetLines()[0]->UpdatePositionBasedOnPoints();
		points[0]->GetLines()[1]->UpdatePositionBasedOnPoints();
		points[1]->GetLines()[0]->UpdatePositionBasedOnPoints();
		points[1]->GetLines()[1]->UpdatePositionBasedOnPoints();

		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
			dragging = false;
	}
}

void Line::Draw()
{
	Scene::currentShader->SetUniformMat4f("u_Model", model);
	GLCall(glBindVertexArray(m_Vao));
	GLCall(glDrawArrays(GL_LINES, 0, 2));
}
