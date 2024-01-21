#pragma once
#include "Shader.h"

struct ModelAsset
{
	const float* m_Vertices;
	const unsigned int* m_Indices;
	const unsigned int m_VerticesSize;
	const unsigned int m_IndicesSize;
	const unsigned int m_IndicesCount;
};

class DrawableEntity
{
private:
	unsigned int m_VAO, m_VBO, m_IBO;
protected:
	ModelAsset* m_ModelAsset;

	glm::mat4 m_TranslationMatrix;
	glm::mat4 m_RotationMatrix;
	glm::mat4 m_ScaleMatrix;
	glm::mat4 m_ModelMatrix;

	glm::vec3 color;

	virtual void InitializeModelAsset() = 0;
	virtual void InitializeObject() sealed;

	inline void UpdateModelMatrix()
	{
		m_ModelMatrix = m_TranslationMatrix * m_RotationMatrix * m_ScaleMatrix;
	}
public:
	DrawableEntity(glm::vec3 startingPos = { 0.f, 0.f, 0.f }, float scale = 1.f);
	~DrawableEntity();

	void SetPosition(glm::vec3 position = { 0.f, 0.f, 0.f });
	void Move(glm::vec3 position);
	void Scale(float scale);
	void SetScaling(float scale);
	void Rotate(glm::mat4 rotationMatrix);
	void SetRotation(glm::mat4 rotationMatrix);
	inline void SetModelMatrix(glm::mat4 modelMatrix) { m_ModelMatrix = modelMatrix; }
	inline void SetColor(glm::vec3 color) { this->color = color; }

	virtual void Update(const float& deltaTime);
	inline virtual glm::vec3 GetPosition() const { return { 0.f, 0.f, 0.f }; }

	void Draw(Shader* shader);
};

