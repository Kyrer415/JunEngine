#pragma once

#include <unordered_map>
#include <string>

class Shader; // чтобы видеть класс Shader

class ShaderManager
{
public:

	// Конструктор принимает структуру данных(пиксели, размеры, каналы)
	ShaderManager();
	~ShaderManager();

	// ЗАпрещаем копирование
	ShaderManager(const ShaderManager&) = delete; // Копировать нельзя!
	ShaderManager& operator=(const ShaderManager&) = delete; // присваивать копию нельзя!

	// Разрешаем перемещение
	ShaderManager(ShaderManager&& other) noexcept;
	ShaderManager& operator=(ShaderManager&& other) noexcept;


	// принимает имя шейдера путь к файлам компилирует шейдер и кладёт его в мапу
	void LoadShader(const std::string& name, const std::string& vertPath, const std::string& fragPath);
	
	// Возвращает ссылку на готовый шейдер по имени
	Shader& GetShader(const std::string& name) { return m_Shaders.at(name); };

	// Активация шейдера по имени
	void UseShader(const std::string& name);
private:
	std::unordered_map<std::string, Shader> m_Shaders;
};