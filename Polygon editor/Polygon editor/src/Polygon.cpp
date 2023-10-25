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

void Polygon::MoveByMouse()
{
    Vertex delta =
    {
        ImGui::GetMouseDragDelta(ImGuiMouseButton_Left, 0.f).x,
        ImGui::GetMouseDragDelta(ImGuiMouseButton_Left, 0.f).y
    };
    ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
    for (int i = 0; i < m_Points.size(); i++)
    {
        m_Points[i]->SetPosition(
            {
                m_Points[i]->GetPosition().x + delta.x,
                m_Points[i]->GetPosition().y - delta.y,
            });
    }

    for (int i = 0; i < m_Lines.size(); i++)
        m_Lines[i]->UpdateBasedOnPointsBinded();
}

void Polygon::DrawActive()
{
    Shader* currentShader = Scene::currentShader;
    glm::vec3 NotActiveColor = GetColor(ActivityState::NOT_ACTIVE);
    glm::vec3 ActiveColor = GetColor(ActivityState::ACTIVE);
    glm::vec3 HoveredColor = GetColor(ActivityState::WILL_BE_ACTIVE);

    currentShader->SetUniform4f("u_Color", ActiveColor.x, ActiveColor.y, ActiveColor.z, 1.f);
    for (int i = 0; i < m_Lines.size(); i++)
        if(m_HoveredLineIndex != i)
            m_Lines[i]->Draw();
    currentShader->SetUniform4f("u_Color", NotActiveColor.x, NotActiveColor.y, NotActiveColor.z, 1.f);
    for (int i = 0; i < m_Points.size(); i++)
        if(m_ActivePointIndex != i && m_HoveredPointIndex != i)
            m_Points[i]->Draw();

    currentShader->SetUniform4f("u_Color", ActiveColor.x, ActiveColor.y, ActiveColor.z, 1.f);
    if (m_ActivePointIndex >= 0) m_Points[m_ActivePointIndex]->Draw();
    currentShader->SetUniform4f("u_Color", HoveredColor.x, HoveredColor.y, HoveredColor.z, 1.f);
    if (m_HoveredPointIndex >= 0) m_Points[m_HoveredPointIndex]->Draw();
    if (m_HoveredLineIndex >= 0) m_Lines[m_HoveredLineIndex]->Draw();
    currentShader->SetUniform4f("u_Color", NotActiveColor.x, NotActiveColor.y, NotActiveColor.z, 1.f);
}

void Polygon::DrawNonActive(bool isHovered)
{
    Shader* currentShader = Scene::currentShader;
    glm::vec3 NotActiveColor = GetColor(ActivityState::NOT_ACTIVE);
    glm::vec3 Color = GetColor(isHovered ? ActivityState::WILL_BE_ACTIVE : ActivityState::NOT_ACTIVE);

    currentShader->SetUniform4f("u_Color", Color.x, Color.y, Color.z, 1.f);
    for (int i = 0; i < m_Lines.size(); i++)
        m_Lines[i]->Draw();
    for (int i = 0; i < m_Points.size(); i++)
            m_Points[i]->Draw();
    currentShader->SetUniform4f("u_Color", NotActiveColor.x, NotActiveColor.y, NotActiveColor.z, 1.f);
}

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
    :m_IsCursorVisible{true}, currentState{ UpdatingMode::NOT_ACTIVE }, m_Dragging{false},
    m_ShouldDisplayCursor{true}
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

    for(int i = 0; i < m_Points.size(); i++)
        m_Points[i]->BindLines(m_Lines[i], m_Lines[GetPreviousPointIndex(i)]);
}

void Polygon::AddVertexInLine(int indexOfLine) // 3 size 3
{
    m_Points.insert(m_Points.begin() + indexOfLine + 1, new Point(m_Lines[indexOfLine]->GetMiddlePoint())); // 0

    DeleteLine(indexOfLine);

    m_Lines.insert(m_Lines.begin() + indexOfLine,
        new Line(m_Points[indexOfLine + 1], m_Points[GetNextPointIndex(indexOfLine + 1)]));

    m_Lines.insert(m_Lines.begin() + indexOfLine,
        new Line(m_Points[indexOfLine + 1], m_Points[indexOfLine]));
    
    for (int i = 0; i < m_Points.size(); i++)
        m_Points[i]->BindLines(m_Lines[i], m_Lines[GetPreviousPointIndex(i)]);
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

    for (int i = 0; i < m_Points.size(); i++)
        m_Points[i]->BindLines(m_Lines[i], m_Lines[GetPreviousPointIndex(i)]);
}

void Polygon::UpdateExpectedPoint()
{
    auto& io = ImGui::GetIO();
    Vertex CurrentMousePos = { io.MousePos.x, Scene::m_Height - io.MousePos.y };
    m_ExpectedPointPosition->SetPosition(CurrentMousePos);
    
    if (m_Points.empty()) return;
    m_ExpectedLinePositions[0].SetPosition(m_Points[GetNextPointIndex(m_ActivePointIndex)]->GetPosition(), CurrentMousePos);
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
    m_HoveredPointIndex = -1;
    m_HoveredLineIndex = -1;

    if (m_Dragging)
    {
        MoveByMouse();

        if (io.MouseReleased[ImGuiMouseButton_Left])
            m_Dragging = false;
    }

    switch (currentState)
    {
    case UpdatingMode::NOT_ACTIVE:
        return;
    case UpdatingMode::ADD_VERTICES:
        if (!io.WantCaptureMouse && io.MouseClicked[ImGuiMouseButton_Left])
            AddPointAfterActive(CurrentMousePos);
        if (m_ActivePointIndex >= 0 && m_Points[m_ActivePointIndex]->ShouldRemove())
            RemoveActivePoint();
        UpdateExpectedPoint();
        break;
    case UpdatingMode::EDIT_POLYGON:
        if (io.KeyShift && io.MouseClicked[ImGuiMouseButton_Left])
            m_Dragging = true;

        for (int i = 0; i < m_Points.size(); i++)
            if (m_Points[i]->IsHovered())
            {
                m_HoveredPointIndex = i;
                if (io.MouseClicked[ImGuiMouseButton_Left])
                {
                    m_ActivePointIndex = i;
                    m_Points[i]->dragging = true;
                }
            }
        if (m_HoveredPointIndex == -1)
            for (int i = 0; i < m_Lines.size(); i++)
                if (m_Lines[i]->IsHovered())
                {
                    m_HoveredLineIndex = i;
                    if (io.MouseDoubleClicked[ImGuiMouseButton_Left])
                        AddVertexInLine(i);
                    else if (io.MouseClicked[ImGuiMouseButton_Left])
                        m_Lines[m_HoveredLineIndex]->dragging = true;
                    else continue;
                    break;
                }
        
        if (m_HoveredLineIndex == -1 && m_HoveredPointIndex == -1 &&
            !io.WantCaptureMouse && io.MouseDoubleClicked[ImGuiMouseButton_Left])
            AddPointAfterActive(CurrentMousePos);
        if (m_ActivePointIndex != -1 && m_Points[m_ActivePointIndex]->ShouldRemove())
            RemoveActivePoint();
        if(m_ShouldDisplayCursor)UpdateExpectedPoint();

        if(m_ActivePointIndex != -1)
            m_Points[m_ActivePointIndex]->Update();
        for(int i = 0; i < m_Lines.size(); i++)
            m_Lines[i]->Update();
        break;
    }
    for (int i = 0; i < m_Lines.size(); i++)
        m_Lines[i]->UpdatePositionBasedOnPoints();
}

void Polygon::DisplayMenu()
{
    int tempActivePointIndex = m_ActivePointIndex;
    if (!ImGui::BeginChild("Polygon Editor"))
    {
        ImGui::EndChild();
        return;
    }

    ImGui::PushItemWidth(180);
    ImGui::SliderInt("Select Active Vertice", &tempActivePointIndex, 0, static_cast<unsigned int>(m_Points.size()) - 1);
    ImGui::SameLine(); HelpMarker("CTRL+click to input value.");
    ImGui::PopItemWidth();

    ImGui::SeparatorText("Select mode of current polygon:");
    if (ImGui::Selectable("Editing Mode", currentState == UpdatingMode::EDIT_POLYGON))
        currentState = UpdatingMode::EDIT_POLYGON;
    if (ImGui::Selectable("Adding Mode", currentState == UpdatingMode::ADD_VERTICES))
        currentState = UpdatingMode::ADD_VERTICES;
    
    ImGui::Checkbox("Should display lines for new point", &m_ShouldDisplayCursor);

    if (m_ActivePointIndex >= 0) m_Points[m_ActivePointIndex]->DisplayMenu();
    ImGui::EndChild();
    if(tempActivePointIndex >= 0 && tempActivePointIndex < m_Points.size()) m_ActivePointIndex = tempActivePointIndex;
}

void Polygon::Draw(bool isHovered)
{
    if (isHovered)
    {
        DrawNonActive(true);
        return;
    }
    switch (currentState)
    {
    case UpdatingMode::NOT_ACTIVE:
        DrawNonActive(false);
        return;
    case UpdatingMode::ADD_VERTICES:
        DrawActive();
        DrawExpectedPoint();
        break;
    case UpdatingMode::EDIT_POLYGON:
        DrawActive();
        if (m_ShouldDisplayCursor)
            DrawExpectedPoint();
        break;
    }
}
