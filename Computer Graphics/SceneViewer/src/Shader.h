#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

enum class ShaderType
{
	VERTEX_SHADER = 0,
	GEOMETRY_SHADER = 1,
	FRAGMENT_SHADER = 2
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::string filepath;
	std::unordered_map<std::string, int> m_UniformLocationCache;

	bool shadersUsed[3];
	unsigned int shaders[3];
public:
	Shader();
	~Shader();

	void CreateShader();
	void Bind() const;
	void Unbind() const;
	void AddShader(const std::string& filename, ShaderType type);

	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformVec3f(const std::string& name, const glm::vec3& vector);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
	int GetUniformLocation(const std::string& name);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	std::string ParseShader(const std::string& filepath);
};