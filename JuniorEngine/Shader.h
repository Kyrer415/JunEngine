#pragma once

#include <string>

class Shader
{
public:
	// Конструктор принимает пути к файлам шейдеров на диске
	Shader(const std::string& vertexPath, const std::string& fragmentPath);

	// Деструктор удалит шейдерную программу из памяти видеокарты
	~Shader();

	// Активация шейдерной программы (замена glUseProgram)
	void Use() const;

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

private:
	unsigned int m_ID; // ID скомпилированной шейдерной программы


};