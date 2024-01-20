#include "ModelManager.h"

ModelManager* ModelManager::m_Instance = nullptr;

ModelManager* ModelManager::GetModelManager()
{
    if (ModelManager::m_Instance == nullptr)
        m_Instance = new ModelManager();
    return m_Instance;
}

void ModelManager::AddModel(std::string name, ModelAsset model)
{
    m_Models.try_emplace(name, model);
}

const ModelAsset* ModelManager::GetModel(std::string name)
{
    auto iter = m_Models.find(name);
    if (iter == m_Models.end())
        return nullptr;
    return &(*iter).second;
}

ModelManager::ModelManager()
{

}