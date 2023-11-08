#pragma once
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

class Circle : public Polygon
{
public:
	float x, y;
	float r;
	bool m_Dragging = false;
	void Update() override;
	void DisplayMenu() override;
	void Draw(bool isHovered = false) override;
};