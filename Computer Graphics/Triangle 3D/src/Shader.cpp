#include "Shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Utils.h"

Shader::Shader()
{
    for (int i = 0; i < 3; i++)
        shadersUsed[i] = false;
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::AddShader(const std::string& filename, ShaderType type)
{
    unsigned int shaderTypeCompilation = 0;
    switch (type)
    {
    case ShaderType::VERTEX_SHADER:
        shaderTypeCompilation = GL_VERTEX_SHADER;
        break;
    case ShaderType::GEOMETRY_SHADER:
        shaderTypeCompilation = GL_GEOMETRY_SHADER;
        break;
    case ShaderType::FRAGMENT_SHADER:
        shaderTypeCompilation = GL_FRAGMENT_SHADER;
        break;
    }

    shaders[static_cast<size_t>(type)] = CompileShader(shaderTypeCompilation, ParseShader(filename));
    shadersUsed[static_cast<size_t>(type)] = true;
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformVec3f(const std::string& name, const glm::vec3& vector)
{
    GLCall(glUniform3f(GetUniformLocation(name), vector.x, vector.y, vector.z));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!\n";

    m_UniformLocationCache[name] = location;

    return location;
}

void Shader::CreateShader()
{
    unsigned int program = glCreateProgram();

    for(int i = 0; i < 3; i++)
        if(shadersUsed[i]) glAttachShader(program, shaders[i]);

    glLinkProgram(program);
    glValidateProgram(program);

    for (int i = 0; i < 3; i++)
        if (shadersUsed[i]) glDeleteShader(shaders[i]);

    m_RendererID = program;
}

std::string Shader::ParseShader(const std::string& filepath)
{
    std::ifstream ifs(filepath);

    std::string line;
    std::stringstream ss;

    while (getline(ifs, line))
        ss << line << '\n';
    
    return ss.str();
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}