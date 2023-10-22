#include "Polygon.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Utils.h"

#include "Scene.h"
#include "Point.h"
#include "Line.h"

void Polygon::DeleteLine(unsigned int index)
{
    if (index < 0 || index >= m_Lines.size()) return;
    delete m_Lines[index];
    m_Lines.erase(m_Lines.begin() + index);
}

void Polygon::DeletePoint(unsigned int index)
{
    if (index < 0 || index >= m_Points.size()) return;
    delete m_Points[index];
    m_Points.erase(m_Points.begin() + index);
}

Polygon::Polygon()
{
    m_ExpectedPointPosition = new Point(0, 0);
    m_ExpectedLinePositions = new Line[2];
}

Polygon::~Polygon()
{
	for (auto& point : m_Points)
		delete point;
	for (auto& line : m_Lines)
		delete line;

    delete m_ExpectedPointPosition;
    delete[] m_ExpectedLinePositions;
}

void Polygon::AddPointAfterActive(Vertex position)
{
    m_Points.insert(m_Points.begin() + ++m_ActivePointIndex, new Point(position));
    if (m_Points.size() == 1) return;

    unsigned int previousPointIndex = GetPreviousPointIndex(m_ActivePointIndex);
    unsigned int nextPointIndex = GetNextPointIndex(m_ActivePointIndex);

    DeleteLine(previousPointIndex);

    m_Lines.insert(m_Lines.begin() + previousPointIndex, 
        new Line(m_Points[previousPointIndex], m_Points[m_ActivePointIndex]));
    m_Lines.insert(m_Lines.begin() + m_ActivePointIndex,    
        new Line(m_Points[m_ActivePointIndex], m_Points[nextPointIndex]));
}

void Polygon::RemoveActivePoint()
{
    if (m_Points.size() <= 0) return;
    else if (m_Points.size() == 1)
    {
        DeletePoint(m_ActivePointIndex);
        m_ActivePointIndex--;
        return;
    }
    else if (m_Points.size() == 2)
    {
        for (auto& line : m_Lines)
            delete line;
        m_Lines.clear();

        DeletePoint(m_ActivePointIndex);

        m_ActivePointIndex = 0;
        return;
    }
    
    unsigned int previousPointIndex = GetPreviousPointIndex(m_ActivePointIndex);
    unsigned int nextPointIndex = GetNextPointIndex(m_ActivePointIndex);

    DeleteLine(m_ActivePointIndex);
    DeleteLine(previousPointIndex - (m_ActivePointIndex == 0));
    if(m_ActivePointIndex == 0)
        m_Lines.push_back(new Line(m_Points[previousPointIndex], m_Points[nextPointIndex]));
    else m_Lines.insert(m_Lines.begin() + previousPointIndex,
        new Line(m_Points[previousPointIndex], m_Points[nextPointIndex]));
    DeletePoint(m_ActivePointIndex);
    m_ActivePointIndex = GetPreviousPointIndex(m_ActivePointIndex);
}

void Polygon::UpdateExpectedPoint()
{
    if (m_Points.empty()) return;

    auto& io = ImGui::GetIO();
    Vertex CurrentMousePos = { io.MousePos.x, Scene::m_Height - io.MousePos.y };

    unsigned int nextPointIndex = m_ActivePointIndex + 1;
    if (nextPointIndex >= m_Points.size())nextPointIndex = 0;

    m_ExpectedPointPosition->SetPosition(CurrentMousePos);
    m_ExpectedLinePositions[0].SetPosition(m_Points[nextPointIndex]->GetPosition(), CurrentMousePos);
    m_ExpectedLinePositions[1].SetPosition(m_Points[m_ActivePointIndex]->GetPosition(), CurrentMousePos);
    m_IsCursorVisible = !io.WantCaptureMouse;
}

void Polygon::DrawExpectedPoint()
{
    Scene::currentShader->SetUniform4f("u_Color", 0.5f, 0.5f, 0.5f, 1.f);
    if (m_IsCursorVisible)
    {
        m_ExpectedPointPosition->Draw();

        if (m_Points.empty()) return;
        m_ExpectedLinePositions[0].Draw();
        m_ExpectedLinePositions[1].Draw();
    }
}

void Polygon::Update()
{
    auto& io = ImGui::GetIO();
    Vertex CurrentMousePos = { io.MousePos.x, Scene::m_Height - io.MousePos.y };
    if (!io.WantCaptureMouse && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        AddPointAfterActive(CurrentMousePos);

    for (int i = 0; i < m_Lines.size(); i++)
        m_Lines[i]->UpdateBasedOnPointsBinded();

    if (m_ActivePointIndex >= 0 && m_Points[m_ActivePointIndex]->ShouldRemove())
        RemoveActivePoint();
}

void Polygon::DisplayMenu()
{
    if(m_ActivePointIndex >= 0) m_Points[m_ActivePointIndex]->DisplayMenu();

    int tempActivePointIndex = m_ActivePointIndex;
    ImGui::Begin("Polygon Editor");
    ImGui::PushItemWidth(180);
    ImGui::SliderInt("Select Active Vertice", &tempActivePointIndex, 0, m_Points.size() - 1);
    ImGui::SameLine(); HelpMarker("CTRL+click to input value.");
    ImGui::PopItemWidth();
    ImGui::End();
    if(tempActivePointIndex >= 0 && tempActivePointIndex < m_Points.size()) m_ActivePointIndex = tempActivePointIndex;
}

void Polygon::Draw()
{
    Shader* currentShader = Scene::currentShader;
    currentShader->SetUniform4f("u_Color", 0.5f, 0.5f, 0.5f, 1.f);
    for (int i = 0; i < m_Points.size(); i++)
        if (i != m_ActivePointIndex)
            m_Points[i]->Draw();

    currentShader->SetUniform4f("u_Color", 0.f, 1.f, 0.f, 1.f);
    if (m_ActivePointIndex >= 0) m_Points[m_ActivePointIndex]->Draw();
    currentShader->SetUniform4f("u_Color", 0.5f, 0.5f, 0.5f, 1.f);

    currentShader->SetUniform4f("u_Color", 0.f, 1.f, 0.f, 1.f);
    for (int i = 0; i < m_Lines.size(); i++)
        m_Lines[i]->Draw();
    
}
