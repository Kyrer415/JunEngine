#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map> // 1. Подключаем хэш-таблицу

class Shader
{
public:
	// Конструктор принимает пути к файлам шейдеров на диске
	Shader(const std::string& vertexPath, const std::string& fragmentPath);

	// Деструктор удалит шейдерную программу из памяти видеокарты
	~Shader();

	// Активация шейдерной программы (замена glUseProgram)
	void Use() const;

	void SetMatrix4(const std::string& name, const glm::mat4& matrix) const;

	// Метод для передачи 4-х чисел с плавающей точкой (например, цвет RGBA)
	void SetFloat4(const std::string& name, float v0, float v1, float v2, float v3) const;
	void SetFloat(const std::string& name, float value) const;
	void SetInt(const std::string& name, int value) const;

	// В будущем здесь будут методы для передачи данных в шейдер (Юниформы)
	// SetFloat
	// SetMatrix4

	// Получить ID программы
	unsigned int GetID() const { return m_ID; }

private:
	// Внутренний метод для чтения текстового файла с диска
	std::string ReadFile(const std::string& filePath);

	// Внутренний метод для компиляции и проверки на ошибки
	void Compile(const std::string& vertexCode, const std::string& fragmentCode);

	// 2. Внутренний метод для поиска локации с кэшированием
	// mutable позволяет менять кэш даже внутри константных методов класса
	int GetUniformLocation(const std::string& name) const;

private:
	unsigned int m_ID; // ID скомпилированной шейдерной программы
	mutable std::unordered_map<std::string, int> m_UniformCache; // 3. "Имя -> Id ячейки"

};