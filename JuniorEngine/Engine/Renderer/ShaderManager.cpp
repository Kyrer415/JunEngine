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

void ShaderManager::LoadShader(const std::string& name, const std::string& vertPath, const std::string& fragPath)
{
	m_Shaders.try_emplace(name, vertPath, fragPath);
}

void ShaderManager::UseShader(const std::string& name)
{
	GetShader(name).Use();
}

