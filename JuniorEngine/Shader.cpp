#include "Shader.h"
#include <glad/glad.h>
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

