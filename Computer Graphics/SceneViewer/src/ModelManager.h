#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include <string>

struct ModelAsset
{
	std::vector<glm::vec3> verticies;
	std::vector<glm::vec3> indicies;
};

class ModelManager
{
private:
	static ModelManager* m_Instance;
	
	std::unordered_map<std::string, ModelAsset> m_Models;

	ModelManager();
public:
	static ModelManager* GetModelManager();

	void AddModel(std::string name, ModelAsset model);
	const ModelAsset* GetModel(std::string name);
};

