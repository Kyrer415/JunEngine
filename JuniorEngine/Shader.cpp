#include "Shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	: m_ID(0)
{
	// читаем исходный код шейдеров из файлов на диске
	std::string vertexCode = ReadFile(vertexPath);
	std::string fragmentCode = ReadFile(fragmentPath);

	// Если файлы успешно прочитаны, компилируем их
	if (!vertexCode.empty() && !fragmentCode.empty())
	{
		Compile(vertexCode, fragmentCode);
	}
}

Shader::~Shader()
{
	// Удаляем програму с видеокарты, когда объект класса уничтожается
	if (m_ID != 0)
	{
		glDeleteProgram(m_ID);
	}
}

void Shader::Use() const
{
	// Активируем шейдерную программу для отрисвки
	glUseProgram(m_ID);
}

std::string Shader::ReadFile(const std::string& filePath)
{
	std::string code;
	std::ifstream shaderFile;

	// Гарантируем, что объекты ifstream могут генерировать исключения
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// открываем файл
		shaderFile.open(filePath);
		std::stringstream shaderStream;

		// Читаем содержиммое файла в буфер потока
		shaderStream << shaderFile.rdbuf();

		// Закрываем файл
		shaderFile.close();

		// Конвертируем поток в строку
		code = shaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cerr << "ОШИБКА: Шейдерный файл не был успешно прочитан по пути:" << filePath << std::endl;
	}
	
	return code;
}


void Shader::Compile(const std::string& vertexCode, const std::string& fragmentCode)
{
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// 1. Компиляция вершинного шейдера
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		std::cerr << "ОШИБКА: Компиляция Вершинного Шейдера внутри класса провалена: \n" << infoLog << std::endl;
	}

	// 2. Компиляция фрагментного шейдреа
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		std::cerr << "ОШИБКА: Компиляция фрагментного шейдера внутри класса провалена: \n" << infoLog << std::endl;
	}

	// 3. Создание шейдерной программы и линковка
	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertex);
	glAttachShader(m_ID, fragment);
	glLinkProgram(m_ID);

	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ID, 512, nullptr, infoLog);
		std::cerr << "ОШИБКА: Линковка Шейдерной Программы ванутри класса провалена: \n" << infoLog << std::endl;
	}

	// Удаляем шейдера, они нам больше не нужны, так как они уже связаны в программе m_ID
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	 
}

void Shader::SetFloat4(const std::string& name, float v0, float v1, float v2, float v3) const
{
	// 1. Находим "адрес" переменной по нашему методу
	int vertexColorLocation = GetUniformLocation(name);

	// 2. Если переменная найдена, загружаем в неё 4 наших значения
	if (vertexColorLocation != -1)
	{
		glUniform4f(vertexColorLocation, v0, v1, v2, v3);
	}
}
void Shader::SetFloat(const std::string& name, float value) const
{
	// 1. Находим "адрес" с помощью нашего метода
	int Location = GetUniformLocation(name);

	if (Location != -1)
	{
		glUniform1f(Location, value);
	}
}

int Shader::GetUniformLocation(const std::string& name) const
{
	// 1. Проверяем, есть ли уже это имя в кжше
	auto it = m_UniformCache.find(name);
	if (it != m_UniformCache.end())
	{
		return it->second; // Нашли! Мнгновенно возвращаем сохранённый ID ячейки
	}


// 2. Если в кеше нет, справшивает у видеокарты
int Location = glGetUniformLocation(m_ID, name.c_str());

if (Location == -1)
{
}

// 3. Запоминаем в кэш
m_UniformCache[name] = Location;
return Location;
}

void Shader::SetMatrix4(const std::string& name, const glm::mat4& matrix) const
{
	int Location = GetUniformLocation(name); // наш оптимизированный кеш метод получения имени порта
	if (Location != -1) // если он равен -1 значит порта несуществует 
	{
		// Передаём матрицу 4x4 на видеокарту
		glUniformMatrix4fv(Location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}