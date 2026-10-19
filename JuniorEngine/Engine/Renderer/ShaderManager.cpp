#include "Shader.h"
#include "ShaderManager.h"
#include <glad/glad.h>

ShaderManager::ShaderManager()
{

}

ShaderManager::ShaderManager(ShaderManager&& o) noexcept
{
}

ShaderManager& ShaderManager::operator=(ShaderManager&& o) noexcept
{
	m_Shaders = std::move(o.m_Shaders);
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::LoadShader(const std::string& name, const std::string& vertPath, const std::string& fragPath)
{
	m_Shaders.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(vertPath, fragPath));
}

void ShaderManager::UseShader(const std::string& name)
{
	GetShader(name).Use();
}