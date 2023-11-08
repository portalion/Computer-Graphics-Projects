#include "Circle.h"

void PutPixel(int x, int y)
{
	float vertices[] = { x, y };
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
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void Circle::Update()
{
	if (currentState == UpdatingMode::NOT_ACTIVE) return;
	auto& io = ImGui::GetIO();

	if (io.MouseClicked[ImGuiMouseButton_Left] && io.KeyShift)
		m_Dragging = true;

	if (m_Dragging)
	{
		Vertex delta =
		{
			ImGui::GetMouseDragDelta(ImGuiMouseButton_Left, 0.f).x,
			ImGui::GetMouseDragDelta(ImGuiMouseButton_Left, 0.f).y
		};
		ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
		x += delta.x;
		y -= delta.y;

		if (io.MouseReleased[ImGuiMouseButton_Left])
			m_Dragging = false;
	}
	
}

void Circle::DisplayMenu()
{
	if (!ImGui::BeginChild("Circle Editor"))
	{
		ImGui::EndChild();
		return;
	}
	ImGui::PushItemWidth(180);
	ImGui::SliderFloat("Select Radius", &r, 0, 1000);
	ImGui::EndChild();
}

void Circle::Draw(bool color)
{
	int deltaE = 3;
	int deltaSE = 5 - 2 * r;
	int d = 1 - r;
	int x = 0;
	int y = r;
	Scene::currentShader->SetUniformMat4f("u_Model", glm::mat4(1.f));
	Scene::currentShader->SetUniform4f("u_Color", 1.f, 1.f, 1.f, 1.f);
	while (y > x)
	{
		if (d < 0) //Select E
		{
			d += deltaE;
			deltaE += 2;
			deltaSE += 2;
		}
		else //Select SE
		{
			d += deltaSE;
			deltaE += 2;
			deltaSE += 4;
			y--;
		}
		x++;
		
		PutPixel(this->x + x, this->y + y);
		PutPixel(this->x - x, this->y + y);
		PutPixel(this->x - x, this->y - y);
		PutPixel(this->x + x, this->y - y);

		PutPixel(this->x + y, this->y + x);
		PutPixel(this->x - y, this->y + x);
		PutPixel(this->x - y, this->y - x);
		PutPixel(this->x + y, this->y - x);
	}

	for (x = -r; x <= r; x++)
	{
		float yTemp = sqrt(r * r - x * x);

		int x1 = this->x + x;
		int x2 = this->x + x;
		int y1 = (int)(this->y - yTemp);
		int y2 = (int)(this->y + yTemp);

		float alpha = 1.f - (yTemp - (int)yTemp);

		if (alpha == 1.f)
		{
			Scene::currentShader->SetUniform4f("u_Color", 1.f, 1.f, 1.f, 1.f);
			PutPixel(x1, y1);
			PutPixel(x2, y2);
		}
		else
		{
			float alpha1 = (alpha);
			float alpha2 = ((1.0 - alpha));
			Scene::currentShader->SetUniform4f("u_Color", 0.3f + alpha1, 0.3f + alpha1, 0.3f + alpha1, 1.f);
			PutPixel(x1, y1);
			Scene::currentShader->SetUniform4f("u_Color", 0.3f + alpha2, 0.3f + alpha2, 0.3f + alpha2, 1.f);
			PutPixel(x2, y2);
		}
	}
}