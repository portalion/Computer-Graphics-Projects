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
 int Line::m_Freeid = 0;
 
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

void Line::DrawIcon()
{
	const int size = 20;
	const int offset = 10;
	int posX = points[0]->GetPosition().x / 2 + points[1]->GetPosition().x / 2;
	int posY = Scene::m_Height - points[0]->GetPosition().y / 2 - points[1]->GetPosition().y / 2;

	if (horizontal)
		posY -= offset + size;
	else if (vertical)
		posX += offset;
	
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMouseInputs |
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_NoBringToFrontOnFocus;

	ImGui::SetNextWindowPos(ImVec2(posX - 5, posY - 5), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(25, 25), ImGuiCond_Always);
	ImGui::Begin(("icon" + std::to_string(id)).c_str(), NULL, windowFlags);
	ImVec2 rectPosition(posX, posY);  // Position of the rectangle
	ImU32 color = IM_COL32(255, 0, 0, 255);  // Red color

	ImGui::GetWindowDrawList()->AddRect(rectPosition, ImVec2(posX + size, posY + size), color);
	if(vertical)
		ImGui::GetWindowDrawList()->AddLine(ImVec2(posX + size / 2 - 1, posY), ImVec2(posX + size / 2 - 1, posY + size - 1), color, 1.0f);
	else if (horizontal)
		ImGui::GetWindowDrawList()->AddLine(ImVec2(posX, posY + size / 2 - 1), ImVec2(posX + size - 1, posY + size / 2 - 1), color, 1.0f);
	ImGui::End();
}

void Line::DeleteModel()
{
	GLCall(glDeleteBuffers(1, &m_Vbo));
	GLCall(glDeleteVertexArrays(1, &m_Vao));
}

Line::Line(Vertex v1, Vertex v2)
	:model(glm::mat4(1.f)), m_PointDragged{false}, dragging{ false }
{
	id = m_Freeid++;
	if (m_Vbo == 0 || m_Vao == 0)
		GenerateModel();

	SetPosition(v1, v2);
}

Line::Line(Point* first, Point* second)
	:model(glm::mat4(1.f)), m_PointDragged{ false }, dragging{false}
{
	id = m_Freeid++;
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

void Line::AddRelation(bool horizontal)
{
	int movedPointIndex = points[0]->Moved() || points[0]->dragging ? 0 : 1;
	int otherPointIndex = 1 - movedPointIndex;

	this->horizontal = horizontal;
	this->vertical = !horizontal;

	if (horizontal)
		points[otherPointIndex]->SetPosition(points[otherPointIndex]->GetPosition().x, points[movedPointIndex]->GetPosition().y);
	else
		points[otherPointIndex]->SetPosition(points[movedPointIndex]->GetPosition().x, points[otherPointIndex]->GetPosition().y);
}

void Line::UpdatePositionBasedOnPoints()
{
	int movedPointIndex = points[0]->Moved() || points[0]->dragging ? 0 : 1;
	int otherPointIndex = 1 - movedPointIndex;

	 if (horizontal)
		 points[otherPointIndex]->SetPosition(points[otherPointIndex]->GetPosition().x, points[movedPointIndex]->GetPosition().y);
	 
	 if (vertical)
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

		if (GetNeighbour(true)->horizontal)
		{
			GetNeighbour(true)->points[0]->SetPosition(GetNeighbour(true)->points[0]->GetPosition().x, first.y);
			GetNeighbour(true)->points[1]->SetPosition(GetNeighbour(true)->points[1]->GetPosition().x, first.y);
		}

		if (GetNeighbour(false)->horizontal)
		{
			GetNeighbour(false)->points[0]->SetPosition(GetNeighbour(false)->points[0]->GetPosition().x, second.y);
			GetNeighbour(false)->points[1]->SetPosition(GetNeighbour(false)->points[1]->GetPosition().x, second.y);
		}

		if (GetNeighbour(true)->vertical)
		{
			GetNeighbour(true)->points[0]->SetPosition(first.x, GetNeighbour(true)->points[0]->GetPosition().y);
			GetNeighbour(true)->points[1]->SetPosition(first.x, GetNeighbour(true)->points[1]->GetPosition().y);
		}

		if (GetNeighbour(false)->vertical)
		{
			GetNeighbour(false)->points[0]->SetPosition(second.x, GetNeighbour(false)->points[0]->GetPosition().y);
			GetNeighbour(false)->points[1]->SetPosition(second.x, GetNeighbour(false)->points[1]->GetPosition().y);
		}

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
	if (vertical || horizontal)
		DrawIcon();

	if (Scene::BresenhamLine && points[0] && points[1])
	{
		std::vector<float> vertices;
		int x2 = points[0]->GetPosition().x;
		int y2 = points[0]->GetPosition().y;
		int x1 = points[1]->GetPosition().x;
		int y1 = points[1]->GetPosition().y;
		int dx = abs(x2 - x1);
		int dy = -abs(y2 - y1);
		int sx = (x1 < x2) ? 1 : -1;
		int sy = (y1 < y2) ? 1 : -1;
		int err = dx + dy;
		Scene::currentShader->SetUniformMat4f("u_Model", glm::mat4(1.f));
		while (true) 
		{
			float vertices[] = { x1 , y1 };
			unsigned int VBO, VAO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glBindVertexArray(VAO);
			glDrawArrays(GL_POINTS, 0, 1); // Draw a single point (pixel)

			if (x1 == x2 && y1 == y2) {
				break;
			}

			int e2 = 2 * err;
			if (e2 >= dy) {
				err += dy;
				x1 += sx;
			}
			if (e2 <= dx) {
				err += dx;
				y1 += sy;
			}

			glDeleteBuffers(1, &VBO);
			glDeleteVertexArrays(1, &VAO);
		}
	}
	else
	{
		Scene::currentShader->SetUniformMat4f("u_Model", model);
		GLCall(glBindVertexArray(m_Vao));
		GLCall(glDrawArrays(GL_LINES, 0, 2));
	}
}
