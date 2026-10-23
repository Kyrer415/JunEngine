#include "Shader.h"
#include "ShaderManager.h"
#include <glad/glad.h>

ShaderManager::ShaderManager()
{

}

ShaderManager::ShaderManager(ShaderManager&& o) noexcept
	: m_Shaders(std::move(o.m_Shaders))
{
}

ShaderManager& ShaderManager::operator=(ShaderManager&& o) noexcept
{
	if (this != &o)
	{
		m_Shaders = std::move(o.m_Shaders);
	}
	return *this;
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::LoadShader(const std::string& name, const std::string& filepath)
{
	Shader newShader(filepath);
	m_Shaders.insert({ name, std::move(newShader) });
}

void ShaderManager::UseShader(const std::string& name)
{
	GetShader(name).Use();
}

